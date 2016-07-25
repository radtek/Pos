//---------------------------------------------------------------------------

#ifndef PrinterControlPanelH
#define PrinterControlPanelH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "MM_DBCore.h"
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmPrinterControlPanel : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *pnlOptions;
	TPanel *pnlControl;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
	TGroupBox *GroupBox3;
	TGroupBox *GroupBox4;
	TMemo *Memo1;
	TMemo *Memo2;
	TTouchBtn *TouchBtn1;
	TTouchBtn *tbPrintingOrder;
	TTouchBtn *tbtnCallAwayPrinting;
	TTouchBtn *TouchBtn4;
	TTouchBtn *TouchBtn5;
	TGroupBox *GroupBox5;
	TTouchBtn *TouchBtn2;
	TMemo *Memo3;
	TMemo *Memo4;
	TTouchBtn *TouchBtn3;
	TGroupBox *GroupBox6;
	TMemo *Memo5;
	TMemo *Memo6;
	TGroupBox *GroupBox7;
	TMemo *Memo7;
	TTouchBtn *tbtnPrintingByCat;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tbPrintingOrderMouseClick(TObject *Sender);
	void __fastcall tbtnCallAwayPrintingMouseClick(TObject *Sender);
	void __fastcall TouchBtn4MouseClick(TObject *Sender);
	void __fastcall TouchBtn5MouseClick(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
	void __fastcall TouchBtn2MouseClick(TObject *Sender);
	void __fastcall TouchBtn3MouseClick(TObject *Sender);
	void __fastcall BreakdownCatMouseclick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmPrinterControlPanel(TComponent* Owner,Database::TDBControl &inDBControl);
	
	Database::TDBControl &DBControl;
public:		// User declarations
	void TfrmPrinterControlPanel::UpdateDisplay();
	bool TfrmPrinterControlPanel::PrintingByCourse();
	bool TfrmPrinterControlPanel::PrintingByCat();
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
