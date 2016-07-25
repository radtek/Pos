//---------------------------------------------------------------------------

#ifndef SelectMenuH
#define SelectMenuH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <DBGrids.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBQuery.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSelectMenu : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel2;
	TPanel *Panel8;
	TPanel *Panel9;
	TBitBtn *BitBtn1;
	TBitBtn *BitBtn2;
	TDBGrid *DBGrid1;
	TLabel *lbeTitle;
	TDataSource *dsMenu;
	TIBQuery *qrMenu;
	TIBTransaction *Transaction;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall DBGrid1DblClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSelectMenu(TComponent* Owner);

	int MenuKey;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSelectMenu *frmSelectMenu;
//---------------------------------------------------------------------------
#endif

