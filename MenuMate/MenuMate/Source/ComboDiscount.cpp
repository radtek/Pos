//---------------------------------------------------------------------------


#pragma hdrstop

#include "ComboDiscount.h"
#include "ItemComplete.h"
#include "DBOrder.h"



//---------------------------------------------------------------------------
TComboDiscount::TComboDiscount()
{
}


TComboDiscount::TComboDiscount(TForm *inDisplayOwner, TDiscount inDiscount, TList *inOrders):
Discount(inDiscount), ComboGUI(inDisplayOwner, inDiscount, inOrders)
{
	Initialise(inOrders);
}

void TComboDiscount::Initialise(TList *inOrders)
{
	int pos = 0;
	for (int i = 0; i < inOrders->Count; i++)
	{
		TItemMinorComplete *or = reinterpret_cast<TItemMinorComplete *>(inOrders->Items[i]);
		int k = or->GetQty();

		for(int j = 0; j < or->GetQty() ; j++)
		{
            if(!or->DiscountFind(DiscModeCombo) || (j < or->GetQty() - or->SelectedItems))
			{
                addOrderToCollection(or, pos);
				pos++;
			}
     	}
	}
    ComboGUI.MemberSelected = IsMemberSelected;
	ComboGUI.DisplayGUI(Discount.MaximumValue);
}

TList * TComboDiscount::GetOrders(void)
{
	TComboOrders<int> temp = ComboGUI.GetSelectedOrders();
	if(ComboGUI.MResult == mrCancel || (temp.size() == 0))
		return NULL;
	List = new TList;
	TItemMinorComplete *or = NULL ;
	TItemMinorComplete *prev = NULL ;

	std::vector<int>::iterator it = temp.begin();
	for(; it != temp.end(); ++it)
	{
		 or = Orders.getelement(*it);
		 if(or != prev)
		 {
			or->SelectedItems++;
			int l = or->SubOrders->Count;
			for (int m = 0; m < l; m++)
			{
				TItemCompleteSub *sub = reinterpret_cast<TItemCompleteSub *>(or->SubOrders->SubOrderGet(m));
				sub->SelectedItems++;
			}
			List->Add(or);
		 }
		 else
		 {
			prev->SelectedItems++;
			int l = or->SubOrders->Count;
			for (int m = 0; m < l; m++)
			{
				TItemCompleteSub *sub = reinterpret_cast<TItemCompleteSub *>(or->SubOrders->SubOrderGet(m));
				sub->SelectedItems++;
			}
		 }
		 prev = or;
	}
	return List;
}

void TComboDiscount::addOrderToCollection(TItemMinorComplete* or, int index)
{
    Currency subprice = 0;
    int l = or->SubOrders->Count;
    for (int m = 0; m < l; m++)
    {
        TItemCompleteSub *sub = reinterpret_cast<TItemCompleteSub *>(or->SubOrders->SubOrderGet(m));
        subprice += sub->GrandTotalExcCombo();
    }
    Orders.insert(or);
    ComboGUI.LoadGUI(or->Item, (or->GrandTotalExcCombo() + subprice)/or->GetQty(), index);
}

TfrmComboGUI::TfrmComboGUI()
{
}

TfrmComboGUI::TfrmComboGUI(TForm *inOwner, TDiscount inDiscount, TList *inOrders) : frmListManager(new TfrmListManager(inOwner)),
Owner(inOwner), Total(0), Max(0), MemberSelected(MemberSelected)
{
	int pos = 0;
    MaxItemAffected = inDiscount.MaxItemAffected;
    MinItemRequired = inDiscount.MinItemRequired;
	for (int i = 0; i < inOrders->Count; i++)
	{
		TItemMinorComplete *or = reinterpret_cast<TItemMinorComplete *>(inOrders->Items[i]);
		for(int j = 0; j < or->GetQty() ; j++)
		{
            if(!or->DiscountFind(DiscModeCombo) || (j < or->GetQty() - or->SelectedItems))
            {
				pos++;
            }
        }
    }
	InitialiseGUI(inDiscount.Description == "" ? inDiscount.Name : inDiscount.Description, pos);
}

TfrmComboGUI::TfrmComboGUI(TForm *inOwner, TDiscount inDiscount) : frmListManager(new TfrmListManager(inOwner)),
Owner(inOwner), Total(0), Max(0), MemberSelected(MemberSelected)
{
}

void TfrmComboGUI::InitialiseGUI(AnsiString Caption, const int rows)
{
	const int size = 6;
	frmListManager->sgDisplay->RowCount = rows;
	frmListManager->sgDisplay->Options =
    frmListManager->sgDisplay->Options << goRowSelect;
	frmListManager->Width = 600;
	frmListManager->pnlMinor->Width = 200;
	frmListManager->sgDisplay->ColWidths[0] = 300;
	frmListManager->tbtnEdit->Visible = false;
	frmListManager->tbtnDelete->Visible = false;
	frmListManager->tbtnAdd->Caption = "Cancel";
	frmListManager->tbtnAdd->Color = clWebGreenYellow;
	frmListManager->tbtnAdd->Width = 172;
	frmListManager->btnClose->Width = 172;
	frmListManager->btnClose->Enabled = false;
	frmListManager->Caption = Caption;
	frmListManager->Select.RegisterForEvent(OnSelect);
	frmListManager->Close.RegisterForEvent(OnClose);
	frmListManager->Add.RegisterForEvent(OnCancel);
	frmListManager->DrawCell.RegisterForEvent(OnDrawCell);

	frmListManager->Panel2->Align = alClient;
	frmListManager->Panel3->Align = alClient;
	frmListManager->pnlMajor->Align = alLeft;
	frmListManager->pnlMinor->Align = alRight;
	frmListManager->pnlMajor->Width = 500;
	frmListManager->pnlMinor->Width = 190;

	frmListManager->Height = 750;
	frmListManager->BorderWidth = 0;
	frmListManager->Panel3->Top = 30;
	frmListManager->Panel3->Left = 5;
	frmListManager->Width = 720;
	frmListManager->Panel2->Width = 700;
	frmListManager->Panel3->BorderWidth = 5;

	TLabel *Labels[3];
	Labels[0] = new TLabel(frmListManager->Panel2);
	Labels[0]->Top    = 10;
	Labels[0]->Left   = 10;
	Labels[0]->Width  = 150;
	Labels[0]->Height = 50;
	Labels[0]->Font->Style = TFontStyles() << fsBold;
	Labels[0]->Font->Size = 13;
	Labels[0]->Caption = Caption;
	Labels[0]->Parent = frmListManager->Panel2;

	Labels[1] = new TLabel(frmListManager->pnlLabel);
	Labels[1]->Top    = 5;
	Labels[1]->Left   = 20;
	Labels[1]->Width  = frmListManager->pnlLabel->Width/2;
	Labels[1]->Height = frmListManager->pnlLabel->Height;
	Labels[1]->Font->Color = clWhite;
	Labels[1]->Font->Size = 15;
	Labels[1]->Caption = "Item";
	Labels[1]->Parent = frmListManager->pnlLabel;

	Labels[2] = new TLabel(frmListManager->pnlLabel);
	Labels[2]->Top    = 5;
	Labels[2]->Left   = 320;
	Labels[2]->Width  = 100;
	Labels[2]->Height = frmListManager->pnlLabel->Height;
	Labels[2]->Font->Color = clWhite;
	Labels[2]->Font->Size = 15;
	Labels[2]->Caption = "Price";
	Labels[2]->Parent = frmListManager->pnlLabel;

	Edit[0] = new TEdit(frmListManager->pnlMinor);
	Edit[0]->Top    = 530;
	Edit[0]->Left   = 8;
	Edit[0]->Width  = 172;
	Edit[0]->Height = 50;
	Edit[0]->Enabled = false;
	Edit[0]->Font->Style = TFontStyles() << fsBold;
	Edit[0]->Parent = frmListManager->pnlMinor;

	Edit[1] = new TEdit(frmListManager->pnlMinor);
	Edit[1]->Top    = 580;
	Edit[1]->Left   = 8;
	Edit[1]->Width  = 172;
	Edit[1]->Height = 50;
	Edit[1]->Enabled = false;
	Edit[0]->Font->Style = Edit[0]->Font->Style;
	Edit[1]->Parent = frmListManager->pnlMinor;

	Edit[2] = new TEdit(frmListManager->pnlMinor);
	Edit[2]->Top    = 630;
	Edit[2]->Left   = 8;
	Edit[2]->Width  = 172;
	Edit[2]->Height = 50;
	Edit[2]->Enabled = false;
	Edit[0]->Font->Style = Edit[0]->Font->Style;
	Edit[2]->Parent = frmListManager->pnlMinor;

	Label[0] = new TLabel(frmListManager->pnlMinor);
	Label[0]->Top    = 510;
	Label[0]->Left   = 8;
	Label[0]->Width  = 150;
	Label[0]->Height = 50;
	Label[0]->Font->Style = TFontStyles() << fsBold;
	Label[0]->Font->Size = 13;
    Label[0]->Caption = "Maximum Price";
	Label[0]->Parent = frmListManager->pnlMinor;

	Label[1] = new TLabel(frmListManager->pnlMinor);
	Label[1]->Top    = 560;
	Label[1]->Left   = 8;
	Label[1]->Width  = 150;
	Label[1]->Height = 50;
	Label[1]->Font->Style = TFontStyles() << fsBold;
	Label[1]->Font->Size = 13;
	Label[1]->Caption = "Price Selected";
	Label[1]->Parent = frmListManager->pnlMinor;

	Label[2] = new TLabel(frmListManager->pnlMinor);
	Label[2]->Top    = 610;
	Label[2]->Left   = 8;
	Label[2]->Width  = 150;
	Label[2]->Height = 50;
	Label[2]->Font->Style = TFontStyles() << fsBold;
	Label[2]->Font->Size = 13;
	Label[2]->Caption = "Price Remaining";
	Label[2]->Parent = frmListManager->pnlMinor;




}

void TfrmComboGUI::ChangeObject(TControl *Object,
											int height,
											int width,
											int top,
											int left)
{
	Object->Height  = height;
	Object->Width   = width;
	Object->Top     = top;
	Object->Left    = left;
}

void TfrmComboGUI::LoadGUI(AnsiString Name, Currency Price, int pos)
{
	frmListManager->sgDisplay->Cols[0]->AddObject(Name,(TObject*)pos);
	frmListManager->sgDisplay->Cols[1]->AddObject(FormatFloat("$0.00", Price),(TObject*)pos);
	Prices.push_back(Price);
}

void TfrmComboGUI::DisplayGUI(Currency inMax, const int ItemCount)
{
	frmListManager->sgDisplay->RowCount = ItemCount;
    DisplayGUI(inMax);
}

void TfrmComboGUI::DisplayGUI(Currency inMax)
{
	Max = inMax;
	Edit[0]->Text = FormatFloat("0.00", Max);
	Edit[1]->Text = FormatFloat("0.00", Total);
	Edit[2]->Text = FormatFloat("0.00", Max - Total);
	if(frmListManager->ShowModal() == mrRetry)
		DisplayGUI(Max);
}

void TfrmComboGUI::OnDrawCell(int Row, int Col)
{
	TCanvas *pCanvas = frmListManager->sgDisplay->Canvas;
	pCanvas->Font = frmListManager->sgDisplay->Font;
	pCanvas->Brush->Color = frmListManager->sgDisplay->Color;
	TRect Rect = frmListManager->sgDisplay->CellRect(Col, Row);

	TColor Color = pCanvas->Brush->Color;
	TBrushStyle Style = pCanvas->Brush->Style;

	std::vector<int>::iterator it = Orders.find(Row);
	if(it != NULL)
	{
		pCanvas->Font->Color = clWhite;
		pCanvas->Font->Style = TFontStyles() << fsBold;
		pCanvas->Brush->Color = clNavy;
		pCanvas->Brush->Style = bsSolid;
		pCanvas->FillRect(Rect); // clear the rectangle
	}
	else
	{
		pCanvas->Font->Color = clBlack;
		pCanvas->Font->Style = TFontStyles();
		pCanvas->FillRect(Rect); // clear the rectangle
	}


	UnicodeString Caption = frmListManager->sgDisplay->Cells[Col][Row];
	TRect DrawRegRect(Rect.Left+2, Rect.Top, Rect.Right-2, Rect.Bottom);

	int TextHeight = DrawTextExW(pCanvas->Handle, Caption.w_str(), Caption.Length(),&DrawRegRect,DT_CALCRECT|DT_WORDBREAK|DT_NOPREFIX,NULL) + 4;

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

void TfrmComboGUI::OnSelect(int Row, int Col)
{
	if(Row < Prices.size())
	{
		std::vector<int>::iterator it = Orders.find(Row);
		if(it == NULL && (Total + Prices.at(Row)) <= Max &&
          (MaxItemAffected == 0 || MaxItemAffected >  Orders.size()))
		{
			Orders.insert(Row);
			Orders.organiseints();
			Total = Total + Prices.at(Row);
			Edit[1]->Text = FormatFloat("0.00", Total);
		}
		else if(it != NULL)
		{
			Orders.erase(it);
			Edit[1]->Text = FormatFloat("0.00", Total = Total - Prices.at(Row));
		}
		Edit[2]->Text = FormatFloat("0.00", Max - Total);

		frmListManager->btnClose->Enabled = (Orders.size() > 1 && Orders.size() >= MinItemRequired);
	}
}

void TfrmComboGUI::OnClose(int Row, int Col)
{
	frmListManager->ModalResult = mrOk;
	MResult = mrOk;
}

void TfrmComboGUI::OnCancel(int Row, int Col)
{
	frmListManager->ModalResult = mrCancel;
	MResult = mrCancel;
}

