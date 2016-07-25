//---------------------------------------------------------------------------
#ifndef StockMainH
#define StockMainH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
//---------------------------------------------------------------------------
class TfrmStockMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel13;
	TBitBtn *btnClose;
	TBitBtn *btnPrint;
	TPanel *Panel5;
	TPanel *Panel7;
	TPanel *Panel8;
	TLabel *Label1;
	TPanel *Panel9;
	TBitBtn *btnCreateOrder;
	TPanel *Panel10;
	TPanel *Panel11;
	TLabel *Label3;
	TPanel *Panel12;
	TBitBtn *btnEditStock;
	TBitBtn *btnEditContacts;
	TBitBtn *btnReceiveOrder;
	TBitBtn *btnReceiveInvoice;
	TBitBtn *btnTransferMulti;
	TBitBtn *btnExport;
	TBitBtn *btnTransfer;
	TBitBtn *btnHistory;
	TBitBtn *btnStocktake;
	TBitBtn *BitBtn1;
    TBitBtn *BitBtn2;
    TBitBtn *BitBtn4;
   TBitBtn *btnUpdateSupllierCosts;
      TBitBtn *btnStockRequest;

    TIBTransaction *Transaction;
    TIBSQL *qrCreateStockRequestStockTable;
    TIBSQL *qrCreateStockRequestTable;
    TIBSQL *qrCheckTableExists;
    TIBSQL *qrStockRequestForeignKey;
    TIBSQL *qrStockRequestStockForeignKey;
    TIBSQL *qrStockRequestSupplierForeignKey;

   TBitBtn *btnSubstitute;
    TIBSQL *qrcheckfield;
    TIBSQL *qrStockrequestKey;
    TIBSQL *qrupdatecode;
	void __fastcall btnHistoryClick(TObject *Sender);
	void __fastcall btnStocktakeClick(TObject *Sender);
	void __fastcall btnPrintClick(TObject *Sender);
	void __fastcall btnUsersClick(TObject *Sender);
	void __fastcall btnReceiveOrderClick(TObject *Sender);
	void __fastcall btnReceiveInvoiceClick(TObject *Sender);
	void __fastcall btnEditStockClick(TObject *Sender);
	void __fastcall btnCreateOrderClick(TObject *Sender);
	void __fastcall btnEditContactsClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall btnTransferMultiClick(TObject *Sender);
	void __fastcall btnExportClick(TObject *Sender);
	void __fastcall BitBtn1Click(TObject *Sender);
    void __fastcall WriteOffBtnClick(TObject *Sender);
    void __fastcall btnRecipesMouseClick(TObject *Sender);
   void __fastcall btnUpdateSupllierCostsClick(TObject *Sender);
      void __fastcall btnStockRequestClick(TObject *Sender);
   void __fastcall btnSubstituteClick(TObject *Sender);
protected:
	void __fastcall CMDialogKey(TCMDialogKey &Msg);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(CM_DIALOGKEY, TCMDialogKey, CMDialogKey)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	bool Registered;
	unsigned int Modules;
    void CreateStockRequestTable();
    
public:		// User declarations
	__fastcall TfrmStockMain(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStockMain *frmStockMain;
//---------------------------------------------------------------------------
#endif
