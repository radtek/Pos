//---------------------------------------------------------------------------

#ifndef SelectPurchaseOrderH
#define SelectPurchaseOrderH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <Db.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <ExtCtrls.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TfrmSelectPurchaseOrder : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *BitBtn2;
	TDBGrid *dbgResult;
	TBitBtn *btnCancel;
	TBitBtn *btnOk;
	TIBQuery *qrOrder;
	TDataSource *dsOrder;
	TIBTransaction *Transaction;
	TPanel *Panel1;
	TLabel *Label2;
	TLabel *Label3;
	TEdit *edSearch;
	TBitBtn *btnGo;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnGoClick(TObject *Sender);
	void __fastcall edSearchKeyPress(TObject *Sender, char &Key);
	void __fastcall dbgResultDblClick(TObject *Sender);
	void __fastcall dbgResultKeyPress(TObject *Sender, char &Key);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormKeyUp(TObject *Sender, WORD &Key, TShiftState Shift);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void ResizeGrids();
public:		// User declarations
	__fastcall TfrmSelectPurchaseOrder(TComponent* Owner);
	bool Reset;
	AnsiString OrderNumber;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectPurchaseOrder *frmSelectPurchaseOrder;
//---------------------------------------------------------------------------
#endif
