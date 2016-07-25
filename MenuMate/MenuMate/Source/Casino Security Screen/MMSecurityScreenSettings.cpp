//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "MMSecurityScreenSettings.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmMMSecurityScreenSettings *frmMMSecurityScreenSettings;
//---------------------------------------------------------------------------
__fastcall TfrmMMSecurityScreenSettings::TfrmMMSecurityScreenSettings(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::FormCreate(TObject *Sender)
{
    Reset();
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::
// PUBLIC
//::::::::::::::::::::::::::::::::::::::

void TfrmMMSecurityScreenSettings::Reset()
{
    populateFilters();

    cbLocationSelectAll->Checked   = true;
    cbLocationUnselectAll->Checked = false;

    cbTerminalSelectAll->Checked   = true;
    cbTerminalUnselectAll->Checked = false;

    cbCashierSelectAll->Checked    = true;
    cbCashierUnselectAll->Checked  = false;

    cbPaymentTypeSelectAll->Checked   = true;
    cbPaymentTypeUnselectAll->Checked = false;

    refreshFilters();
}
//---------------------------------------------------------------------------

void  TfrmMMSecurityScreenSettings::GetFilters( ALL_AUDIT_FILTERS& outFilters )
{
    AUDIT_FILTER_LIST locationFilterList;
    AUDIT_FILTER_LIST terminalFilterList;
    AUDIT_FILTER_LIST cashierFilterList;
    AUDIT_FILTER_LIST paymentTypeFilterList;

    selectedFilters( sbLocationFilter,    locationFilterList );
    selectedFilters( sbTerminalFilter,    terminalFilterList );
    selectedFilters( sbCashierFilter,     cashierFilterList );
    selectedFilters( sbPaymentTypeFilter, paymentTypeFilterList );

    outFilters[infoNameWithScrollBox( sbLocationFilter    )] = locationFilterList;
    outFilters[infoNameWithScrollBox( sbTerminalFilter    )] = terminalFilterList;
    outFilters[infoNameWithScrollBox( sbCashierFilter     )] = cashierFilterList;
    outFilters[infoNameWithScrollBox( sbPaymentTypeFilter )] = paymentTypeFilterList;
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::
// PRIVATE
//::::::::::::::::::::::::::::::::::::::

void TfrmMMSecurityScreenSettings::populateFilters()
{
    populateLocationFilter();
    populateTerminalFilter();
    populateCashierFilter();
    populatePaymentTypeFilter();
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::populateLocationFilter()
{
    TScrollBox *scrollBox = this->sbLocationFilter;

    loadLocationList( location_filter_list );

    addCheckBoxes( scrollBox, location_filter_list, true );

    //..............................................................

    initSBInfo( scrollBox, location_filter_list );
    refreshScrollButtonWithScrollBox( scrollBox );
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::populateTerminalFilter()
{
    TScrollBox *scrollBox = this->sbTerminalFilter;

    loadTerminalList( terminal_filter_list );

    addCheckBoxes( scrollBox, terminal_filter_list, true );

    //..............................................................

    initSBInfo( scrollBox, terminal_filter_list );
    refreshScrollButtonWithScrollBox( scrollBox );
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::populateCashierFilter()
{
    TScrollBox *scrollBox = this->sbCashierFilter;

    loadCashierList( cashier_filter_list );

    addCheckBoxes( scrollBox, cashier_filter_list, true );

    //..............................................................

    initSBInfo( scrollBox, cashier_filter_list );
    refreshScrollButtonWithScrollBox( scrollBox );
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::populatePaymentTypeFilter()
{
    TScrollBox *scrollBox = this->sbPaymentTypeFilter;

    loadPaymentTypeList( payment_type_filter_list );

    addCheckBoxes( scrollBox, payment_type_filter_list, true );

    //..............................................................

    initSBInfo( scrollBox, payment_type_filter_list );
    refreshScrollButtonWithScrollBox( scrollBox );
}
//---------------------------------------------------------------------------

void TfrmMMSecurityScreenSettings::refreshFilters()
{
    refreshFilter( sbLocationFilter,    location_filter_list );
    refreshFilter( sbTerminalFilter,    terminal_filter_list );
    refreshFilter( sbCashierFilter,     cashier_filter_list  );
    refreshFilter( sbPaymentTypeFilter, payment_type_filter_list );
}
//---------------------------------------------------------------------------

void TfrmMMSecurityScreenSettings::initSBInfo(
                                     TScrollBox* inScrollBox,
                                     AUDIT_FILTER_LIST inNameList )
{
    int maxItems = inScrollBox->Height / cb_VSPACE;
    int maxPos   = inNameList.size() - maxItems;

    if( maxPos < 0 ) maxPos = 0;

    TScrollBoxInfo* info = new TScrollBoxInfo();
    info->Position    = 0;
    info->MaxPosition = maxPos;

    sb_info[infoNameWithScrollBox( inScrollBox )] = info;
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::refreshScrollButtonWithScrollBox(
                                     TScrollBox* inScrollBox )
{
   TTouchBtn *scrollUpBtn   = scrollUpBtnWithScrollBox(   inScrollBox );
   TTouchBtn *scrollDownBtn = scrollDownBtnWithScrollBox( inScrollBox );

   TScrollBoxInfo *info = sb_info[infoNameWithScrollBox( inScrollBox )];

   scrollUpBtn->Enabled   = info->Position < info->MaxPosition;
   scrollDownBtn->Enabled = info->Position > 0;
}
//---------------------------------------------------------------------------
TTouchBtn* TfrmMMSecurityScreenSettings::scrollUpBtnWithScrollBox(
                                              TScrollBox* inScrollBox )
{
    if( inScrollBox == sbLocationFilter )    return tbtnLocationScrollUp;
    if( inScrollBox == sbTerminalFilter )    return tbtnTerminalScrollUp;
    if( inScrollBox == sbCashierFilter  )    return tbtnCashierScrollUp;
    if( inScrollBox == sbPaymentTypeFilter ) return tbtnPaymentTypeScrollUp;

    throw new Exception( "Invalid scroll box" );
}
//---------------------------------------------------------------------------
TTouchBtn* TfrmMMSecurityScreenSettings::scrollDownBtnWithScrollBox(
                                               TScrollBox* inScrollBox )
{
    if( inScrollBox == sbLocationFilter )    return tbtnLocationScrollDown;
    if( inScrollBox == sbTerminalFilter )    return tbtnTerminalScrollDown;
    if( inScrollBox == sbCashierFilter  )    return tbtnCashierScrollDown;
    if( inScrollBox == sbPaymentTypeFilter ) return tbtnPaymentTypeScrollDown;

    throw new Exception( "Invalid scroll box" );
}
//---------------------------------------------------------------------------

void TfrmMMSecurityScreenSettings::loadLocationList( AUDIT_FILTER_LIST& outList )
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    dbTransaction.Start();

    TDBSecurityScreenSettings::GetLocations( dbTransaction, outList );

    dbTransaction.Commit();
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::loadTerminalList( AUDIT_FILTER_LIST& outList )
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    dbTransaction.Start();

    TDBSecurityScreenSettings::GetTerminals( dbTransaction, outList );

    dbTransaction.Commit();
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::loadCashierList( AUDIT_FILTER_LIST& outList )
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    dbTransaction.Start();

    TDBSecurityScreenSettings::GetCashiers( dbTransaction, outList );

    dbTransaction.Commit();
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::loadPaymentTypeList( AUDIT_FILTER_LIST& outList )
{
    Database::TDBTransaction dbTransaction(TDeviceRealTerminal::Instance().DBControl);
    dbTransaction.Start();

    TDBSecurityScreenSettings::GetPaymentTypes( dbTransaction, outList );

    dbTransaction.Commit();
}
//---------------------------------------------------------------------------

void TfrmMMSecurityScreenSettings::addCheckBoxes(
                                     TScrollBox* inContainer,
                                     AUDIT_FILTER_LIST& inList,
                                     bool inClearFirst )
{
    if( inClearFirst )
    {
        deleteAllCheckBoxesFromContainer( inContainer );
    }

    int left = 10;
    int top  = 10;

    int width  = 196;
    int height = cb_HEIGHT;

    AUDIT_FILTER_LIST::iterator filterIT = inList.begin();

    for( ; filterIT != inList.end(); filterIT++ )
    {
        TCheckBox *cb = createCheckBox( left, top, width, height, filterIT->Name, true, inContainer );
        top += cb_VSPACE;
    }
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::deleteAllCheckBoxesFromContainer( TScrollBox* inContainer )
{
   int i = 0;

    while( i < inContainer->ControlCount  )
    {
        TCheckBox *checkBox = dynamic_cast<TCheckBox*>( inContainer->Controls[i] );

        if( checkBox != NULL )
        {
            delete checkBox;
        }
        else
        {
            i--;
        }
    }
}
//---------------------------------------------------------------------------
TCheckBox* TfrmMMSecurityScreenSettings::createCheckBox(
                                     int           inLeft,
                                     int           inTop,
                                     int           inWidth,
                                     int           inHeight,
                                     UnicodeString inCaption,
                                     bool          inChecked,
                                     TScrollBox*  inContainer )
{
    TCheckBox* result = new TCheckBox( inContainer );

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    result->Parent  = inContainer;
    result->Left    = inLeft;
    result->Top     = inTop;
    result->Width   = inWidth;
    result->Height  = inHeight;
    result->Caption = inCaption;
    result->Checked = inChecked;
    result->Visible = true;
    result->Enabled = true;
    result->Color   = clBtnFace;

    result->Font->Color = clBlack;
    result->Font->Style = TFontStyles();
    result->Font->Name 	= "Tahoma";
    result->Font->Size  = cb_FONT_SIZE;

    //:::::::::::::::::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

void TfrmMMSecurityScreenSettings::selectAllWithContainer( TScrollBox* inContainer )
{
    for( int i = 0; i < inContainer->ControlCount; i++  )
    {
        TCheckBox *checkBox = dynamic_cast<TCheckBox*>( inContainer->Controls[i] );

        if( checkBox != NULL )
        {
            checkBox->Checked = true;
        }
    }
}
//---------------------------------------------------------------------------
void  TfrmMMSecurityScreenSettings::unselectAllWithContainer( TScrollBox* inContainer )
{
    for( int i = 0; i < inContainer->ControlCount; i++  )
    {
        TCheckBox *checkBox = dynamic_cast<TCheckBox*>( inContainer->Controls[i] );

        if( checkBox != NULL )
        {
            checkBox->Checked = false;
        }
    }
}
//---------------------------------------------------------------------------

void TfrmMMSecurityScreenSettings::scrollUpWithContainer( TScrollBox* inScrollBox )
{
    AnsiString name = infoNameWithScrollBox( inScrollBox );
    int pos    = sb_info[name]->Position;
    int maxPos = sb_info[name]->MaxPosition;

    if( pos < maxPos )
    {
        inScrollBox->ScrollBy( 0, -cb_VSPACE );
        inScrollBox->Update();

        sb_info[name]->Position += 1;
    }

    refreshScrollButtonWithScrollBox( inScrollBox );
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::scrollDownWithContainer( TScrollBox* inScrollBox )
{
    AnsiString name = infoNameWithScrollBox( inScrollBox );
    int pos = sb_info[name]->Position;

    if( pos > 0 )
    {
        inScrollBox->ScrollBy( 0, cb_VSPACE );
        inScrollBox->Update();

        sb_info[name]->Position -= 1;
    }

    refreshScrollButtonWithScrollBox( inScrollBox );
}
//---------------------------------------------------------------------------

UnicodeString TfrmMMSecurityScreenSettings::infoNameWithScrollBox( TScrollBox* inScrollBox )
{
    if( inScrollBox == sbLocationFilter )    return "locations";
    if( inScrollBox == sbTerminalFilter )    return "terminals";
    if( inScrollBox == sbCashierFilter  )    return "cashiers";
    if( inScrollBox == sbPaymentTypeFilter ) return "payment types";

    throw new Exception( "Invalid scroll box" );
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::filterListWithScrollBox(
                                     TScrollBox* inScrollBox,
                                     AUDIT_FILTER_LIST& outList )
{
    if(      inScrollBox == sbLocationFilter    ) outList = location_filter_list;
    else if( inScrollBox == sbTerminalFilter    ) outList = terminal_filter_list;
    else if( inScrollBox == sbCashierFilter     ) outList = cashier_filter_list;
    else if( inScrollBox == sbPaymentTypeFilter ) outList = payment_type_filter_list;
    else throw new Exception( "Invalid scroll box" );
}
//---------------------------------------------------------------------------

void TfrmMMSecurityScreenSettings::refreshFilter(
                                     TScrollBox* inContainer,
                                     AUDIT_FILTER_LIST& outList )
{
    int index = 0;
    AUDIT_FILTER_LIST::iterator filterIT = outList.begin();

    for( ; filterIT != outList.end(); filterIT++ )
    {
        TCheckBox *checkBox = dynamic_cast<TCheckBox*>( inContainer->Controls[index++] );

        if( checkBox != NULL )
        {
           filterIT->Selected = checkBox->Checked;
        }
    }
}
//---------------------------------------------------------------------------
void TfrmMMSecurityScreenSettings::selectedFilters(
                                     TScrollBox* inScrollBox,
                                     AUDIT_FILTER_LIST& outList )
{
    AUDIT_FILTER_LIST filterList;
    filterListWithScrollBox( inScrollBox, filterList );

    AUDIT_FILTER_LIST::iterator filterIT = filterList.begin();

    for( ; filterIT != filterList.end(); filterIT++ )
    {
        if( filterIT->Selected )
        {
            TFilterInfo info( filterIT->DBKey, filterIT->Name );
            outList.push_back( info );
        }
    }
}
//---------------------------------------------------------------------------

//::::::::::::::::::::::::::::::::::::::::::::::::::::
// Event handlers
//::::::::::::::::::::::::::::::::::::::::::::::::::::
void __fastcall TfrmMMSecurityScreenSettings::btnOKAutoRepeat(TObject *Sender)
{
    refreshFilters();

	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::btnCancelAutoRepeat(TObject *Sender)

{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::tbtnReloadMouseClick(TObject *Sender)
{
    Reset();
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::cbLocationSelectAllClick(TObject *Sender)
{
    selectAllWithContainer( sbLocationFilter );

    cbLocationUnselectAll->OnClick = NULL;
    cbLocationUnselectAll->Checked = false;
    cbLocationUnselectAll->OnClick = cbLocationUnselectAllClick;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::cbLocationUnselectAllClick(TObject *Sender)
{
    unselectAllWithContainer( sbLocationFilter );

    cbLocationSelectAll->OnClick = NULL;
    cbLocationSelectAll->Checked = false;
    cbLocationSelectAll->OnClick = cbLocationSelectAllClick;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::cbTerminalSelectAllClick(TObject *Sender)
{
    selectAllWithContainer( sbTerminalFilter );

    cbTerminalUnselectAll->OnClick = NULL;
    cbTerminalUnselectAll->Checked = false;
    cbTerminalUnselectAll->OnClick = cbTerminalUnselectAllClick;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::cbTerminalUnselectAllClick(TObject *Sender)
{
    unselectAllWithContainer( sbTerminalFilter );

    cbTerminalSelectAll->OnClick = NULL;
    cbTerminalSelectAll->Checked = false;
    cbTerminalSelectAll->OnClick = cbTerminalSelectAllClick;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::cbCashierSelectAllClick(TObject *Sender)
{
    selectAllWithContainer( sbCashierFilter );

    cbCashierUnselectAll->OnClick = NULL;
    cbCashierUnselectAll->Checked = false;
    cbCashierUnselectAll->OnClick = cbCashierUnselectAllClick;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::cbCashierUnselectAllClick(TObject *Sender)
{
    unselectAllWithContainer( sbCashierFilter );

    cbCashierSelectAll->OnClick = NULL;
    cbCashierSelectAll->Checked = false;
    cbCashierSelectAll->OnClick = cbCashierSelectAllClick;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::cbPaymentTypeSelectAllClick(TObject *Sender)
{
    selectAllWithContainer( sbPaymentTypeFilter );

    cbPaymentTypeUnselectAll->OnClick = NULL;
    cbPaymentTypeUnselectAll->Checked = false;
    cbPaymentTypeUnselectAll->OnClick = cbPaymentTypeUnselectAllClick;
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::cbPaymentTypeUnselectAllClick(TObject *Sender)
{
    unselectAllWithContainer( sbPaymentTypeFilter );

    cbPaymentTypeSelectAll->OnClick = NULL;
    cbPaymentTypeSelectAll->Checked = false;
    cbPaymentTypeSelectAll->OnClick = cbPaymentTypeSelectAllClick;
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::tbtnLocationScrollUpMouseClick(TObject *Sender)
{
    scrollUpWithContainer( sbLocationFilter );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::tbtnLocationScrollDownMouseClick(TObject *Sender)
{
    scrollDownWithContainer( sbLocationFilter );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::tbtnTerminalScrollUpMouseClick(TObject *Sender)
{
    scrollUpWithContainer( sbTerminalFilter );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::tbtnTerminalScrollDownMouseClick(TObject *Sender)
{
    scrollDownWithContainer( sbTerminalFilter );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::tbtnCashierScrollUpMouseClick(TObject *Sender)
{
    scrollUpWithContainer( sbCashierFilter );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::tbtnCashierScrollDownMouseClick(TObject *Sender)
{
    scrollDownWithContainer( sbCashierFilter );
}
//---------------------------------------------------------------------------

void __fastcall TfrmMMSecurityScreenSettings::tbtnPaymentTypeScrollUpMouseClick(TObject *Sender)
{
    scrollUpWithContainer( sbPaymentTypeFilter );
}
//---------------------------------------------------------------------------
void __fastcall TfrmMMSecurityScreenSettings::tbtnPaymentTypeScrollDownMouseClick(TObject *Sender)
{
    scrollDownWithContainer( sbPaymentTypeFilter );
}
//---------------------------------------------------------------------------
 bool TfrmMMSecurityScreenSettings::IsAllPaymentTypesSelected()
{
    return  cbPaymentTypeSelectAll->Checked;
}


