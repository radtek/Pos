//---------------------------------------------------------------------------

#ifndef PasswordH
#define PasswordH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmPassword : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label1;
	TEdit *edPassword;
	TButton *btnOk;
	TButton *btnCancel;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmPassword(TComponent* Owner);
	bool InputQuery(AnsiString &Text);
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmPassword *frmPassword;
//---------------------------------------------------------------------------
#endif
