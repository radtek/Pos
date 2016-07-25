//---------------------------------------------------------------------------

#ifndef PalmSetupH
#define PalmSetupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include "CPortCtl.hpp"
#include <Dialogs.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmPalmSetup : public TForm
{
__published:	// IDE-managed Components
	TPanel *GroupBox3;
	TComboBox *cbReceiptPrinter;
	TLabel *Label1;
	TTouchBtn *btnClose;
	TTouchBtn *TouchButton2;
   TGroupBox *GroupBox1;
   TPanel *Panel2;
   TCheckBox *btnBlueToothOn;
	TComComboBox *ComcbBlueTooth;
	TCheckBox *cbOnLineLogging;
	TTouchBtn *btnHappyHour;
	TColorDialog *ColorDialog;
	TTouchBtn *tbChefMateIP;
	TTouchBtn *TouchButton1;
	TCheckBox *cbRememberLastServingCourse;
	TCheckBox *chkLogGetOrders;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall cbReceiptPrinterChange(TObject *Sender);
	void __fastcall cbReceiptPrinterDropDown(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
   void __fastcall tbtnChangeServer(TObject *Sender);
   void __fastcall btnBlueToothOnClick(TObject *Sender);
   void __fastcall comcbBlueToothChange(TObject *Sender);
	void __fastcall cbOnLineLoggingClick(TObject *Sender);
	void __fastcall ComcbBlueToothClick(TObject *Sender);
	void __fastcall btnHappyHourClick(TObject *Sender);
	void __fastcall tbChefMateColourClick(TObject *Sender);
	void __fastcall tbtnSeatName(TObject *Sender);
	void __fastcall cbRememberLastServingCourseClick(TObject *Sender);
	void __fastcall chkLogGetOrdersClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPalmSetup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPalmSetup *frmPalmSetup;
//---------------------------------------------------------------------------
#endif
