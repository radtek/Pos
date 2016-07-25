//---------------------------------------------------------------------------

#ifndef SelectLocationH
#define SelectLocationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <IBSQL.hpp>
//---------------------------------------------------------------------------
class TfrmSelectLocation : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TLabel *Label1;
	TLabel *Label2;
	TDBGrid *DBGrid1;
	TBitBtn *BitBtn1;
	TBitBtn *btnGo;
	TBitBtn *BitBtn2;
	TIBSQL *qrAddLocation;
	TIBSQL *sqlAddLK;
	TIBQuery *qrLocationList;
	TDataSource *dsLocationList;
	TIBTransaction *Transaction;
	TIBSQL *qrFindLocation;
	TIBSQL *qrUpdateLocation;
    TIBQuery *qrLocation;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall BitBtn2Click(TObject *Sender);
	void __fastcall DBGrid1DblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSelectLocation(TComponent* Owner);
	bool Execute(AnsiString &Location);
    	AnsiString	LocationName;
        TStringList *SelectedLocationList;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSelectLocation *frmSelectLocation;
//---------------------------------------------------------------------------
#endif
