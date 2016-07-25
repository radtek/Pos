//---------------------------------------------------------------------------
#ifndef AddUserH
#define AddUserH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <DBCtrls.hpp>
#include <ExtCtrls.hpp>
#include <Mask.hpp>
#include <Db.hpp>
#include <IBCustomDataSet.hpp>
#include <IBDatabase.hpp>
#include <DB.hpp>
#include <Buttons.hpp>
#include <ComCtrls.hpp>
#include <CheckLst.hpp>
#include "AuthenticationService.h"
//---------------------------------------------------------------------------
enum TUserMode { umEdit, umAdd };
class TfrmAddUser : public TForm
{
__published:	// IDE-managed Components
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TIBTransaction *Transaction;
	TIBDataSet *dtUsers;
	TDataSource *dsUsers;
	TPageControl *PageControl1;
	TTabSheet *tsUserDetails;
	TTabSheet *tsOldSecurity;
	TLabel *Label7;
	TLabel *Label15;
	TDBEdit *dbePIN;
	TEdit *edPINConfirm;
	TLabel *Label2;
	TDBEdit *dbeLoginID;
	TLabel *Label13;
	TLabel *Label14;
	TLabel *Label12;
	TDBEdit *dbeLastName;
	TDBEdit *dbeMiddleName;
	TDBEdit *dbeFirstName;
	TLabel *Label16;
	TLabel *Label1;
	TDBEdit *dbeInitials;
	TLabel *Label4;
	TDBMemo *DBMemo1;
	TLabel *Label3;
	TDBEdit *DBEdit2;
	TLabel *Label5;
	TDBEdit *DBEdit3;
	TLabel *Label6;
	TDBEdit *DBEdit4;
	TLabel *Label8;
	TDBEdit *DBEdit6;
	TLabel *Label9;
	TLabel *Label10;
	TDBEdit *dbeNextOrderNumber;
	TDBEdit *DBEdit8;
	TLabel *Label11;
	TDBText *dbtLoginID;
	TTabSheet *tsSecurity;
	TLabel *Label17;
	TDBEdit *dbePassword;
	TLabel *Label18;
	TEdit *edPasswordConfirm;
	TLabel *Label19;
	TCheckListBox *lbSecurityRoles;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall dbeKeyPress(TObject *Sender, char &Key);
	void __fastcall dbePINChange(TObject *Sender);
	void __fastcall dbePINKeyPress(TObject *Sender, char &Key);
private:	// User declarations
	bool PINChanged;
	bool passwordChanged;
	std::set<Security::Role> roles;
	std::set<Security::Role> userRoles;
public:		// User declarations
	__fastcall TfrmAddUser(TComponent* Owner);
	TUserMode UserMode;
	AnsiString UserID;
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmAddUser *frmAddUser;
//---------------------------------------------------------------------------
#endif
