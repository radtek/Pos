//---------------------------------------------------------------------------

#ifndef StockGroupH
#define StockGroupH
//---------------------------------------------------------------------------
#include <Buttons.hpp>
#include <Classes.hpp>
#include <ComCtrls.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <Dialogs.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
#include <Menus.hpp>
#include <StdCtrls.hpp>
#include <Graphics.hpp>
#include <memory>
#include "AddStock.h"
#include "AddStockCategoryGroup.h"
//---------------------------------------------------------------------------
const UWM_REFRESH_STOCK_GROUP = WM_USER + 2;
enum TStockMode { smEdit, smSelect };
class TfrmStockGroup : public TForm
{
__published:	// IDE-managed Components
   TPanel *Panel3;
   TPanel *Panel1;
   TPanel *Panel10;
   TBitBtn *btnClose;
   TIBTransaction *Transaction;
   TIBQuery *qrStockLocation;
   TTimer *tiTreeScroll;
   TFindDialog *FindDialog;
   TIBSQL *qrFindStockGroup;
   TIBSQL *qrFindStockCategory;
   TIBSQL *qrUpdateStockCategory;
   TIBSQL *qrFindStock;
   TIBSQL *qrStockList;
   TIBSQL *qrStockRequestLocation;
   TPanel *Panel2;
   TPanel *Panel8;
   TPanel *Panel9;
   TLabel *lbeTitle;
   TListBox *lstCategory;
   TBitBtn *btnOk;
   TBitBtn *btnCancel;
   TIBSQL *qrStockCat;
    TIBSQL *qrStockCatAll;
   void __fastcall FormShow(TObject *Sender);
   void __fastcall btnCancelClick(TObject *Sender);
    void __fastcall btnOkClick(TObject *Sender);

private:	// User declarations
        void LoadTree();
 
           	void CreateStockCategory(TTreeNode *ItemNode,
		AnsiString Category, int CategoryKey, AnsiString GLCode,
		bool Deleted);

	TTreeNode *DeletedNode;
	bool Registered;
const std::auto_ptr<TfrmAddStock> frmAddStock;
	const std::auto_ptr<TfrmAddStockCategoryGroup> frmAddStockCategoryGroup;
public:		// User declarations
  bool ignorePendingDeletions;
	int			StockKey;
	int			StockGroupKey;
	int			StockCategoryKey;
	bool			StockSelectedDeleted;



	TGridCoord SGC;

   int StockItemIconIndex;

	enum TStockNodeType { snDeletedBin, snStockCategory, snStockGroup, snStockItem };
	class TStockNodeData
	{
	public:
		TStockNodeData(TStockNodeType SNT) : Type(SNT)
		{
			Key				= 0;
			CategoryKey		= 0;
			GroupKey			= 0;
			Deleted			= false;
		}

		TStockNodeType	Type;

		AnsiString		Description;
		int				Key;
		AnsiString		StockCode;
		bool				Deleted;

		AnsiString		Category;
		int				CategoryKey;
		AnsiString		Group;
		int				GroupKey;

		AnsiString		StocktakeUnit;
		AnsiString		GLCode;
	};



        AnsiString Location;
  	TStockMode	StockMode;
	AnsiString	NewItemName;
TStringList *SelectedCategoryList;
 int ContactKey;
 AnsiString Mode;

   __fastcall TfrmStockGroup(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStockGroup *frmStockGroup;
//---------------------------------------------------------------------------
#endif
