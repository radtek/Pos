//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "TransferItems.h"
#include "enum.h"

#include "ItemComplete.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
TfrmTransferItem *frmTransferItem;
//---------------------------------------------------------------------------
__fastcall TfrmTransferItem::TfrmTransferItem(TComponent* Owner)
   : TZForm(Owner)
{
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::FormCreate(TObject *Sender)
{
   SourceItems = new TList;
   DestItems = new TList;
}

//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::FormDestroy(TObject *Sender)
{
   while(SourceItems->Count != 0)
   {
      delete (TItemComplete *)SourceItems->Items[0];
      SourceItems->Delete(0);
   }
   delete SourceItems;

   while(DestItems->Count != 0)
   {
      delete (TItemComplete *)DestItems->Items[0];
      DestItems->Delete(0);
   }
   delete DestItems;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::FormResize(TObject *Sender)
{
   //	if (Tag != Screen->Width)
  // {
      int Temp = Tag;
		Tag = Screen->Width;
 	if((double)Screen->Width / Screen->Height < 1.4)
       {
   		ScaleBy(Screen->Width, Temp);
      }

      Top = 0;
      Left = 0;
      Width = Screen->Width;
      Height = Screen->Height;
  // }
	Align = alClient;
   Panel2->Left = ClientWidth - Panel2->Width;
   Panel3->Left = ClientWidth - Panel2->Width - Panel3->Width;
   Panel4->Left = ClientWidth - Panel2->Width - Panel3->Width - Panel4->Width;
   Panel6->Left = ClientWidth - Panel2->Width - Panel3->Width - Panel4->Width - Panel6->Width;
   Panel3->Width = BorderWidth;
   Panel6->Width = BorderWidth;
   Panel2->Height = ClientHeight;
   Panel3->Height = ClientHeight;
   Panel4->Height = ClientHeight;
   Panel6->Height = ClientHeight;
   Panel5->Height = ClientHeight;

	StringGrid1->Width = Panel5->ClientWidth - StringGrid1->Left - Panel5->BorderWidth;
   Label7->Width = StringGrid1->Width * 77 / 100;
   Label8->Left = Label7->Left + Label7->Width;
   Label8->Width = StringGrid1->Width - Label7->Width;

   StringGrid1->ColWidths[0] = Label7->Width - 2;
   StringGrid1->ColWidths[1] = Label8->Width - 1;
   StringGrid1->Height = Panel5->ClientHeight - StringGrid1->Top - btnDishDown->Height - (Panel5->BorderWidth * 2);

	StringGrid2->Width = Panel4->ClientWidth - StringGrid2->Left - Panel5->BorderWidth;
   Label4->Width = StringGrid2->Width * 77 / 100;
   Label6->Left = Label4->Left + Label4->Width;
   Label6->Width = StringGrid2->Width - Label4->Width;
   StringGrid2->ColWidths[0] = Label7->Width - 2;
   StringGrid2->ColWidths[1] = Label8->Width - 1;
   StringGrid2->Height = Panel4->ClientHeight - StringGrid2->Top - btnDestDishDown->Height - (Panel5->BorderWidth * 2);

   btnDishDown->Top = Panel5->ClientHeight - btnDishDown->Height - Panel5->BorderWidth;
   btnDishUp->Top = btnDishDown->Top;
   btnDishUp->Width = (StringGrid1->Width / 2) - (Panel5->BorderWidth);
   btnDishDown->Left = btnDishUp->Left + btnDishUp->Width + Panel5->BorderWidth;
   btnDishDown->Width = Panel5->ClientWidth - btnDishDown->Left - Panel5->BorderWidth;

   btnDestDishDown->Top = Panel4->ClientHeight - btnDestDishDown->Height - Panel4->BorderWidth;
   btnDestDishUp->Top = btnDestDishDown->Top;
   btnDestDishUp->Width = (StringGrid2->Width / 2) - (Panel5->BorderWidth);
   btnDestDishDown->Left = btnDestDishUp->Left + btnDestDishUp->Width + Panel5->BorderWidth;
   btnDestDishDown->Width = Panel4->ClientWidth - btnDestDishDown->Left - Panel5->BorderWidth;

	stCancel->Top = Panel5->BorderWidth;
	stOk->Top = stCancel->Top + stCancel->Height + Panel5->BorderWidth;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferItem::FormShow(TObject *Sender)
{
   Label1->Caption = "Transfer From " + From;
   Label3->Caption = "Transfer To " + To;
   isItemTransferred = false;
   FormResize(this);
   ShowItems();

}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferItem::stCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;   
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferItem::stOkClick(TObject *Sender)
{
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTransferItem::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}
void __fastcall TfrmTransferItem::btnDishUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	ScrollUp = true;
   ScrollingControl = StringGrid1;
	SendMessage(ScrollingControl->Handle, WM_VSCROLL, SB_LINEUP, 0L);
	ButtonClicked = btnDishUp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::btnDishUpMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
   ScrollingControl = NULL;
   ButtonClicked = NULL;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::btnDishDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	ScrollUp = false;
   ScrollingControl = StringGrid1;
	SendMessage(ScrollingControl->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
	ButtonClicked = btnDishDown;
}
//---------------------------------------------------------------------------



void __fastcall TfrmTransferItem::btnDestDishUpMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	ScrollUp = true;
   ScrollingControl = StringGrid2;
	SendMessage(ScrollingControl->Handle, WM_VSCROLL, SB_LINEUP, 0L);
	ButtonClicked = btnDestDishUp;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::btnDestDishDownMouseDown(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	ScrollUp = false;
   ScrollingControl = StringGrid2;
	SendMessage(ScrollingControl->Handle, WM_VSCROLL, SB_LINEDOWN, 0L);
	ButtonClicked = btnDestDishDown;
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::ShowItems()
{
   int SelectedRow = StringGrid1->Row;
   StringGrid1->RowCount = 1;
   StringGrid1->Cells[0][0] = "";
   StringGrid1->Cells[1][0] = "";
   StringGrid1->Cells[2][0] = "";
   int Row = 0;
   sourceItemCount = SourceItems->Count;

   for (int i = 0; i < SourceItems->Count; i++)
   {
      TItemComplete *Item = (TItemComplete *)SourceItems->Items[i];
     	StringGrid1->RowCount = Row + 1;
      if (Item->Size.UpperCase() == "DEFAULT")
      {
   		StringGrid1->Cells[0][Row] = Item->Item;
      }
      else
      {
   		StringGrid1->Cells[0][Row] = Item->Size + " " + Item->Item;
      }
      StringGrid1->Cells[1][Row++] = CurrToStrF(Item->TotalPriceAdjustment(), ffCurrency, CurrencyDecimals);
   }

   Classes::TNotifyEvent OnClick = StringGrid1->OnClick;
   StringGrid1->OnClick = NULL;
   try
   {
      if (SelectedRow < StringGrid1->RowCount)
      {
         StringGrid1->Row = SelectedRow;
      }
      else
      {
         StringGrid1->Row = StringGrid1->RowCount - 1;
      }
   }
   __finally
   {
      StringGrid1->OnClick = OnClick;
   }

   SelectedRow = StringGrid2->Row;
   StringGrid2->RowCount = 1;
   StringGrid2->Cells[0][0] = "";
   StringGrid2->Cells[1][0] = "";
   StringGrid2->Cells[2][0] = "";
   Row = 0;

   for (int i = 0; i < DestItems->Count; i++)
   {
      TItemComplete *Item = (TItemComplete *)DestItems->Items[i];
     	StringGrid2->RowCount = Row + 1;
      if (Item->Size.UpperCase() == "DEFAULT")
      {
   		StringGrid2->Cells[0][Row] = Item->Item;
      }
      else
      {
   		StringGrid2->Cells[0][Row] = Item->Size + " " + Item->Item;
      }
      StringGrid2->Cells[1][Row++] = CurrToStrF(Item->TotalPriceAdjustment(), ffCurrency, CurrencyDecimals);
   }

   OnClick = StringGrid2->OnClick;
   StringGrid2->OnClick = NULL;
   try
   {
      if (SelectedRow < StringGrid2->RowCount)
      {
         StringGrid2->Row = SelectedRow;
      }
      else
      {
         StringGrid2->Row = StringGrid2->RowCount - 1;
      }
   }
   __finally
   {
      StringGrid2->OnClick = OnClick;
   }
}




//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::StringGrid2DrawCell(TObject *Sender,
      int ACol, int ARow, TRect &Rect, TGridDrawState State)
{
   if(DestItems->Count > ARow)
   {
      TItemComplete *Item = (TItemComplete *)DestItems->Items[ARow];
		if(Item->OrderKey == 0)
      {
         StringGrid2->Canvas->Font->Style = TFontStyles()<< fsItalic;
         if(ACol == 0)
            StringGrid2->Canvas->TextRect(Rect,Rect.Left,Rect.Top,StringGrid2->Cells[ACol][ARow] /*, AlignLeft + AlignHCenter*/);
         if(ACol == 1)
            StringGrid2->Canvas->TextRect(Rect,Rect.Left,Rect.Top,StringGrid2->Cells[ACol][ARow]/*, AlignLeft + AlignHCenter*/);
      }
      else
      {
         StringGrid2->Canvas->Font->Style = TFontStyles()<< fsBold;
         if(ACol == 0)
            StringGrid2->Canvas->TextRect(Rect,Rect.Left,Rect.Top,StringGrid2->Cells[ACol][ARow]/*, AlignLeft + AlignHCenter*/);
         if(ACol == 1)
            StringGrid2->Canvas->TextRect(Rect,Rect.Left,Rect.Top,StringGrid2->Cells[ACol][ARow]/*, AlignLeft + AlignHCenter*/);
      }
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::StringGrid1Click(TObject *Sender)
{
   if(SourceItems->Count > StringGrid1->Row)
   {
      TItemComplete *Item = (TItemComplete *)SourceItems->Items[StringGrid1->Row];
      if(DestItems->IndexOf(Item) == -1)
      {
         DestItems->Add(Item);
      }
      if(SourceItems->IndexOf(Item) != -1)
      {
         SourceItems->Remove(Item);
         isItemTransferred = true;
      }
      ShowItems();
   }
}
//---------------------------------------------------------------------------

void __fastcall TfrmTransferItem::StringGrid2Click(TObject *Sender)
{
   if(DestItems->Count > StringGrid2->Row)
   {
      TItemComplete *Item = (TItemComplete *)DestItems->Items[StringGrid2->Row];
      if(Item->OrderKey != 0)
      {
         if(SourceItems->IndexOf(Item) == -1)
         {
            SourceItems->Add(Item);
         }
         if(DestItems->IndexOf(Item) != -1)
         {
            DestItems->Remove(Item);
         }
         ShowItems();
      }
   }
}
//---------------------------------------------------------------------------


