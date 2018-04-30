//---------------------------------------------------------------------------

#ifndef EFTPOSConfigurationH
#define EFTPOSConfigurationH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmEFTPOSConfig : public TZForm
{
    friend TZForm;
__published:	// IDE-managed Components
    TPanel *MainPanel;
    TPanel *SidePanel;
    TTouchBtn *tbEFTPOSURL;
    TTouchBtn *tbAPIKey;
    TTouchBtn *tbDeviceID;
    TTouchBtn *tbOK;
    void __fastcall FormShow(TObject *Sender);
    void __fastcall tbOKMouseClick(TObject *Sender);
    void __fastcall tbEFTPOSURLMouseClick(TObject *Sender);
    void __fastcall tbAPIKeyMouseClick(TObject *Sender);
    void __fastcall tbDeviceIDMouseClick(TObject *Sender);
private:	// User declarations
    __fastcall TfrmEFTPOSConfig(TComponent* Owner);
    void UpdateGUI();
public:		// User declarations
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmEFTPOSConfig *frmEFTPOSConfig;
//---------------------------------------------------------------------------
#endif
