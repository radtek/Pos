//---------------------------------------------------------------------------
#pragma hdrstop

#include "TouchEditors.h"
#include "GridEditorForm.h"
#include "TouchPages.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
void __fastcall TTouchGridEditor::Edit(void)
{
	TTouchGrid *Grid = static_cast<TTouchGrid*>(Component);

	std::auto_ptr<TfrmEditGrid> frmEditGrid(new TfrmEditGrid(NULL));

	frmEditGrid->TouchGrid->DesignMode						= true;
	frmEditGrid->TouchGrid->RowCount							= Grid->RowCount;
	frmEditGrid->seRowCount->Value							= Grid->RowCount;
	frmEditGrid->TouchGrid->ColCount							= Grid->ColCount;
	frmEditGrid->seColCount->Value							= Grid->ColCount;

	frmEditGrid->TouchGrid->LatchingType					= ltCheckbox;

	frmEditGrid->TouchGrid->ButtonGapHeight				= Grid->ButtonGapHeight;
	frmEditGrid->TouchGrid->ButtonGapWidth					= Grid->ButtonGapWidth;
	frmEditGrid->TouchGrid->Color								= Grid->Color;
	frmEditGrid->TouchGrid->DefaultButtonColor			= Grid->DefaultButtonColor;
	frmEditGrid->TouchGrid->DefaultButtonLatchedColor	= Grid->DefaultButtonLatchedColor;
	frmEditGrid->TouchGrid->DefaultColWidth				= Grid->DefaultColWidth;
	frmEditGrid->TouchGrid->DefaultRowHeight				= Grid->DefaultRowHeight;
	frmEditGrid->TouchGrid->Font								= Grid->Font;
	frmEditGrid->TouchGrid->FontShadow						= Grid->FontShadow;

	for (int i=0; i<Grid->RowCount; i++)
	{
		frmEditGrid->TouchGrid->RowHeights[i] = Grid->RowHeights[i];
		for (int j=0; j<Grid->ColCount; j++)
		{
			frmEditGrid->TouchGrid->Buttons[i][j]->Font							= Grid->Buttons[i][j]->Font;
			frmEditGrid->TouchGrid->Buttons[i][j]->FontColor					= Grid->Buttons[i][j]->FontColor;
			frmEditGrid->TouchGrid->Buttons[i][j]->ParentFont					= Grid->Buttons[i][j]->ParentFont;
			frmEditGrid->TouchGrid->Buttons[i][j]->ParentFontColor			= Grid->Buttons[i][j]->ParentFontColor;

			frmEditGrid->TouchGrid->Buttons[i][j]->LatchedFont					= Grid->Buttons[i][j]->LatchedFont;
			frmEditGrid->TouchGrid->Buttons[i][j]->LatchedFontColor			= Grid->Buttons[i][j]->LatchedFontColor;
			frmEditGrid->TouchGrid->Buttons[i][j]->ParentLatchedFont			= Grid->Buttons[i][j]->ParentLatchedFont;
			frmEditGrid->TouchGrid->Buttons[i][j]->ParentLatchedFontColor	= Grid->Buttons[i][j]->ParentLatchedFontColor;

			frmEditGrid->TouchGrid->Buttons[i][j]->Glyph							= Grid->Buttons[i][j]->Glyph;
			frmEditGrid->TouchGrid->Buttons[i][j]->GlyphAlpha					= Grid->Buttons[i][j]->GlyphAlpha;
			frmEditGrid->TouchGrid->Buttons[i][j]->TransparentGlyph			= Grid->Buttons[i][j]->TransparentGlyph;
			frmEditGrid->TouchGrid->Buttons[i][j]->TransparentColor			= Grid->Buttons[i][j]->TransparentColor;
			frmEditGrid->TouchGrid->Buttons[i][j]->KeepGlyphAspect			= Grid->Buttons[i][j]->KeepGlyphAspect;
			frmEditGrid->TouchGrid->Buttons[i][j]->GlyphScale					= Grid->Buttons[i][j]->GlyphScale;
			frmEditGrid->TouchGrid->Buttons[i][j]->Color							= Grid->Buttons[i][j]->Color;
			frmEditGrid->TouchGrid->Buttons[i][j]->LatchedColor				= Grid->Buttons[i][j]->LatchedColor;
			frmEditGrid->TouchGrid->Buttons[i][j]->DisabledColor				= Grid->Buttons[i][j]->DisabledColor;
			frmEditGrid->TouchGrid->Buttons[i][j]->Caption						= Grid->Buttons[i][j]->Caption;
			frmEditGrid->TouchGrid->Buttons[i][j]->Enabled						= Grid->Buttons[i][j]->Enabled;
			frmEditGrid->TouchGrid->Buttons[i][j]->Visible						= Grid->Buttons[i][j]->Visible;
			frmEditGrid->TouchGrid->Buttons[i][j]->Tag							= Grid->Buttons[i][j]->Tag;
			if (i==0)
			{
				frmEditGrid->TouchGrid->ColWidths[j] = Grid->ColWidths[j];
			}
		}
	}

	if (frmEditGrid->ShowModal() == mrOk)
	{
		Grid->RowCount = frmEditGrid->seRowCount->Value;
		Grid->ColCount = frmEditGrid->seColCount->Value;

		for (int i=0; i<Grid->RowCount; i++)
		{
			Grid->RowHeights[i] = frmEditGrid->TouchGrid->RowHeights[i];
			for (int j=0; j<Grid->ColCount; j++)
			{
				Grid->Buttons[i][j]->Font							= frmEditGrid->TouchGrid->Buttons[i][j]->Font;
				Grid->Buttons[i][j]->FontColor					= frmEditGrid->TouchGrid->Buttons[i][j]->FontColor;
				Grid->Buttons[i][j]->ParentFont					= frmEditGrid->TouchGrid->Buttons[i][j]->ParentFont;
				Grid->Buttons[i][j]->ParentFontColor			= frmEditGrid->TouchGrid->Buttons[i][j]->ParentFontColor;

				Grid->Buttons[i][j]->LatchedFont					= frmEditGrid->TouchGrid->Buttons[i][j]->LatchedFont;
				Grid->Buttons[i][j]->LatchedFontColor			= frmEditGrid->TouchGrid->Buttons[i][j]->LatchedFontColor;
				Grid->Buttons[i][j]->ParentLatchedFont			= frmEditGrid->TouchGrid->Buttons[i][j]->ParentLatchedFont;
				Grid->Buttons[i][j]->ParentLatchedFontColor	= frmEditGrid->TouchGrid->Buttons[i][j]->ParentLatchedFontColor;

				Grid->Buttons[i][j]->Glyph							= frmEditGrid->TouchGrid->Buttons[i][j]->Glyph;
				Grid->Buttons[i][j]->GlyphAlpha					= frmEditGrid->TouchGrid->Buttons[i][j]->GlyphAlpha;
				Grid->Buttons[i][j]->TransparentGlyph			= frmEditGrid->TouchGrid->Buttons[i][j]->TransparentGlyph;
				Grid->Buttons[i][j]->TransparentColor			= frmEditGrid->TouchGrid->Buttons[i][j]->TransparentColor;
				Grid->Buttons[i][j]->KeepGlyphAspect			= frmEditGrid->TouchGrid->Buttons[i][j]->KeepGlyphAspect;
				Grid->Buttons[i][j]->GlyphScale					= frmEditGrid->TouchGrid->Buttons[i][j]->GlyphScale;
				Grid->Buttons[i][j]->Color							= frmEditGrid->TouchGrid->Buttons[i][j]->Color;
				Grid->Buttons[i][j]->LatchedColor				= frmEditGrid->TouchGrid->Buttons[i][j]->LatchedColor;
				Grid->Buttons[i][j]->DisabledColor				= frmEditGrid->TouchGrid->Buttons[i][j]->DisabledColor;
				Grid->Buttons[i][j]->Caption						= frmEditGrid->TouchGrid->Buttons[i][j]->Caption;
				Grid->Buttons[i][j]->Enabled						= frmEditGrid->TouchGrid->Buttons[i][j]->Enabled;
				Grid->Buttons[i][j]->Visible						= frmEditGrid->TouchGrid->Buttons[i][j]->Visible;
				Grid->Buttons[i][j]->Tag							= frmEditGrid->TouchGrid->Buttons[i][j]->Tag;
				if (i==0)
				{
					Grid->ColWidths[j] = frmEditGrid->TouchGrid->ColWidths[j];
				}
			}
		}
		Designer->Modified();
	}
}
//---------------------------------------------------------------------------
int __fastcall TTouchGridEditor::GetVerbCount()
{
	return 1;
}
//---------------------------------------------------------------------------
AnsiString __fastcall TTouchGridEditor::GetVerb(int Index)
{
	switch (Index)
	{
		case 0: return "Edit TouchGrid";
	}
	return "";
}
//---------------------------------------------------------------------------
void __fastcall TTouchGridEditor::ExecuteVerb(int Index)
{
	switch (Index)
	{
		case 0: Edit();	break;
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
int __fastcall TTouchPagesEditor::GetVerbCount()
{ 
	if (Component->ClassType() == __classid(TTouchPages))
	{
		return 3;
	}
	else if (Component->ClassType() == __classid(TTouchSheet))
	{
		return 4;
	}
	else
	{
		return 0;
	}
}
//----------------------------­------------------------------­-----------------
String __fastcall TTouchPagesEditor::GetVerb(int Index)
{ 
	switch (Index)
	{
		case 0:	return "New Page";
		case 1:	return "Next Page";
		case 2:	return "Previous Page";
		case 3:	return "Delete Page";
	}
	return "";
}
//----------------------------­------------------------------­-----------------
//void __fastcall TTouchPagesEditor::PrepareItem(int Index, const _di_IMenuItem AItem)
//{
//	if (Index == 3)
//	{
//		if (Component->ClassType() == __classid(TTouchPages))
//		{
//			Menus::TMenuItem* Item = reinterpret_cast<Menus::TMenuItem*>(const_cast<_di_IMenuItem>(AItem));
//			AItem->Enabled = false;
//		}
//	}
//}
//----------------------------­------------------------------­-----------------
void __fastcall TTouchPagesEditor::ExecuteVerb(int Index)
{ 
	TTouchPages	*PageControl	= NULL;
	TTouchSheet *TouchSheet		= NULL;

	if (Component->ClassType() == __classid(TTouchPages))
	{
		PageControl = dynamic_cast<TTouchPages*>(Component);
	}
	else if (Component->ClassType() == __classid(TTouchSheet))
	{
		if ((TouchSheet = dynamic_cast<TTouchSheet*>(Component)) != NULL)
		{
			PageControl = TouchSheet->PageControl;
		}
	}
//	Control->ControlStyle = Control->ControlStyle << csAcceptsControls;
// DefineProperties
	if (PageControl)
	{
		switch (Index)
		{
			case 0:
			{
				TTouchSheet* NewPage		= (TTouchSheet*)Designer->CreateComponent(__classid(TTouchSheet), PageControl, 0, 0, 0, 0);
				NewPage->Parent			= PageControl;
				NewPage->PageControl		= PageControl;
				PageControl->ActivePage	= NewPage;
				Designer->Modified();
				break;
			}
			case 1:
			{
				PageControl->SelectNextPage(true);
				Designer->Modified();
				break;
			}
			case 2:
			{
				PageControl->SelectNextPage(false);
				Designer->Modified();
				break;
			}
			case 3:
			{
				if (TouchSheet)
				{
					delete TouchSheet;
					Designer->Modified();
				}
				break;
			}
		}
	}
} 
//---------------------------------------------------------------------------
