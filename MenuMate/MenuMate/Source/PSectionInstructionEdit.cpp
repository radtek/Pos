//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "PSectionInstructionEdit.h"
#include "MMTouchKeyboard.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmPSectionInstructionEdit *frmPSectionInstructionEdit;
//---------------------------------------------------------------------------
__fastcall TfrmPSectionInstructionEdit::TfrmPSectionInstructionEdit(TComponent* Owner)
	: TZForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TfrmPSectionInstructionEdit::btnOkClick(TObject *Sender)
{
	ModalResult = mrOk;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmPSectionInstructionEdit::FormClose(TObject *Sender,
      TCloseAction &Action)
{
	Instruction = NULL;	
}
//---------------------------------------------------------------------------
void __fastcall TfrmPSectionInstructionEdit::FormShow(TObject *Sender)
{
	FormResize(Sender);
	edCaption->Text =	Instruction->Caption;
	tcbLAS->Latched = Instruction->LineAbove;
	tcbLBS->Latched = Instruction->LineBelow;
	tcbDLAS->Latched = Instruction->DrawLineAbove;
	tcbDLBS->Latched = Instruction->DrawLineBelow;
	tcbCAS->Latched = Instruction->Cut;
	tcbHS->Latched = !Instruction->Visible;
	tchUSC->Latched = Instruction->FontInfo.Underlined;
	tcbBSC->Latched = Instruction->FontInfo.Bold;
	tcbIWC->Latched = Instruction->FontInfo.Width == fsDoubleSize ? true : false;
	tcbIHC->Latched = Instruction->FontInfo.Height == fsDoubleSize ? true : false;
	tcbPCR->Latched = Instruction->FontInfo.Colour == fcRed ? true : false;
	tcbSPF->Latched = Instruction->FontInfo.Font == ftFontB ? true : false;
}
//---------------------------------------------------------------------------
void  TfrmPSectionInstructionEdit::UpdateDisplay()
{
	Instruction->Caption                     = edCaption->Text;
	Instruction->LineAbove                   = tcbLAS->Latched;
	Instruction->LineBelow                   = tcbLBS->Latched;
	Instruction->DrawLineAbove               = tcbDLAS->Latched;
	Instruction->DrawLineBelow               = tcbDLBS->Latched;
	Instruction->Cut                         = tcbCAS->Latched;
	Instruction->Visible                     = !tcbHS->Latched;
	Instruction->FontInfo.Underlined         = tchUSC->Latched;
	Instruction->FontInfo.Bold               = tcbBSC->Latched;
	Instruction->FontInfo.Width   			  = tcbIWC->Latched == true ? fsDoubleSize : fsNormalSize;
	Instruction->FontInfo.Height  			  = tcbIHC->Latched == true ? fsDoubleSize : fsNormalSize;
	Instruction->FontInfo.Colour				  = tcbPCR->Latched == true ? fcRed : fcBlack;
	Instruction->FontInfo.Font					  = tcbSPF->Latched == true ? ftFontB : ftFontA;

	AfterPropertyChanged.Occured();
}
void __fastcall TfrmPSectionInstructionEdit::tcbLASMouseUp(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	UpdateDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPSectionInstructionEdit::edCaptionClick(
      TObject *Sender)
{
  	std::auto_ptr<TfrmTouchKeyboard> frmTouchKeyboard(TfrmTouchKeyboard::Create<TfrmTouchKeyboard>(this));
	frmTouchKeyboard->MaxLength = 255;
	frmTouchKeyboard->AllowCarriageReturn = false;
	frmTouchKeyboard->StartWithShiftDown = false;
	frmTouchKeyboard->KeyboardText = edCaption->Text;
	frmTouchKeyboard->Caption = "Enter Sections Caption i.e Tables";
	if (frmTouchKeyboard->ShowModal() == mrOk)
	{
		edCaption->Text = frmTouchKeyboard->KeyboardText;
	}
	UpdateDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmPSectionInstructionEdit::FormResize(TObject *Sender)
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
	pnlButtons->Left = Panel1->Left + Panel1->Width + Panel1->Left;
	Width = pnlButtons->Left + pnlButtons->Width + Panel1->Left;
}
//---------------------------------------------------------------------------


void __fastcall TfrmPSectionInstructionEdit::tbPrintingMouseClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

