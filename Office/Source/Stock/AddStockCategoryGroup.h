//---------------------------------------------------------------------------

#ifndef AddStockCategoryGroupH
#define AddStockCategoryGroupH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
//---------------------------------------------------------------------------
enum TStockGroupType { gtCategory, gtGroup };
//---------------------------------------------------------------------------
class TfrmAddStockCategoryGroup : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TEdit *edName;
	TEdit *edGLCode;
	TLabel *lbeCategoryGroup;
	TLabel *Label1;
	TBitBtn *BitBtn2;
	TIBSQL *qrUpdateInvoiceGLCode;
	TIBTransaction *Transaction;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmAddStockCategoryGroup(TComponent* Owner);
	TStockGroupType StockGroupType;
	AnsiString Name, GLCode;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmAddStockCategoryGroup *frmAddStockCategoryGroup;
//---------------------------------------------------------------------------
#endif
