//---------------------------------------------------------------------------

#ifndef SelectActiveMenusH
#define SelectActiveMenusH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchButton.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmSelectActiveMenus : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel3;
	TPanel *Panel1;
	TTouchButton *btnOk;
	TTouchButton *btnCancel;
	TPanel *pnlMenus;
	TTouchButton *btnNone;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnNoneClick(TObject *Sender);
	void __fastcall FormDestroy(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void __fastcall MenuClick(TObject *Sender);
	bool MenuIsActive(int MenuKey);
public:		// User declarations
	__fastcall TfrmSelectActiveMenus(TComponent* Owner);
	AnsiString MenuName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectActiveMenus *frmSelectActiveMenus;
//---------------------------------------------------------------------------
#endif

