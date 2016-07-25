// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "SelectSizesAsList.h"
#include "MMLogging.h"
#include "SelectDish.h"
#include "ItemSize.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------

#pragma warn -par
void
TfrmSelectSizes::ForwardItemDefinitionRefresh(
  Messages::TMessage &message)
{
   frmSelectDish->Perform(
     item_management::network_messaging::refresh_item_display, 0, 0);

   FormShow(this);
}
#pragma warn .par

__fastcall TfrmSelectSizes::TfrmSelectSizes(TComponent* Owner) : TZForm(Owner)
{
    Sizes = new TListSizeContainer;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectSizes::FormCreate(TObject *Sender)
{
//
}

// ---------------------------------------------------------------------------

void __fastcall TfrmSelectSizes::FormDestroy(TObject *Sender)
{
//
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectSizes::FormShow(TObject *Sender)
{
    SelectedItemSizeGrid = -1;
	FormResize(NULL);
    RefreshDisplay();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectSizes::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}

//--------------------------------------------------------------------
void __fastcall TfrmSelectSizes::BtnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//----------------------------------------------------------------------

void __fastcall TfrmSelectSizes::btnMsgDownClick(TObject *Sender)
{
    ItemSizeGrid->Perform(WM_VSCROLL, SB_LINEDOWN, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectSizes::btnMsgUpClick(TObject *Sender)
{
    ItemSizeGrid->Perform(WM_VSCROLL, SB_LINEUP, 0);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectSizes::FormResize(TObject *Sender)
{
    SetColumnWidths();
}
// ---------------------------------------------------------------------------
void TfrmSelectSizes::SetColumnWidths()
{
    double colWidth = (ItemSizeGrid->Width/3);
    ItemSizeGrid->ColWidths[0] =  ItemSizeGrid->Width - colWidth;
    ItemSizeGrid->ColWidths[1] =  colWidth-5;
}
//---------------------------------------------------------------------------

void __fastcall TfrmSelectSizes::FormClose(TObject *Sender, TCloseAction &Action)
{
//
}
// ---------------------------------------------------------------------------
void __fastcall TfrmSelectSizes::tbtnCancelMouseClick(TObject *Sender)
{
    SelectedItemSizeGrid = -1;
	ModalResult = mrCancel;
}
//-----------------------------------------------------------------------------------------------------------------------
void TfrmSelectSizes::RefreshDisplay()
{
    ItemSizeGrid->RowCount = Sizes->Count;
    for(int i = 0; i < ItemSizeGrid->RowCount; i++)
    {
        TItemSize *Size = Sizes->SizeGet(i);
         if(Size != NULL)
         {
            ItemSizeGrid->Cells[0][i] = "  "+ Size->GetDecoratedName();
            ItemSizeGrid->Cells[1][i] = FormatFloat("0.00", Size->Price);
            ItemSizeGrid->Cells[2][i] = i;
         }
    }
}
//--------------------------------------------------------------------------------------------------------------------

void __fastcall TfrmSelectSizes::ItemSizeDisplayDrawCell(TObject *Sender, int ACol, int ARow,
                                                    TRect &Rect, TGridDrawState State)
{
    TStringGrid* StringGrid = static_cast<TStringGrid*>(Sender);
	assert(StringGrid != NULL);
    StringGrid->Canvas->FillRect(Rect);
	AnsiString text(StringGrid->Cells[ACol][ARow]);
	RECT RText = static_cast<RECT>(Rect);
	InflateRect(&RText, -3, -3);
    unsigned formatFlags = 0;
    TItemSize *Size = Sizes->SizeGet(ARow);

    if(ACol == 0)
    {
       formatFlags |= DT_LEFT;
    }

    if(ACol == 1)
    {
       formatFlags |= DT_RIGHT;
    }

    if(!Size->IsEnabled())
    {
        StringGrid->Canvas->Font->Color = clGray;
    }

   DrawText(StringGrid->Canvas->Handle, text.c_str(), text.Length(), &RText, formatFlags | DT_SINGLELINE | DT_VCENTER);
}

//---------------------------------------------------------------------------

void __fastcall TfrmSelectSizes::ItemSizeDisplaySelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
    if(ItemSizeGrid->Cells[2][ARow] != "")
    {
        TItemSize *Size = Sizes->SizeGet(ARow);
         if(!Size->IsEnabled())
         {
            SelectedItemSizeGrid = -1;
            ItemSizeGrid->Canvas->Font->Color = clRed;
         }
         else
         {
            SelectedItemSizeGrid = StrToInt(ItemSizeGrid->Cells[2][ARow]);
            ModalResult = mrOk;
         }
     }
}
//---------------------------------------------------------------------------

