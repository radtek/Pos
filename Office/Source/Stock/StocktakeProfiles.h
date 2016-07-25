//---------------------------------------------------------------------------
#ifndef StockGroupVisibleH
#define StockGroupVisibleH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCGrids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
#include <dbcgrids.hpp>
#include <IBSQL.hpp>
#include <ComCtrls.hpp>
#include <Buttons.hpp>
#include <ImgList.hpp>
#include <Graphics.hpp>
#include <memory>
#include <vector>
#include "CreateProfile.h"
#include "MM_StockTransactions.h"
//---------------------------------------------------------------------------
class TfrmStocktakeProfiles : public TForm
{
__published:	// IDE-managed Components
	TIBTransaction *Transaction;
	TIBSQL *sqlLocation;
	TPanel *Panel6;
	TPanel *Panel1;
	TSplitter *Splitter1;
	TPanel *Panel2;
	TBitBtn *btnEditProfile;
	TBitBtn *btnCreateProfile;
	TPanel *Panel4;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TListBox *lbLocations;
	TPanel *Panel3;
	TTreeView *tvProfiles;
	TPanel *Panel5;
	TIBSQL *qrStockProfiles;
	TImage *Image1;
	TLabel *Label6;
	TIBSQL *sqlInitialised;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnCreateProfileClick(TObject *Sender);
	void __fastcall btnEditProfileClick(TObject *Sender);
	void __fastcall tvProfilesMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall tvProfilesKeyPress(TObject *Sender, char &Key);
	void __fastcall lbLocationsClick(TObject *Sender);
private:	// User declarations
	void LoadTree();
	void ToggleCheckbox(TTreeNode *Node);
	bool CreateStocktakeExportFile(AnsiString Location);
	int StocktakeProfileKey;

public:		// User declarations
	__fastcall TfrmStocktakeProfiles(TComponent* Owner);

	bool Execute(Stock::TStocktakeProfiles &StocktakeProfiles, AnsiString &Location);
};
//---------------------------------------------------------------------------
#endif
