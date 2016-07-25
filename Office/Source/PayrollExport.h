//---------------------------------------------------------------------------

#ifndef PayrollExportH
#define PayrollExportH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmPayrollExport : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *btnExport;
	TBitBtn *BitBtn1;
	void __fastcall BitBtn1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPayrollExport(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmPayrollExport *frmPayrollExport;
//---------------------------------------------------------------------------
#endif
