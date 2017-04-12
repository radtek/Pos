//---------------------------------------------------------------------------

#ifndef AddSalesTypeH
#define AddSalesTypeH
//---------------------------------------------------------------------------
#include "DBSalesTypeAssignment.h"
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmAddSalesType : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel1;
    TTouchBtn *btnName;
    TTouchBtn *btnCode;
    TTouchBtn *btnOk;
    TTouchBtn *btnCancel;
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall AddSalesTypeDetails(TObject *Sender);

private:	// User declarations
    UnicodeString SalesTypeInfoPointers[2];
    void DisplaySalesTypeInfoFromPointers();
    UnicodeString salesCode;
    UnicodeString salesTypeName;
public:		// User declarations
    __fastcall TfrmAddSalesType(TComponent* Owner);
    int SalesTypeId;
	bool Editing;
};
//---------------------------------------------------------------------------
#endif
