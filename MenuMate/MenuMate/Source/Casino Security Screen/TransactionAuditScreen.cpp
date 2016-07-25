//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TransactionAuditScreen.h"
#include "DBTransactionAudit.h"
#include "DeviceRealTerminal.h"
#include "SelectReceipt.h"
#include <algorithm>

#include "MMSecurityScreenSettings.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmTransactionAuditScreen *frmTransactionAuditScreen;
//---------------------------------------------------------------------------
__fastcall TfrmTransactionAuditScreen::TfrmTransactionAuditScreen(TComponent* Owner)
    : TZForm(Owner)
{
}
//---------------------------------------------------------------------------

//:::::::::::::::::::::::::::::::::::::::::::::::
// PUBLIC
//:::::::::::::::::::::::::::::::::::::::::::::::
void TfrmTransactionAuditScreen::Reset()
{
    frmMMSecurityScreenSettings->Reset();
    frmMMSecurityScreenSettings->GetFilters( audit_filters );
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransactionAuditScreen::FormCreate(TObject *Sender)
{
	FormResize(Sender);
    loadGridHeaders();

	frmMMSecurityScreenSettings = TfrmMMSecurityScreenSettings::Create<TfrmMMSecurityScreenSettings>(this);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransactionAuditScreen::FormDestroy(TObject *Sender)
{
    //
}
//---------------------------------------------------------------------------


void __fastcall TfrmTransactionAuditScreen::FormShow(TObject *Sender)
{
    FormResize(Sender);
    securityScreenManager = new TMMSecurityScreenManager();
    securityScreenManager->ResetDatetime();
    event = securityScreenManager->CreateSecurityEventProvider();
    securityScreenManager->SetSecurityTransactionDelegate(event.Handle, SecurityManagerUpdateHandler);

    Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
    DBTransaction.Start();
    transactionList = TDBTransactionAudit::GetEventsFromTheLastHour(DBTransaction);
    DBTransaction.Commit();

    std::stable_sort(transactionList.begin(), transactionList.end(), TSecurityTransactionData::comparator);

    refreshDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransactionAuditScreen::FormClose(TObject *Sender, TCloseAction &Action)
{
    securityScreenManager->CloseSecurityEventProvider(event.Handle);
    delete securityScreenManager;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransactionAuditScreen::FormResize(TObject *Sender)
{
 //	if (Tag != Screen->Width)
 //   {
        int Temp = Tag;
        Tag = Screen->Width;
        if((double)Screen->Width/Screen->Height < 1.4)
        {
            ScaleBy(Screen->Width, Temp);
        }
  //  }

	pnlButtons->Left = ClientWidth - pnlButtons->Width;
	pnlButtons->Height = ClientHeight;
    setColumnWidths((double)Screen->Width/1024.00);
	pnlTransactions->Width = pnlButtons->Left - BorderWidth;
	pnlTransactions->Height = ClientHeight;
    gridTransactions->Width  = pnlTransactions->Width  - (gridTransactions->Left * 2);
    gridTransactions->Height = pnlTransactions->Height - (gridTransactions->Top  * 2);
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransactionAuditScreen::btnCloseMouseClick(TObject *Sender)
{
    Close();
}
//---------------------------------------------------------------------------

void TfrmTransactionAuditScreen::setColumnWidths(double widthScale)
{
    gridTransactions->ColWidths[0] =  55 * widthScale;
    gridTransactions->ColWidths[1] = 120 * widthScale;
    gridTransactions->ColWidths[2] =  93 * widthScale;
    gridTransactions->ColWidths[3] =  93 * widthScale;
    gridTransactions->ColWidths[4] =  93 * widthScale;
    gridTransactions->ColWidths[5] = 105 * widthScale;
    gridTransactions->ColWidths[6] =  87 * widthScale;
    gridTransactions->ColWidths[7] =  87 * widthScale;
    gridTransactions->ColWidths[8] =  87 * widthScale;
}
//---------------------------------------------------------------------------

void TfrmTransactionAuditScreen::loadGridHeaders()
{
    gridTransactions->Cells[0][0] = "Time";
    gridTransactions->Cells[1][0] = "Transaction Number";
    gridTransactions->Cells[2][0] = "Location";
    gridTransactions->Cells[3][0] = "Terminal";
    gridTransactions->Cells[4][0] = "Cashier";
    gridTransactions->Cells[5][0] = "Payment Type";
    gridTransactions->Cells[6][0] = "Amount";
    gridTransactions->Cells[7][0] = "Cashout";
    gridTransactions->Cells[8][0] = "Adjustment";
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransactionAuditScreen::SecurityManagerUpdateHandler( System::TObject *inSender, TSecurityTransactionData* inData )
{
    while(transactionList.size() >= NUMBER_OF_TRANSACTIONS_STORED)
    {
        transactionList.pop_back();
    }
    transactionList.push_front(*inData);

    refreshDisplay();
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransactionAuditScreen::gridTransactionsDrawCell(TObject *Sender,
          int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    TStringGrid* StringGrid = static_cast<TStringGrid*>(Sender);
    if(StringGrid != NULL)
    {
        unsigned formatFlags = 0;
        AnsiString text(StringGrid->Cells[ACol][ARow]);

       if(ACol <= 1 || ACol >= 6)
        {
            if(ACol == 8 && ARow > 0)
            {
              StringGrid->Canvas->Font->Color = clRed;
            }
            formatFlags |= DT_RIGHT;
        }
        else
        {
            formatFlags |= DT_LEFT;
        }

if((ACol == 1 && (text.Pos("Open Drawer") || text.Pos("Security Checked Removal") || text.Pos("Float Adjust")||text.Pos("Set Float")) == 1) ||
          (ACol == 5 && text == "Delayed Payment"))            {
                StringGrid->Canvas->Font->Color = clRed;
            }

        drawCellText(formatFlags, StringGrid, ACol, ARow, Rect, State);
    }
}
//---------------------------------------------------------------------------

void TfrmTransactionAuditScreen::drawCellText(unsigned &Flags, TStringGrid *StringGrid, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    StringGrid->Canvas->FillRect(Rect);

    AnsiString text(StringGrid->Cells[ACol][ARow]);
    RECT RText = static_cast<RECT>(Rect);
    InflateRect(&RText, -3, -3);

    DrawText(StringGrid->Canvas->Handle, text.c_str(), text.Length(), &RText, Flags | DT_SINGLELINE | DT_VCENTER);
}
//---------------------------------------------------------------------------

void TfrmTransactionAuditScreen::refreshDisplay()
{
    gridTransactions->RowCount = transactionList.size() + 1;
    int counter = 0;
    int i = 1;
    std::list<TSecurityTransactionData>::iterator it;
    for(it = transactionList.begin(); it != transactionList.end(); it++)
    {
        if( recordPassesTheFilters( ( TSecurityTransactionData* )( &( *it ) ) ) )
        {
            gridTransactions->Cells[0][i] = it->DateTime.FormatString("hh:mm");//("h:nn");
            gridTransactions->Cells[1][i] = it->TransactionNo;
            gridTransactions->Cells[2][i] = it->Location;
            gridTransactions->Cells[3][i] = it->Terminal;
            gridTransactions->Cells[4][i] = it->Cashier;
            gridTransactions->Cells[5][i] = it->PaymentType;
            gridTransactions->Cells[6][i] = it->Amount == Currency(0) ? UnicodeString("") : CurrToStrF(it->Amount, ffNumber, CurrencyDecimals);

            if((it->PaymentType=="Eftpos")&&(it->Amount==0))
            gridTransactions->Cells[6][i]= "0.00";

            gridTransactions->Cells[7][i] = it->Cashout == Currency(0) ? UnicodeString("") : CurrToStrF(it->Cashout, ffNumber, CurrencyDecimals);
            gridTransactions->Cells[8][i] = it->Adjustment == Currency(0) ? UnicodeString("") : CurrToStrF(it->Adjustment, ffNumber, CurrencyDecimals);
            i++;
        }
        else counter++;
    }

    gridTransactions->RowCount -= counter;
}
//---------------------------------------------------------------------------
bool TfrmTransactionAuditScreen::recordPassesTheFilters( TSecurityTransactionData* inRecord )
{
    bool result = false;

    //:::::::::::::::::::::::::::::::::::::::::::

     result = recordPassesLocationFilter(    inRecord, audit_filters["locations"] ) &&
              recordPassesTerminalFilter(    inRecord, audit_filters["terminals"] ) &&
              recordPassesCashierFilter(     inRecord, audit_filters["cashiers" ]  ) &&
              recordPassesPaymentTypeFilter( inRecord, audit_filters["payment types"] );

    //:::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
bool TfrmTransactionAuditScreen::recordPassesLocationFilter(
                                   TSecurityTransactionData* inRecord,
                                   AUDIT_FILTER_LIST inFilterList )
{
    bool result = false;

    //:::::::::::::::::::::::::::::::::::::::::::

    AUDIT_FILTER_LIST::iterator filterIT = inFilterList.begin();

    for( ; filterIT != inFilterList.end(); filterIT++ )
    {
        UnicodeString name = filterIT->Name;
        if( inRecord->Location == name )
        {
            result = true;
            break;
        }
    }

    //:::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
bool TfrmTransactionAuditScreen::recordPassesTerminalFilter(
                                   TSecurityTransactionData* inRecord,
                                   AUDIT_FILTER_LIST inFilterList )
{
    bool result = false;

    //:::::::::::::::::::::::::::::::::::::::::::

    AUDIT_FILTER_LIST::iterator filterIT = inFilterList.begin();

    for( ; filterIT != inFilterList.end(); filterIT++ )
    {
        UnicodeString name = filterIT->Name;
        if( inRecord->Terminal == name )
        {
            result = true;
            break;
        }
    }

    //:::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
bool TfrmTransactionAuditScreen::recordPassesCashierFilter(
                                   TSecurityTransactionData* inRecord,
                                   AUDIT_FILTER_LIST inFilterList )
{
    bool result = false;

    //:::::::::::::::::::::::::::::::::::::::::::

    AUDIT_FILTER_LIST::iterator filterIT = inFilterList.begin();

    for( ; filterIT != inFilterList.end(); filterIT++ )
    {
        if( inRecord->Cashier == filterIT->Name )
        {
            result = true;
            break;
        }
    }

    //:::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------
bool TfrmTransactionAuditScreen::recordPassesPaymentTypeFilter(
                                   TSecurityTransactionData* inRecord,
                                   AUDIT_FILTER_LIST inFilterList )
{
    bool result = false;

    //:::::::::::::::::::::::::::::::::::::::::::

    AUDIT_FILTER_LIST::iterator filterIT = inFilterList.begin();

    for( ; filterIT != inFilterList.end(); filterIT++ )
    {
         if( inRecord->PaymentType == filterIT->Name ||
        (frmMMSecurityScreenSettings->IsAllPaymentTypesSelected() && inRecord->PaymentType == "" ))
        {
            result = true;
            break;
        }
    }

    //:::::::::::::::::::::::::::::::::::::::::::

    return result;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransactionAuditScreen::gridTransactionsDblClick(TObject *Sender)

{
    if(gridTransactions->Cells[5][gridTransactions->Row] != "Delayed Payment")
    {
        std::auto_ptr<TfrmSelectReceipt>frmSelectReceipt(TfrmSelectReceipt::Create<TfrmSelectReceipt>(this));
        frmSelectReceipt->SearchUsingTransactionNumber(gridTransactions->Cells[1][gridTransactions->Row]);
        frmSelectReceipt->ShowModal();
    }
}
//---------------------------------------------------------------------------


//::::::::::::::::::::::::::::::::::::::::::::::::
// Settings
//::::::::::::::::::::::::::::::::::::::::::::::::
void TfrmTransactionAuditScreen::showSecurityScreenSettings()
{
	if( frmMMSecurityScreenSettings->ShowModal() == mrOk )
    {
         frmMMSecurityScreenSettings->GetFilters( audit_filters );
    }
}
//.............................................................................


void __fastcall TfrmTransactionAuditScreen::tbtnFiltersAutoRepeat(TObject *Sender)
{
    showSecurityScreenSettings();
    refreshDisplay();
}
//---------------------------------------------------------------------------

