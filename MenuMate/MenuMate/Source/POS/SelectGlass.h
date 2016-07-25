//---------------------------------------------------------------------------
#ifndef SelectGlassH
#define SelectGlassH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchGrid.h"
#include <vector>
#include "SelectDish.h"
#include "ZForm.h"
#include "ListSizeContainer.h"

#include <item_management_network_messaging_fwd.hh>

//---------------------------------------------------------------------------
class TfrmSelectGlass : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
	TPanel *Panel1;
	TPanel *pnlGlasses;
   TPanel *Panel2;
   TTouchBtn *TouchBtn1;
   TStaticText *lbeTitle;
   TTouchGrid *tgridItemSizes;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall FormResize(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
	void __fastcall FormHide(TObject *Sender);
   void __fastcall CancelClick(TObject *Sender);
   void __fastcall tgridItemSizesMouseDown(TObject *Sender,
          TMouseButton Button, TShiftState Shift, TGridButton *GridButton,
          int X, int Y);
protected:
	void __fastcall WMDisplayChange(TWMDisplayChange& Message);
   BEGIN_MESSAGE_MAP
     MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
     MESSAGE_HANDLER(item_management::network_messaging::refresh_item_display,
                     Messages::TMessage,
                     ForwardItemDefinitionRefresh);
   END_MESSAGE_MAP(TComponent)
private:	// User declarations
	__fastcall TfrmSelectGlass(TComponent* Owner);

   bool is_set_menu;

   void ForwardItemDefinitionRefresh(Messages::TMessage &message) const;
public:		// User declarations

   TListSizeContainer *Sizes;
   int SelectedGlass;
   unsigned long SetMenuMask;
   int Preprocess(bool PickingSetMenuItem = false);
   virtual __fastcall ~TfrmSelectGlass();
};
//---------------------------------------------------------------------------
#endif
