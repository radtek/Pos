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
    void __fastcall tbtnPortNumberMouseClick(TObject *Sender);
    void __fastcall tbtnConfigureMouseClick(TObject *Sender);
    void __fastcall tbtnValidateMouseClick(TObject *Sender);
    void __fastcall tbtnCloseMouseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
    __fastcall TfrmSetUpPosPlus(TComponent* Owner);
    void __fastcall FormShow(TObject *Sender);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmSetUpPosPlus *frmSetUpPosPlus;
//---------------------------------------------------------------------------
#endif
