//---------------------------------------------------------------------------

#ifndef DealEditorH
#define DealEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "ItemSizeCategory.h"
#include "IStoredDeal.h"
//---------------------------------------------------------------------------
class TfrmDealEditorForm : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *BackgroundPanel;
	TTouchBtn *btnName;
	TTouchBtn *btnPriority;
	TTouchBtn *btnCategory;
	TTouchBtn *btnPriceLevels;
	TPanel *OkCancelPanel;
	TTouchBtn *btnOk;
	TTouchBtn *btnCancel;
	void __fastcall btnCancelMouseClick(TObject *Sender);
	void __fastcall btnNameMouseClick(TObject *Sender);
	void __fastcall btnPriorityMouseClick(TObject *Sender);
	void __fastcall btnCategoryMouseClick(TObject *Sender);
	void __fastcall btnPriceLevelsMouseClick(TObject *Sender);
	void __fastcall btnOkMouseClick(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
private:	// User declarations


	Database::TDBControl &DBControl;

	UnicodeString dealName;
	int priority;
	int dealCategoryKey;
	UnicodeString dealCategoryName;
	// Price Levels

	IStoredDeal *deal;

	void ok();
	void cancel();
	void setName();
	void setPriority();
	void setCategory();
    void setPriceLevels();
public:		// User declarations
    __fastcall TfrmDealEditorForm(TComponent* Owner, Database::TDBControl &inDBControl);
	void SetDeal(IStoredDeal *inDeal);
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
