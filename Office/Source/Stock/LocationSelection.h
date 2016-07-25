//---------------------------------------------------------------------------

#ifndef LocationSelectionH
#define LocationSelectionH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DB.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>

//---------------------------------------------------------------------------
//const UWM_REFRESH_STOCK_GROUP = WM_USER + 2;
//enum TStockMode { smEdit, smSelect };
class TfrmLocationSelection : public TForm
{
__published:	// IDE-managed Components
    TPanel *Panel3;
    TPanel *Panel1;
    TPanel *Panel10;
    TBitBtn *btnClose;
    TPanel *Panel2;
    TPanel *Panel8;
    TPanel *Panel9;
    TLabel *lbeTitle;
    TListBox *lstLocation;
    TBitBtn *btnOk;
    TBitBtn *btnCancel;
    TIBTransaction *Transaction;
    TTimer *tiTreeScroll;
    TFindDialog *FindDialog;
    TIBSQL *qrStockCat;
    TIBSQL *qrStockLocation;
    TIBTransaction *Transaction1;
    TIBSQL *sqlLocation;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);
    void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
void LoadTree();

public:		// User declarations

        AnsiString Location;
  	 
	AnsiString	NewItemName;
TStringList *SelectedLocationList;
 int ContactKey;
  

    __fastcall TfrmLocationSelection(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLocationSelection *frmLocationSelection;
//---------------------------------------------------------------------------
#endif
