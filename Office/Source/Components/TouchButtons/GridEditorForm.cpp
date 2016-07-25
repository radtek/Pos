//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop           

#include "GridEditorForm.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "CSPIN"
#pragma link "TouchControls"  
#pragma link "TouchGrid"
#pragma link "TouchBtn"
#pragma link "TouchKeyboard"
#pragma link "TouchNumpad"
#pragma link "TouchControls"
#pragma link "TouchGrid"
#pragma resource "*.dfm"
__fastcall TfrmEditGrid::TfrmEditGrid(TComponent* Owner)
	: TForm(Owner)
{                        
}               
//---------------------------------------------------------------------------
bool TfrmEditGrid::SelectedButtons(TButtons &Buttons)
{
	Buttons.clear();
	for (int i=0; i<TouchGrid->RowCount; i++)
	{
		for (int j=0; j<TouchGrid->ColCount; j++)
		{
			if (TouchGrid->Buttons[i][j]->Latched)
			{
				Buttons.push_back(TouchGrid->Buttons[i][j]);
			}
		}
	}
	return Buttons.size() > 0;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::seRowCountChange(TObject *Sender)
{
	try
	{
		TouchGrid->RowCount = seRowCount->Value;
	}
	__finally
	{
		seRowCount->Value	= TouchGrid->RowCount;
		seColCount->Value	= TouchGrid->ColCount;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::seColCountChange(TObject *Sender)
{
	try
	{
		TouchGrid->ColCount = seColCount->Value;
	}
	__finally
	{
		seRowCount->Value	= TouchGrid->RowCount;
		seColCount->Value	= TouchGrid->ColCount;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::edCaptionChange(TObject *Sender)
{
	TButtons CurrentButtons;  
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->Caption = edCaption->Text;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::chbVisibleClick(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->Visible = chbVisible->Checked;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::chbEnabledClick(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->Enabled = chbEnabled->Checked;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::seColWidthChange(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			TouchGrid->ColWidths[TouchGrid->Col(*iButton)] = seColWidth->Value;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::seRowHeightChange(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			TouchGrid->RowHeights[TouchGrid->Row(*iButton)] = seRowHeight->Value;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button1Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		if (ColorDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->Color = ColorDialog->Color;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button18Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		ColorDialog->Color = (*CurrentButtons.begin())->Color;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button3Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		if (ColorDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->LatchedColor = ColorDialog->Color;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button21Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		if (ColorDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->DisabledColor = ColorDialog->Color;
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button22Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		ColorDialog->Color = (*CurrentButtons.begin())->DisabledColor;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button17Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		ColorDialog->Color = (*CurrentButtons.begin())->LatchedColor;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button2Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		FontDialog->Font = CurrentButtons[0]->Font;
		if (FontDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->Font->Assign(FontDialog->Font);
			}
			chbParentFont->Checked = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button4Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		if (ColorDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->FontColor = ColorDialog->Color;
			}
			chbParentFontColor->Checked = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button19Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		ColorDialog->Color = (*CurrentButtons.begin())->FontColor;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button5Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		if (OpenDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->Glyph->LoadFromFile(OpenDialog->FileName);
			}
		}
		else
		{
			if (Application->MessageBox("Do you wish to clear the glyph/s?", "Clear Glyphs?", MB_ICONQUESTION + MB_YESNO) == ID_YES)
			{
				for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
				{
					(*iButton)->Glyph->Assign(NULL);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::seAlphaChange(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->GlyphAlpha = seAlpha->Value;
		}
		seAlpha->Value = CurrentButtons[0]->GlyphAlpha;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::TouchGridMouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, TGridButton *GridButton,
		int X, int Y)
{
	if (GridButton)
	{
		if (Shift.Contains(ssShift))
		{
			TouchGrid->LatchingType = ltCheckbox;
		}
		else
		{
			TouchGrid->LatchingType = ltRadioButton;
		}
	}
	else
	{
		if (!Shift.Contains(ssShift))
		{
			TouchGrid->LatchingType = ltRadioButton;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::TouchGridMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, TGridButton *GridButton,
      int X, int Y)
{
	edCaption->OnChange							= NULL;
	edTag->OnChange								= NULL;
	seColWidth->OnChange							= NULL;
	seRowHeight->OnChange						= NULL;
	seAlpha->OnChange								= NULL;
	chbTransparent->OnClick						= NULL;
	chbParentFont->OnClick						= NULL;
	chbParentFontColor->OnClick				= NULL;
	chbParentLatchedFont->OnClick				= NULL;
	chbParentLatchedFontColor->OnClick		= NULL;
	chbKeepAspect->OnClick						= NULL;
	seGlyphScale->OnChange						= NULL;
	chbVisible->OnClick							= NULL;
	chbEnabled->OnClick							= NULL;

	if (GridButton)
	{
		if (Shift.Contains(ssShift))
		{
			TouchGrid->LatchingType = ltCheckbox;
		}
		else
		{
			TouchGrid->LatchingType = ltRadioButton;
		}
		edCaption->Enabled						= true;
		edTag->Enabled								= true;
		seColWidth->Enabled						= true;
		seRowHeight->Enabled						= true;
		seAlpha->Enabled							= true;
		chbTransparent->Enabled					= true;
		chbParentFont->Enabled					= true;
		chbParentFontColor->Enabled			= true;
		chbParentLatchedFont->Enabled			= true;
		chbParentLatchedFontColor->Enabled	= true;
		chbKeepAspect->Enabled					= true;
		seGlyphScale->Enabled					= true;
		chbVisible->Enabled						= true;
		chbEnabled->Enabled						= true;

		edCaption->Text							= GridButton->Caption;
		edTag->Text									= IntToStr(GridButton->Tag);
		seColWidth->Value							= TouchGrid->ColWidths[TouchGrid->Col(GridButton)];
		seRowHeight->Value						= TouchGrid->RowHeights[TouchGrid->Row(GridButton)];
		seAlpha->Value								= GridButton->GlyphAlpha;
		chbTransparent->Checked					= GridButton->TransparentGlyph;
		chbParentFont->Checked					= GridButton->ParentFont;
		chbParentFontColor->Checked			= GridButton->ParentFontColor;
		chbParentLatchedFont->Checked			= GridButton->ParentLatchedFontColor;
		chbParentLatchedFontColor->Checked	= GridButton->ParentLatchedFontColor;
		chbKeepAspect->Checked					= GridButton->KeepGlyphAspect;
		seGlyphScale->Value						= GridButton->GlyphScale;
		chbVisible->Checked						= GridButton->Visible;
		chbEnabled->Checked						= GridButton->Enabled;
	}
	else
	{
		if (!Shift.Contains(ssShift))
		{
			TouchGrid->LatchingType = ltRadioButton;
		}
		edCaption->Text							= "";
		edTag->Text									= "";
		seColWidth->Value							= 0;
		seRowHeight->Value						= 0;
		seAlpha->Value								= 0;
		seGlyphScale->Value						= 0;
		chbKeepAspect->Checked					= false;
		chbVisible->Checked						= false;
		chbEnabled->Checked						= false;
		chbTransparent->Checked					= false;
		chbParentFont->Checked					= false;
		chbParentFontColor->Checked			= false;
		chbParentLatchedFont->Checked			= false;
		chbParentLatchedFontColor->Checked	= false;

		chbTransparent->Enabled					= false;
		chbParentFont->Enabled					= false;
		chbParentFontColor->Enabled			= false;
		chbParentLatchedFont->Enabled			= false;
		chbParentLatchedFontColor->Enabled	= false;
		chbKeepAspect->Enabled					= false;
		seGlyphScale->Enabled					= false;
		edCaption->Enabled						= false;
		edTag->Enabled								= false;
		seColWidth->Enabled						= false;
		seRowHeight->Enabled						= false;
		seAlpha->Enabled							= false;
		seGlyphScale->Enabled					= false;
		chbVisible->Enabled						= false;
		chbEnabled->Enabled						= false;
	}
	edCaption->OnChange							= edCaptionChange;
	edTag->OnChange								= edTagChange;
	seColWidth->OnChange							= seColWidthChange;
	seRowHeight->OnChange						= seRowHeightChange;
	seAlpha->OnChange								= seAlphaChange;
	chbTransparent->OnClick						= chbTransparentClick;
	chbParentFont->OnClick						= chbParentFontClick;
	chbParentFontColor->OnClick				= chbParentFontColorClick;
	chbParentLatchedFont->OnClick				= chbParentLatchedFontClick;
	chbParentLatchedFontColor->OnClick		= chbParentLatchedFontColorClick;
	chbKeepAspect->OnClick						= chbKeepAspectClick;
	seGlyphScale->OnChange						= seGlyphScaleChange;
	chbVisible->OnClick							= chbVisibleClick;
	chbEnabled->OnClick							= chbEnabledClick;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button10Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		if (ColorDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->TransparentColor = ColorDialog->Color;
			}
		}
	}
}                                         
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::chbTransparentClick(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->TransparentGlyph = chbTransparent->Checked;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::chbKeepAspectClick(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->KeepGlyphAspect = chbKeepAspect->Checked;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::seGlyphScaleChange(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		try
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->GlyphScale = seGlyphScale->Value;
			}
		}
		__finally
		{
			seGlyphScale->Value = CurrentButtons[0]->GlyphScale;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button8MouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Scroll = true;
	TouchGrid->Scroll(::sdUp);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button9MouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Scroll = true;
	TouchGrid->Scroll(::sdDown);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button6MouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Scroll = true;
	TouchGrid->Scroll(::sdLeft);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button7MouseDown(TObject *Sender,
		TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Scroll = true;
	TouchGrid->Scroll(::sdRight);
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button7MouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	Scroll = false;
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::TouchGridScrollComplete(TObject *Sender,
		TScrollDirection Direction)
{
	if (Scroll)
	{
		TouchGrid->Scroll(Direction);
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::chbParentFontClick(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->ParentFont = chbParentFont->Checked;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::chbParentFontColorClick(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->ParentFontColor = chbParentFontColor->Checked;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::chbParentLatchedFontClick(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->ParentLatchedFont = chbParentLatchedFont->Checked;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button11Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		FontDialog->Font = CurrentButtons[0]->LatchedFont;
		if (FontDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->LatchedFont->Assign(FontDialog->Font);
			}
			chbParentLatchedFont->Checked = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::chbParentLatchedFontColorClick(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			(*iButton)->ParentLatchedFontColor = chbParentLatchedFontColor->Checked;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button12Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		if (ColorDialog->Execute())
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->LatchedFontColor = ColorDialog->Color;
			}
			chbParentLatchedFontColor->Checked = false;
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::edTagChange(TObject *Sender)
{
	if (edTag->Text != "")
	{
		TButtons CurrentButtons;
		if (SelectedButtons(CurrentButtons))
		{
			for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
			{
				(*iButton)->Tag = StrToInt(edTag->Text);
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button20Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		ColorDialog->Color = (*CurrentButtons.begin())->LatchedFontColor;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button13Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			TouchGrid->DeleteRow(TouchGrid->Row(*iButton));
		}
		seColCount->Value = TouchGrid->ColCount;
		seRowCount->Value = TouchGrid->RowCount;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button14Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			TouchGrid->DeleteCol(TouchGrid->Col(*iButton));
		}
		seColCount->Value = TouchGrid->ColCount;
		seRowCount->Value = TouchGrid->RowCount;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button15Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			TouchGrid->InsertRow(TouchGrid->Row(*iButton));
		}
		seColCount->Value = TouchGrid->ColCount;
		seRowCount->Value = TouchGrid->RowCount;
	}
}
//---------------------------------------------------------------------------
void __fastcall TfrmEditGrid::Button16Click(TObject *Sender)
{
	TButtons CurrentButtons;
	if (SelectedButtons(CurrentButtons))
	{
		for (TButtonsIterator iButton=CurrentButtons.begin(); iButton!=CurrentButtons.end(); iButton++)
		{
			TouchGrid->InsertCol(TouchGrid->Col(*iButton));
		}
		seColCount->Value = TouchGrid->ColCount;
		seRowCount->Value = TouchGrid->RowCount;
	}
}
//---------------------------------------------------------------------------

