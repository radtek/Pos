//---------------------------------------------------------------------------
#ifndef UsersH
#define UsersH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DBGrids.hpp>
#include <Grids.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <IBDatabase.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <DB.hpp>
//---------------------------------------------------------------------------
class TfrmUsers : public TForm
{
__published:	// IDE-managed Components
	TIBTransaction *Transaction;
	TDataSource *dsUsers;
	TIBDataSet *dtUsers;
	TPanel *Panel3;
	TPanel *Panel2;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TPanel *Panel10;
	TDBGrid *DBGrid1;
	TLabel *Label7;
	TDBText *DBText6;
	TDBText *DBText2;
	TLabel *Label1;
	TLabel *Label4;
	TDBMemo *DBMemo1;
	TDBText *DBText3;
	TLabel *Label3;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label8;
	TLabel *Label2;
	TDBText *DBText5;
	TDBText *DBText4;
	TBitBtn *btnClose;
	TBitBtn *btnAdd;
	TBitBtn *btnEdit;
	TBitBtn *btnDelete;
	TPanel *Panel1;
	void __fastcall btnAddClick(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall btnEditClick(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall dtUsersAfterScroll(TDataSet *DataSet);
	void __fastcall btnCloseClick(TObject *Sender);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
	BEGIN_MESSAGE_MAP
	  MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
	END_MESSAGE_MAP(TComponent)
private:	// User declarations
	void ResizeGrids();
public:		// User declarations
	__fastcall TfrmUsers(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmUsers *frmUsers;
//---------------------------------------------------------------------------
#endif
