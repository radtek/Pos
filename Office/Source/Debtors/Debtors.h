//---------------------------------------------------------------------------

#ifndef DebtorsH
#define DebtorsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "VirtualTrees.hpp"
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <DB.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <IBDatabase.hpp>
#include <Menus.hpp>
#include <Dialogs.hpp>
#include <IBSQL.hpp>
#include <map>
#include <vector>
//---------------------------------------------------------------------------
enum TDebtorsMode	{ mmEdit, mmSelect };
//---------------------------------------------------------------------------
namespace Debtors
{
class Debtor
{
public:
	int Key;
	AnsiString Name;
	AnsiString FirstName;
	AnsiString MiddleName;
	AnsiString LastName;
	AnsiString CustomerNumber;
	AnsiString Phone;
	AnsiString PhoneExt;
	AnsiString Fax;
	AnsiString Mobile;
	AnsiString Email;
	AnsiString LocationAddress;
	AnsiString PostalAddress;
};
//---------------------------------------------------------------------------
//typedef std::map<int, Debtor> DebtorList;
typedef std::vector<Debtor> DebtorList;
//---------------------------------------------------------------------------
class FindDebtorPredicate
{
public:
	FindDebtorPredicate(int key) : _key(key) {}

	bool operator() (const Debtors::Debtor& d) const
	{
		return d.Key == _key;
	}
private:
	int _key;
};
//---------------------------------------------------------------------------
class Company
{
public:
	Company() : Key(0) {}
	
	int Key;
	AnsiString Name;
	AnsiString Phone;
	AnsiString Fax;
	AnsiString Mobile;
	AnsiString Email;
	AnsiString LocationAddress;
	AnsiString PostalAddress;
	AnsiString Note;

	DebtorList Debtors;
};
//---------------------------------------------------------------------------
class CompanyKey
{
public:
	CompanyKey() : Key(0) {}

	CompanyKey(int key) : Key(key) {}

	CompanyKey(int key, const AnsiString& name) :
		Key(key),
		Name(name) {}

	int Key;
	AnsiString Name;

	bool operator < (const CompanyKey& other) const
	{
		if (other.Name == "")
		{
			return Key < other.Key;
		}
		else
		{
			return (Name == other.Name && Key < other.Key) ||
				(Name < other.Name);
		}
	}
};
//---------------------------------------------------------------------------
typedef std::map<CompanyKey, Debtors::Company> CompanyDebtors;
//typedef std::vector<Debtors::Company> CompanyDebtors;
//---------------------------------------------------------------------------
/*class FindCompanyPredicate
{
public:
	FindCompanyPredicate(int key) : _key(key) {}

	bool operator() (const Debtors::Company& c) const
	{
		return c.Key == _key;
	}
private:
	int _key;
};*/
//---------------------------------------------------------------------------
}
class TfrmDebtors : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TSplitter *Splitter1;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TPanel *Panel10;
	TBitBtn *btnClose;
	TBitBtn *btnFind;
	TPageControl *pcDebtors;
	TTabSheet *tsCompanies;
	TTabSheet *tsDebtors;
	TCheckBox *chbShowCompanies;
	TBitBtn *btnOk;
	TTreeView *tvDebtors;
	TIBQuery *qrDebtors;
	TIBTransaction *Transaction;
	TPopupMenu *pmTreeView;
	TMenuItem *miNewCompany;
	TMenuItem *miNewDebtor;
	TMenuItem *N1;
	TMenuItem *miEdit;
	TMenuItem *miRename;
	TMenuItem *miDelete;
	TBitBtn *btnDeleteDebtor;
	TBitBtn *btnEditDebtor;
	TButton *btnAddDebtor;
	TButton *btnAddCompany;
	TBitBtn *btnEditCompany;
	TBitBtn *btnDeleteCompany;
	TMenuItem *N2;
	TPanel *pnlCompanyDetails;
	TLabel *Label19;
	TLabel *lbeCompanyName;
	TLabel *lbeCompanyPhone;
	TLabel *Label20;
	TLabel *Label22;
	TLabel *Label23;
	TLabel *Label21;
	TLabel *Label24;
	TLabel *Label25;
	TLabel *Label26;
	TMemo *memCompanyNote;
	TMemo *memCompanyPostalAddress;
	TMemo *memCompanyLocationAddress;
	TLabel *lbeCompanyEMail;
	TLabel *lbeCompanyMobile;
	TLabel *lbeCompanyFax;
	TPanel *pnlDebtorDetails;
	TLabel *lbeFirstName;
	TLabel *lbeMiddleName;
	TLabel *lbeLastName;
	TLabel *lbeDebtorPhone;
	TLabel *lbeDebtorFax;
	TLabel *lbeDebtorMobile;
	TLabel *lbeDebtorEMail;
	TLabel *Label11;
	TLabel *Label13;
	TLabel *Label12;
	TLabel *Label10;
	TLabel *Label18;
	TLabel *Label17;
	TLabel *Label16;
	TLabel *Label14;
	TLabel *Label15;
	TMemo *memDebtorPostalAddress;
	TMemo *memDebtorLocationAddress;
	TLabel *Label4;
	TLabel *lbeDebtorPhoneExt;
	TFindDialog *FindDialog;
	TIBQuery *qrDeleteDebtor;
	TIBQuery *qrDebtorInvoices;
	TBitBtn *btnCancel;
	TIBSQL *sqlCheckDebtors;
	TLabel *Label1;
	TLabel *lbeCustomerNumber;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall chbShowCompaniesClick(TObject *Sender);
	void __fastcall tvDebtorsChange(TObject *Sender, TTreeNode *Node);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall miEditClick(TObject *Sender);
	void __fastcall tvDebtorsDblClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnAddDebtorClick(TObject *Sender);
	void __fastcall btnEditClick(TObject *Sender);
	void __fastcall miNewDebtorClick(TObject *Sender);
	void __fastcall tvDebtorsMouseDown(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall btnAddCompanyClick(TObject *Sender);
	void __fastcall miNewCompanyClick(TObject *Sender);
	void __fastcall btnDeleteDebtorClick(TObject *Sender);
	void __fastcall btnFindClick(TObject *Sender);
	void __fastcall FindDialogFind(TObject *Sender);
private:	// User declarations
	void LoadTree();
	Debtors::CompanyDebtors companies;
public:		// User declarations
	__fastcall TfrmDebtors(TComponent* Owner);
   bool HasMembershipInstalled(bool ShowTheMessage);

	TDebtorsMode DebtorsMode;
	int DebtorKey;
	int CompanyKey;
	AnsiString DebtorName;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDebtors *frmDebtors;
//---------------------------------------------------------------------------
#endif
