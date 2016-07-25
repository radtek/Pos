//---------------------------------------------------------------------------

#ifndef SelectDiscountsH
#define SelectDiscountsH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include "TouchNumpad.h"
#include "MMContactInfo.h"
#include "ZForm.h"
#include "ItemMinorComplete.h"
#include "ItemRedirector.h"
#include <ExtCtrls.hpp>

#include "SHDocVw_OCX.h"
#include <OleCtrls.hpp>
#include <SHDocVw.hpp>

#include <vector>

//---------------------------------------------------------------------------
class TfrmSelectDiscounts : public TZForm
{
__published:	// IDE-managed Components
   TPanel *Panel1;
   TPanel *Panel10;
   TLabel *Label4;
   TPanel *pnlItemDiscountGridContainer;
   TPanel *Panel11;
   TTouchGrid *tgridItemPrice;
   TPanel *Panel5;
   TPanel *Panel15;
   TLabel *Label5;
   TPanel *pnlMiscPrice;
   TPanel *Panel13;
   TLabel *Label1;
   TPanel *Panel8;
   TPanel *Panel16;
   TTouchNumpad *TouchNumpadItemPrice;
   TNumpadDisplay *NumpadDisplayItemPrice;
   TPanel *Panel2;
   TPanel *Panel3;
   TPanel *Panel4;
   TTouchBtn *btnOk;
   TPanel *Panel6;
   TTouchBtn *tbtnReason;
   TTouchBtn *tbtnItemPriceDiscountClear;
   TWebBrowser *webDisplay;
   void __fastcall btnOkMouseClick(TObject *Sender);
   void __fastcall ItemPriceDescriptionClick(TObject *Sender);
   void __fastcall FormResize(TObject *Sender);
   void __fastcall tgridItemPriceMouseClick(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton);
   void __fastcall tbtnItemPriceDiscountClearMouseClick(TObject *Sender);
   void __fastcall TouchNumpadItemPriceClick(TObject *Sender,
          TNumpadKey Key);
   void __fastcall FormShow(TObject *Sender);
private:	// User declarations
   AnsiString DiscountSingleItem(Database::TDBTransaction &DBTransaction,TGridButton *GridButton);
   void __fastcall ApplyDiscount(Database::TDBTransaction &DBTransaction,int DiscountKey, TList *Orders,TDiscountSource DiscountSource = dsMMUser);
   void __fastcall UpdateDiscountDisplay();
   void Navigate(TWebBrowser *WebBrowser,TStringList *Html);

   TItemMinorComplete *SelectedItem;
   TItemRedirector *Redirector;
	TMMContactInfo Member;

	std::vector<bool> original_discount_states;
	Currency          original_price;

    TList *Orders;

public:		// User declarations
   __fastcall TfrmSelectDiscounts(TComponent* Owner);
   void __fastcall Execute(TItemMinorComplete *SelectedItem,TItemRedirector *inRedirector,bool ShowDiscounts,bool ShowPriceAdjsut, TList *OrdersList);
   void UpdateManualDiscount();
	void SetMember(TMMContactInfo inMember);

	bool alterations_have_been_made;

};
//---------------------------------------------------------------------------

//---------------------------------------------------------------------------
#endif
