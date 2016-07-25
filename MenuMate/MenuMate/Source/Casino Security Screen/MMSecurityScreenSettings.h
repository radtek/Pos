//---------------------------------------------------------------------------

#ifndef MMSecurityScreenSettingsH
#define MMSecurityScreenSettingsH

//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>

#include "TouchBtn.h"
#include "TouchControls.h"
#include <ExtCtrls.hpp>

#include "ZForm.h"

#include "DBSecurityScreenSettings.h"
#include <CheckLst.hpp>

#include <map>

//---------------------------------------------------------------------------

const cb_FONT_SIZE = 12;
const cb_HEIGHT    = 17;
const cb_VSPACE    = ( cb_HEIGHT + 10 );

struct TScrollBoxInfo
{
    TScrollBoxInfo() { Position = 0; MaxPosition = 0; }
    //TScrollBoxInfo( int inPos, int inMaxPos ) { Position = inPos; MaxPosition = inMaxPos; }

    int Position;
    int MaxPosition;
};

typedef std::map<UnicodeString, AUDIT_FILTER_LIST> ALL_AUDIT_FILTERS;

class TfrmMMSecurityScreenSettings : public TZForm
{
__published:	// IDE-managed Components
    TPanel *pLefSep;
    TPanel *pRightSep;
    TPanel *pBottomSep;
    TPanel *pTopSep;
    TPanel *pMiddle;
    TPanel *pBtnContainer;
    TPanel *pBtnSep;
    TPanel *pControls;
    TLabel *Label1;
    TLabel *Label2;
    TGroupBox *gbLocationFilter;
    TGroupBox *gbTerminalFilter;
    TGroupBox *gbCashierFilter;
    TGroupBox *gbPaymentTypeFilter;
    TTouchBtn *btnOK;
    TTouchBtn *btnCancel;
    TScrollBox *sbTerminalFilter;
    TPanel *pTerminalScroll;
    TTouchBtn *tbtnTerminalScrollDown;
    TTouchBtn *tbtnTerminalScrollUp;
    TPanel *pSelectAllTerminals;
    TCheckBox *cbTerminalSelectAll;
    TCheckBox *cbTerminalUnselectAll;
    TBevel *Bevel1;
    TPanel *pLocationScroll;
    TTouchBtn *tbtnLocationScrollDown;
    TTouchBtn *tbtnLocationScrollUp;
    TScrollBox *sbLocationFilter;
    TPanel *Panel2;
    TCheckBox *cbLocationSelectAll;
    TCheckBox *cbLocationUnselectAll;
    TBevel *Bevel2;
    TPanel *pCashierScroll;
    TTouchBtn *tbtnCashierScrollDown;
    TTouchBtn *tbtnCashierScrollUp;
    TScrollBox *sbCashierFilter;
    TPanel *Panel4;
    TCheckBox *cbCashierSelectAll;
    TCheckBox *cbCashierUnselectAll;
    TBevel *Bevel3;
    TPanel *pPaymentTypeScroll;
    TTouchBtn *tbtnPaymentTypeScrollDown;
    TTouchBtn *tbtnPaymentTypeScrollUp;
    TScrollBox *sbPaymentTypeFilter;
    TPanel *Panel6;
    TCheckBox *cbPaymentTypeSelectAll;
    TCheckBox *cbPaymentTypeUnselectAll;
    TBevel *Bevel4;
    TTouchBtn *tbtnReload;
    void __fastcall btnOKAutoRepeat(TObject *Sender);
    void __fastcall btnCancelAutoRepeat(TObject *Sender);
    void __fastcall cbLocationSelectAllClick(TObject *Sender);
    void __fastcall cbLocationUnselectAllClick(TObject *Sender);
    void __fastcall cbTerminalSelectAllClick(TObject *Sender);
    void __fastcall cbTerminalUnselectAllClick(TObject *Sender);
    void __fastcall cbCashierSelectAllClick(TObject *Sender);
    void __fastcall cbCashierUnselectAllClick(TObject *Sender);
    void __fastcall cbPaymentTypeSelectAllClick(TObject *Sender);
    void __fastcall cbPaymentTypeUnselectAllClick(TObject *Sender);
    void __fastcall tbtnLocationScrollUpMouseClick(TObject *Sender);
    void __fastcall tbtnLocationScrollDownMouseClick(TObject *Sender);
    void __fastcall tbtnTerminalScrollUpMouseClick(TObject *Sender);
    void __fastcall tbtnTerminalScrollDownMouseClick(TObject *Sender);
    void __fastcall tbtnCashierScrollUpMouseClick(TObject *Sender);
    void __fastcall tbtnCashierScrollDownMouseClick(TObject *Sender);
    void __fastcall tbtnPaymentTypeScrollUpMouseClick(TObject *Sender);
    void __fastcall tbtnPaymentTypeScrollDownMouseClick(TObject *Sender);
    void __fastcall FormCreate(TObject *Sender);
    void __fastcall tbtnReloadMouseClick(TObject *Sender);

private:	// User declarations
    std::map<UnicodeString, TScrollBoxInfo*> sb_info;

    AUDIT_FILTER_LIST location_filter_list;
    AUDIT_FILTER_LIST terminal_filter_list;
    AUDIT_FILTER_LIST cashier_filter_list;
    AUDIT_FILTER_LIST payment_type_filter_list;

    void populateFilters();
    void populateLocationFilter();
    void populateTerminalFilter();
    void populateCashierFilter();
    void populatePaymentTypeFilter();

    void refreshFilters();

    void initSBInfo( TScrollBox* inScrollBox, AUDIT_FILTER_LIST inNameList );

    void refreshScrollButtonWithScrollBox( TScrollBox* inScrollBox );
    TTouchBtn* scrollUpBtnWithScrollBox(   TScrollBox* inScrollBox );
    TTouchBtn* scrollDownBtnWithScrollBox( TScrollBox* inScrollBox );

    void loadLocationList(    AUDIT_FILTER_LIST& outList );
    void loadTerminalList(    AUDIT_FILTER_LIST& outList );
    void loadCashierList(     AUDIT_FILTER_LIST& outList );
    void loadPaymentTypeList( AUDIT_FILTER_LIST& outList );

    void addCheckBoxes(
             TScrollBox* inContainer,
             AUDIT_FILTER_LIST& inNames,
             bool inClearFirst );

    void deleteAllCheckBoxesFromContainer( TScrollBox* inContainer );

    TCheckBox* createCheckBox(
                 int           inLeft,
                 int           inTop,
                 int           inWidth,
                 int           inHeight,
                 UnicodeString inCaption,
                 bool          inChecked,
                 TScrollBox*  inContainer );

    void selectAllWithContainer(   TScrollBox* inControl );
    void unselectAllWithContainer( TScrollBox* inControl );

    void scrollUpWithContainer(   TScrollBox* inScrollBox );
    void scrollDownWithContainer( TScrollBox* inScrollBox );

    UnicodeString infoNameWithScrollBox( TScrollBox* inScrollBox );
    void filterListWithScrollBox( TScrollBox* inScrollBox, AUDIT_FILTER_LIST& outList );

    void refreshFilter(
           TScrollBox* inContainer,
           AUDIT_FILTER_LIST& outFilter );
    void selectedFilters(
           TScrollBox* inScrollBox,
           AUDIT_FILTER_LIST& outList );

public:		// User declarations
    __fastcall TfrmMMSecurityScreenSettings(TComponent* Owner);

    void Reset();
    void GetFilters( ALL_AUDIT_FILTERS& outFilters );
    bool IsAllPaymentTypesSelected();
};
//---------------------------------------------------------------------------
extern PACKAGE TfrmMMSecurityScreenSettings *frmMMSecurityScreenSettings;
//---------------------------------------------------------------------------
#endif
