//---------------------------------------------------------------------------

#ifndef DiscountEditorH
#define DiscountEditorH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "ManagerDiscount.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
//---------------------------------------------------------------------------
class TfrmDiscountEdit : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TGroupBox *GroupBox1;
	TMemo *memDescription;
	TPanel *Panel2;
	TRadioGroup *rgDiscountType;
   TTouchBtn *tbRounding;
   TTouchBtn *TouchBtn1;
   TTouchBtn *TouchBtn2;
   TTouchBtn *btnName;
   TTouchBtn *btnAmount;
   TTouchBtn *tbtnFilter;
   TTouchBtn *tbtnDiscountID;
   TTouchBtn *tbtnPriority;
   TTouchBtn *tbtnAppearanceOrder;
	TTouchBtn *tbMembersExempt;
	TTouchBtn *tbMembersOnly;
	TPanel *Panel3;
	TPanel *Panel4;
	TTouchBtn *tbMaxValue;
	TTouchBtn *tbtnDiscountGroup;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall memDescriptionMouseUp(TObject *Sender, TMouseButton Button,
          TShiftState Shift, int X, int Y);
	void __fastcall Panel19Click(TObject *Sender);
	void __fastcall Panel20Click(TObject *Sender);
	void __fastcall btnNameClick(TObject *Sender);
	void __fastcall btnAmountClick(TObject *Sender);
	void __fastcall tbtnFilterClick(TObject *Sender);
	void __fastcall rgDiscountTypeClick(TObject *Sender);
   void __fastcall tbRoundingMouseClick(TObject *Sender);
   void __fastcall tbtnDiscountIDMouseClick(TObject *Sender);
   void __fastcall tbtnPriorityMouseClick(TObject *Sender);
   void __fastcall tbtnAppearanceOrderMouseClick(TObject *Sender);
	void __fastcall tbMembersOnlyMouseClick(TObject *Sender);
	void __fastcall tbMembersExemptMouseClick(TObject *Sender);
	void __fastcall tbMaxValueMouseClick(TObject *Sender);
	void __fastcall tbtnDiscountGroupMouseClick(TObject *Sender);


private:	// User declarations
	__fastcall TfrmDiscountEdit(TComponent* Owner,Database::TDBControl &inDBControl);

	Database::TDBControl &DBControl;
public:		// User declarations
	TDiscount CurrentDiscount;
	int DiscountKey;
	bool Editing;
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmDiscountEdit *frmDiscountEdit;
//---------------------------------------------------------------------------
#endif
