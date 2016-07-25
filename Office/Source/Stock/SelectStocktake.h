//---------------------------------------------------------------------------

#ifndef SelectStocktakeH
#define SelectStocktakeH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "MM_StockTransactions.h"
#include <ComCtrls.hpp>
#include <IBSQL.hpp>
#include "VirtualTrees.hpp"
#include <IBDatabase.hpp>
//---------------------------------------------------------------------------
class TfrmSelectStocktake : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel3;
	TPanel *Panel4;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TPanel *Panel7;
	TPanel *Panel10;
	TPanel *Panel11;
	TLabel *Label3;
	TBitBtn *btnNewStocktake;
	TBitBtn *btnDeleteStocktake;
	TVirtualStringTree *vtvStocktake;
	TIBSQL *qrStoctakeItems;
	TIBTransaction *Transaction;
	void __fastcall btnNewStocktakeClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnDeleteStocktakeClick(TObject *Sender);
	void __fastcall vtvStocktakeAfterPaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStocktakeBeforePaint(TBaseVirtualTree *Sender,
          TCanvas *TargetCanvas);
	void __fastcall vtvStocktakeGetText(TBaseVirtualTree *Sender,
          PVirtualNode Node, TColumnIndex Column, TVSTTextType TextType,
          WideString &CellText);
	void __fastcall vtvStocktakeGetImageIndex(TBaseVirtualTree *Sender,
          PVirtualNode Node, TVTImageKind Kind, TColumnIndex Column,
          bool &Ghosted, int &ImageIndex);
	void __fastcall vtvStocktakeChange(TBaseVirtualTree *Sender,
          PVirtualNode Node);
	void __fastcall vtvStocktakeFocusChanging(TBaseVirtualTree *Sender,
          PVirtualNode OldNode, PVirtualNode NewNode,
          TColumnIndex OldColumn, TColumnIndex NewColumn, bool &Allowed);
private:	// User declarations
	Stock::TStocktakeControl &fStocktakeControl;
	void fLoadList();
	Stock::TStocktakeList fStocktakeList;
	Stock::TStocktakeDetails *fStocktake;
	int fCurrentStocktake;

	void CreateStocktakeExportFile(int StocktakeKey);
public:		// User declarations
	__fastcall TfrmSelectStocktake(Stock::TStocktakeControl &StocktakeControl);

	bool Execute(Stock::TStocktakeDetails &Stocktake);
};
//---------------------------------------------------------------------------
#endif
