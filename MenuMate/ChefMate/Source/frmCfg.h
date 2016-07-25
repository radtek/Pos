//---------------------------------------------------------------------------

#ifndef frmCfgH
#define frmCfgH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include "Enum.h"
#include "MM_DBCore.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "touchbtn.h"
#include "touchcontrols.h"
#include "ManagerVariable.h"
//---------------------------------------------------------------------------
class TfrmConfig : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *lbeVersion;
	TPanel *Panel2;
	TLabel *Label1;
	TTouchBtn *TouchBtn2;
	TTouchBtn *TouchBtn1;
	TTouchBtn *btnMon;
	TTouchBtn *tbExit;
	TTouchBtn *TouchBtn5;
	void __fastcall tbConfigureFontSizeClick(TObject *Sender);
	void __fastcall tbExitClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnMonClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall TouchButton4Click(TObject *Sender);
	void __fastcall tbReconfigureClick(TObject *Sender);
	void __fastcall TouchBtn2MouseClick(TObject *Sender);
private:	// User declarations
	void __fastcall CardSwipe(TMessage& Message);
	Database::TDBControl &DBControl;
    TManagerVariable &Vars;
protected:
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(UWM_CARDSWIPE, TMessage, CardSwipe);
	END_MESSAGE_MAP(TComponent)

public:		// User declarations
	__fastcall TfrmConfig(TComponent* Owner,Database::TDBControl &inDBControl, TManagerVariable &inVars);
   int MyMonitorNum;
	int MainsMonitorNum;
	UnicodeString CardSwipeString;
};
//---------------------------------------------------------------------------
#endif
