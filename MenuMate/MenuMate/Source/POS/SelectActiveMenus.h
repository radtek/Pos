//---------------------------------------------------------------------------

#ifndef SelectActiveMenusH
#define SelectActiveMenusH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include "MM_DBCore.h"
#include "ManagerMenus.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmSelectActiveMenus : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel3;
	TPanel *Panel1;
	TPanel *pnlMenus;
	TTouchBtn *btnMinimise;
	TTouchBtn *TouchBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnMinimiseMouseClick(TObject *Sender);
	void __fastcall TouchBtn1MouseClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmSelectActiveMenus(TComponent* Owner,TManagerMenus *inManagerMenus,Database::TDBTransaction &DBTransaction);

	static TForm *WinOwner;
	void __fastcall CreateParams(Controls::TCreateParams &params);

	void __fastcall MenuClick(TObject *Sender);
	bool MenuIsActive(int MenuKey);
	Database::TDBTransaction &DBTransaction;
	TManagerMenus *ManagerMenus;
public:		// User declarations
	TfrmSelectActiveMenus static *Create(TForm* Owner,TManagerMenus *inManagerMenus,Database::TDBTransaction &inDBTransaction);

};
//---------------------------------------------------------------------------
#endif

