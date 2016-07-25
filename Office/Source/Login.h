//---------------------------------------------------------------------------
#ifndef LoginH
#define LoginH
#include <Buttons.hpp>
#include <Classes.hpp>
#include <Controls.hpp>
#include <DB.hpp>
#include <ExtCtrls.hpp>
#include <IBCustomDataSet.hpp>
#include <IBQuery.hpp>
#include <StdCtrls.hpp>
//---------------------------------------------------------------------------
enum TLoginMode { lmTestAccess, lmLogin, lmLoginAdmin };
//---------------------------------------------------------------------------
struct TLoginInfo
{
	AnsiString UserID;
	AnsiString UserName;
	AnsiString FirstName;
	AnsiString MiddleName;
	AnsiString LastName;
	AnsiString Initials;
	AnsiString Address;
	AnsiString Phone;
	AnsiString EMail;
//	TAccessLevel Access;
	AnsiString SwipeCard;
	AnsiString NextOrderNumber;
};
//---------------------------------------------------------------------------
class TfrmLogin : public TForm
{
__published:	// IDE-managed Components
	TIBQuery *qrLogin;
	TIBQuery *qrUsers;
	TIBQuery *qrInfo;
	TPanel *Panel2;
	TPanel *Panel3;
	TPanel *Panel4;
	TPanel *Panel5;
	TLabel *lbeInstructions;
	TBitBtn *btnOk;
	TBitBtn *btnCancel;
	TPanel *pnlOldLogin;
	TLabel *Label1;
	TLabel *Label2;
	TEdit *edPIN;
	TComboBox *cbUserName;
	TTimer *Timer1;
	TPanel *pnlNewLogin;
	TLabel *Label3;
	TLabel *Label4;
	TEdit *edPassword;
	TEdit *edUserName;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall edPINKeyPress(TObject *Sender, char &Key);
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall cbUserNameCloseUp(TObject *Sender);
	void __fastcall Timer1Timer(TObject *Sender);
private:	// User declarations
	AnsiString fCurrentUserID;
	AnsiString fCurrentUserName;
	AnsiString fLastAccessUserID;

	bool useOldLogin;

	void InitialiseOldLogin();
	void InitialiseNewLogin();

	void UseOldLogin();
	void UseNewLogin();

public:		// User declarations
	__fastcall TfrmLogin(TComponent* Owner);
	TLoginMode LoginMode;
	__property AnsiString CurrentUserID = {read=fCurrentUserID};
	__property AnsiString CurrentUserName = {read=fCurrentUserName};
	__property AnsiString LastAccessUserID = {read=fLastAccessUserID};
	void LogoutCurrentUser();
	void LogoutAccessUser();
	bool GetUserInfo(TLoginInfo &UserInfo);

	TLoginInfo CurrentUser;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmLogin *frmLogin;
//---------------------------------------------------------------------------
#endif
