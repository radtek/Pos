//---------------------------------------------------------------------------

#ifndef SelectCurrentMenuH
#define SelectCurrentMenuH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmSelectCurrentMenu : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel3;
	TPanel *pnlMenus;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall tbtnCancelClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmSelectCurrentMenu(TComponent* Owner);
public:		// User declarations

	bool MenuIsCurrent(int MenuKey);
	void __fastcall MenuClick(TObject *Sender);
   int SelectedMenuIndex;      
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectCurrentMenu *frmSelectCurrentMenu;
//---------------------------------------------------------------------------
#endif
