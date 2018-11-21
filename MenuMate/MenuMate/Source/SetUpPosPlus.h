//---------------------------------------------------------------------------

#ifndef SetUpPosPlusH
#define SetUpPosPlusH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include "touchbtn.h"
#include "touchcontrols.h"
#include "ZForm.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
enum eFiscalStorage { PosPlus = 1, AustriaFiscal};
class TfrmSetUpPosPlus : public TZForm
{
__published:	// IDE-managed Components
    TPanel *pnlMain;
    TPanel *pnlSide;
    TTouchBtn *tbtnClose;
    TLabel *labelPortNumber;
    TTouchBtn *tbtnConfigure;
    TTouchBtn *tbtnValidate;
    TLabel *labelConfigure;
    TLabel *labelValidate;
    TTouchBtn *tbtnPortNumber;
    TLabel *labelOrganization;
    TTouchBtn *tbtnOrganizationNumber;
    TLabel *labelTerminalId;
    TTouchBtn *tbtnTerminalId;
    void __fastcall tbtnPortNumberMouseClick(TObject *Sender);
    void __fastcall tbtnConfigureMouseClick(TObject *Sender);
    void __fastcall tbtnValidateMouseClick(TObject *Sender);
    void __fastcall tbtnCloseMouseClick(TObject *Sender);
    void __fastcall tbtnOrganizationNumberMouseClick(TObject *Sender);
    void __fastcall tbtnTerminalIdMouseClick(TObject *Sender);
private:	// User declarations
    UnicodeString ShowKeyBoard(int maxLength,UnicodeString value,UnicodeString caption);
    bool AreDetailsProvidedForAustria();
public:		// User declarations
    __fastcall TfrmSetUpPosPlus(TComponent* Owner);
    void __fastcall FormShow(TObject *Sender);
    eFiscalStorage StorageType;
    void ConfigureForMode();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSetUpPosPlus *frmSetUpPosPlus;
//---------------------------------------------------------------------------
#endif
