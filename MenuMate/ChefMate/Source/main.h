//---------------------------------------------------------------------------

#ifndef mainH
#define mainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Grids.hpp>
#include <Menus.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>

#include "KitchenMod.h"
#include "MMBarcodeReader.h"
#include "Enum.h"
#include <ComCtrls.hpp>
#include "MM_DBCore.h"
#include "TouchControls.h"
#include "TouchPages.h"
#include "TouchBtn.h"
#include "touchpages.h"
#include "TouchGrid.h"
#include "TouchNumpad.h"
#include "ManagerVariable.h"
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel3;
	TPanel *Panel2;
	TLabel *Label1;
	TLabel *AvgSales;
	TLabel *Label5;
	TLabel *AvgMake;
	TLabel *Label7;
	TLabel *LongestMake;
	TLabel *Label9;
	TTimer *RefreshTimer;
	TTimer *tiCardSwipe;
	TFontDialog *Fonts;
	TTouchPages *tpDisplays;
	TTouchSheet *tsCurrentOrders;
	TTouchSheet *tsAgedOrders;
	TTouchBtn *tbtnConfig;
   TSplitter *Splitter3;
   TSplitter *Splitter1;
   TSplitter *Splitter2;
   TSplitter *SplitterAged1;
   TSplitter *SplitterAged2;
   TSplitter *SplitterAged3;
	TRichEdit *Display4;
	TRichEdit *Display3;
	TRichEdit *Display1;
	TRichEdit *Display2;
	TRichEdit *AgedDisplay4;
	TRichEdit *AgedDisplay3;
	TRichEdit *AgedDisplay2;
	TRichEdit *AgedDisplay1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall RefreshTimerTimer(TObject *Sender);
	void __fastcall tbtnConfigClick(TObject *Sender);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
			 TShiftState Shift);
	void __fastcall tiCardSwipeTimer(TObject *Sender);
	void __fastcall Display1MouseUp(TObject *Sender, TMouseButton Button,
			 TShiftState Shift, int X, int Y);
	void __fastcall tpDisplaysChange(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall Splitter1Moved(TObject *Sender);
   void __fastcall Splitter2Moved(TObject *Sender);
   void __fastcall Splitter3Moved(TObject *Sender);
   void __fastcall SplitterAged1Moved(TObject *Sender);
   void __fastcall SplitterAged2Moved(TObject *Sender);
   void __fastcall SplitterAged3Moved(TObject *Sender);
private:	// User declarations
	void __fastcall CardSwipe(TMessage& Message);
protected:
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(UWM_CARDSWIPE, TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)
public:		// User declarations
	bool OpenDB(bool ShowConfig = true);
	void CloseDB();
	
	Database::TDBControl DBControl;
	std::auto_ptr<TKitchenMod> Kitchen;
	std::auto_ptr<TMMBarcodeReader> BarCodeReader;

	void __fastcall AppMessage(tagMSG &Msg, bool &Handled);
	__fastcall TfrmMain(TComponent* Owner);
	AnsiString CardSwipeString;
	bool ConfirmBeforeBumping;
	void ConfigSplitters();
    TManagerVariable *VM;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
