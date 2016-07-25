//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ComboForm.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmComboForm *frmComboForm;
//---------------------------------------------------------------------------
__fastcall TfrmComboForm::TfrmComboForm(TComponent* Owner)
    : TZForm(Owner)
{
   OrdersList = new TList;
}
//---------------------------------------------------------------------------
void __fastcall TfrmComboForm::FormShow(TObject *Sender)
{
    SetRowCount();
    if(sgShowItemList->RowCount > 0)
    {
      AddItemsToGrid();
    }
    UnicodeString value = edPointsSelected->Text;
    btnOk->Enabled = false;
    if(MemberCurrentPoints > 0)
    {
       edCurrentPoints->Text = CurrToStr(MemberCurrentPoints);
    }
    lblMemberName->Caption = MemberName;
    edPointsRemaining->Text = CurrToStr(MemberCurrentPoints);
    edPointsSelected->Text = "0";
    MembersPoints = MemberCurrentPoints;
}
//---------------------------------------------------------------------------
void TfrmComboForm::AddItemsToGrid()
{
    try
    {
        int rowPosition = 0;
        for (int i = 0; i < OrdersList->Count; i++)
        {
            TItemMinorComplete *order = reinterpret_cast<TItemMinorComplete *>(OrdersList->Items[i]);
            if(order->ItemPriceForPointsOriginal > 0 && order->IsCanBePaidForUsingPoints && order->BillCalcResult.FinalPrice > 0)
             {
                 for(int j = 0; j < (int)order->GetQty() ; j++)
                  {
                     AddItemToGrid(order,rowPosition);
                     rowPosition++;
                  }
             }
            for (int m = 0; m < order->SubOrders->Count; m++)
            {
                TItemMinorComplete *subOrder = reinterpret_cast<TItemMinorComplete *>(order->SubOrders->SubOrderGet(m));
                if(subOrder->ItemPriceForPointsOriginal > 0 && subOrder->IsCanBePaidForUsingPoints && subOrder->BillCalcResult.FinalPrice > 0)
                {
                  for(int j = 0; j < (int)subOrder->GetQty() ; j++)
                  {
                     AddItemToGrid(subOrder,rowPosition);
                     rowPosition++;
                  }
                }
            }
        }
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
        throw;
    }
}
//---------------------------------------------------------------------------
void TfrmComboForm::SetRowCount()
{
    try
    {
        int index = 0;
        for(int i = 0; i < OrdersList->Count; i++)
        {
             TItemComplete *order = (TItemComplete *)OrdersList->Items[i];
             order->IsPayByPoints = false;
             if(order->ItemPriceForPoints > 0 && order->IsCanBePaidForUsingPoints && (order->BillCalcResult.FinalPrice > 0))
                index += (int)order->GetQty();

            for (int j = 0; j < order->SubOrders->Count; j++)
            {
                TItemCompleteSub *SubOrder = (TItemCompleteSub*)order->SubOrders->Items[j];
                SubOrder->IsPayByPoints = false;
                if(SubOrder->ItemPriceForPoints > 0 && SubOrder->IsCanBePaidForUsingPoints && (SubOrder->BillCalcResult.FinalPrice > 0))
                    index += (int)order->GetQty();
            }
        }
        sgShowItemList->RowCount = index;
    }
    catch(Exception & E)
    {
        TManagerLogs::Instance().Add(__FUNC__, ERRORLOG, E.Message);
        throw;
    }
}
//---------------------------------------------------------------------------
void TfrmComboForm::AddItemToGrid(TItemMinorComplete* item, int index)
{
    Items.push_back(item);
	sgShowItemList->Cols[0]->AddObject(item->Item,(TObject*)index);
	sgShowItemList->Cols[1]->AddObject(FormatFloat("0.00", item->ItemPriceForPointsOriginal),(TObject*)index);
}
//---------------------------------------------------------------------------
void __fastcall TfrmComboForm::btnCloseAutoRepeat(TObject *Sender)
{
    for(int i = 0; i < OrdersList->Count; i++)
    {
        TItemComplete *order = (TItemComplete *)OrdersList->Items[i];
        if(order->IsPayByPoints)
        {
           order->IsPayByPoints = false;
        }
    }
   SelectedPoints = 0;
   PriceSelected = 0;
   ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmComboForm::btnOkAutoRepeat(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmComboForm::sgShowItemListSelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect)
{
    if(sgShowItemList->Cells[ACol][ARow] != "")
    {
        std::vector<int>::iterator it = RowOrders.find(ARow);
        TItemMinorComplete * item = Items.at(ARow);
        if(it == NULL)
        {
           InsertItemRowInDiscountMap(item,ARow);
        }
        else if(it != NULL)
        {
           RowOrders.erase(it);
           DeSelectRow(item,ARow);
           TDiscount CurrentDiscount = item->GetLastPointDiscount();
           if(CurrentDiscount.DiscountKey > 0)
            {
               std::vector<int>::iterator it2 = DiscountedRowMap[CurrentDiscount.DiscountKey].Rows.find(ARow);
               DiscountedRowMap[CurrentDiscount.DiscountKey].Rows.erase(it2);
            }
        }
        edPointsRemaining->Text = FormatFloat("0.00", MemberCurrentPoints - SelectedPoints);
        edPointsSelected->Text = FormatFloat("0.00", SelectedPoints);
    }
    btnOk->Enabled = SelectedPoints > 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmComboForm::sgShowItemListDrawCell(TObject *Sender, int ACol, int ARow,
          TRect &Rect, TGridDrawState State)
{
	TCanvas *pCanvas = sgShowItemList->Canvas;
	pCanvas->Font = sgShowItemList->Font;
	pCanvas->Brush->Color = sgShowItemList->Color;
	Rect = sgShowItemList->CellRect(ACol, ARow);

	TColor Color = pCanvas->Brush->Color;
	TBrushStyle Style = pCanvas->Brush->Style;
	std::vector<int>::iterator it = RowOrders.find(ARow);

	if(it != NULL)
	{
		pCanvas->Font->Color = clWhite;
		pCanvas->Font->Style = TFontStyles() << fsBold;
		pCanvas->Brush->Color = RGB(251,170,30);
		pCanvas->Brush->Style = bsSolid;
		pCanvas->FillRect(Rect);
	}
	else
	{
		pCanvas->Font->Color = RGB(251,170,30);
		pCanvas->Font->Style = TFontStyles();
		pCanvas->FillRect(Rect);
	}
	UnicodeString Caption = sgShowItemList->Cells[ACol][ARow];
	TRect DrawRegRect(Rect.Left+2, Rect.Top, Rect.Right-2, Rect.Bottom);
	int TextHeight = DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &DrawRegRect, DT_CALCRECT|DT_WORDBREAK|DT_NOPREFIX, NULL) + 4;
    TRect CentredRect = DrawRegRect;
    int RowHeight = Rect.Bottom - Rect.Top;
    if (RowHeight > TextHeight)
    {
        CentredRect.Top += (RowHeight - TextHeight) / 2;
        CentredRect.Bottom = CentredRect.Top + TextHeight;
    }
    DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(), &CentredRect, DT_WORDBREAK|DT_NOPREFIX, NULL);
    pCanvas->Brush->Color = Color;
    pCanvas->Brush->Style = Style;
}
//---------------------------------------------------------------------------
Currency TfrmComboForm::GetPrice(TItemMinorComplete *order )
{
    Currency itemprice = TGlobalSettings::Instance().ItemPriceIncludeTax ? order->BillCalcResult.FinalPrice : order->BillCalcResult.BasePrice/order->GetQty();
    itemprice = itemprice/order->GetQty();
    return itemprice;
}
//---------------------------------------------------------------------------
void TfrmComboForm::InsertItemRowInDiscountMap(TItemMinorComplete *item,int rowIndex)
{
    bool removeItem = false;
    bool canSelect = true;
    TDiscount CurrentDiscount = item->GetLastPointDiscount();
    if(CurrentDiscount.DiscountKey > 0)
    {
      if(CurrentDiscount.MaxItemAffected > 0 && DiscountedRowMap[CurrentDiscount.DiscountKey].Rows.size() == CurrentDiscount.MaxItemAffected)
       {
          std::vector<int>::iterator it = DiscountedRowMap[CurrentDiscount.DiscountKey].Rows.begin();
          int i = *it;
          TItemMinorComplete * itemToRemove = Items.at(*it);
          canSelect = (SelectedPoints - itemToRemove->ItemPriceForPoints + item->ItemPriceForPoints) <= MemberCurrentPoints;
          if(canSelect)
          {
             std::vector<int>::iterator it1 = RowOrders.find(*it);
             RowOrders.erase(it1);
             DeSelectRow(itemToRemove,*it);
             std::vector<int>::iterator it2 = DiscountedRowMap[CurrentDiscount.DiscountKey].Rows.find(*it);
             int j = *it2;
             DiscountedRowMap[CurrentDiscount.DiscountKey].Rows.erase(it2);
          }
       }
       else
       {
          canSelect = (SelectedPoints + item->ItemPriceForPoints) <= MemberCurrentPoints;
       }
    }
    else
    {
       canSelect = (SelectedPoints + item->ItemPriceForPoints) <= MemberCurrentPoints;
     //   _itemprice = order->BillCalcResult.BasePrice;///order->GetQty();
    }

    if(canSelect)
    {
       sgShowItemList->Cells[1][rowIndex] = FormatFloat("0.00", item->ItemPriceForPoints);
       SelectedPoints = SelectedPoints + item->ItemPriceForPoints;
       PriceSelected = PriceSelected + GetPrice(item);
       RowOrders.insert(rowIndex);
       RowOrders.organiseints();
       SelectedRowMap[item->TransNo].Rows.insert(rowIndex);
       item->IsPayByPoints = true;
       if(CurrentDiscount.DiscountKey > 0)
        {
          DiscountedRowMap[CurrentDiscount.DiscountKey].MaxQuantityAffected = CurrentDiscount.MaxItemAffected;
          DiscountedRowMap[CurrentDiscount.DiscountKey].Rows.insert(rowIndex);
        }
    }
    else
    {
       MessageBox(AnsiString("Sum of Item Prices should be less than or equal to value of Points.").c_str(), "Error", MB_OK + MB_ICONINFORMATION);
    }
}

void TfrmComboForm::DeSelectRow(TItemMinorComplete *item,int rowIndex)
{
    SelectedPoints = SelectedPoints - item->ItemPriceForPoints;
    PriceSelected = PriceSelected - GetPrice(item);
    std::vector<int>::iterator it1 = SelectedRowMap[item->TransNo].Rows.find(rowIndex);
    SelectedRowMap[item->TransNo].Rows.erase(it1);
    if(SelectedRowMap[item->TransNo].Rows.size() == 0)
       item->IsPayByPoints = false;
    sgShowItemList->Cells[1][rowIndex] = FormatFloat("0.00", item->ItemPriceForPointsOriginal);
    sgShowItemList->Cells[0][rowIndex] = item->Item;
}


