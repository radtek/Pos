//---------------------------------------------------------------------------
#ifndef StockH
#define StockH

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

#include <stdint.h>

//---------------------------------------------------------------------------
#define MAX_STOCK_STOCK_GROUP_LENGTH	25
#define MAX_STOCK_ITEM_LENGTH			50

const UWM_REFRESH_STOCK_GROUP = WM_USER + 2;
//---------------------------------------------------------------------------
enum TStockMode { smEdit, smSelect };
//---------------------------------------------------------------------------
class TfrmStock : public TForm
{
__published:	// IDE-managed Components
	TIBTransaction *Transaction;
	TIBSQL *sqlAddLK;
	TIBQuery *qrStockSuppliers;
	TIBQuery *qrStockLocation;
	TIBSQL *qrUpdateStock;
	TIBSQL *qrUpdateStockGroup;
	TPopupMenu *pmTreeView;
	TMenuItem *miDelete;
	TMenuItem *miNewStockGroup;
	TMenuItem *miRename;
	TMenuItem *miNewStockItem;
	TMenuItem *miEdit;
	TMenuItem *N1;
	TTimer *tiTreeScroll;
	TFindDialog *FindDialog;
	TIBSQL *qrFindStockGroup;
	TPanel *Panel3;
	TPanel *Panel1;
	TTreeView *tvStock;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TPageControl *pcStockDetails;
	TTabSheet *tsStockGroup;
	TPanel *Panel2;
	TButton *btnEditStock;
	TButton *btnAddStock;
	TButton *btnDeleteStockGroup;
	TListBox *lbStockItems;
	TTabSheet *tsStockItem;
	TPanel *Panel10;
	TBitBtn *btnClose;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TBevel *Bevel1;
	TLabel *Label4;
	TBitBtn *btnFind;
	TSplitter *Splitter1;
	TIBSQL *qrFindStockCategory;
	TIBSQL *qrUpdateStockCategory;
	TLabel *Label3;
	TBevel *Bevel2;
	TButton *btnAddStockGroup;
	TButton *btnRenameStockGroup;
	TButton *btnMoveGroup;
	TMenuItem *miNewStockCategory;
	TTabSheet *tsStockCategory;
	TListBox *lbStockGroups;
	TPanel *Panel4;
	TLabel *Label8;
	TBevel *Bevel5;
	TLabel *Label9;
	TBevel *Bevel6;
	TButton *Button5;
	TButton *btnAddCategory;
	TButton *btnRenameCategory;
	TButton *btnDeleteCategory;
	TButton *btnSortCategories;
	TButton *btnSortStockGroups;
	TButton *btnRenameStockGroup2;
	TPanel *Panel5;
	TButton *btnAdd;
	TBitBtn *btnEdit;
	TBitBtn *btnDelete;
	TStringGrid *sgSuppliers;
	TLabel *Label5;
	TLabel *Label2;
	TPanel *Panel6;
	TLabel *Label21;
	TLabel *lbeStockCode;
	TLabel *lbeUnit;
	TLabel *Label13;
	TLabel *Label1;
	TStringGrid *sgLocations;
	TSplitter *Splitter2;
	TMenuItem *N2;
	TMenuItem *ExpandTree1;
	TMenuItem *CollapseTree1;
	TMenuItem *N3;
	TMenuItem *miRestore;
	TIBSQL *qrFindStock;
	TIBSQL *qrStockList;
	TIBSQL *qrGroupList;
	TIBSQL *qrDeletedList;
	TTabSheet *tsDeletedItems;
	TPanel *Panel7;
	TLabel *Label7;
	TBevel *Bevel4;
	TButton *Button7;
	TButton *btnRestore;
	TListView *ListView1;
	TButton *Button2;
	TButton *btnRestoreAll;
	TButton *btnEmptyDeletedBin;
	TTabSheet *tsDeletedItem;
	TButton *Button1;
	TButton *Button3;
	TPanel *pnlStocktake;
	TLabel *Label6;
	TImage *Image1;
    TIBSQL *qrStockRequestLocation;
    TIBSQL *qrStockReqPO;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall tvStockMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall tvStockDragOver(TObject *Sender, TObject *Source, int X,
          int Y, TDragState State, bool &Accept);
	void __fastcall tvStockEdited(TObject *Sender, TTreeNode *Node,
          AnsiString &S);
	void __fastcall tvStockDblClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall tvStockChange(TObject *Sender, TTreeNode *Node);
	void __fastcall miNewStockGroupClick(TObject *Sender);
	void __fastcall miNewStockItemClick(TObject *Sender);
	void __fastcall miDeleteClick(TObject *Sender);
	void __fastcall miRenameClick(TObject *Sender);
	void __fastcall tvStockKeyDown(TObject *Sender, WORD &Key,
          TShiftState Shift);
	void __fastcall tvStockDragDrop(TObject *Sender, TObject *Source, int X,
          int Y);
	void __fastcall miEditClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall btnEditClick(TObject *Sender);
	void __fastcall btnAddStockClick(TObject *Sender);
	void __fastcall btnAddStockGroupClick(TObject *Sender);
	void __fastcall btnDeleteStockGroupClick(TObject *Sender);
	void __fastcall btnEditStockClick(TObject *Sender);
	void __fastcall lbStockItemsDblClick(TObject *Sender);
	void __fastcall tiTreeScrollTimer(TObject *Sender);
	void __fastcall tvStockEndDrag(TObject *Sender, TObject *Target, int X,
          int Y);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
	void __fastcall tvStockKeyPress(TObject *Sender, char &Key);
	void __fastcall GridDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall Splitter1CanResize(TObject *Sender, int &NewSize,
          bool &Accept);
	void __fastcall btnRenameStockGroupClick(TObject *Sender);
	void __fastcall Button1Click(TObject *Sender);
	void __fastcall Splitter1Moved(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall pmTreeViewPopup(TObject *Sender);
	void __fastcall miNewStockCategoryClick(TObject *Sender);
	void __fastcall lbStockGroupsDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall lbStockItemsDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall btnDeleteCategoryClick(TObject *Sender);
	void __fastcall btnRenameCategoryClick(TObject *Sender);
	void __fastcall btnSortCategoriesClick(TObject *Sender);
	void __fastcall btnSortStockGroupsClick(TObject *Sender);
	void __fastcall btnAddCategoryClick(TObject *Sender);
	void __fastcall lbStockGroupsDblClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall Splitter2CanResize(TObject *Sender, int &NewSize,
          bool &Accept);
	void __fastcall ExpandTree1Click(TObject *Sender);
	void __fastcall CollapseTree1Click(TObject *Sender);
	void __fastcall tvStockEditing(TObject *Sender, TTreeNode *Node,
          bool &AllowEdit);
	void __fastcall tvStockDeletion(TObject *Sender, TTreeNode *Node);
	void __fastcall miRestoreClick(TObject *Sender);
	void __fastcall btnRestoreClick(TObject *Sender);
	void __fastcall btnRestoreAllClick(TObject *Sender);
	void __fastcall btnEmptyDeletedBinClick(TObject *Sender);
	void __fastcall ListView1DblClick(TObject *Sender);
protected:
	void __fastcall WMRefreshStockGroup(TMessage& Message);
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	void __fastcall CMDialogChar(TWMKey &Msg);
	BEGIN_MESSAGE_MAP
		MESSAGE_HANDLER(UWM_REFRESH_STOCK_GROUP, TMessage, WMRefreshStockGroup)
		MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
		MESSAGE_HANDLER(CM_DIALOGCHAR, TWMKey, CMDialogChar)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void ResizeGrids();
	void LoadTree();
    void LoadTreeforStockRequest();
	void SetSortOrder();
//	void RefreshStockCategory();
	void RefreshStockGroup();
	void RefreshStockGroup(TTreeNode *StockGroupNode, int StockGroupKey);
	void RefreshDeletedBin();

	TTreeNode *DeletedNode;
	TTreeNode *LocateInTree(int StockCategoryKey, int StockGroupKey, int StockKey, bool Deleted=false);
	void SelectInTree(TTreeNode *Node);
//	void InitCategories();
	bool FindStockCategory(AnsiString Text);
	bool FindStockGroup(AnsiString Text);
	bool FindStockItem(AnsiString Text);

	TGridCoord SGC;
	bool Registered;
	int GetStockItemCount();
	bool StockInStocktake( int StockKey );
	int StockItemIconIndex;
    int Decimalpalaces ;
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
//	TStockNodeData *CreateStockItem(TTreeNode *ItemNode, TIBSQL *Query, bool Deleted);
//	TStockNodeData *CreateStockCategory(TTreeNode *ItemNode, TIBSQL *Query, bool Deleted);
//	TStockNodeData *CreateStockGroup(TTreeNode *ItemNode, TIBSQL *Query, bool Deleted);
	void CreateStockItem(TTreeNode *ItemNode,
		AnsiString Description, int StockKey, AnsiString Code, AnsiString StocktakeUnit,
		AnsiString Category, int CategoryKey,
		AnsiString Group, int GroupKey,
		bool Deleted);
	void CreateStockGroup(TTreeNode *ItemNode,
		AnsiString Category, int CategoryKey, AnsiString GLCode,
		AnsiString Group, int GroupKey,
		bool Deleted);
	void CreateStockCategory(TTreeNode *ItemNode,
		AnsiString Category, int CategoryKey, AnsiString GLCode,
		bool Deleted);

void
RemoveKickstart(
unsigned char removal_class,
unsigned long long category_key,
unsigned long long group_key,
unsigned long long item_key);

void
RemoveCategory(
TIBQuery *query,
TIBTransaction *transaction,
uint64_t category_key,
uint64_t group_key,
uint64_t item_key);

void
RemoveGroup(
TIBQuery *query,
TIBTransaction *transaction,
uint64_t category_key,
uint64_t group_key,
uint64_t item_key);

void
RemoveItem(
TIBQuery *query,
TIBTransaction *transaction,
uint64_t category_key,
uint64_t group_key,
uint64_t item_key);

void
RemoveRelatedRecipes(AnsiString &);
void
RemoveRelatedRecipes();
     void TreeviewData();

	const std::auto_ptr<TfrmAddStock> frmAddStock;
	const std::auto_ptr<TfrmAddStockCategoryGroup> frmAddStockCategoryGroup;
public:		// User declarations
	__fastcall TfrmStock(TComponent* Owner);

    bool ignorePendingDeletions;
	int			StockKey;
	int			StockGroupKey;
	int			StockCategoryKey;
	bool			StockSelectedDeleted;
	 AnsiString Location;
	TStockMode	StockMode;
	AnsiString	NewItemName;		// Allows a name to be entered from Menu Editor into new stock item
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmStock *frmStock;
//---------------------------------------------------------------------------
#endif
