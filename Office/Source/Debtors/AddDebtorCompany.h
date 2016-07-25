//---------------------------------------------------------------------------

#ifndef AddDebtorCompanyH
#define AddDebtorCompanyH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <DB.hpp>
#include <DBCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <Mask.hpp>
//---------------------------------------------------------------------------
class TfrmAddDebtorCompany : public TForm
{
__published:	// IDE-managed Components
	TDataSource *dsCompany;
	TIBDataSet *dtCompanyEdit;
	TGroupBox *GroupBox2;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label16;
	TDBMemo *dbmLocationAddress;
	TDBMemo *dbmPostalAddress;
	TDBEdit *dbePhone;
	TDBEdit *dbeEmail;
	TDBEdit *dbeFax;
	TDBEdit *dbeMobile;
	TDBEdit *dbeCompanyName;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TDBMemo *DBMemo1;
	TLabel *Label1;
	TIBTransaction *Transaction;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall btnOkClick(TObject *Sender);
private:	// User declarations
	virtual int __fastcall ShowModal(void);
public:		// User declarations
	__fastcall TfrmAddDebtorCompany(TComponent* Owner);
	int AddCompany();
	int EditCompany(int iCompanyKey);
	int CompanyKey;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddDebtorCompany *frmAddDebtorCompany;
//---------------------------------------------------------------------------
#endif
