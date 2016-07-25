//---------------------------------------------------------------------------

#ifndef DiscountsH
#define DiscountsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "MM_DBCore.h"
#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
//---------------------------------------------------------------------------
enum DiscountListOrder {edloPriority,edloAppearance};

class TfrmDiscounts : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *Panel28;
	TPanel *pnlLabel;
	TPanel *Panel2;
   TTouchGrid *tgridSelection;
   TTouchBtn *tbtnOk;
   TTouchBtn *btnAddDiscount;
   TTouchBtn *tbtnEditDiscount;
   TTouchBtn *btnDelete;
   TTouchBtn *tbtnAssignCard;
   TTouchBtn *tbtnClearCard;
   TPanel *Panel3;
   TTouchGrid *tgListOrder;
   TTouchGrid *tgMove;
   TTouchBtn *TouchBtn1;
	TPanel *Panel4;
    TTouchBtn *tbtnDiscountGroup;
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormShow(TObject *Sender);
	void __fastcall btnDeleteClick(TObject *Sender);
	void __fastcall btnAddDiscountClick(TObject *Sender);
	void __fastcall tbtnEditDiscountClick(TObject *Sender);
	void __fastcall tbtnClearCardClick(TObject *Sender);
	void __fastcall tbtnAssignCardClick(TObject *Sender);
	void __fastcall Panel19Click(TObject *Sender);
   void __fastcall tgridSelectionMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tgListOrderMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tgMoveMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall TouchBtn1MouseClick(TObject *Sender);
    void __fastcall tbtnDiscountGroupMouseClick(TObject *Sender);
private:	// User declarations


	Database::TDBControl &DBControl;
   int CurrentDiscountKey;
   AnsiString CurrentDiscountName;
   DiscountListOrder ListOrder;

public:		// User declarations
    __fastcall TfrmDiscounts(TComponent* Owner,Database::TDBControl &inDBControl);
	void LoadDiscountList();   
};
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#endif
