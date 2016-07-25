//---------------------------------------------------------------------------

#ifndef SelectCategoryH
#define SelectCategoryH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
//---------------------------------------------------------------------------
class TfrmSelectCategory : public TForm
{
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel8;
	TPanel *Panel9;
	TLabel *lbeTitle;
	TBitBtn *btnCancel;
	TBitBtn *BitBtn1;
	TListBox *lbCategory;
	void __fastcall lbCategoryDblClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TfrmSelectCategory(TComponent* Owner);
};
//---------------------------------------------------------------------------
//extern PACKAGE TfrmSelectCategory *frmSelectCategory;
//---------------------------------------------------------------------------
#endif
