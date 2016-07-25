//---------------------------------------------------------------------------

#ifndef TransferMultiH
#define TransferMultiH
#define PreviousTransfer "History"
#define CurrentTransfer "Current"
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "NumericEdit.h"
#include "VirtualTrees.hpp"
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Dialogs.hpp>
#include <ComCtrls.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <IBSQL.hpp>
//---------------------------------------------------------------------------
struct TStockNodeData
{
	WideString	Text;
	int			Key;
    int StockRequestKey;
	AnsiString	Unit;

	double		SourceOnHand;
	double		DestOnHand;

	double		Quantity;

	bool			Initialised;
	TDateTime	InitialisedTime;


    double Transfer_Real_Quantity;

};




//---------------------------------------------------------------------------
const WM_LOADTRANSFER = WM_USER + 7;
class TfrmTransferMulti : public TForm
{
__published:	// IDE-managed Components
	TIBQuery *qrStockLoc1Old;
        TIBQuery *qrGetTransferStock;
	TDataSource *dsTransferedStock;
        TDBGrid *dbgTransferedStock; 
	TDataSource *dsStockLoc1;
	TIBQuery *qrStockLoc2Old;
	TDataSource *dsStockLoc2;
	TIBTransaction *Transaction;
	TIBQuery *qrUsersOld;
        TIBQuery *qrGetPreviousTransfers;
	TVirtualStringTree *vtvStockQty;
	TNumericEdit *neStockQty;
        TTreeView *tvTransfers;
	TIBQuery *qrStock;
	TPanel *Panel1;
        TPanel *Panel2;
        TPanel *Panel9;
	TLabel *Label5;
	TLabel *lbeFrom;
	TLabel *lbeTo;
	TLabel *Label3;

	TLabel *lbeTitle;
    TLabel *lbeTransferNumber;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TBitBtn *btnFind;
	TFindDialog *FindDialog;
	TBitBtn *btnScan;
	TIBQuery *qrFindBarcode;
	TDateTimePicker *dtpTransferDate;
	TLabel *Label1;
	TEdit *edReference;
	TLabel *Label2;
    TIBQuery *IBQuery1;
    TIBQuery *qrUpdateStockRequest;
	TIBQuery *qrStockTransferManual;
    TIBQuery *qrdelete_stock_request;
    TIBQuery *qrfetch_request_number;
    TIBQuery *qrUpdateStockRequestQuantity;
    TIBSQL *sqlTransferNumber;
    TIBQuery *qrfindCommitedRequest;
    TBitBtn *btnReprint;
    TIBQuery *qrfetch_request_no;
    TIBQuery *qrUpdateRealRequestedQty;
    TIBQuery *qrQuantityUpdate;
    TIBQuery *qrStockReqUnit;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall vtvStockQtyGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
	void __fastcall vtvStockQtyGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex);
	void __fastcall vtvStockQtyAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStockQtyBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStockQtyFocusChanged(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall vtvStockQtyFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
	void __fastcall vtvStockQtyKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall vtvStockQtyCreateEditor(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, IVTEditLink *EditLink);
	void __fastcall vtvStockQtyEditing(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, bool &Allowed);
	void __fastcall vtvStockQtyEdited(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column);
	void __fastcall neStockQtyKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall btnScanClick(TObject *Sender);
	void __fastcall neStockQtyKeyPress(TObject *Sender, char &Key);
        void __fastcall tvTransfersChange(TObject *Sender,
          TTreeNode *Node);
    
    void __fastcall btnReprintClick(TObject *Sender);
   
private:	// User declarations
	void __fastcall WMLoadTransfer(TMessage& Message);
	BEGIN_MESSAGE_MAP
			MESSAGE_HANDLER(WM_LOADTRANSFER, TMessage, WMLoadTransfer)
	END_MESSAGE_MAP(TForm)
        void LoadTreeView();
        TStringList *BatchKeyList;
		    void LoadStocksForManualMode();
    void LoadStocksForStockRequestMode();
public:		// User declarations
	__fastcall TfrmTransferMulti(TComponent* Owner);
	AnsiString Source;
	AnsiString Destination;

    TStringList* SelectedStockRequestKeys;
    TStringList* TransfferedStockRequestKeys;
    TStringList*  StockRequestToBeDeletedKeys;
	bool isStockRequestMode;
    TDateTime StartTime;

    TStringList*   CommittedStockRequestKeys;
    int Transfer_no;

    	float	StockTakeQty  ;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmTransferMulti *frmTransferMulti;
//---------------------------------------------------------------------------
#endif
