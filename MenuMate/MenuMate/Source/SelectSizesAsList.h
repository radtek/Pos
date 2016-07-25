//---------------------------------------------------------------------------

#ifndef SelectSizesAsListH
#define SelectSizesAsListH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ExtCtrls.hpp>

#include "DeviceRealTerminal.h"

#include "ZForm.h"
#include "TouchBtn.h"
#include "TouchControls.h"

#include <item_management_network_messaging_fwd.hh>
#include <Grids.hpp>

//---------------------------------------------------------------------------
class TItemSizeBtn : public TTouchBtn//TCustomStaticText
{
private:
public:
	__fastcall TItemSizeBtn(Classes::TComponent* AOwner);
	TItemSize *ptrSize;
//	TItemCompleteSub *ptrSubItem;
	bool Selected;
};

class TfrmSelectSizes : public TZForm
{
	friend TZForm;
__published:	// IDE-managed Components
        TPanel *pnlCondiments;
        TPanel *pnlSide;
        TTouchBtn *btnMsgUp;
        TTouchBtn *btnMsgDown;
        TTouchBtn *tbtnCancel;
        TStringGrid *ItemSizeGrid;
        void __fastcall FormShow(TObject *Sender);
        void __fastcall BtnCancelClick(TObject *Sender);
        void __fastcall btnMsgDownClick(TObject *Sender);
        void __fastcall btnMsgUpClick(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormDestroy(TObject *Sender);
        void __fastcall tbtnCancelMouseClick(TObject *Sender);
        void __fastcall ItemSizeDisplayDrawCell(TObject *Sender, int ACol, int ARow, TRect &Rect,
            TGridDrawState State);
        void __fastcall ItemSizeDisplaySelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
protected:
        void __fastcall WMDisplayChange(TWMDisplayChange& Message);
        BEGIN_MESSAGE_MAP
        MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
        MESSAGE_HANDLER(item_management::network_messaging::refresh_item_display,
                     Messages::TMessage,
                     ForwardItemDefinitionRefresh);
        END_MESSAGE_MAP(TComponent)
private:	// User declarations
   int ButtonWidth;
	int ButtonHeight;

   void ForwardItemDefinitionRefresh(Messages::TMessage &message);

	__fastcall TfrmSelectSizes(TComponent* Owner);
//	Database::TDBTransaction &DBTransaction;
    void SetColumnWidths();
    void RefreshDisplay();
public:		// User declarations

    TListSizeContainer *Sizes;
    unsigned long SetMenuMask;
    int SelectedItemSizeGrid;
  //  int Preprocess(bool PickingSetMenuItem = false);

//   TListSideContainer *ForcedSidesList;
//	TItemComplete *MasterItem;
   TItemSizeBtn *NextButton;
   TItemSizeBtn *CloseButton;
};

#endif



