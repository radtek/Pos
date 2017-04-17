//---------------------------------------------------------------------------

#ifndef WalletConfigurationH
#define WalletConfigurationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
class TfrmWalletConfiguration : public TForm
{
__published:	// IDE-managed Components

    TPanel *Panel1;
    TPanel *Panel2;
    TLabel *lbePassword;
    TLabel *lbeUserName;
    TLabel *lbeTerminalId;
    TLabel *lbeSecurityToken;
    TLabel *lbeMechentId;
    TTouchBtn *btnMerchentId;
    TTouchBtn *btnUserName;
    TTouchBtn *btnWalletPassword;
    TTouchBtn *btnTerminalId;
    TTouchBtn *btnCancel;
    TTouchBtn *btnOk;
    TTouchBtn *btnSecurityToken;


    void __fastcall FormShow(TObject *Sender);
    void __fastcall FormResize(TObject *Sender);
    void __fastcall btnMerchentIdMouseClick(TObject *Sender);
    void __fastcall btnSecurityTokenMouseClick(TObject *Sender);
    void __fastcall btnTerminalIdMouseClick(TObject *Sender);
    void __fastcall btnUserNameMouseClick(TObject *Sender);
    void __fastcall btnOkMouseClick(TObject *Sender);
    void __fastcall btnCancelMouseClick(TObject *Sender);
    void __fastcall btnWalletPasswordMouseClick(TObject *Sender);
public:		// User declarations
    __fastcall TfrmWalletConfiguration(TComponent* Owner);
    UnicodeString MerchentId;
    UnicodeString TerminalId;
    UnicodeString WalletUserName;
    UnicodeString WalletPassword;
    UnicodeString WalletSecurityToken;
private:
    UnicodeString GetData(UnicodeString inValue,UnicodeString inCaption,int inMaxLength);
};
//---------------------------------------------------------------------------

#endif
