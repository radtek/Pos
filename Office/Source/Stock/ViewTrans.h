//---------------------------------------------------------------------------
#ifndef ViewTransH
#define ViewTransH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <ComCtrls.hpp>
#include <IBDatabase.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
//enum TViewMode { vmStock, vmContact };
//---------------------------------------------------------------------------
class TfrmViewTrans : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *pnlStock;
	TGroupBox *GroupBox2;
	TLabel *Label6;
	TLabel *Label8;
	TDBText *dbtStockCode;
	TLabel *Label7;
	TLabel *Label11;
	TDBText *DBText2;
	TDBText *DBText3;
	TDBText *DBText4;
	TDBText *DBText5;
	TGroupBox *GroupBox3;
	TLabel *Label12;
	TEdit *edFind;
	TPanel *Panel4;
	TLabel *Label1;
	TRadioButton *rbThisMonth;
	TRadioButton *rbLastMonth;
	TRadioButton *rbAll;
	TButton *btnPrintAll;
	TButton *Button1;
	TPanel *Panel2;
	TButton *btnPrintClose;
	TDataSource *dsTransaction;
	TIBQuery *qrStock;
	TDataSource *dsStock;
	TLabel *Label2;
	TRadioButton *RadioButton1;
	TRadioButton *RadioButton2;
	TIBTransaction *Transaction;
	TLabel *Label3;
	TDBLookupComboBox *dbluLocation;
	TPageControl *pcFilter;
	TTabSheet *tsReceived;
	TTabSheet *tsStocktake;
	TTabSheet *tsManualSales;
	TTabSheet *tsSales;
	TIBQuery *qrReceipt;
	TDBGrid *dbgReceipt;
	TDBGrid *dbgStocktake;
	TIBQuery *qrStocktake;
	TIBQuery *qrManualSales;
	TIBQuery *qrSales;
	TDBGrid *dbgSales;
	TDBGrid *dbgManualSales;
	TTabSheet *tsTransfer;
	TDBGrid *dbgTransfer;
	TIBQuery *qrTransfer;
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall rbRangeClick(TObject *Sender);
	void __fastcall edFindKeyUp(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnPrintAllClick(TObject *Sender);
	void __fastcall btnPrintCloseClick(TObject *Sender);
	void __fastcall pcFilterChange(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall qrAfterOpen(TDataSet *DataSet);
	void __fastcall dbluLocationCloseUp(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void ResizeGrids();
public:		// User declarations
	__fastcall TfrmViewTrans(TComponent* Owner);
//	TViewMode ViewMode;
	AnsiString StockCode;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmViewTrans *frmViewTrans;
//---------------------------------------------------------------------------
#endif
