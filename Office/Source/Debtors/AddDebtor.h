//---------------------------------------------------------------------------

#ifndef AddDebtorH
#define AddDebtorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DBCtrls.hpp>
#include <Mask.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <IBSQL.hpp>
#include <IBQuery.hpp>
//---------------------------------------------------------------------------
enum TAdjustMode { amAddDebtor, amEditDebtor };
//---------------------------------------------------------------------------
class TfrmAddDebtor : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TGroupBox *GroupBox2;
	TLabel *Label10;
	TLabel *Label11;
	TLabel *Label12;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label15;
	TLabel *Label4;
	TLabel *Label16;
	TLabel *Label17;
	TLabel *Label18;
	TDBMemo *DBMemo4;
	TDBMemo *DBMemo5;
	TDBEdit *DBEdit2;
	TDBEdit *DBEdit7;
	TDBEdit *DBEdit8;
	TDBEdit *DBEdit9;
	TDBEdit *DBEdit1;
	TDBEdit *dbeFirstName;
	TDBEdit *DBEdit11;
	TDBEdit *DBEdit12;
	TIBTransaction *Transaction;
	TIBDataSet *dtDebtorEdit;
	TDataSource *dsDetors;
	TButton *btnAddCompany;
	TIBQuery *qrCompanies;
	TDataSource *dsCompanies;
	TComboBox *cbCompany;
	TLabel *Label1;
	TLabel *Label2;
	TDBEdit *dbeName;
	TBitBtn *btnCopyCompany;
	TLabel *Label3;
	TDateTimePicker *dtpDateOfBirth;
	TLabel *Label5;
	TComboBox *cbSex;
	TLabel *Label6;
	TComboBox *cbTitle;
	TCheckBox *chbAllowInvoice;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnAddCompanyClick(TObject *Sender);
	void __fastcall dbeFirstNameExit(TObject *Sender);
	void __fastcall btnCopyCompanyClick(TObject *Sender);
private:	// User declarations
	AnsiString GetMemberNumber();
	int GetSiteID();
    void UpdateMembershipSubsDetails(int debtorKey);
    bool CheckLocalMembership();
    bool CheckLoyaltymateEnabled();
    void UpdateContactsPointsRules(int pointsRules,int debtorKey);   
public:		// User declarations
	__fastcall TfrmAddDebtor(TComponent* Owner);
	TAdjustMode Mode;
	int DebtorKey;
	int CompanyKey;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmAddDebtor *frmAddDebtor;
//---------------------------------------------------------------------------
#endif
