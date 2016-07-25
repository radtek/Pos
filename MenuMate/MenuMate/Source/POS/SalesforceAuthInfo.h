//---------------------------------------------------------------------------

#ifndef SalesforceAuthInfoH
#define SalesforceAuthInfoH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"
//---------------------------------------------------------------------------
class TfrmSalesforceAuthInfo : public TZForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;	
	TBevel *Bevel2;
	TBevel *Bevel3;
	TBevel *Bevel4;
	TPanel *Panel1;
	TPanel *Panel2;
	TGroupBox *GroupBox1;
	TGroupBox *GroupBox2;
    TLabel *Label1;
    TLabel *Label2;
    TLabel *Label3;
    TLabel *Label4;
    TEdit *txtSFUsername;
    TEdit *txtSFPassword;
    TEdit *txtSFToken;
    TTouchBtn *btnUpdateCredentials;
    TTouchBtn *btnCancel;
	void __fastcall btnUpdateCredentialsMouseClick(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall txtSFUsernameClick(TObject *Sender);
    void __fastcall txtSFPasswordClick(TObject *Sender);
    void __fastcall txtSFTokenClick(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
private:	// User declarations
    bool EditText( UnicodeString inCaption, UnicodeString &inText );
    bool isValid();
    void displaySalesforceCredentials();
    bool validateUsername();
    bool validatePassword();
    bool validateSecurityToken();
    UnicodeString EmptyUnicodeString();
public:		// User declarations
    __fastcall TfrmSalesforceAuthInfo(TComponent* Owner);
    static TfrmSalesforceAuthInfo* Create( TComponent* Owner );

    UnicodeString Username;
    UnicodeString Password;
    UnicodeString SecurityToken;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSalesforceAuthInfo *frmSalesforceAuthInfo;
//---------------------------------------------------------------------------
#endif
