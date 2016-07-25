//---------------------------------------------------------------------------

#ifndef XeroSettingsH
#define XeroSettingsH
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
class TfrmXeroSettings : public TZForm
{
__published:	// IDE-managed Components
	TBevel *Bevel1;
	TPanel *Panel1;
	TBevel *Bevel2;
	TBevel *Bevel3;
	TBevel *Bevel4;
	TTouchBtn *btnOK;
	TBevel *Bevel5;
	TPanel *Panel2;
	TTouchBtn *btnCancel;
	TGroupBox *GroupBox2;
	TRadioButton *rbXeroLocal;
	TLabel *Label1;
	TRadioButton *rbXeroRemote;
	TLabel *Label2;
	TGroupBox *GroupBox1;
	TLabel *Label4;
	TLabel *Label5;
	TLabel *Label6;
	TLabel *Label10;
	TEdit *edHostName;
	TEdit *edFolderName;
	TEdit *edUserName;
	TEdit *edPassword;
    TGroupBox *GroupBox3;
    TCheckBox *cbSendAwaitingPayment;
	void __fastcall btnOKMouseClick(TObject *Sender);
	void __fastcall btnCancelMouseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall rbXeroLocalClick(TObject *Sender);
	void __fastcall rbXeroRemoteClick(TObject *Sender);
	void __fastcall btnEditHostNameMouseClick(TObject *Sender);
	void __fastcall btnEditFolderNameMouseClick(TObject *Sender);
	void __fastcall btnEditUserNameMouseClick(TObject *Sender);
	void __fastcall btnEditPasswordMouseClick(TObject *Sender);
   // void __fastcall cbSendAwaitingPaymentClick(TObject *Sender);
private:	// User declarations

	bool FValidated;

	UnicodeString FRemoteHostName;
	UnicodeString FRemoteUserName;
	UnicodeString FRemotePassword;
	UnicodeString FLocalFolderName;
	UnicodeString FRemoteFolderName;

	UnicodeString LocalHost();
	UnicodeString POS1();
	UnicodeString XeroInvoices();
	UnicodeString EmptyUnicodeString();

	UnicodeString CheckMachineName(       UnicodeString inMachineName );
	UnicodeString CheckLocalMachineName(  UnicodeString inMachineName );
	UnicodeString CheckRemoteMachineName( UnicodeString inMachineName );

	UnicodeString CheckFolderName(       UnicodeString inFolderName );
	UnicodeString CheckLocalFolderName(  UnicodeString inFolderName );
	UnicodeString CheckRemoteFolderName( UnicodeString inFolderName );

	UnicodeString CheckUserName( UnicodeString inUserName );
	UnicodeString CheckPassword( UnicodeString inPassword );

	UnicodeString readXeroMachineName();
	UnicodeString readXeroFolderPath();
	UnicodeString readXeroUserName();
	UnicodeString readXeroPassword();

	void writeXeroMachineName( UnicodeString inName  );
	void writeXeroFolderPath(  UnicodeString inPath  );
	void writeXeroUserName(    UnicodeString inName  );
	void writeXeroPassword(    UnicodeString inPassw );

	void InitSettings();
	void RefreshSettings();
	void EnableAndClearEdit( bool inEnabled, TEdit* inEdit, UnicodeString inLocalText, UnicodeString &inRemoteText );
	bool LocalHostChecked();
	void InitEdit( bool inLocal, TEdit* inEdit, UnicodeString inLocalText, UnicodeString &inRemoteText );
	void RefreshEdit( bool inLocal, TEdit* inEdit, UnicodeString inLocalText, UnicodeString &inRemoteText );

	bool EditText( UnicodeString inCaption, UnicodeString &inText );

public:		// User declarations
	__fastcall TfrmXeroSettings(TComponent* Owner);

	__property UnicodeString XeroMachineName  = { read = readXeroMachineName, write = writeXeroMachineName };
	__property UnicodeString XeroFolderPath   = { read = readXeroFolderPath,  write = writeXeroFolderPath  };
	__property UnicodeString XeroUserName     = { read = readXeroUserName,    write = writeXeroUserName    };
	__property UnicodeString XeroPassword     = { read = readXeroPassword,    write = writeXeroPassword    };

	void SetAndValidate( AnsiString inMachineName, AnsiString inFolderPath, AnsiString inUserName, AnsiString inPassword );
	void Validate();
    bool AwaitingPayment;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmXeroSettings *frmXeroSettings;
//---------------------------------------------------------------------------
#endif
