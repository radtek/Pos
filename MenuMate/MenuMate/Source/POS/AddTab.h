//---------------------------------------------------------------------------
#ifndef AddTabH
#define AddTabH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <ComCtrls.hpp>
#include "enum.h"
#include "MM_DBCore.h"
#include "Modules.h"
#include "TouchBtn.h"
#include "TouchControls.h"

//---------------------------------------------------------------------------

class TfrmAddTab : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *lbeName;
	TLabel *lbeType;
	TLabel *lbeNumber;
	TLabel *lbeDate;
	TTouchBtn *btnName;
	TTouchBtn *btnType;
	TTouchBtn *btnNumber;
	TTouchBtn *btnDate;
	TTouchBtn *btnNewTabCancel;
	TTouchBtn *btnAddTab;
	TTouchBtn *btnRecordCard;
	TPanel *Panel2;
	TLabel *Label2;
	TPanel *Panel7;
	TTouchBtn *btn7;
	TTouchBtn *btn8;
	TTouchBtn *btn9;
	TTouchBtn *btn6;
	TTouchBtn *btn5;
	TTouchBtn *btn4;
	TTouchBtn *btn1;
	TTouchBtn *btn0;
	TTouchBtn *btnCLR;
	TTouchBtn *btn2;
	TTouchBtn *btn3;
	TPanel *Panel3;
	TLabel *Label1;
	TLabel *Label3;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnNewTabCancelClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall btnNameClick(TObject *Sender);
	void __fastcall btnAddTabClick(TObject *Sender);
	void __fastcall NumberBtnMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnCLRMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnRecordCardClick(TObject *Sender);
private:	// User declarations
	static TForm *WinOwner;
	void __fastcall CreateParams(Controls::TCreateParams &params);
	__fastcall TfrmAddTab(TComponent* Owner);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)

public:		// User declarations
	static TfrmAddTab *Create(TForm* Owner);


	void __fastcall LoadDetails(Database::TDBTransaction &DBTransaction,int inTabKey);
   bool AllowCancel;
   int TabKey;
   UnicodeString TabName;
   UnicodeString TabIDType;
   UnicodeString TabIDNumber;
   UnicodeString TabIDExpiry;
   UnicodeString CardString;

   //MM2038
   UnicodeString IdName;
   //the variable here is following the name in database itself (get permission to use this one not creating the new one)
   UnicodeString IdNumber;
};
//---------------------------------------------------------------------------
#endif
