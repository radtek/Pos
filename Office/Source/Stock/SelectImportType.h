//---------------------------------------------------------------------------

#ifndef ImportStockH
#define ImportStockH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmSelectImportType : public TForm
{
__published:	// IDE-managed Components
   TButton *btnImportStock;
   TButton *btnImportSupplier;
   TPanel *Panel1;
   void __fastcall btnImportStockClick(TObject *Sender);
   void __fastcall btnImportSupplierClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmSelectImportType(TComponent* Owner);
   bool IsImportStock;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSelectImportType *frmSelectImportType;
//---------------------------------------------------------------------------
#endif
