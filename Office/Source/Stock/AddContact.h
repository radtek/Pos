//---------------------------------------------------------------------------
#ifndef AddContactH
#define AddContactH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <DB.hpp>
#include <IBQuery.hpp>
//---------------------------------------------------------------------------
enum TAdjustContactMode { amAddContact, amEditContact };
//---------------------------------------------------------------------------
class TfrmAddContact : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TDataSource *dsContact;
	TIBDataSet *dtContact;
	TIBTransaction *Transaction;
	TGroupBox *GroupBox1;
	TLabel *Label3;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *Label6;
	TLabel *Label7;
	TLabel *Label8;
	TLabel *Label9;
	TDBEdit *dbeCompanyName;
	TDBMemo *DBMemo1;
	TDBMemo *DBMemo2;
	TDBEdit *dbeContact;
	TDBEdit *DBEdit3;
	TDBEdit *DBEdit4;
	TDBEdit *DBEdit5;
	TDBEdit *DBEdit6;
	TDBMemo *DBMemo3;
	TBitBtn *BitBtn1;
	TIBQuery *qrContactGroup;
	TIBQuery *qrAddContactGroup;
	TIBSQL *sqlAddLK;
	TDBEdit *DBEdit1;
	TLabel *Label10;
	TBitBtn *BitBtn2;
	TIBSQL *qrUpdateInvoiceSupplierID;
    TLabel *lblWebOrderTarget;
    TDBEdit *dbeWeborderTarget;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall dbeKeyPress(TObject *Sender, char &Key);
	void __fastcall BitBtn1Click(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall BitBtn2Click(TObject *Sender);
private:	// User declarations
    bool validate_weborder_target_uri();
public:		// User declarations
	__fastcall TfrmAddContact(TComponent* Owner);
	TAdjustContactMode Mode;
	int ContactKey;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmAddContact *frmAddContact;
//---------------------------------------------------------------------------
#endif
