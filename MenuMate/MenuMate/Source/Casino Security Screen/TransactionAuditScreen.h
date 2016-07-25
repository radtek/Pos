//---------------------------------------------------------------------------

#ifndef TransactionAuditScreenH
#define TransactionAuditScreenH
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
#include "MMSecurityScreenManager.h"
#include "MMSecurityScreenSettings.h"
#include <list>
//---------------------------------------------------------------------------
const int NUMBER_OF_TRANSACTIONS_STORED = 5000;
//---------------------------------------------------------------------------
class TfrmTransactionAuditScreen : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
    TPanel *pnlTransactions;
    TTouchBtn *btnClose;
    TStringGrid *gridTransactions;
    TPanel *pnlButtons;
    TTouchBtn *tbtnFilters;
    void __fastcall btnCloseMouseClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormDestroy(TObject *Sender);
    void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
    void __fastcall gridTransactionsDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
    void __fastcall gridTransactionsDblClick(TObject *Sender);
    void __fastcall tbtnFiltersAutoRepeat(TObject *Sender);


private:	// User declarations
    TMMSecurityScreenManager *securityScreenManager;
    TMMSecurityEventResponse event;

    std::list<TSecurityTransactionData> transactionList;
    ALL_AUDIT_FILTERS audit_filters;

    void setColumnWidths(double widthScale);
    void loadGridHeaders();
    void __fastcall SecurityManagerUpdateHandler( System::TObject *inSender, TSecurityTransactionData* inData );

    void drawCellText(unsigned &Flags, TStringGrid *StringGrid, int ACol, int ARow, TRect &Rect, TGridDrawState State);

    void refreshDisplay();
    bool recordPassesTheFilters(        TSecurityTransactionData* inRecord );
    bool recordPassesLocationFilter(    TSecurityTransactionData* inRecord, AUDIT_FILTER_LIST inFilterList );
    bool recordPassesTerminalFilter(    TSecurityTransactionData* inRecord, AUDIT_FILTER_LIST inFilterList );
    bool recordPassesCashierFilter(     TSecurityTransactionData* inRecord, AUDIT_FILTER_LIST inFilterList );
    bool recordPassesPaymentTypeFilter( TSecurityTransactionData* inRecord, AUDIT_FILTER_LIST inFilterList );

    //::::::::::::::::::::::::::::::::::::::::::::::::
    // Settings
    //::::::::::::::::::::::::::::::::::::::::::::::::
    void showSecurityScreenSettings();

public:		// User declarations
    __fastcall TfrmTransactionAuditScreen(TComponent* Owner);

    void Reset();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmTransactionAuditScreen *frmTransactionAuditScreen;
//---------------------------------------------------------------------------
#endif
