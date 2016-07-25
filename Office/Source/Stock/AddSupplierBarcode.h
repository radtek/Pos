//---------------------------------------------------------------------------

#ifndef AddSupplierBarcodeH
#define AddSupplierBarcodeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
//---------------------------------------------------------------------------
class TfrmAddSupplierBarcode : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TBitBtn *btnCancel;
	TBitBtn *BitBtn1;
	TDBGrid *dbgSupplierUnits;
	TDataSource *dsSupplierUnits;
	void __fastcall dbgSupplierUnitsDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAddSupplierBarcode(TComponent* Owner);
   void SetDataSet(TDataSet *theDataSet);

};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddSupplierBarcode *frmAddSupplierBarcode;
//---------------------------------------------------------------------------
#endif
