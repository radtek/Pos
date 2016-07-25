//---------------------------------------------------------------------------

#ifndef PrinterRedirectH
#define PrinterRedirectH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "MM_DBCore.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmPrinterRedirect : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *lbCaption;
	TPanel *pnlButtons;
	TTouchBtn *tbCancel;
	TListBox *lbPrinters;
	TTouchBtn *TouchBtn1;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall tbCancelClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
	__fastcall TfrmPrinterRedirect(TComponent* Owner,Database::TDBTransaction &inDBTransaction);
	Database::TDBTransaction &DBTransaction;
public:		// User declarations

	int SelectedPrinterKey;
	void GetPrinterList();
	bool ShowAll;
};
//---------------------------------------------------------------------------
#endif
