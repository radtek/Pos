//---------------------------------------------------------------------------

#ifndef palmmainH
#define palmmainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "Trayicon.h"
#include <ImgList.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ActnList.hpp>
#include <Menus.hpp>
#include <CheckLst.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <Graphics.hpp>
#include "CPort.hpp"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <IdBaseComponent.hpp>
#include <IdComponent.hpp>
#include <IdUDPBase.hpp>
#include <IdUDPServer.hpp>
#include <IdIPWatch.hpp>
#include <IdSNTP.hpp>
#include <IdUDPClient.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TTimer *tmLogs;
	TPanel *Panel1;
	TTouchBtn *btnChangeDrinks;
	TLabel *Label1;
	TPanel *Panel2;
	TTouchBtn *btnMinimise;
	TLabel *Label2;
	TTouchBtn *btnChangeFood;
   TPanel *Panel3;
	TTouchBtn *btnSetup;
	TTouchBtn *btnRegisterPalms;
   TLabel *Label3;
   TLabel *Label4;
	TTouchBtn *btnHappyHour;
   TTimer *tiMenuUpdateRequired;
	TTouchBtn *TouchBtn1;
	TLabel *lbeVersion;
   void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tmLogsTimer(TObject *Sender);
	void __fastcall btnChangeDrinksClick(TObject *Sender);
	void __fastcall btnChangeFoodClick(TObject *Sender);
	void __fastcall btnRegisterPalmsClick(TObject *Sender);
	void __fastcall btnSetupClick(TObject *Sender);
	void __fastcall btnQuitClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnMinimiseClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
	void __fastcall lbeVersionClick(TObject *Sender);
	void __fastcall btnHappyHourClick(TObject *Sender);
   void __fastcall tiMenuUpdateRequiredTimer(TObject *Sender);
	void __fastcall tchbtn1MouseClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
public:		// User declarations
   __fastcall TfrmMain(TComponent* Owner);
   bool RequestClose;
   int SomeStrings(TStrings *Strings);
   bool OpenDB();
   void Init();
	virtual __fastcall ~TfrmMain();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMain *frmMain;
//---------------------------------------------------------------------------
#endif
