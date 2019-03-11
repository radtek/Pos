//---------------------------------------------------------------------------
#ifndef MainH
#define MainH
#include <Classes.hpp>
#include <Controls.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include <StdCtrls.hpp>
#include <Buttons.hpp>
#include <jpeg.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "pngimage.hpp"
#include "touchbtn.h"
#include "touchcontrols.h"
#include <IBSQL.hpp>


//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TImage *Image1;
	TLabel *Label2;
	TLabel *lbeVersion;
	TImage *Image2;
	TTouchBtn *btnEditMenus;
	TTouchBtn *btnReports;
	TTouchBtn *btnStock;
	TTouchBtn *btnLoyalty;
	TTouchBtn *btnBackup;
	TTouchBtn *btnWages;
	TTouchBtn *btnSetup;
	TTouchBtn *btnExit;
	TTouchBtn *btnCashup;
    TIBSQL *qrComflag;
    TTimer *tiFailedXeroInvoices;
  void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall btnReportsMouseClick(TObject *Sender);
	void __fastcall btnEditMenusMouseClick(TObject *Sender);
	void __fastcall btnStockMouseClick(TObject *Sender);
	void __fastcall btnLoyaltyMouseClick(TObject *Sender);
	void __fastcall btnBackupMouseClick(TObject *Sender);
	void __fastcall btnSetupMouseClick(TObject *Sender);
	void __fastcall btnWagesMouseClick(TObject *Sender);
	void __fastcall btnExitMouseClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall btnCashupMouseClick(TObject *Sender);
    void __fastcall tiFailedXeroInvoicesTimer(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	TPanel *Panels[9];
	bool JustBooted;
    AnsiString DefaultCompany;
//	AnsiString PayrollKey;

//	bool GetUserPeriods(TDateTime *Periods);
//	TDateTime GetEndOfTime(TDateTime ClockInTime,TDateTime ClockOutTime,TDateTime *Periods);
//	int GetPaymentCatagoryIndex(TDateTime ClockInTime, TDateTime *Periods);
//	bool TfrmMain::CheckRegistration();

   void       InitXeroIntegration();
   AnsiString LocalHost();
   AnsiString ReadXeroFolderPath();
   AnsiString ReadCompanyName();
   bool       FailedXeroInvoivesToSend();
   void       SendFailedXeroInvoices();
   void       ResetFailedXeroInvoiceTimerInterval( unsigned inInvoiceCount );
   bool       IsDisplayOfficePath();
   bool       IsPosRegistered();
   void       IsSyncRequired();

public:		// User declarations
  __fastcall TfrmMain(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
