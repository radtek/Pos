//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DelayedPaymentTabs.h"
#include "DeviceRealTerminal.h"
#include "DBTab.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//TfrmDelayedPaymentTabs *frmDelayedPaymentTabs;
//---------------------------------------------------------------------------
__fastcall TfrmDelayedPaymentTabs::TfrmDelayedPaymentTabs(TComponent* Owner)
        : TZForm(Owner)
{
}

//---------------------------------------------------------------------------
void __fastcall TfrmDelayedPaymentTabs::FormCreate(TObject *Sender)
{
  FormResize(Sender);
  LoadGridHeaders();
}

//---------------------------------------------------------------------------
void __fastcall TfrmDelayedPaymentTabs::FormShow(TObject *Sender)
{
  SelectedTabKey = 0;
  SelectedTabName = "";
  FormResize(Sender);
  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
  DBTransaction.Start();
  transactionList = TDBTransactionAudit::GetAllDelayedPayments(DBTransaction);
  DBTransaction.Commit();
  RefreshDisplay();
  gridTransactions->Row = 1;
}

//---------------------------------------------------------------------------
void __fastcall TfrmDelayedPaymentTabs::FormResize(TObject *Sender)
{
    if (Tag != Screen->Width)
    {
        int Temp = Tag;
        Tag = Screen->Width;
        if((double)Screen->Width/Screen->Height < 1.4)
            {
            ScaleBy(Screen->Width, Temp);
            }
    }

    pnlButtons->Left = ClientWidth - pnlButtons->Width;
    pnlButtons->Height = ClientHeight;
    tbtnCancel->Top =  pnlButtons->Height - tbtnCancel->Height - 8;
    btnScrollDown->Top = tbtnCancel->Top -  btnScrollDown->Height - 8;
    pnlTransactions->Width = pnlButtons->Left - BorderWidth;
    pnlTransactions->Height = ClientHeight;
    gridTransactions->Width  = pnlTransactions->Width  - (gridTransactions->Left * 2);
    gridTransactions->Height = pnlTransactions->Height - (gridTransactions->Top  * 2);
    SetColumnWidths();
}

//---------------------------------------------------------------------------
void __fastcall TfrmDelayedPaymentTabs::FormClose(TObject *Sender, TCloseAction &Action)
{
 //
}

//---------------------------------------------------------------------------
void __fastcall TfrmDelayedPaymentTabs::FormDestroy(TObject *Sender)
{
 //
}

//---------------------------------------------------------------------------
void TfrmDelayedPaymentTabs::SetColumnWidths()
{
    double colWidth = (gridTransactions->Width - 10)/8;
    gridTransactions->ColWidths[0] =  colWidth;
    gridTransactions->ColWidths[1] =  colWidth;
    gridTransactions->ColWidths[2] =  colWidth;
    gridTransactions->ColWidths[3] =  colWidth;
    gridTransactions->ColWidths[4] =  colWidth;
    gridTransactions->ColWidths[5] =  colWidth;
    gridTransactions->ColWidths[6] =  colWidth;
    gridTransactions->ColWidths[7] =  colWidth;
}

//---------------------------------------------------------------------------
void TfrmDelayedPaymentTabs::LoadGridHeaders()
{
    gridTransactions->Cells[0][0] = "Date";
    gridTransactions->Cells[1][0] = "Time";
    gridTransactions->Cells[2][0] = "Transaction Number";
    gridTransactions->Cells[3][0] = "Location";
    gridTransactions->Cells[4][0] = "Terminal";
    gridTransactions->Cells[5][0] = "Cashier";
    gridTransactions->Cells[6][0] = "From Table";
    gridTransactions->Cells[7][0] = "Amount";

}

//---------------------------------------------------------------------------
void __fastcall TfrmDelayedPaymentTabs::tbtnCancelMouseClick(TObject *Sender)
{
  ModalResult = mrCancel;;
}

//---------------------------------------------------------------------------
void __fastcall TfrmDelayedPaymentTabs::btnSelectMouseClick(TObject *Sender)
{
  DelayedInvoiceNumber = gridTransactions->Cells[2][gridTransactions->Row];
  Database::TDBTransaction DBTransaction(TDeviceRealTerminal::Instance().DBControl);
  DBTransaction.Start();
  SelectedTabKey = TDBTab::GetTabByDelyedInvoiceNumber(DBTransaction,DelayedInvoiceNumber,SelectedTabName);
  DBTransaction.Commit();
  ModalResult = mrOk;
}

//---------------------------------------------------------------------------
void __fastcall TfrmDelayedPaymentTabs::gridTransactionsDrawCell(TObject *Sender, int ACol, int ARow,
      TRect &Rect, TGridDrawState State)
{
    TStringGrid* StringGrid = static_cast<TStringGrid*>(Sender);
    if(StringGrid != NULL)
    {
        unsigned formatFlags = 0;
        AnsiString text(StringGrid->Cells[ACol][ARow]);

       if(ARow == 0)
       {
         formatFlags |= DT_CENTER;
         StringGrid->Canvas->Font->Style = TFontStyles() << fsBold;
       }
       else if(ACol <= 6 && ACol >= 3)
        {
            formatFlags |= DT_LEFT ;
        }
        else
        {
            formatFlags |= DT_RIGHT;
        }

        DrawCellText(formatFlags, StringGrid, ACol, ARow, Rect, State);
    }
}

//---------------------------------------------------------------------------
void TfrmDelayedPaymentTabs::DrawCellText(unsigned &Flags, TStringGrid *StringGrid, int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
    StringGrid->Canvas->FillRect(Rect);
    AnsiString text(StringGrid->Cells[ACol][ARow]);
    RECT RText = static_cast<RECT>(Rect);
    InflateRect(&RText, -3, -3);
    DrawText(StringGrid->Canvas->Handle, text.c_str(), text.Length(), &RText, Flags | DT_SINGLELINE | DT_VCENTER);
}

//---------------------------------------------------------------------------
void TfrmDelayedPaymentTabs::RefreshDisplay()
{
    gridTransactions->RowCount = transactionList.size() + 1;
    int i = 1;
    std::list<TDelayedPaymentData>::iterator it;
    for(it = transactionList.begin(); it != transactionList.end(); it++)
    {
        gridTransactions->Cells[0][i] = it->DateTime.FormatString("dd/mm/yy");
        gridTransactions->Cells[1][i] = it->DateTime.FormatString("hh:mm");
        gridTransactions->Cells[2][i] = it->TransactionNo;
        gridTransactions->Cells[3][i] = it->Location;
        gridTransactions->Cells[4][i] = it->Terminal;
        gridTransactions->Cells[5][i] = it->Cashier;
        gridTransactions->Cells[6][i] = it->FromTable;
        gridTransactions->Cells[7][i] = FormatFloat("0.00",RoundToNearest(it->Amount, 0.01, TGlobalSettings::Instance().MidPointRoundsDown));
        i++;
    }
}


void __fastcall TfrmDelayedPaymentTabs::btnScrollDownMouseClick(TObject *Sender)
{
  gridTransactions->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
//---------------------------------------------------------------------------

void __fastcall TfrmDelayedPaymentTabs::btnScrollUpMouseClick(TObject *Sender)
{
  gridTransactions->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
//---------------------------------------------------------------------------

