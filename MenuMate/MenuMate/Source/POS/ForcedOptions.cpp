// ---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "ForcedOptions.h"
#include "MMMessageBox.h"
#include "MMLogging.h"

// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

#include "SelectDish.h"

// ---------------------------------------------------------------------------

#pragma warn -par
void
TfrmSelectForcedOptions::ForwardItemDefinitionRefresh(
  Messages::TMessage &message)
const
{
   frmSelectDish->Perform(
     item_management::network_messaging::refresh_item_display, 0, 0);
}
#pragma warn .par

__fastcall TfrmSelectForcedOptions::TfrmSelectForcedOptions(TComponent* Owner) : TZForm(Owner)
{
}

void __fastcall TfrmSelectForcedOptions::FormCreate(TObject *Sender)
{
	SelectedOptions = new TListOptionContainer;
	NextButton = new TOptionBtn(this);
	CloseButton = new TOptionBtn(this);
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedOptions::FormDestroy(TObject *Sender)
{
	delete SelectedOptions;
	delete NextButton;
	delete CloseButton;
}
// ---------------------------------------------------------------------------

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectForcedOptions::FormShow(TObject *Sender)
{
	FormResize(NULL);

	const int PixelGap = pnlCondiments->Left;
	SelectedOptions->OptionClear();

	bool AllowSkip = false;
	bool AllowMultiSelect = true;

	if (ForcedOptionsList != NULL)
	{
		if (ForcedOptionsList->Count > 0)
		{
			AllowSkip = ForcedOptionsList->OptionGet(0)->AllowSkip;
            AllowMultiSelect = !ForcedOptionsList->OptionGet(0)->DisallowMuliSelect;
		}
		else
		{
			AllowSkip = true;
		}
	}

	CloseButton->Parent = this->pnlSide;
	CloseButton->Visible = true;
	CloseButton->Font->Color = clWhite;
	CloseButton->Font->Name = "Arial";
	CloseButton->Font->Size = 14;
	CloseButton->Font->Style = TFontStyles() << fsBold;
	CloseButton->Caption = "Cancel Order";
	CloseButton->ptrOption = NULL;
	CloseButton->Left = btnMsgUp->Left;
	CloseButton->Height = btnMsgUp->Height;
	CloseButton->Width = btnMsgUp->Width;
	CloseButton->Top = pnlCondiments->Top;
	CloseButton->ButtonColor = clMaroon;
	CloseButton->OnMouseClick = BtnCancelClick;

	if (AllowSkip || AllowMultiSelect)
	{
		NextButton->Parent = this->pnlSide;
		NextButton->Visible = true;
		NextButton->Font->Color = clWhite;
		NextButton->Font->Name = "Arial";
		NextButton->Font->Size = 14;
		NextButton->Font->Style = TFontStyles() << fsBold;

		NextButton->Caption = "Next";
		NextButton->ptrOption = NULL;
		NextButton->Left = btnMsgUp->Left;
		NextButton->Height = btnMsgUp->Height;
		NextButton->Width = btnMsgUp->Width;
		NextButton->Top = (CloseButton->Top * 2) + CloseButton->Height;
		NextButton->ButtonColor = clNavy;
		NextButton->OnMouseClick = BtnCloseClick;
	}
	else
	{
		NextButton->Visible = false;
	}

	try
	{

		int ButtonWidth;
		int x = 0;
		int y = 0;
		int ButtonCount = 0;

		if (ForcedOptionsList != NULL)
		{
			int ButtonWidth = (pnlCondiments->Width) / TGlobalSettings::Instance().DefaultColumns;
			int ButtonHeight = pnlCondiments->Height / TGlobalSettings::Instance().DefaultRows;
            int font_size = TGlobalSettings::Instance().posButtonFontSize & ~0x80;

			for (int i = 0; i < ForcedOptionsList->Count; i++)
			{
				TOptionBtn *NewButton = new TOptionBtn(this);
				NewButton->Parent = sbOptions;
				NewButton->Visible = true;
				NewButton->Font->Color = clWhite;
				NewButton->Font->Name = "Arial";
				NewButton->Font->Size = font_size;
				NewButton->Font->Style = TFontStyles() << fsBold;
				NewButton->ptrOption = ForcedOptionsList->OptionGet(i);
				NewButton->Caption = NewButton->ptrOption->Name;
				NewButton->ButtonColor = clNavy;
				NewButton->LatchedColor = clGreen;
				NewButton->LatchingType = ltCheckbox;
				NewButton->Height = 65;
				NewButton->Width = ButtonWidth;
				NewButton->OnMouseClick = BtnOptionClick;
				NewButton->Tag = i;

				NewButton->Left = x;
				NewButton->Top = y;
				NewButton->Width = ButtonWidth;
				NewButton->Height = ButtonHeight;

				x += NewButton->Width;

				if (ButtonCount ==  TGlobalSettings::Instance().DefaultColumns-1)
				{
					x = 0;
					y += NewButton->Height;
					ButtonCount = 0;
				}
				else
				{
					ButtonCount++;
				}

			}
		}
	}
	catch(Exception & E)
	{
		TManagerLogs::Instance().Add(__FUNC__, EXCEPTIONLOG, E.Message);
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedOptions::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(this);
}

void __fastcall TfrmSelectForcedOptions::BtnCloseClick(TObject *Sender)
{
	ModalResult = mrOk;
	// Close();
}

void __fastcall TfrmSelectForcedOptions::BtnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
	// Close();
}

__fastcall TOptionBtn::TOptionBtn(Classes::TComponent* AOwner) : TTouchBtn(AOwner) // TCustomStaticText(AOwner)
{
	Caption = "";
	Font->Color = clBlack;
	Font->Name = "Arial";
	Font->Style = TFontStyles() << fsBold;
}

void __fastcall TfrmSelectForcedOptions::btnMsgDownClick(TObject *Sender)
{
	if (sbOptions->VertScrollBar->Position < (sbOptions->VertScrollBar->Range - sbOptions->ClientHeight))
	{
		for (int i = 0; i < (sbOptions->VertScrollBar->Range - sbOptions->ClientHeight); i += 3)
		{
			sbOptions->VertScrollBar->Position = i;
			sbOptions->Update();
		}
		sbOptions->VertScrollBar->Position = sbOptions->VertScrollBar->Range - sbOptions->ClientHeight;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedOptions::btnMsgUpClick(TObject *Sender)
{
	if (sbOptions->VertScrollBar->Position > 0)
	{
		for (int i = sbOptions->VertScrollBar->Position; i > 0; i -= 3)
		{
			sbOptions->VertScrollBar->Position = i;
			sbOptions->Update();
		}
		sbOptions->VertScrollBar->Position = 0;
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedOptions::FormResize(TObject *Sender)
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
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedOptions::FormClose(TObject *Sender, TCloseAction &Action)
{
	while (sbOptions->ControlCount > 0)
	{
		delete sbOptions->Controls[0];
	}
}
// ---------------------------------------------------------------------------

void __fastcall TfrmSelectForcedOptions::pnlCoursesResize(TObject *Sender)
{
	sbOptions->Align = alClient;
	sbOptions->Align = alNone;
	sbOptions->Width += 20; // sbCourses->VertScrollBar->Size;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmSelectForcedOptions::BtnOptionClick(TObject *Sender)
{
	if (ForcedOptionsList != NULL)
	{
		TOptionBtn *Button = (TOptionBtn*)Sender;
		if (Button->Color == clGreen)
		{
			SelectedOptions->OptionDelete(Button->ptrOption);
			Button->ptrOption = ForcedOptionsList->OptionGet(Button->Tag);
			Button->Color = clNavy;
		}
		else
		{
			if (Button->ptrOption->PlusOption && Button->ptrOption->MinusOption)
			{
				// Prompt
				if (CustomMessageBox("Select Option Type +/-", "Select Option Type", MB_ICONQUESTION, "+", "-") == IDOK)
				{
					Button->ptrOption->IsPlus = true;
				}
				else
				{
					Button->ptrOption->IsPlus = false;
				}
			}
			else if (Button->ptrOption->PlusOption)
			{
				Button->ptrOption->IsPlus = true;
			}
			else if (Button->ptrOption->MinusOption)
			{
				Button->ptrOption->IsPlus = false;
			}
			int index = SelectedOptions->OptionAdd(Button->ptrOption);
			Button->ptrOption = SelectedOptions->OptionGet(index);
			Button->Color = clGreen;
		}

		if (ForcedOptionsList->OptionGet(0)->DisallowMuliSelect)
		{
			ModalResult = mrOk;
		}
	}
}
