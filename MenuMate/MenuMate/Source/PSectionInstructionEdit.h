//---------------------------------------------------------------------------

#ifndef PSectionInstructionEditH
#define PSectionInstructionEditH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "PrintingSections.h"
#include "SystemEvents.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmPSectionInstructionEdit : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlButtons;
	TTouchBtn *tbPrinting;
	TPanel *Panel1;
	TEdit *edCaption;
	TLabel *Label1;
	TTouchBtn *tcbLAS;
	TTouchBtn *tcbLBS;
	TTouchBtn *tcbHS;
	TTouchBtn *tcbDLAS;
	TTouchBtn *tcbDLBS;
	TGroupBox *GroupBox1;
	TTouchBtn *tcbCAS;
	TTouchBtn *tchUSC;
	TTouchBtn *tcbBSC;
	TTouchBtn *tcbIWC;
	TTouchBtn *tcbIHC;
	TTouchBtn *tcbPCR;
	TTouchBtn *tcbSPF;
	TTouchBtn *btnAddTab;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tcbLASMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edCaptionClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tbPrintingMouseClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmPSectionInstructionEdit(TComponent* Owner);
public:		// User declarations

	TPSectionInstruction *Instruction;
	TSystemEvents AfterPropertyChanged;
	void UpdateDisplay();	
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPSectionInstructionEdit *frmPSectionInstructionEdit;
//---------------------------------------------------------------------------
#endif
