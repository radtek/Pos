//---------------------------------------------------------------------------

#ifndef SelectItemH
#define SelectItemH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <Buttons.hpp>
#include <ExtCtrls.hpp>
#include <Grids.hpp>
#include "enum.h"
#include <vector.h>
#include "MM_DBCore.h"
#include "Modules.h"
#include "ZForm.h"
#include "GroupGUI.h"
#include "TouchControls.h"
#include "TouchKeyboard.h"
#include "TouchBtn.h"
#include "TouchNumpad.h"
#include "TouchGrid.h"
#include "TouchPages.h"

#include "DeviceRealTerminal.h"
#include "StringTools.h"

//---------------------------------------------------------------------------

enum eItemSearchType {eSearchByItemName, eSearchByItemBarcode};

class TfrmSelectItem : public TZForm
{
    friend TZForm;

    __published:	// IDE-managed Components
        TPanel *pnlMain;
        TPanel *pnlKeyboard;
        TPanel *pnlSelectedItemDetails;
        TPanel *pnlSearchCriteria;
        TPanel *pnlOuterMargin;
        TPanel *pnlList;
        TPanel *pnlScrollButtons;
        TPanel *pnlFunctionButtons;
        TPanel *pnlMargin;
        TPanel *pnlScrollButtonMargin;
        TPanel *pnlSearchButtonMargin;
        TMMTouchKeyboard *tpnlKeyboard;
        TTouchNumpad *tpnlNumKeypad;
        TTouchBtn *btnSearch;
        TTouchBtn *btnClear;
        TTouchBtn *tbtnScrollDown;
        TTouchBtn *tbtnScrollUp;
        TTouchPages *tpItemDetails;
        TTouchSheet *tsItemDetails;
        TListBox *lbItemText;
        TListBox *tListItems;
        TTouchBtn *tbtnCancel;
        TTouchBtn *tbtnOk;
        TTouchBtn *tbtnSearchByName;
        TTouchBtn *tbtnSearchByBarcode;
        TTouchBtn *tbtnInterface;
        TEdit *tedtSearchText;

        void __fastcall FormShow(TObject *Sender);
        void __fastcall FormResize(TObject *Sender);
        void __fastcall btnOkClick(TObject *Sender);
        void __fastcall FormCreate(TObject *Sender);
        void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
        void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
        void __fastcall tbtnClearClick(TObject *Sender);
        void __fastcall tbtnSearchByNameClick(TObject *Sender);
        void __fastcall tbtnSearchByBarcodeClick(TObject *Sender);
        void __fastcall tpnlKeyboardClick(TObject *Sender, char Char);
        void __fastcall btnSearchMouseClick(TObject *Sender);
        void __fastcall tpnlNumKeypadClick(TObject *Sender, TNumpadKey Key);
        void __fastcall tbtnInterfaceMouseClick(TObject *Sender);
        void __fastcall btnClearMouseClick(TObject *Sender);
        void __fastcall tListItemsClick(TObject *Sender);
        void __fastcall tbtnScrollUpAutoRepeat(TObject *Sender);
        void __fastcall tbtnScrollDownAutoRepeat(TObject *Sender);
        void __fastcall pnlScrollButtonsResize(TObject *Sender);
        void __fastcall tbtnCancelMouseClick(TObject *Sender);
        void __fastcall tedtSearchTextChange(TObject *Sender);
        void __fastcall tedtSearchTextKeyPress(TObject *Sender, wchar_t &Key);
        void __fastcall tListItemsDblClick(TObject *Sender);

    private:
        __fastcall TfrmSelectItem(TComponent* Owner);
        AnsiString SearchText;
        eVisibleInterface CurrentVisibleInterface;
        bool QuickSearch(AnsiString SearchString);
        void DisplayData();
        eItemSearchType SearchType;
        bool SelectionVisible;
        TItem *SelectedItem;
        void FetchMenuItems(Database::TDBTransaction &DBTransaction);
        void  SetSelectedItem();
        int GetItemKeyByItemSize(int itemSizeKey);
        void LoadSelectedItemInformation();
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
        bool ItemIncrementalSearch;
        int LastSearchIndex;
        bool MustHaveValidItem;
        bool StartWithShiftDown;
        void UpdateInterface();
        std::pair<TItem*, TItemSize*> GetSelectedItemAndItemSize();
};

#endif
