//---------------------------------------------------------------------------

#ifndef helpH
#define helpH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmHelp : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TButton *btnClose;
	TMemo *Memo1;
	void __fastcall btnCloseClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmHelp(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmHelp *frmHelp;
//---------------------------------------------------------------------------
#endif
