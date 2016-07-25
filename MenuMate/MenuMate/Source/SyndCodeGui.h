//---------------------------------------------------------------------------

#ifndef SyndCodeGuiH
#define SyndCodeGuiH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "ManagerSyndCode.h"

//---------------------------------------------------------------------------
class TfrmSyndCodeGui : public TZForm
{
__published:	// IDE-managed Components
   TPanel *pnlMajor;
   TPanel *pnlLabel;
   TPanel *pnlMinor;
   TTouchBtn *btnClose;
   TTouchBtn *TouchBtn4;
   TTouchBtn *tbtnName;
   TTouchBtn *tbtnEnabled;
   TTouchBtn *TouchBtn3;
   TTouchBtn *tbtnEncryption;
   void __fastcall TouchBtn3MouseClick(TObject *Sender);
   void __fastcall tbtnNameMouseClick(TObject *Sender);
   void __fastcall tbtnEnabledMouseClick(TObject *Sender);
   void __fastcall btnCloseMouseClick(TObject *Sender);
   void __fastcall TouchBtn4MouseClick(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall tbtnEncryptionMouseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmSyndCodeGui(TComponent* Owner);

   TSyndCode SyndCode;   
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
