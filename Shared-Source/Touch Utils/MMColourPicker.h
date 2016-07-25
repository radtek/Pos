//---------------------------------------------------------------------------

#ifndef MMColourPickerH
#define MMColourPickerH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "cgrid.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmColourPicker : public TZForm
{
__published:	// IDE-managed Components
	TPanel *Panel16;
	TTouchBtn *TouchBtn1;
	TTouchBtn *TouchBtn3;
	TPanel *Panel2;
	TLabel *lbeCaption;
	TCColorGrid *CColorGrid;
	TColorDialog *ColorDialog;
	TTouchBtn *tbtnOtherColour;
	void __fastcall tbtnOtherColourMouseClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall CColorGridChange(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
	void __fastcall TouchBtn3MouseClick(TObject *Sender);
	void __fastcall CColorGridClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmColourPicker(TComponent* Owner);
   int SelectedColour;
};
//---------------------------------------------------------------------------
#endif
