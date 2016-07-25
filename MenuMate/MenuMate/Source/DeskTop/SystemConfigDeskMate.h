//---------------------------------------------------------------------------

#ifndef SystemConfigDeskMateH
#define SystemConfigDeskMateH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
#define MMBaseKey "\\Software\\IQWorks\\MenuMate\\"

//---------------------------------------------------------------------------
class TfrmSystemConfig : public TForm
{
__published:	// IDE-managed Components
   TPanel *pButtons;
	TTouchBtn *tbOk;
	TTouchBtn *tbInterbase;
	TTouchBtn *tbDBLocation;
   TPanel *Panel1;
	TTouchBtn *TouchButton2;
   void __fastcall tbInterbaseClick(TObject *Sender);
   void __fastcall TouchButton2Click(TObject *Sender);
   void __fastcall tbOkClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall FormShow(TObject *Sender);
	void __fastcall tbDBLocationClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
   __fastcall TfrmSystemConfig(TComponent* Owner);
};
//---------------------------------------------------------------------------
#endif
