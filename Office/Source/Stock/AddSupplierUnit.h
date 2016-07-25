//---------------------------------------------------------------------------

#ifndef AddSupplierUnitH
#define AddSupplierUnitH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DB.hpp>
#include <ExtCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBDatabase.hpp>
#include <Buttons.hpp>
//---------------------------------------------------------------------------
class TfrmAddSupplierUnit : public TForm
{
__published:	// IDE-managed Components
	TIBQuery *qrSupplierUnits;
	TIBTransaction *Transaction;
	TPanel *Panel1;
	TPanel *Panel8;
	TPanel *Panel2;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TGroupBox *GroupBox1;
	TLabel *Label1;
	TLabel *Label2;
	TComboBox *cbUnits;
	TLabel *lbeTitle;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAddSupplierUnit(TComponent* Owner);
	AnsiString SupplierName;
	AnsiString ItemName;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmAddSupplierUnit *frmAddSupplierUnit;
//---------------------------------------------------------------------------
#endif
