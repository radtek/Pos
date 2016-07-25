//---------------------------------------------------------------------------

#ifndef ProductSearchH
#define ProductSearchH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include "TouchBtn.h"
#include "TouchControls.h"
#include "TouchKeyboard.h"
#include "TouchNumpad.h"
#include "TouchPages.h"
#include <ExtCtrls.hpp>
#include <Buttons.hpp>
#include <Grids.hpp>
#include "enum.h"
#include <vector.h>
#include "MM_DBCore.h"
#include "Modules.h"
#include "ZForm.h"
#include "GroupGUI.h"
#include "TouchGrid.h"
//---------------------------------------------------------------------------
enum eItemSearchCategory {eNameSearch, eBarcodeSearch, eNameAndBarCode};
//--------------------------------------------------------------------

class TfrmProductSearch : public TZForm
{
    __published:	// IDE-managed Components
        TPanel *pnlMain;
        TPanel *pnlSelectedItemDetails;
        TPanel *pnlSearchCriteria;
        TPanel *pnlOuterMargin;
        TPanel *pnlList;
        TPanel *pnlScrollButtons;
        TPanel *pnlFunctionButtons;
        TPanel *pnlMargin;
        TPanel *pnlScrollButtonMargin;
        TTouchBtn *tbtnScrollDown;
        TTouchBtn *tbtnScrollUp;
        TTouchPages *tpItemDetails;
        TTouchSheet *tsItemDetails;
        TListBox *lbItemText;
        TListBox *tListItems;
        TTouchBtn *tbtnCancel;
        TTouchBtn *tbtnOk;

        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall btnOkClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall tListItemsClick(TObject *Sender);
        void __fastcall tbtnScrollUpAutoRepeat(TObject *Sender);
        void __fastcall tbtnScrollDownAutoRepeat(TObject *Sender);
        void __fastcall pnlScrollButtonsResize(TObject *Sender);
        void __fastcall tbtnCancelMouseClick(TObject *Sender);
        void __fastcall tListItemsDblClick(TObject *Sender);

    private:
        eVisibleInterface CurrentVisibleInterface;
        bool SelectionVisible;
        TItem *SelectedItem;
        void  SetSelectedItem();
        std::pair<TItem*, TItemSize*> ItemAndItemSize;
        std::auto_ptr<TList> SelectedItems;

    protected:
        void __fastcall WMDisplayChange(TWMDisplayChange& Message);
        void __fastcall CardSwipe(Messages::TMessage& Message);
        BEGIN_MESSAGE_MAP
            MESSAGE_HANDLER(WM_DISPLAYCHANGE, TWMDisplayChange, WMDisplayChange)
            MESSAGE_HANDLER(UWM_CARDSWIPE, Messages::TMessage, CardSwipe);
        END_MESSAGE_MAP(TComponent)

    public:
        __fastcall TfrmProductSearch(TComponent* Owner);
        __fastcall ~TfrmProductSearch();
        bool ItemIncrementalSearch;
        int LastSearchIndex;
        bool MustHaveValidItem;
        bool StartWithShiftDown;
        std::pair<TItem*, TItemSize*> GetSelectedItemAndItemSize();
        bool QuickSearch(AnsiString SearchString);
        void DisplayData();
        eItemSearchCategory SearchType;
        AnsiString SearchText;
        void LoadSelectedItemInformation();
};
//---------------------------------------------------------------------------
#endif
