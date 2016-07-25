//---------------------------------------------------------------------------

#ifndef SetupLocationsH
#define SetupLocationsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ComCtrls.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <Grids.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSetupLocations : public TForm
{
__published:	// IDE-managed Components
	TIBTransaction *Transaction;
	TIBSQL *sqlLocation;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TLabel *Label6;
	TPanel *Panel5;
	TPanel *Panel1;
	TLabel *Label1;
	TStringGrid *StringGrid1;
	TBitBtn *btnOk;
	TLabel *Label2;
	TIBSQL *sqlUpdateLocation;
	TIBSQL *qrFindLocation;
	TIBSQL *sqlAddLK;
	TIBSQL *qrAddLocation;
	TBitBtn *btnAdd;
	TBitBtn *btnDelete;
	TIBSQL *qrDeleteLocation;
	TBitBtn *btnRename;
	TBitBtn *btnSync;
	TIBSQL *qrRenameLocation;
	TBitBtn *btnInfo;
	TIBSQL *qrLocInfo;
	TIBTransaction *StockTransaction;
	TIBSQL *qrStockLocInfo;
    TPanel *Panel6;
    TPanel *Panel7;
    TPanel *Panel8;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall StringGrid1DrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State);
	void __fastcall StringGrid1MouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall StringGrid1MouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall StringGrid1KeyPress(TObject *Sender, char &Key);
	void __fastcall StringGrid1MouseMove(TObject *Sender, TShiftState Shift,
          int X, int Y);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall btnSyncClick(TObject *Sender);
	void __fastcall btnRenameClick(TObject *Sender);
	void __fastcall btnInfoClick(TObject *Sender);
private:	// User declarations
	bool SGMouseDown;
public:		// User declarations
	__fastcall TfrmSetupLocations(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSetupLocations *frmSetupLocations;
//---------------------------------------------------------------------------
#endif
