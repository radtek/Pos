// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ForcedSides.h"
#include "SelectDish.h"
#include "MMLogging.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------

#pragma warn -par
void
TfrmSelectForcedSides::ForwardItemDefinitionRefresh(
  Messages::TMessage &message)
{
   frmSelectDish->Perform(
     item_management::network_messaging::refresh_item_display, 0, 0);

   FormShow(this);
}
#pragma warn .par

__fastcall TfrmSelectForcedSides::TfrmSelectForcedSides(TComponent* Owner, Database::TDBTransaction &inDBTransaction) : TZForm(Owner), DBTransaction(inDBTransaction)
{
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectForcedSides::FormCreate(TObject *Sender)
{
	CloseButton = new TItemBtn(this);
	NextButton = new TItemBtn(this);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedSides::FormDestroy(TObject *Sender)
{
	delete CloseButton;
	delete NextButton;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectForcedSides::FormShow(TObject *Sender)
{
	FormResize(NULL);

	const int PixelGap = pnlCondiments->Left;

	bool AllowSkip = false;

	if (ForcedSidesList != NULL && ForcedSidesList->Count > 0)
	{
		AllowSkip = ForcedSidesList->SideGet(0)->AllowSkip;
		if (!AllowSkip)
		{
			if (ForcedSidesList->SideGet(0)->MaxSelect == 0)
			{
				AllowSkip = true;
			}
		}
	}

	CloseButton->Parent = this->pnlSide;
	CloseButton->Visible = true;
	CloseButton->Font->Color = clWhite;
	CloseButton->Font->Name = "Arial";
	CloseButton->Font->Size = 14;
	CloseButton->Font->Style = TFontStyles() << fsBold;

	CloseButton->Caption = "Cancel Order";
	CloseButton->ptrSide = NULL;
	CloseButton->Left = btnMsgUp->Left;
	CloseButton->Height = btnMsgUp->Height;
	CloseButton->Width = btnMsgUp->Width;
	CloseButton->Top = pnlCondiments->Top;
	CloseButton->ButtonColor = clMaroon;
	CloseButton->OnMouseClick = BtnCancelClick;

	if (AllowSkip)
	{
		NextButton->Parent = this->pnlSide;
		NextButton->Visible = true;
		NextButton->Font->Color = clWhite;
		NextButton->Font->Name = "Arial";
		NextButton->Font->Size = 14;
		NextButton->Font->Style = TFontStyles() << fsBold;

		NextButton->Caption = "Next";
		NextButton->ptrSide = NULL;
		NextButton->Left = btnMsgUp->Left;
		NextButton->Height = btnMsgUp->Height;
		NextButton->Width = btnMsgUp->Width;
		NextButton->Top = (2 * CloseButton->Top) + CloseButton->Height;
		NextButton->ButtonColor = clNavy;
		NextButton->OnMouseClick = BtnCloseClick;
	}
	else
	{
		NextButton->Visible = false;
	}

	try
	{
		int x = 0;
		int y = 0;
		int ButtonCount = 0;
		sbOptions->VertScrollBar->Position = 0;

		if (ForcedSidesList != NULL)
		{
         int font_size = TGlobalSettings::Instance().posButtonFontSize & ~0x80;

			for (int i = 0; i < ForcedSidesList->Count; i++)
			{
				TItemBtn *NewButton = new TItemBtn(this);
				NewButton->Parent = sbOptions;
				NewButton->Visible = true;
				NewButton->Font->Color = clWhite;
				NewButton->Font->Name = "Arial";
				NewButton->Font->Size = font_size;
				NewButton->Font->Style = TFontStyles() << fsBold;
				NewButton->ptrSide = ForcedSidesList->SideGet(i);
				NewButton->Caption = NewButton->ptrSide->Item->GetDecoratedName();
				NewButton->Enabled = NewButton->ptrSide->Item->Enabled;
				NewButton->OnMouseClick = BtnOptionClick;
				NewButton->Tag = i;

				double R, G, B; // input RGB values
				R = GetRValue(NewButton->ptrSide->Item->SetColour);
				G = GetGValue(NewButton->ptrSide->Item->SetColour);
				B = GetBValue(NewButton->ptrSide->Item->SetColour);
				double L = (0.2126 * R) + (0.7152 * G) + (0.0722 * B);
				if (L > 128)
				{
					NewButton->Font->Color = clBlack;
				}
				else
				{
					NewButton->Font->Color = clWhite;
				}
				NewButton->ButtonColor = NewButton->ptrSide->Item->SetColour;
				NewButton->LatchedColor = TColor(NewButton->ptrSide->Item->SetColour - 100);
				NewButton->LatchingType = ltCheckbox;
				NewButton->Left = x;
				NewButton->Top = y;
				NewButton->Width = ButtonWidth;
				NewButton->Height = ButtonHeight;

				x += NewButton->Width;

				if (ButtonCount == (TGlobalSettings::Instance().DefaultColumns-1))
				{
					x = 0;
					y += NewButton->Height;
					ButtonCount = 0;
				}
				else
				{
					ButtonCount++;
				}
				sbOptions->VertScrollBar->Range = NewButton->Top + NewButton->Height;
			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedSides::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}

void __fastcall TfrmSelectForcedSides::BtnCloseClick(TObject *Sender)
{
	ModalResult = mrOk;
}

void __fastcall TfrmSelectForcedSides::BtnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}

__fastcall TItemBtn::TItemBtn(Classes::TComponent* AOwner) : TTouchBtn(AOwner) // TCustomStaticText(AOwner)
{
	Caption = "";
	BevelInner = bvNone;
	Selected = false;
}

void __fastcall TfrmSelectForcedSides::btnMsgDownClick(TObject *Sender)
{
	sbOptions->VertScrollBar->Position += ButtonHeight << 1;
	sbOptions->Update();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedSides::btnMsgUpClick(TObject *Sender)
{
	sbOptions->VertScrollBar->Position -= ButtonHeight << 1;
	sbOptions->Update();
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedSides::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
		int Temp = Tag;
		Tag = Screen->Width;
		ScaleBy(Screen->Width, Temp);
	}
	pnlSide->Left = pnlCondiments->Left + pnlCondiments->Left + pnlCondiments->Width;
	pnlSide->Top = pnlCondiments->Top;
	pnlSide->Height = pnlCondiments->Height;
	ClientWidth = pnlSide->Left + pnlSide->Width + pnlCondiments->Left;
	ClientHeight = pnlCondiments->Height + (pnlCondiments->Top * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;

	btnMsgUp->Top = pnlSide->Height - (btnMsgUp->Height * 2) - (pnlCondiments->Left * 2);
	btnMsgDown->Top = pnlSide->Height - btnMsgDown->Height - pnlCondiments->Left;
	btnMsgUp->Left = pnlCondiments->Left;
	btnMsgDown->Left = pnlCondiments->Left;

	sbOptions->Align = alClient;
	sbOptions->Align = alNone;
	sbOptions->Width += 20; // sbCourses->VertScrollBar->Size;

	ButtonWidth =
	  pnlCondiments->Width / TGlobalSettings::Instance().DefaultColumns;
	ButtonHeight =
	  pnlCondiments->Height / TGlobalSettings::Instance().DefaultRows;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedSides::FormClose(TObject *Sender, TCloseAction &Action)
{
	while (sbOptions->ControlCount > 0)
	{
		delete sbOptions->Controls[0];
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedSides::pnlCoursesResize(TObject *Sender)
{
	sbOptions->Align = alClient;
	sbOptions->Align = alNone;
	sbOptions->Width += 20; // sbCourses->VertScrollBar->Size;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectForcedSides::BtnOptionClick(TObject *Sender)
{
	if (ForcedSidesList != NULL)
	{
		TItemBtn *Button = (TItemBtn*)Sender;
		if (Button->Selected)
		{
			Button->Color = Button->ptrSide->Item->SetColour;
			Button->Selected = false;

         frmSelectDish->RemoveSideItemFromItem(Button->ptrSubItem, MasterItem);

			// Retreive the Forced Options, Glass sise, etc.
			MasterItem->SubOrders->SubOrderDelete(Button->ptrSubItem);
		}
		else
		{
			Button->Color = TColor(Button->ptrSide->Item->SetColour - 100);
			Button->Selected = true;
			// Retreive the Forced Options, Glass sise, etc.
			Button->ptrSubItem = frmSelectDish->AddSubItemToItem(DBTransaction, Button->ptrSide->Item, MasterItem);
		}

		if (Button->ptrSide->MaxSelect == 1)
		{
			ModalResult = mrOk;
		}
	}
}

