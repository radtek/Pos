//---------------------------------------------------------------------------

#ifndef SelectSupplierH
#define SelectSupplierH
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


class TSelectSupplierNodeData
{
public:
       AnsiString CompanyName;
       int ContactKey;
        int StockKey;
};

class TfrmSelectSupplier : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *btnFind;
	TDBGrid *dbgResult;
	TBitBtn *btnCancel;
	TBitBtn *btnOk;
	TIBQuery *qrContact;
	TDataSource *dsContact;
	TIBTransaction *Transaction;
	TPanel *Panel1;
	TLabel *Label1;
	TBitBtn *btnGo;
	TEdit *edSearch;
	TLabel *Label2;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnGoClick(TObject *Sender);
	void __fastcall dbgResultDblClick(TObject *Sender);
	void __fastcall dbgResultKeyPress(TObject *Sender, char &Key);
	void __fastcall edSearchKeyPress(TObject *Sender, char &Key);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall dbgResultKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall edSearchChange(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CMDialogChar(TWMKey &Msg);

	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
		MESSAGE_HANDLER(CM_DIALOGCHAR, TWMKey, CMDialogChar)
	END_MESSAGE_MAP(TComponent)

private:	// User declarations
	void ResizeGrids();
public:		// User declarations
	__fastcall TfrmSelectSupplier(TComponent* Owner);
	bool Reset;
	AnsiString CompanyName;
	int ContactKey;
 TList *SelectedSupplierList;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSelectSupplier *frmSelectSupplier;
//---------------------------------------------------------------------------
#endif
