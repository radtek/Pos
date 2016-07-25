//---------------------------------------------------------------------------

#ifndef LocationRenameH
#define LocationRenameH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
#include "MM_DBCore.h"
#include <map>
#include <set>
#include <list>
//---------------------------------------------------------------------------
struct LocationDetails
{
	LocationDetails() : quantity(0), operationType(LocationDetails::Unknown), inStocktake(false) {}

	LocationDetails(AnsiString _name, double _quantity, bool _initialised) :
		name(_name), quantity(_quantity), operationType(LocationDetails::Unknown), inStocktake(_initialised) {}

	LocationDetails(const LocationDetails& rhs) :
		name(rhs.name), quantity(rhs.quantity), operationType(rhs.operationType), inStocktake(rhs.inStocktake) {}

	bool operator<(const LocationDetails& rhs) const { return name < rhs.name; }

	const LocationDetails& operator=(const LocationDetails& rhs)
		{ name = rhs.name; quantity = rhs.quantity; operationType = rhs.operationType; inStocktake = rhs.inStocktake; return *this; }

	enum OperationType { Unknown, None, Rename, CreateAndTransfer, Transfer, Create };

	AnsiString name;
	double quantity;

	bool inStocktake;

	OperationType operationType;
};
//---------------------------------------------------------------------------
struct StockDetails
{
	StockDetails() : key(0) {}
	StockDetails(int _key, AnsiString _code, AnsiString _description) :
			key(_key), code(_code), description(_description) {}
	StockDetails(const StockDetails& rhs) :
			key(rhs.key), code(rhs.code), description(rhs.description), locations(rhs.locations) {}
	bool operator<(const StockDetails& rhs) const { return key < rhs.key; }
	const StockDetails& operator=(const StockDetails& rhs)
	{
		key = rhs.key;
		code = rhs.code;
		description = rhs.description;
		locations.insert(rhs.locations.begin(), rhs.locations.end());
		return *this;
	}
	int key;
	AnsiString code;
	AnsiString description;
	typedef std::map<AnsiString, LocationDetails> Locations;
	Locations locations;

	bool containsLocation(const AnsiString& location) const
	{
		return locations.find(location) != locations.end();
	}
};
//---------------------------------------------------------------------------
class TfrmUpdateLocations : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel13;
	TBitBtn *btnClose;
	TPanel *Panel5;
	TPanel *Panel10;
	TPageControl *pcReportProperties;
	TTabSheet *tsSelectItems;
	TBevel *Bevel4;
	TPanel *Panel11;
	TLabel *lbeTypeTitle;
	TBitBtn *btnTypePreview;
	TBitBtn *btnProductsRename;
	TTabSheet *tsLocations;
	TPanel *Panel1;
	TLabel *Label3;
	TTreeView *tvFilterTree;
	TListBox *lbLocations;
	TLabel *Label1;
	TBevel *Bevel1;
	TBitBtn *btnLocationsNext;
	TTabSheet *tsClashes;
	TBevel *Bevel3;
	TPanel *Panel4;
	TLabel *Label5;
	TBitBtn *btnFinish;
	TCheckBox *chbConfirm;
	TComboBox *cbLocation;
	TMemo *memProcess;
	TProgressBar *ProgressBar1;
	TBitBtn *btnAddLocations;
	TTabSheet *tsLocationToAdd;
	TListBox *lbMMLocations;
	TBevel *Bevel2;
	TBitBtn *btnAddLocationNext;
	TPanel *Panel3;
	TLabel *Label4;
	TBitBtn *btnAddLocationBack;
	TBitBtn *btnLocationsBack;
	void __fastcall tvFilterTreeKeyPress(TObject *Sender, char &Key);
	void __fastcall tvFilterTreeMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, int X, int Y);
	void __fastcall btnProductsRenameClick(TObject *Sender);
	void __fastcall btnLocationsNextClick(TObject *Sender);
	void __fastcall btnFinishClick(TObject *Sender);
	void __fastcall lbLocationsDrawItem(TWinControl *Control, int Index,
          TRect &Rect, TOwnerDrawState State);
	void __fastcall btnAddLocationsClick(TObject *Sender);
	void __fastcall btnAddLocationNextClick(TObject *Sender);
	void __fastcall btnAddLocationBackClick(TObject *Sender);
	void __fastcall btnLocationsBackClick(TObject *Sender);
	void __fastcall btnCloseClick(TObject *Sender);
private:	// User declarations
	enum SelectionState { All, Some, None };
	Database::TDBTransaction dbStockTransaction;
	Database::TDBTransaction dbMMTransaction;
	TIBSQL* stockListQuery;
	TIBSQL* updateStockQuery;
	TIBSQL* mmLocationsQuery;
	TIBSQL* mmUpdateStock;

	void loadTree();
	std::map<int, StockDetails> stockKeyToCodeMap;
	std::map<int, StockDetails> selectedStock;
	std::map<int, StockDetails> stockToUpdate;

	void toggleCheck(TTreeNode *Node);
	void setChildState(TTreeNode *node);

	void deleteLocation(int stockKey, const AnsiString& location);
	void renameLocation(int stockKey, const AnsiString& oldLocation, const AnsiString& newLocation);
	void renamePurchaseStock(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation);
	void renameMMRecipeStock(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation);
	void renameMMOrderRecipeStock(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation);
	void renameMMStockTrack(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation);
	void renameMMStockCosts(const AnsiString& stockCode, const AnsiString& oldLocation, const AnsiString& newLocation);

public:		// User declarations
	__fastcall TfrmUpdateLocations(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmUpdateLocations *frmUpdateLocations;
//---------------------------------------------------------------------------
#endif
