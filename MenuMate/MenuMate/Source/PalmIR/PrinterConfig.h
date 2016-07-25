//---------------------------------------------------------------------------

#ifndef PrinterConfigH
#define PrinterConfigH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchButton.h"
//---------------------------------------------------------------------------
class TfrmPrinterConfiguration : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label9;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *laCutterGap;
	TLabel *Label14;
	TListBox *lbPrinters;
	TListBox *lbLargeFnt;
	TListBox *lbNormalFnt;
	TListBox *lbCutterGap;
	TCheckBox *cbKitchenFont;
	TListBox *lbTopMargin;
	TTouchButton *btnClose;
	TCheckBox *cbCourseHeader;
	TCheckBox *cbCarbonCopy;
	TGroupBox *GroupBox1;
	TRadioButton *rbCutByCourse;
	TRadioButton *rbCutByNone;
	TTouchButton *btnTestPrint;
	TCheckBox *chbAlwaysFullCut;
   TCheckBox *cbPrintNoteWithDiscount;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall lbPrintersClick(TObject *Sender);
	void __fastcall lbNormalFntClick(TObject *Sender);
	void __fastcall lbLargeFntClick(TObject *Sender);
	void __fastcall lbCutterGapClick(TObject *Sender);
	void __fastcall lbTopMarginClick(TObject *Sender);
	void __fastcall cbCarbonCopyClick(TObject *Sender);
	void __fastcall cbPrintNoteWithDiscountClick(TObject *Sender);
	void __fastcall cbKitchenFontClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall rbCutByNoneClick(TObject *Sender);
	void __fastcall cbCourseHeaderClick(TObject *Sender);
	void __fastcall rbCutByCourseClick(TObject *Sender);
	void __fastcall btnTestPrintClick(TObject *Sender);
	void __fastcall chbAlwaysFullCutClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPrinterConfiguration(TComponent* Owner);
   TList *PrinterList;
	void UpdateLists();      
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPrinterConfiguration *frmPrinterConfiguration;
//---------------------------------------------------------------------------
#endif
