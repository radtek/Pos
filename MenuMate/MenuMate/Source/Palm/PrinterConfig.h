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
	TListBox *lbTopMargin;
	TTouchButton *btnClose;
	TCheckBox *cbCarbonCopy;
	TTouchButton *btnTestPrint;
	TCheckBox *chbAlwaysFullCut;
   TCheckBox *cbPrintNoteWithDiscount;
	TGroupBox *GroupBox5;
	TLabel *Label10;
	TLabel *Label15;
	TLabel *Label16;
	TLabel *Label18;
	TLabel *Label19;
	TLabel *Label21;
	TEdit *edSideBullet;
	TEdit *edOptionsBullet;
	TEdit *edNoteHeader;
	TEdit *edSidesHeader;
	TEdit *edMixHeader;
	TEdit *edPerSeatMultiplier;
	TCheckBox *cbUnravelKitchenPrinting;
	TCheckBox *cbKitchenFont;
	TCheckBox *cbCourseHeader;
	TGroupBox *GroupBox1;
	TRadioButton *rbCutByCourse;
	TRadioButton *rbCutByNone;
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
	void __fastcall cbUnravelKitchenPrintingClick(TObject *Sender);
	void __fastcall edSideBulletMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edOptionsBulletMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall edPerSeatMultiplierMouseUp(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall edNoteHeaderMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edSidesHeaderMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall edMixHeaderMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
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
