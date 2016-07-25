//---------------------------------------------------------------------------

#ifndef DelayedPaymentTabsH
#define DelayedPaymentTabsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <Grids.hpp>
#include "ZForm.h"
#include <list>
#include "DBTransactionAudit.h"
//---------------------------------------------------------------------------
class TfrmDelayedPaymentTabs : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    TPanel *pnlTransactions;
        TTouchBtn *btnSelect;
    TStringGrid *gridTransactions;
    TPanel *pnlButtons;
        TTouchBtn *tbtnCancel;
        TTouchBtn *btnScrollUp;
        TTouchBtn *btnScrollDown;
    void __fastcall btnSelectMouseClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall gridTransactionsDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall tbtnCancelMouseClick(TObject *Sender);
        void __fastcall btnScrollDownMouseClick(TObject *Sender);
        void __fastcall btnScrollUpMouseClick(TObject *Sender);
private:	// User declarations
    void SetColumnWidths();
    void LoadGridHeaders();
    void DrawCellText(unsigned &Flags, TStringGrid *StringGrid, int ACol, int ARow, TRect &Rect, TGridDrawState State);
    std::list<TDelayedPaymentData> transactionList;
    void RefreshDisplay();
public:		// User declarations
        __fastcall TfrmDelayedPaymentTabs(TComponent* Owner);
        int SelectedTabKey;
        AnsiString SelectedTabName;
        AnsiString DelayedInvoiceNumber;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmDelayedPaymentTabs *frmDelayedPaymentTabs;
//---------------------------------------------------------------------------
#endif
