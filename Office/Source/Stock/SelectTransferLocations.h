//---------------------------------------------------------------------------

#ifndef SelectTransferLocationsH
#define SelectTransferLocationsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <memory>
#include "MMData.h"
#include "StockData.h"
#include "MM_Menu.h"
#include "MM_StockCore.h"
//---------------------------------------------------------------------------
struct TTransferLocations
{
//	TTransferLocations();
//	TTransferLocations(AnsiString iSource, AnsiString iDestination) :
//			Source(iSource), Destination(iDestination) {}
	AnsiString Source;
	AnsiString Destination;
};
//---------------------------------------------------------------------------
class TfrmSelectTransferLocations : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TLabel *Label6;
	TPanel *Panel5;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TLabel *Label5;
	TLabel *Label19;
	TLabel *Label3;
	TLabel *Label20;
	TListBox *lbSource;
	TListBox *lbDestination;
	void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSelectTransferLocations(TComponent* Owner);
};
//---------------------------------------------------------------------------
class TSelectTransferLocations
{
public:
	TSelectTransferLocations() :
			frmSelectTransferLocations(new TfrmSelectTransferLocations(NULL)),
			fMenu(dmMMData->dbMenuMate),
			fStockControl(dmStockData->dbStock)
	{}
//	~TSelectTransferLocations();

	bool Execute();
	TTransferLocations	Locations();
private:
	const std::auto_ptr<TfrmSelectTransferLocations> frmSelectTransferLocations;

	Menu::TMenuLoadDB		fMenu;
	Menu::TLocationsInfo	fLocationsInfo;
	TTransferLocations	fTransferLocations;

	Stock::TStockControl	fStockControl;
};
//---------------------------------------------------------------------------
#endif
