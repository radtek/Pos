//---------------------------------------------------------------------------

#ifndef InputCategoryH
#define InputCategoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
//---------------------------------------------------------------------------
class TfrmInputCategory : public TForm
{
__published:	// IDE-managed Components
	TLabel *lblCategoryName;
	TEdit *tEditCategory;
	TEdit *tEditGlCode;
	TLabel *lblGlCode;
	TButton *btnOk;
	TButton *btnCancel;
	void __fastcall btnOkClick(TObject *Sender);
	void __fastcall btnCancelClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmInputCategory(TComponent* Owner);
	AnsiString CategoryName;
	AnsiString GlCode;
	bool Result;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmInputCategory *frmInputCategory;
//---------------------------------------------------------------------------
#endif
