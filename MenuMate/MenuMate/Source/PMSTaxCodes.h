//---------------------------------------------------------------------------

#ifndef PMSTaxCodesH
#define PMSTaxCodesH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <Message.h>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmTaxCodes : public TZForm
{
    friend TZForm;
    __published:	// IDE-managed Components
    TTouchBtn *btnClose;
    TTouchBtn *btnDelete;
    TTouchBtn *btnEdit;
    TTouchBtn *btnAdd;
    TPanel *pnlSelection;
    TPanel *pnlLabel;
    TPanel *pnlMain;
    TPanel *pnlConfiguration;
    TStringGrid *sgDisplay;

    private:	// User declarations
    public:		// User declarations
	Database::TDBControl &DBControl;
    __fastcall TfrmTaxCodes(TComponent* Owner,Database::TDBControl &inDBControl);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTaxCodes *frmTaxCodes;
//---------------------------------------------------------------------------
#endif
