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
#include "TouchButton.h"
#include <Graphics.hpp>
//---------------------------------------------------------------------------
class TfrmMain : public TForm
{
__published:	// IDE-managed Components
	TTimer *tmLogs;
	TPanel *Panel1;
	TTouchButton *btnChangeDrinks;
	TLabel *Label1;
	TPanel *Panel2;
	TTouchButton *btnMinimise;
	TLabel *Label2;
	TTouchButton *btnChangeFood;
   TPanel *Panel3;
   TTouchButton *btnSetup;
   TTouchButton *btnRegisterPalms;
   TPanel *pnlExit;
   TImage *imgExit;
   TLabel *Label3;
   TLabel *Label4;
	TLabel *lbeVersion;
   void __fastcall FormCreate(TObject *Sender);
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
	void __fastcall FormDestroy(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
public:		// User declarations
   __fastcall TfrmMain(TComponent* Owner);
   bool CheckRegistered();
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
