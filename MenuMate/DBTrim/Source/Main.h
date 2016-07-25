//---------------------------------------------------------------------------

#ifndef MainH
#define MainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchControls.h"
#include "TouchGrid.h"
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TrimUtil.h"
#include <ComCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TPanel *Panel2;
   TLabel *Label1;
   TTouchGrid *tgYear;
   TTouchGrid *tgMonth;
   TTouchGrid *tgDay;
   TTouchBtn *TouchBtn1;
   TTouchBtn *TouchBtn2;
   TStatusBar *StatusBar1;
   TTimer *Timer;
   TTouchBtn *TouchBtn3;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall TouchBtn1MouseClick(TObject *Sender);
   void __fastcall tgMonthMouseClick(TObject *Sender, TMouseButton Button,
          TShiftState Shift, TGridButton *GridButton);
   void __fastcall TouchBtn2MouseClick(TObject *Sender);
   void __fastcall FormDestroy(TObject *Sender);
   void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
   void __fastcall StatusBar1DrawPanel(TStatusBar *StatusBar,
          TStatusPanel *Panel, const TRect &Rect);
   void __fastcall TimerTimer(TObject *Sender);
   void __fastcall TouchBtn3MouseClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmMain(TComponent* Owner);
   void DrawCalendar();
	void __fastcall AppMessage(tagMSG &Msg, bool &Handled);   
   TDBTrimUtil *DBTrimUtil;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
