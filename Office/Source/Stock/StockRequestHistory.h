//---------------------------------------------------------------------------

#ifndef StockRequestHistoryH
#define StockRequestHistoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Db.hpp>
#include <DBCtrls.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <Buttons.hpp>
#include <IBDatabase.hpp>
#include <DB.hpp>
#include <ComCtrls.hpp>
#include <Dialogs.hpp>
#include <memory>
#include "ReceiveStockItem.h"
#include "ReceiveInvoice.h"
#include "StockRequest.h"
//---------------------------------------------------------------------------
class TfrmStockRequestHistory : public TForm
{
    __published:
        TIBTransaction *Transaction;
        TIBQuery *qrStockRequest;
        TDataSource *dsStockRequest;
        TIBQuery *qrStockRequestDetails;
        TDataSource *dsStockRequestDetails;
        TPanel *Panel2;
        TBitBtn *btnUpdateStockRequest;
        TBitBtn *btnNewStockRequest;
        TBitBtn *btnClose;
        TBitBtn *btnFind;
        TFindDialog *FindDialog;
        TPanel *Panel8;
        TPanel *Panel9;
        TLabel *lbeTitle;
        TDBGrid *dbgDetails;
        TTreeView *tvStockRequests;
        TPanel *Panel1;
        TLabel *lbeReference;
   TBitBtn *btnDeleteStockRequest;
   TIBQuery *qrUpdateStockRequest;

        void __fastcall FormShow(TObject *Sender);
        void __fastcall btnCloseClick(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall btnNewStockRequestClick(TObject *Sender);
        void __fastcall btnUpdateStockRequestClick(TObject *Sender);
        void __fastcall tvStockRequestsChange(TObject *Sender, TTreeNode *Node);
        void __fastcall FindDialogFind(TObject *Sender);
        void __fastcall btnFindClick(TObject *Sender);
        void __fastcall tvStockRequestsDblClick(TObject *Sender);
        void __fastcall tvStockRequestsKeyPress(TObject *Sender, char &Key);
   void __fastcall btnDeleteStockRequestClick(TObject *Sender);

    protected:
        void __fastcall WMDisplayChange(TWMDisplayChange& Message);
        BEGIN_MESSAGE_MAP
            MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
        END_MESSAGE_MAP(TComponent)

    private:
        void ResizeGrids();
        void LoadStockRequestsIntoTree();

        const std::auto_ptr<TfrmStockRequest> frmStockRequest;

    public:		
        __fastcall TfrmStockRequestHistory(TComponent* Owner);
};

//---------------------------------------------------------------------------
extern PACKAGE TfrmStockRequestHistory *frmStockRequestHistory;
//---------------------------------------------------------------------------

#endif
