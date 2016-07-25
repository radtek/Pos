//---------------------------------------------------------------------------

#ifndef ftpH
#define ftpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Graphics.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmFTP : public TForm
{
__published:	// IDE-managed Components
	TMemo *Memo;
	TLabel *lbeData;
   TTouchBtn *TouchBtn1;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormPaint(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall lbeOpt1Click(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmFTP(TComponent* Owner);
   int Result; // -1 Proc Messages,1 Quit , 2+ Ok. 
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmFTP *frmFTP;
//---------------------------------------------------------------------------
#endif
