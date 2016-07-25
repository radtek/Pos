//---------------------------------------------------------------------------
#include <vcl.h>

#pragma hdrstop

#include "TouchGrid.h"
#include "Color.h"

#pragma package(smart_init)
//#pragma link "TouchBtn"
//---------------------------------------------------------------------------
const int UP_BUTTON		= 0;
const int DOWN_BUTTON	= 1;
const int LEFT_BUTTON	= 2;
const int RIGHT_BUTTON	= 3;
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TTouchGrid *)
{
	new TTouchGrid(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTouchGrid::TTouchGrid(TComponent* Owner) : TCustomTouchButtonControl(Owner),
		ScrollThread(new TGridScrollThread(true)),
		FUpButton(new TGridButton(this)),
		FDownButton(new TGridButton(this)),
		FLeftButton(new TGridButton(this)),
		FRightButton(new TGridButton(this))
{
//	FUpButton->Caption			= "Up";
//	FDownButton->Caption			= "Down";
//	FLeftButton->Caption			= "Left";
//	FRightButton->Caption		= "Right";

	FUpButton->Color				= DefaultButtonColor;
	FDownButton->Color			= DefaultButtonColor;
	FLeftButton->Color			= DefaultButtonColor;
	FRightButton->Color			= DefaultButtonColor;

	FUpButton->LatchedColor		= DefaultButtonLatchedColor;
	FDownButton->LatchedColor	= DefaultButtonLatchedColor;
	FLeftButton->LatchedColor	= DefaultButtonLatchedColor;
	FRightButton->LatchedColor	= DefaultButtonLatchedColor;

	FUpButton->Font				= Font;
	FDownButton->Font				= Font;
	FLeftButton->Font				= Font;
	FRightButton->Font			= Font;

	FUpButton->FontColor			= Font->Color;
	FDownButton->FontColor		= Font->Color;
	FLeftButton->FontColor		= Font->Color;
	FRightButton->FontColor		= Font->Color;

	FArrowColor						= clBlack;
	FArrowBorderColor				= clBlack;
	FArrowScale						= 30;

	Width								= 120;
	Height							= 55;
	BorderWidth						= 4;
	BevelOuter						= bvLowered;

	FAutoScale						= false;
	FVertScrollButtonRatio		= 0;
	FHorizScrollButtonRatio		= 0;

//	FGridColor						= clBlack;
	ParentGridColor				= true;

	FOnScrollComplete				= NULL;

	FHorizScrollTime				= 100;
	FHorizScrollDistance			= 100;
	FVertScrollTime				= 100;
	FVertScrollDistance			= 100;

	FHorizPosition					= 0;
	FVertPosition					= 0;
	FRowCount						= 0;
	FColCount						= 0;
	FAutoBtnWidth					= false;
	FAutoBtnHeight					= false;
	FDefaultColWidth				= Width;
	FDefaultRowHeight				= Height;
	FBtnGapWidth					= 0;
	FBtnGapHeight					= 0;
	FGridBorder						= true;
	FGridBorderWidth				= 4;
	FVertScrollButtonPos			= vsNone;
	FHorizScrollButtonPos		= hsNone;
	FHorizScrollButtonWidth		= 70;
	FVertScrollButtonHeight		= 55;

	ScrollThread->TouchGrid		= this;
	ScrollThread->UseThread		= true;

//	}

//	TComponentClass StreamUsedClasses[] = { __classid(TTouchBtn) };
//	RegisterClasses(StreamUsedClasses,0); // Index is high position not count
}
//---------------------------------------------------------------------------
__fastcall TTouchGrid::~TTouchGrid()
{
	try
	{
		ScrollThread->Terminate();
		if (ScrollThread->Suspended)
		{
			ScrollThread->Resume();
		}
		ScrollThread->WaitFor();
		delete ScrollThread;

		delete FUpButton;
		delete FDownButton;
		delete FLeftButton;
		delete FRightButton;
	}
	catch(...)
	{
	}
}
//---------------------------------------------------------------------------
TGridButton *TTouchGrid::Selected()
{
	for (int i=0; i<RowCount; i++)
	{
		for (int j=0; j<ColCount; j++)
		{
			if (Buttons[i][j]->Latched)
			{
				return Buttons[i][j];
			}
		}                 
	}
	return NULL;
}
//---------------------------------------------------------------------------
int __fastcall TTouchGrid::GetColWidths(int Index)
{
	return FColWidths[Index].Size;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetColWidths(int Index, int Width)
{
	FColWidths[Index].Size = Width;
	FColWidths[Index].Ratio = 0;
	VerifyScrollPos();
	Invalidate();
}
//---------------------------------------------------------------------------
int __fastcall TTouchGrid::GetRowHeights(int Index)
{
	return FRowHeights[Index].Size;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetRowHeights(int Index, int Height)
{
	FRowHeights[Index].Size = Height;
	FRowHeights[Index].Ratio = 0;
	VerifyScrollPos();
	Invalidate();
}
//---------------------------------------------------------------------------
TGridButton* __fastcall TTouchGrid::GetButton(int Row, int Col)
{
	if (Row >= FRowCount || Col >= FColCount)
	{
		throw Exception("Index out of bounds");
	}
	if (FButtons.size() < unsigned((Row*FColCount) + Col))
	{
		return NULL;
	}
	return FButtons[(Row*FColCount) + Col];
}
//---------------------------------------------------------------------------
/*void __fastcall TTouchGrid::SetButton(int Row, int Col, TGridButton *Button)
{
	if (ColCount < Col) ColCount = Col;
	if (RowCount < Row) RowCount = Row;
	FButtons[(Row*FColCount) + Col] = Button;
}*/
//---------------------------------------------------------------------------
TGridButton *TTouchGrid::GetButtonAt(int X, int Y)
{
	return GetButtonXY(X, Y);
}
//---------------------------------------------------------------------------
TGridButton *TTouchGrid::GetButtonXY(int X, int Y)
{
	if (!PtInRect(&GridInnerRect, TPoint(X, Y)))
	{
		return NULL;
	}
	int x=GridInnerRect.Left + FGridBorderWidth - FHorizPosition;
	int Col=-1;
	while (x < X)
	{
		if (Col+1 >= FColCount)
		{
			return NULL;
		}
		x += FColWidths[++Col].Size + FBtnGapWidth;
		if (X - x <= 0 && X - x > -FBtnGapWidth)
		{
			return NULL;
		}
	}
	int y=GridInnerRect.Top + FGridBorderWidth - FVertPosition;
	int Row=-1;
	while (y < Y)
	{
		if (Row+1 >= FRowCount)
		{
			return NULL;
		}
		y += FRowHeights[++Row].Size + FBtnGapHeight;
		if (Y - y <= 0 && Y - y > -FBtnGapHeight)
		{
			return NULL;
		}
	}
	return GetButton(Row, Col);
}
//---------------------------------------------------------------------------
bool TTouchGrid::GetButtonPos(const TGridButton *const Button, int &Row, int &Col) const
{
	for (int y=0; y<FRowCount; y++)
	{
		for (int x=0; x<FColCount; x++)
		{
			if (FButtons[(y*FColCount) + x] == Button)
			{
				Row = y;
				Col = x;
				return true;
			}
		}
	}
	return false;
}
//---------------------------------------------------------------------------
int TTouchGrid::Row(const TGridButton *const Button) const
{
	int Row, Col;
	if (GetButtonPos(Button, Row, Col))
	{
		return Row;
	}
	else
	{
		return -1;
	}
}
//---------------------------------------------------------------------------
int TTouchGrid::Col(const TGridButton *const Button) const
{
	int Row, Col;
	if (GetButtonPos(Button, Row, Col))
	{
		return Col;
	}
	else
	{
		return -1;
	}
}
//---------------------------------------------------------------------------
TRect TTouchGrid::GetButtonRect(const TGridButton *Button)
{
	int Row, Col;
	if (GetButtonPos(Button, Row, Col))
	{
		return GetButtonRect(Row, Col);
	}
	else
	{
		return TRect(0,0,0,0);
	}
}
//---------------------------------------------------------------------------
TRect TTouchGrid::GetButtonRect(int Row, int Col)
{
	TRect Rect = GridInnerRect;
	InflateRect(&Rect, -FGridBorderWidth, -FGridBorderWidth);

	for (int y=0; y<Row; y++)
	{
		Rect.Top += FRowHeights[y].Size + FBtnGapHeight;
	}
	Rect.Bottom = Rect.Top + FRowHeights[Row].Size;
	for (int x=0; x<Col; x++)
	{
		Rect.Left += FColWidths[x].Size + FBtnGapWidth;
	}
	Rect.Right = Rect.Left + FColWidths[Col].Size;
	return Rect;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetRowCount(int Count)
{
	if (Count < 0)
	{
		throw Exception("Invalid Row Count: " + IntToStr(Count));
	}
	if (Count != FRowCount)
	{
		if (FRowCount > Count)
		{
			for (int Row=Count; Row<FRowCount; Row++)
			{
				for (int Col=0; Col<FColCount; Col++)
				{
					delete FButtons[(Row*FColCount) + Col];
				}
			}
			FButtons.resize(FButtons.size() - ((FRowCount-Count)*FColCount));
			FRowHeights.resize(Count, TColWidth(0,0));
		}
		else
		{
			TRect GridClientRect = GridInnerRect;
			InflateRect(&GridClientRect, -FGridBorderWidth, -FGridBorderWidth);
			if (Count > 0 && FColCount == 0)
			{
				FColCount = 1;
				if (GridClientRect.Width() > 0)
				{
					FColWidths.resize(FColCount, TColWidth(FDefaultColWidth, Extended(FDefaultColWidth) / GridClientRect.Width()));
				}
				else
				{
					FColWidths.resize(FColCount, TColWidth(FDefaultColWidth, 0));
				}
			}
			for (int i=0; i<(Count-FRowCount)*FColCount; i++)
			{
				FButtons.push_back(new TGridButton(this));
			}
			if (GridClientRect.Height() > 0)
			{
				FRowHeights.resize(Count, TRowHeight(FDefaultRowHeight, Extended(FDefaultRowHeight) / GridClientRect.Height()));
			}
			else
			{
				FRowHeights.resize(Count, TRowHeight(FDefaultRowHeight, 0));
			}
		}
		FRowCount = Count;
		if (Count == 0)
		{
			FColCount = 0;
		}
		VerifyScrollPos();
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetColCount(int Count)
{
	if (Count < 0)
	{
		throw Exception("Invalid Col Count: " + IntToStr(Count));
	}
	if (Count != FColCount)
	{
		if (FColCount > Count)
		{
			std::vector<TGridButton *>::iterator iButtons = FButtons.begin() + Count;

			for (int Row=0; Row<FRowCount; Row++)
			{
				for (int Col=Count; Col<FColCount; Col++)
				{
					delete FButtons[(Row*FColCount) + Col];
				}
			}
			for (int Row=0; Row<FRowCount; Row++)
			{
				iButtons = FButtons.erase(iButtons, iButtons+(FColCount-Count)) + Count;
			}
			FColWidths.erase(FColWidths.begin() + Count, FColWidths.end());
		}
		else
		{
			TRect GridClientRect = GridInnerRect;
			InflateRect(&GridClientRect, -FGridBorderWidth, -FGridBorderWidth);

			if (Count > 0 && FRowCount == 0)
			{
				FRowCount = 1;
				if (GridClientRect.Height() > 0)
				{
					FRowHeights.resize(FRowCount, TRowHeight(FDefaultRowHeight, Extended(FDefaultRowHeight) / GridClientRect.Height()));
				}
				else
				{
					FRowHeights.resize(FRowCount, TRowHeight(FDefaultRowHeight, 0));
				}
			}
			for (int Row=0; Row<FRowCount; Row++)
			{
				for (int Col=FColCount; Col<Count; Col++)
				{
					FButtons.insert(FButtons.begin() + (Row*Count+Col), new TGridButton(this));
				}
			}
			if (GridClientRect.Height() > 0)
			{
				FColWidths.insert(FColWidths.end(), Count - FColCount, TRowHeight(FDefaultColWidth, Extended(FDefaultColWidth) / GridClientRect.Width()));
			}
			else
			{
				FColWidths.insert(FColWidths.end(), Count - FColCount, TRowHeight(FDefaultColWidth, 0));
			}
		}
		FColCount = Count;
		if (Count == 0)
		{
			FRowCount = 0;
		}
		VerifyScrollPos();
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void TTouchGrid::InsertRow(int Index)
{
	if (Index < FRowCount)
	{
		for (int i=0; i<FColCount; i++)
		{
			FButtons.insert(FButtons.begin() + (Index * FColCount), new TGridButton(this));
		}
		TRect GridClientRect = GridInnerRect;
		InflateRect(&GridClientRect, -FGridBorderWidth, -FGridBorderWidth);

		if (GridClientRect.Height() > 0)
		{
			FRowHeights.insert(FRowHeights.begin() + Index, TRowHeight(FDefaultRowHeight, Extended(FDefaultRowHeight) / GridClientRect.Height()));
		}
		else
		{
			FRowHeights.insert(FRowHeights.begin() + Index, TRowHeight(FDefaultRowHeight, 0));
		}

		FRowCount++;

		Invalidate();
	}
}
//---------------------------------------------------------------------------
void TTouchGrid::DeleteRow(int Index)
{
	if (Index < FRowCount)
	{
		for (int Col=0; Col<FColCount; Col++)
		{
			delete FButtons[(Index*FColCount) + Col];
		}
		FButtons.erase(FButtons.begin() + (Index * FColCount), FButtons.begin() + ((Index+1) * FColCount));
		FRowHeights.erase(FRowHeights.begin() + Index);

		FRowCount--;

		Invalidate();
	}
}
//---------------------------------------------------------------------------
void TTouchGrid::InsertCol(int Index)
{
	if (Index < FColCount)
	{
		for (int i=FRowCount-1; i>=0; i--)
		{
			FButtons.insert(FButtons.begin() + (i * FColCount) + Index, new TGridButton(this));
		}
		TRect GridClientRect = GridInnerRect;
		InflateRect(&GridClientRect, -FGridBorderWidth, -FGridBorderWidth);
		if (GridClientRect.Width() > 0)
		{
			FColWidths.insert(FColWidths.begin() + Index, TColWidth(FDefaultColWidth, Extended(FDefaultColWidth) / GridClientRect.Width()));
		}
		else
		{
			FColWidths.insert(FColWidths.begin() + Index, TColWidth(FDefaultColWidth, 0));
		}
		FColCount++;

		Invalidate();
	}
}
//---------------------------------------------------------------------------
void TTouchGrid::DeleteCol(int Index)
{
	if (Index < FColCount)
	{
		std::vector<TGridButton *>::iterator iButtons = FButtons.begin() + Index;
		for (int Row=0; Row<FRowCount; Row++)
		{
			TGridButton *Button = *iButtons;
			iButtons = FButtons.erase(iButtons) + FColCount-1;
			delete Button;
		}
		FColWidths.erase(FColWidths.begin() + Index);

		FColCount--;

		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetDefaultColWidth(int Width)
{
	FDefaultColWidth = Width;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetDefaultRowHeight(int Height)
{
	FDefaultRowHeight = Height;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetAutoBtnWidth(bool Auto)
{
	FAutoBtnWidth	= Auto;
	FHorizPosition	= 0;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetAutoBtnHeight(bool Auto)
{
	FAutoBtnHeight	= Auto;
	FVertPosition	= 0;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetGridBorder(bool GridBorder)
{
	FGridBorder = GridBorder;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetGridBorderWidth(int GridBorderWidth)
{
	FGridBorderWidth	= GridBorderWidth;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetGridColor(TColor Value)
{
	if (FGridColor != Value)
	{
		FGridColor			= Value;
		FParentGridColor	= false;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetParentGridColor(bool Value)
{
	if (FParentGridColor != Value)
	{
		FParentGridColor = Value;
		if (FParentGridColor)
		{
			FGridColor = Color;
		}
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetArrowColor(TColor Value)
{
	FArrowColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetArrowBorderColor(TColor Value)
{
	FArrowBorderColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetArrowScale(int Value)
{
	if (Value < 0 || Value > 100)
	{
		throw Exception("Scale must be between 0 and 100");
	}
	FArrowScale = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
/*TGridButton* __fastcall TTouchGrid::GetUpButton()
{
	return FUpButton.get();
}
//---------------------------------------------------------------------------
TGridButton* __fastcall TTouchGrid::GetDownButton()
{
	return FDownButton.get();
}
//---------------------------------------------------------------------------
TGridButton* __fastcall TTouchGrid::GetLeftButton()
{
	return FLeftButton.get();
}
//---------------------------------------------------------------------------
TGridButton* __fastcall TTouchGrid::GetRightButton()
{
	return FRightButton.get();
}*/
//---------------------------------------------------------------------------
/*void __fastcall TTouchGrid::FOnScrollFontChange(TObject* Sender)
{
	FUpButton->FButtons[0]->Font				= FScrollFont;
	FUpButton->FButtons[0]->FontColor		= FScrollFont->Color;
	FDownButton->FButtons[0]->Font			= FScrollFont;
	FDownButton->FButtons[0]->FontColor		= FScrollFont->Color;
	FLeftButton->FButtons[0]->Font			= FScrollFont;
	FLeftButton->FButtons[0]->FontColor		= FScrollFont->Color;
	FRightButton->FButtons[0]->Font			= FScrollFont;
	FRightButton->FButtons[0]->FontColor	= FScrollFont->Color;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetScrollFont(TFont *Font)
{
	FScrollFont->Assign(Font);
}
//---------------------------------------------------------------------------
TColor __fastcall TTouchGrid::GetScrollColor()
{
	return FUpButton->FButtons[0]->Color;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetScrollColor(TColor Value)
{
	FUpButton->FButtons[0]->Color		= Value;
	FDownButton->FButtons[0]->Color	= Value;
	FLeftButton->FButtons[0]->Color	= Value;
	FRightButton->FButtons[0]->Color	= Value;
}
//---------------------------------------------------------------------------
/*void __fastcall TTouchGrid::CMColorChanged(TMessage& Message)
{
	TCustomTouchButtonControl::CMColorChanged(Message);

	if (FUpButton.get() && FDownButton.get() && FLeftButton.get() && FRightButton.get())
	{
		FUpButton->FButtons[0]->Color		= Color;
		FDownButton->FButtons[0]->Color	= Color;
		FLeftButton->FButtons[0]->Color	= Color;
		FRightButton->FButtons[0]->Color	= Color;
	}
} */
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetVertScrollButtonPos(TVertScrollButtonPos VertScrollButtonPos)
{
	FVertScrollButtonPos	= VertScrollButtonPos;
	CalculateVertScrollButtonPos();
	Invalidate();
}
//---------------------------------------------------------------------------
void TTouchGrid::CalculateVertScrollButtonPos()
{
	int ButtonHeight = Extended(Height) * FVertScrollButtonRatio;

	switch (FVertScrollButtonPos)
	{
		case vsNone:
		{
			break;
		}
		case vsAbove:
		{
			ScrollButtonsRect[UP_BUTTON].Left		= GridOuterRect.Left;
			ScrollButtonsRect[UP_BUTTON].Top			= GridOuterRect.Top - ButtonHeight - ButtonGapHeight;
			ScrollButtonsRect[UP_BUTTON].Right		= GridOuterRect.Left + ((GridOuterRect.Width() - ButtonGapWidth) / 2);

			ScrollButtonsRect[DOWN_BUTTON].Left		= GridOuterRect.Left + ScrollButtonsRect[UP_BUTTON].Width() + ButtonGapWidth;
			ScrollButtonsRect[DOWN_BUTTON].Top		= GridOuterRect.Top - ButtonHeight - ButtonGapHeight;
			ScrollButtonsRect[DOWN_BUTTON].Right	= GridOuterRect.Left + GridOuterRect.Width();

			break;
		}
		case vsBelow:
		{
			ScrollButtonsRect[UP_BUTTON].Left		= GridOuterRect.Left;
			ScrollButtonsRect[UP_BUTTON].Top			= GridOuterRect.Bottom + ButtonGapHeight;
			ScrollButtonsRect[UP_BUTTON].Right		= GridOuterRect.Left + ((GridOuterRect.Width() - ButtonGapWidth) / 2);

			ScrollButtonsRect[DOWN_BUTTON].Left		= GridOuterRect.Left + ScrollButtonsRect[UP_BUTTON].Width() + ButtonGapWidth;
			ScrollButtonsRect[DOWN_BUTTON].Top		= GridOuterRect.Bottom + ButtonGapHeight;
			ScrollButtonsRect[DOWN_BUTTON].Right	= GridOuterRect.Left + GridOuterRect.Width();

			break;
		}
		case vsAboveAndBelow:
		{
			ScrollButtonsRect[UP_BUTTON].Left		= GridOuterRect.Left;
			ScrollButtonsRect[UP_BUTTON].Top			= GridOuterRect.Top - ButtonHeight - ButtonGapHeight;
			ScrollButtonsRect[UP_BUTTON].Right		= GridOuterRect.Left + GridOuterRect.Width();

			ScrollButtonsRect[DOWN_BUTTON].Left		= GridOuterRect.Left;
			ScrollButtonsRect[DOWN_BUTTON].Top		= GridOuterRect.Bottom + ButtonGapHeight;
			ScrollButtonsRect[DOWN_BUTTON].Right	= GridOuterRect.Left + GridOuterRect.Width();

			break;
		}
	}
	ScrollButtonsRect[UP_BUTTON].Bottom				= ScrollButtonsRect[UP_BUTTON].Top + ButtonHeight;
	ScrollButtonsRect[DOWN_BUTTON].Bottom			= ScrollButtonsRect[DOWN_BUTTON].Top + ButtonHeight;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetHorizScrollButtonPos(THorizScrollButtonPos HorizScrollButtonPos)
{
	FHorizScrollButtonPos	= HorizScrollButtonPos;
	CalculateHorizScrollButtonPos();
	Invalidate();
}
//---------------------------------------------------------------------------
void TTouchGrid::CalculateHorizScrollButtonPos()
{
	int ButtonWidth = Extended(Width) * FHorizScrollButtonRatio;

	switch (FHorizScrollButtonPos)
	{
		case hsNone:
		{
			break;
		}
		case hsLeft:
		{
			ScrollButtonsRect[LEFT_BUTTON].Left		= GridOuterRect.Left - ButtonWidth - ButtonGapWidth;//ButtonGapWidth;
			ScrollButtonsRect[LEFT_BUTTON].Top		= GridOuterRect.Top;
			ScrollButtonsRect[LEFT_BUTTON].Bottom	= GridOuterRect.Top + ((GridOuterRect.Height() - ButtonGapHeight) / 2);

			ScrollButtonsRect[RIGHT_BUTTON].Left	= GridOuterRect.Left - ButtonWidth - ButtonGapWidth;//ButtonGapWidth;
			ScrollButtonsRect[RIGHT_BUTTON].Top		= ScrollButtonsRect[LEFT_BUTTON].Bottom + ButtonGapHeight;
			ScrollButtonsRect[RIGHT_BUTTON].Bottom	= GridOuterRect.Bottom;
			break;
		}
		case hsRight:
		{
			ScrollButtonsRect[LEFT_BUTTON].Left		= GridOuterRect.Right + ButtonGapWidth;
			ScrollButtonsRect[LEFT_BUTTON].Top		= GridOuterRect.Top;
			ScrollButtonsRect[LEFT_BUTTON].Bottom	= GridOuterRect.Top + ((GridOuterRect.Height() - ButtonGapHeight) / 2);

			ScrollButtonsRect[RIGHT_BUTTON].Left	= GridOuterRect.Right + ButtonGapWidth;
			ScrollButtonsRect[RIGHT_BUTTON].Top		= ScrollButtonsRect[LEFT_BUTTON].Bottom + ButtonGapHeight;
			ScrollButtonsRect[RIGHT_BUTTON].Bottom	= GridOuterRect.Bottom;
			break;
		}
		case hsLeftAndRight:
		{
			ScrollButtonsRect[LEFT_BUTTON].Left		= GridOuterRect.Left - ButtonWidth - ButtonGapWidth;//ButtonGapWidth;
			ScrollButtonsRect[LEFT_BUTTON].Top		= GridOuterRect.Top;
			ScrollButtonsRect[LEFT_BUTTON].Bottom	= GridOuterRect.Bottom;

			ScrollButtonsRect[RIGHT_BUTTON].Left	= GridOuterRect.Right + ButtonGapWidth;
			ScrollButtonsRect[RIGHT_BUTTON].Top		= GridOuterRect.Top;
			ScrollButtonsRect[RIGHT_BUTTON].Bottom	= GridOuterRect.Bottom;
			break;
		}
	}
	ScrollButtonsRect[LEFT_BUTTON].Right			= ScrollButtonsRect[LEFT_BUTTON].Left + ButtonWidth;
	ScrollButtonsRect[RIGHT_BUTTON].Right			= ScrollButtonsRect[RIGHT_BUTTON].Left + ButtonWidth;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetVertScrollButtonHeight(int Value)
{
	FVertScrollButtonHeight	= Value;
	if (Height > 0)
	{
		FVertScrollButtonRatio = Value / Height;
	}
	else
	{
		FVertScrollButtonRatio = 0;
	}
	CalculateVertScrollButtonPos();
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetHorizScrollButtonWidth(int Value)
{
	FHorizScrollButtonWidth = Value;
	if (Width > 0)
	{
		FHorizScrollButtonRatio = Value / Width;
	}
	else
	{
		FHorizScrollButtonRatio = 0;
	}
	CalculateHorizScrollButtonPos();
	Invalidate();
}
//---------------------------------------------------------------------------
TGridButton *TTouchGrid::GetScrollButton(int X, int Y)
{
	if (FUpButton->Visible && FVertScrollButtonPos != vsNone && PtInRect(ScrollButtonsRect[UP_BUTTON], TPoint(X, Y)))
	{
		return FUpButton;
	} else
	if (FDownButton->Visible && FVertScrollButtonPos != vsNone && PtInRect(ScrollButtonsRect[DOWN_BUTTON], TPoint(X, Y)))
	{
		return FDownButton;
	} else
	if (FLeftButton->Visible && FHorizScrollButtonPos != hsNone && PtInRect(ScrollButtonsRect[LEFT_BUTTON], TPoint(X, Y)))
	{
		return FLeftButton;
	} else
	if (FRightButton->Visible && FHorizScrollButtonPos != hsNone && PtInRect(ScrollButtonsRect[RIGHT_BUTTON], TPoint(X, Y)))
	{
		return FRightButton;
	}
	return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	TGridButton *ScrollButton = GetScrollButton(X, Y);

	if (ScrollButton)
	{
		FCurrentClickedScrollButton = ScrollButton;
		if (FCurrentClickedScrollButton)
		{
			FMouseInCurrentScrollButton = true;
			SetButtonDown(ScrollButton);
			Invalidate();
			ApplyScroll();
		}
		else
		{
			FMouseInCurrentScrollButton = false;
		}
	}
	else
	{
		TCustomTouchButtonControl::MouseDown(Button, Shift, X, Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	if (FCurrentClickedScrollButton)
	{
		SetButtonUp(FCurrentClickedScrollButton);
		FCurrentClickedScrollButton = NULL;
		Invalidate();
		FScroll					= false;
		FCurrentScrollButton	= NULL;
	}
	else
	{
		TCustomTouchButtonControl::MouseUp(Button, Shift, X, Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::MouseMove(Classes::TShiftState Shift, int X, int Y)
{
	if (FCurrentClickedScrollButton)
	{
		TGridButton *GridButton = GetScrollButton(X, Y);
		
		bool IsCurrentButton = (GridButton == FCurrentClickedScrollButton);

		if (FMouseInCurrentScrollButton != IsCurrentButton)
		{
			FMouseInCurrentScrollButton = IsCurrentButton;
			SetButtonHighlight(FCurrentClickedScrollButton, FMouseInCurrentScrollButton);
			Invalidate();
		}
		if (FScroll != IsCurrentButton)
		{
			FScroll = IsCurrentButton;
			if (FScroll && FCurrentScrollButton)
			{
				SetButtonDown(FCurrentScrollButton);
				Invalidate();
				ApplyScroll();
			}
		}
/*		if (FScroll && FCurrentScrollButton)
		{
			SetButtonDown(FCurrentScrollButton);
			Invalidate();
		}*/
	}
	else
	{
		TCustomTouchButtonControl::MouseMove(Shift, X, Y);
	}
}
//---------------------------------------------------------------------------
void TTouchGrid::ApplyScroll()
{
	if (FCurrentClickedScrollButton == FUpButton)
	{
		FScroll					= true;
		FCurrentScrollButton	= FUpButton;
		Scroll(sdUp);
	}
	else if (FCurrentClickedScrollButton == FDownButton)
	{
		FScroll					= true;
		FCurrentScrollButton	= FDownButton;
		Scroll(sdDown);
	}
	else if (FCurrentClickedScrollButton == FLeftButton)
	{
		FScroll					= true;
		FCurrentScrollButton	= FLeftButton;
		Scroll(sdLeft);
	}
	else if (FCurrentClickedScrollButton == FRightButton)
	{
		FScroll					= true;
		FCurrentScrollButton	= FRightButton;
		Scroll(sdRight);
	}
}
//---------------------------------------------------------------------------
/*void __fastcall TTouchGrid::UpButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	FScroll					= true;
	FCurrentScrollButton	= FUpButton;//.get();
	VertSmoothScroll(-FVertScrollDistance, FVertScrollTime);
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::UpButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	FScroll = false;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::DownButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	FScroll					= true;
	FCurrentScrollButton	= FDownButton;//.get();
	VertSmoothScroll(FVertScrollDistance, FVertScrollTime);
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::DownButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	FScroll					= false;
	FCurrentScrollButton	= NULL;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::LeftButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	FScroll					= true;
	FCurrentScrollButton	= FLeftButton;//.get();
	HorizSmoothScroll(-FHorizScrollDistance, FHorizScrollTime);
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::LeftButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	FScroll = false;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::RightButtonMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	FScroll					= true;
	FCurrentScrollButton	= FRightButton;//.get();
	HorizSmoothScroll(FHorizScrollDistance, FHorizScrollTime);
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::RightButtonMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	FScroll					= false;
	FCurrentScrollButton	= NULL;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::ScrollButtonsMouseMove(TObject* Sender, TShiftState Shift, TGridButton *GridButton, int X, int Y)
{
	TTouchBtn *Button = static_cast<TTouchBtn *>(Sender);
	bool MouseOnButton = (PtInRect(Button->GetButtonRect(GridButton), TPoint(X, Y)));
	if (MouseOnButton != FScroll)
	{
		FScroll = MouseOnButton;
		if (FScroll && FCurrentScrollButton)
		{
			FCurrentScrollButton->OnMouseDown(FCurrentScrollButton, TMouseButton(), TShiftState(), NULL);
		}
	}
}*/
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::GridCompleteScroll(TScrollDirection ScrollDirection, int Distance)
{
	if (FScroll && FCurrentScrollButton)
	{
		ApplyScroll();
	}
	if (FOnScrollComplete)
	{
		FOnScrollComplete(this, ScrollDirection);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetBtnGapWidth(int Width)
{
	FBtnGapWidth = Width;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetBtnGapHeight(int Height)
{
	FBtnGapHeight = Height;
	Invalidate();
}
//---------------------------------------------------------------------------
int TTouchGrid::GetMaxButtonSize()
{
	int MaxWidth = 0;
	int MaxHeight = 0;
	for (unsigned y=0; y<FRowHeights.size(); y++)
	{
		MaxWidth = max(FRowHeights[y].Size, MaxWidth);
	}
	for (unsigned x=0; x<FColWidths.size(); x++)
	{
		MaxHeight = max(FColWidths[x].Size, MaxHeight);
	}
	for (int i=0; i<4; i++)	MaxWidth = max(MaxWidth, ScrollButtonsRect[i].Width());
	for (int i=0; i<4; i++)	MaxHeight = max(MaxHeight, ScrollButtonsRect[i].Height());
	return MaxWidth * MaxHeight;
}
//---------------------------------------------------------------------------
bool __fastcall TTouchGrid::GetUseThread()
{
	return ScrollThread->UseThread;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::SetUseThread(bool UseThread)
{
	if (UseThread && ScrollThread->UseThread == false)
	{
		ScrollThread->UseThread = true;
	}
	else if (!UseThread)
	{
		if (!ScrollThread->Suspended)
		{
			ScrollThread->Suspend();
		}
		ScrollThread->UseThread = false;
	}
}
//---------------------------------------------------------------------------
void TTouchGrid::Scroll(TScrollDirection Direction)
{
	if (Direction == sdUp)
	{
		VertSmoothScroll(Direction, -FVertScrollDistance, FVertScrollTime);
	}
	else if (Direction == sdDown)
	{
		VertSmoothScroll(Direction, FVertScrollDistance, FVertScrollTime);
	}
	else if (Direction == sdLeft)
	{
		HorizSmoothScroll(Direction, -FHorizScrollDistance, FHorizScrollTime);
	}
	else if (Direction == sdRight)
	{
		HorizSmoothScroll(Direction, FHorizScrollDistance, FHorizScrollTime);
	}
}
//---------------------------------------------------------------------------
/*void TTouchGrid::ScrollInView(const TGridButton *const Button)
{
	TRect rect = GetButtonRect(Button);

	HorizSmoothScroll(sdRight, rect.Left - FHorizPosition, FHorizScrollTime);
//	VertSmoothScroll(sdUp, rect.Top - FVertPosition, FVertScrollTime);
}*/
//---------------------------------------------------------------------------
void TTouchGrid::SetVisibleRow(int Row)
{
	TRect rect = GetButtonRect(Row, 0);
	FVertPosition = rect.Top - GridInnerRect.Top;
	Invalidate();
}
//---------------------------------------------------------------------------
void TTouchGrid::SetVisibleCol(int Col)
{
	TRect rect = GetButtonRect(0, Col);
	FHorizPosition = rect.Left - GridInnerRect.Left;
	Invalidate();
}
//---------------------------------------------------------------------------
void TTouchGrid::HorizSmoothScroll(TScrollDirection Direction, int Distance, int Time)
{
	TRect Rect = GridInnerRect;
	InflateRect(&Rect, -FGridBorderWidth, -FGridBorderWidth);

	int Max = GetHorizMax();
	int Min = GetHorizMin();
	if (FHorizPosition + Distance > Max - Rect.Width())
	{
		int NewTime = (Max - FHorizPosition - Rect.Width()) * Time / Distance;
		if (NewTime > 0 && NewTime <= Time)
		{
			ScrollThread->Scroll(Direction, Max - FHorizPosition - Rect.Width(), NewTime);
		}
		else if (NewTime < 0)
		{
			FHorizPosition = Max - Rect.Width();
			if (FHorizPosition < 0) FHorizPosition = 0;
		}
	}
	else if (FHorizPosition + Distance < Min)
	{
		int NewTime = (Min - FHorizPosition) * Time / Distance;
		if (NewTime > 0 && NewTime <= Time)
		{
			ScrollThread->Scroll(Direction, Min - FHorizPosition, NewTime);
		}
		else if (NewTime < 0)
		{
			FHorizPosition = Min;
		}
	}
	else
	{
		ScrollThread->Scroll(Direction, Distance, Time);
	}
}
//---------------------------------------------------------------------------
void TTouchGrid::VertSmoothScroll(TScrollDirection Direction, int Distance, int Time)
{
	TRect Rect = GridInnerRect;
	InflateRect(&Rect, -FGridBorderWidth, -FGridBorderWidth);

	int Max = GetVertMax();
	int Min = GetVertMin();
	if (FVertPosition + Distance > Max - Rect.Height())
	{
		int NewTime = (Max - FVertPosition - Rect.Height()) * Time / Distance;
		if (NewTime > 0 && NewTime <= Time)
		{
			ScrollThread->Scroll(Direction, Max - FVertPosition - Rect.Height(), NewTime);
		}
		else if (NewTime < 0)
		{
			FVertPosition = Max - Rect.Height();
			if (FVertPosition < 0) FVertPosition = 0;
		}
	}
	else if (FVertPosition + Distance < Min)
	{
		int NewTime = (Min - FVertPosition) * Time / Distance;
		if (NewTime > 0 && NewTime <= Time)
		{
			ScrollThread->Scroll(Direction, Min - FVertPosition, NewTime);
		}
		else if (NewTime < 0)
		{
			FVertPosition = Min;
		}
	}
	else
	{
		ScrollThread->Scroll(Direction, Distance, Time);
	}
}
//---------------------------------------------------------------------------
void TTouchGrid::CalculateAutoSizes()
{
	if (FAutoBtnHeight || FAutoBtnWidth || FAutoScale)
	{
		TRect GridClientRect = GridInnerRect;
		InflateRect(&GridClientRect, -FGridBorderWidth, -FGridBorderWidth);

		if (FAutoBtnHeight)
		{
			int HeightToGo = GridClientRect.Height() - ((FRowCount-1) * FBtnGapHeight);
			for (int i=0; i<FRowCount; i++)
			{
				HeightToGo -= (FRowHeights[i].Size = HeightToGo / (FRowCount - i));
			}
		}
		else if (FAutoScale)
		{
			for (int i=0; i<FRowCount; i++)
			{
				if (FRowHeights[i].Ratio == 0 && GridClientRect.Height() > 0)
				{
					FRowHeights[i].Ratio = Extended(FRowHeights[i].Size) / GridClientRect.Height();
				}
				else
				{
					FRowHeights[i].Size = FRowHeights[i].Ratio * GridClientRect.Height();
				}
			}
		}
		if (FAutoBtnWidth)
		{
			int WidthToGo = GridClientRect.Width() - ((FColCount-1) * FBtnGapWidth);
			for (int i=0; i<FColCount; i++)
			{
				WidthToGo -= (FColWidths[i].Size = WidthToGo / (FColCount - i));
			}
		}
		else if (FAutoScale)
		{
			for (int i=0; i<FColCount; i++)
			{
				if (FColWidths[i].Ratio == 0 && GridClientRect.Width() > 0)
				{
					FColWidths[i].Ratio = Extended(FColWidths[i].Size) / GridClientRect.Width();
				}
				else
				{
					FColWidths[i].Size = FColWidths[i].Ratio * GridClientRect.Width();
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
int __fastcall TTouchGrid::GetHorizMax()
{
	int Width = 0;
	for (unsigned i=0; i<FColWidths.size(); i++)
	{
		Width += (FColWidths[i].Size + FBtnGapWidth);
	}
	return Width - FBtnGapWidth;
}
//---------------------------------------------------------------------------
int __fastcall TTouchGrid::GetHorizMin()
{
	return 0;
}
//---------------------------------------------------------------------------
int __fastcall TTouchGrid::GetVertMax()
{
	int Height = 0;
	for (unsigned i=0; i<FRowHeights.size(); i++)
	{
		Height += (FRowHeights[i].Size + FBtnGapHeight);
	}
	return Height - FBtnGapHeight;
}
//---------------------------------------------------------------------------
int __fastcall TTouchGrid::GetVertMin()
{
	return 0;
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::CMFontChanged(TMessage& Message)
{
	TCustomTouchButtonControl::CMFontChanged(Message);
	
	if (FUpButton->ParentFont)
	{
		FUpButton->Font					= Font;
		FUpButton->ParentFont			= true;
	}
	if (FDownButton->ParentFont)
	{
		FDownButton->Font					= Font;
		FDownButton->ParentFont			= true;
	}
	if (FLeftButton->ParentFont)
	{
		FLeftButton->Font					= Font;
		FLeftButton->ParentFont			= true;
	}
	if (FRightButton->ParentFont)
	{
		FRightButton->Font				= Font;
		FRightButton->ParentFont		= true;
	}

	if (FUpButton->ParentFontColor)
	{
		FUpButton->FontColor				= Font->Color;
		FUpButton->ParentFontColor		= true;
	}
	if (FDownButton->ParentFontColor)
	{
		FDownButton->FontColor			= Font->Color;
		FDownButton->ParentFontColor	= true;
	}
	if (FLeftButton->ParentFontColor)
	{
		FLeftButton->FontColor			= Font->Color;
		FLeftButton->ParentFontColor	= true;
	}
	if (FRightButton->ParentFontColor)
	{
		FRightButton->FontColor			= Font->Color;
		FRightButton->ParentFontColor	= true;
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::WMSize(TWMSize& Message)
{
	TCustomTouchButtonControl::WMSize(Message);
	VerifyScrollPos();
}
//---------------------------------------------------------------------------
void TTouchGrid::VerifyScrollPos()
{
	TRect Rect = GridInnerRect;
	InflateRect(&Rect, -FGridBorderWidth, -FGridBorderWidth);

	int Max = GetVertMax();
	if (FVertPosition > Max - Rect.Height())
	{
		FVertPosition = Max - Rect.Height();
		if (FVertPosition < 0) FVertPosition = 0;
		Invalidate();
	}
	Max = GetHorizMax();
	if (FHorizPosition > Max - Rect.Width())
	{
		FHorizPosition = Max - Rect.Width();
		if (FHorizPosition < 0) FHorizPosition = 0;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::Paint(void)
{
	TRect ImageRect							= GetClientRect();

	FImageBuffer->Width						= ImageRect.Width();
	FImageBuffer->Height						= ImageRect.Height();
	FImageBuffer->Canvas->Brush->Color	= Color;
	FImageBuffer->Canvas->FillRect(ImageRect);

	PanelRect = ImageRect;
	if (BevelOuter != bvNone)
	{
		DrawEdge(FImageBuffer->Canvas->Handle, &PanelRect, EDGE_SUNKEN, BF_ADJUST + BF_RECT);
/*		if (BevelOuter == bvLowered)
		{
			Frame3D(FImageBuffer->Canvas, PanelRect, clBtnShadow, clBtnHighlight, BevelWidth);
		}
		else
		{
			Frame3D(FImageBuffer->Canvas, PanelRect, clBtnHighlight, clBtnShadow, BevelWidth);
		}*/
	}
/*	if (BevelInner != bvNone)
	{
		Frame3D(FImageBuffer->Canvas, PanelRect, Color, Color, BevelWidth);
		if (BevelOuter == bvLowered)
		{
			Frame3D(FImageBuffer->Canvas, PanelRect, clBtnHighlight, clBtnShadow, BevelWidth);
		}
		else
		{
			Frame3D(FImageBuffer->Canvas, PanelRect, clBtnShadow, clBtnHighlight, BevelWidth);
		}
	}*/
	GridOuterRect = PanelRect;

	if ((FVertScrollButtonRatio == 0 || !FAutoScale )&& Height > 0)
	{
		FVertScrollButtonRatio = Extended(FVertScrollButtonHeight) / Height;
	}
	if ((FHorizScrollButtonRatio == 0 || !FAutoScale ) && Width > 0)
	{
		FHorizScrollButtonRatio = Extended(FHorizScrollButtonWidth) / Width;
	}

	if (FVertScrollButtonPos != vsNone)
	{
		if (FHorizScrollButtonPos == hsNone)
		{
			GridOuterRect.Left += BorderWidth;
			GridOuterRect.Right -= BorderWidth;
		}
		GridOuterRect.Top += BorderWidth;
		GridOuterRect.Bottom -= BorderWidth;
		switch (FVertScrollButtonPos)
		{
			case vsAbove:				GridOuterRect.Top += Extended(Height) * FVertScrollButtonRatio + ButtonGapHeight;		break;
			case vsBelow:				GridOuterRect.Bottom -= Extended(Height) * FVertScrollButtonRatio + ButtonGapHeight;	break;
			case vsAboveAndBelow:	GridOuterRect.Top += Extended(Height) * FVertScrollButtonRatio + ButtonGapHeight;
											GridOuterRect.Bottom -= Extended(Height) * FVertScrollButtonRatio + ButtonGapHeight;	break;
//			case vsScrollBarLeft:																						break;
//			case vsScrollBarRight:																						break;
		}
	}
	if (FHorizScrollButtonPos != hsNone)
	{
		if (FVertScrollButtonPos == vsNone)
		{
			GridOuterRect.Top += BorderWidth;
			GridOuterRect.Bottom -= BorderWidth;
		}
		GridOuterRect.Left += BorderWidth;
		GridOuterRect.Right -= BorderWidth;
		switch (FHorizScrollButtonPos)
		{
			case hsLeft:				GridOuterRect.Left += Extended(Width) * FHorizScrollButtonRatio + ButtonGapWidth;	break;
			case hsRight:				GridOuterRect.Right -= Extended(Width) * FHorizScrollButtonRatio + ButtonGapWidth;	break;
			case hsLeftAndRight:		GridOuterRect.Left += Extended(Width) * FHorizScrollButtonRatio + ButtonGapWidth;
											GridOuterRect.Right -= Extended(Width) * FHorizScrollButtonRatio + ButtonGapWidth;	break;
//			case hsScrollBarTop:																								break;
//			case hsScrollBarBottom:																							break;
		}
	}
	GridInnerRect = GridOuterRect;
	if (FGridBorder && (FVertScrollButtonPos != vsNone || FHorizScrollButtonPos != hsNone))
	{
		DrawEdge(FImageBuffer->Canvas->Handle, &GridInnerRect, EDGE_SUNKEN, BF_ADJUST + BF_RECT);
	}
	if (!FParentGridColor && FGridColor != Color)
	{
		FImageBuffer->Canvas->Brush->Color	= FGridColor;
		FImageBuffer->Canvas->FillRect(GridInnerRect);
	}
	CalculateVertScrollButtonPos();
	CalculateHorizScrollButtonPos();
	CalculateAutoSizes();
	if (FButtonBuffer)
	{
		delete [] FButtonBuffer;
	}
	FButtonBuffer = new DWORD[GetMaxButtonSize()];

	if (FUpButton->Visible && FVertScrollButtonPos != vsNone)
	{
		PaintButton(FUpButton, ScrollButtonsRect[UP_BUTTON], ImageRect, FImageBuffer.get(), Color);
	}
	if (FDownButton->Visible && FVertScrollButtonPos != vsNone)
	{
		PaintButton(FDownButton, ScrollButtonsRect[DOWN_BUTTON], ImageRect, FImageBuffer.get(), Color);
	}
	if (FLeftButton->Visible && FHorizScrollButtonPos != hsNone)
	{
		PaintButton(FLeftButton, ScrollButtonsRect[LEFT_BUTTON], ImageRect, FImageBuffer.get(), Color);
	}
	if (FRightButton->Visible && FHorizScrollButtonPos != hsNone)
	{
		PaintButton(FRightButton, ScrollButtonsRect[RIGHT_BUTTON], ImageRect, FImageBuffer.get(), Color);
	}

	int MinDimention = 0;
	for (int i=0; i<4; i++)
	{
		if (ScrollButtonsRect[i].Width() > 0 && (ScrollButtonsRect[i].Width() < MinDimention || MinDimention == 0))
		{
			MinDimention = ScrollButtonsRect[i].Width();
		}
		if (ScrollButtonsRect[i].Height() > 0 && (ScrollButtonsRect[i].Height() < MinDimention || MinDimention == 0))
		{
			MinDimention = ScrollButtonsRect[i].Height();
		}
	}
	for (int i=0; i<4; i++)
	{
		if ((i==UP_BUTTON && FVertScrollButtonPos != vsNone && FUpButton->Caption == "") ||
			 (i==DOWN_BUTTON && FVertScrollButtonPos != vsNone && FDownButton->Caption == "") ||
			 (i==LEFT_BUTTON && FHorizScrollButtonPos != hsNone && FLeftButton->Caption == "") ||
			 (i==RIGHT_BUTTON && FHorizScrollButtonPos != hsNone && FRightButton->Caption == ""))
		{
			int BtnLeft			= ScrollButtonsRect[i].Left + ((ScrollButtonsRect[i].Width() - MinDimention) / 2);
			int BtnTop			= ScrollButtonsRect[i].Top + ((ScrollButtonsRect[i].Height() - MinDimention) / 2);
			int BtnWidth		= MinDimention;//ScrollButtonsRect[i].Width();
			int BtnHeight		= MinDimention;//ScrollButtonsRect[i].Height();

			int ArrowHeight;
			int ArrowWidth;
			int ArrowLeft;
			int ArrowTop;

			TRect ArrowRect;
			if (i == DOWN_BUTTON || i == UP_BUTTON)
			{
				if (BtnWidth * 1000 / 1414 > BtnHeight)
				{
					ArrowHeight		= BtnHeight;
					ArrowWidth		= BtnHeight * 1414 / 1000;
					ArrowLeft		= BtnLeft + ((BtnWidth - ArrowWidth) / 2);

					ArrowRect		= TRect(ArrowLeft, BtnTop, ArrowLeft + ArrowWidth, BtnTop + ArrowHeight);
				}
				else
				{
					ArrowWidth		= BtnWidth;
					ArrowHeight		= BtnWidth * 1000 / 1414;
					ArrowTop			= BtnTop + ((BtnHeight - ArrowHeight) / 2);

					ArrowRect		= TRect(BtnLeft, ArrowTop, BtnLeft + ArrowWidth, ArrowTop + ArrowHeight);
				}
			}
			else if (i == LEFT_BUTTON || i == RIGHT_BUTTON)
			{
				if (BtnWidth * 1414 / 1000 > BtnHeight)
				{
					ArrowHeight		= BtnHeight;
					ArrowWidth		= BtnHeight * 1000 / 1414;
					ArrowLeft		= BtnLeft + ((BtnWidth - ArrowWidth) / 2);

					ArrowRect		= TRect(ArrowLeft, BtnTop, ArrowLeft + ArrowWidth, BtnTop + ArrowHeight);
				}
				else
				{
					ArrowWidth		= BtnWidth;
					ArrowHeight		= BtnWidth * 1414 / 1000;
					ArrowTop			= BtnTop + ((BtnHeight - ArrowHeight) / 2);

					ArrowRect		= TRect(BtnLeft, ArrowTop, BtnLeft + ArrowWidth, ArrowTop + ArrowHeight);
				}
			}
			InflateRect(&ArrowRect, -(100-FArrowScale)*ArrowRect.Width()/200, -(100-FArrowScale)*ArrowRect.Height()/200);

			if (	(i == LEFT_BUTTON		&& FLeftButton->Highlighted) ||
					(i == RIGHT_BUTTON	&& FRightButton->Highlighted) ||
					(i == UP_BUTTON		&& FUpButton->Highlighted) ||
					(i == DOWN_BUTTON		&& FDownButton->Highlighted))
			{
				OffsetRect(&ArrowRect, 1, 1);
			}

			TPoint points[3];

			if (i == DOWN_BUTTON)
			{
				points[0] = Point(ArrowRect.Left, ArrowRect.Top);
				points[1] = Point(ArrowRect.Right, ArrowRect.Top);
				points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Bottom);
			}
			else if (i == UP_BUTTON)
			{
				points[0] = Point(ArrowRect.Left, ArrowRect.Bottom);
				points[1] = Point(ArrowRect.Right, ArrowRect.Bottom);
				points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Top);
			}
			else if (i == RIGHT_BUTTON)
			{
				points[0] = Point(ArrowRect.Left, ArrowRect.Top);
				points[1] = Point(ArrowRect.Right, (ArrowRect.Top + ArrowRect.Bottom) / 2);
				points[2] = Point(ArrowRect.Left, ArrowRect.Bottom);
			}
			else if (i == LEFT_BUTTON)
			{
				points[0] = Point(ArrowRect.Right, ArrowRect.Top);
				points[1] = Point(ArrowRect.Left, (ArrowRect.Top + ArrowRect.Bottom) / 2);
				points[2] = Point(ArrowRect.Right, ArrowRect.Bottom);
			}
			FImageBuffer->Canvas->Pen->Color		= FArrowBorderColor;
			FImageBuffer->Canvas->Brush->Color	= FArrowColor;
			FImageBuffer->Canvas->Polygon(points, 2);
		}
	}
/*	for (int i=0; i<4; i++)
	{
		if ((i==UP_BUTTON && FUpButton->Caption == "") || (i==DOWN_BUTTON && FDownButton->Caption == "") || (i==LEFT_BUTTON && FLeftButton->Caption == "") || (i==RIGHT_BUTTON && FRightButton->Caption == ""))
		{
			int BtnLeft			= ScrollButtonsRect[i].Left;
			int BtnTop			= ScrollButtonsRect[i].Top;
			int BtnWidth		= ScrollButtonsRect[i].Width();
			int BtnHeight		= ScrollButtonsRect[i].Height();

			int ArrowHeight;
			int ArrowWidth;
			int ArrowLeft;
			int ArrowTop;

			TRect ArrowRect	= ScrollButtonsRect[i];
			if (i == DOWN_BUTTON || i == UP_BUTTON)
			{
				if (BtnWidth * 1000 / 1414 > BtnHeight)
				{
					ArrowHeight		= BtnHeight;
					ArrowWidth		= BtnHeight * 1414 / 1000;
					ArrowLeft		= BtnLeft + ((BtnWidth - ArrowWidth) / 2);

					ArrowRect		= TRect(ArrowLeft, BtnTop, ArrowLeft + ArrowWidth, BtnTop + ArrowHeight);
				}
				else
				{
					ArrowWidth		= BtnWidth;
					ArrowHeight		= BtnWidth * 1000 / 1414;
					ArrowTop			= BtnTop + ((BtnHeight - ArrowHeight) / 2);

					ArrowRect		= TRect(BtnLeft, ArrowTop, BtnLeft + ArrowWidth, ArrowTop + ArrowHeight);
				}
//				InflateRect(&ArrowRect, -ArrowWidth*36/100, -ArrowHeight*36/100);
//				InflateRect(&ArrowRect, -ArrowWidth*FArrowScale/100, -ArrowHeight*FArrowScale/100);
			}
			else if (i == LEFT_BUTTON || i == RIGHT_BUTTON)
			{
				if (BtnWidth * 1414 / 1000 > BtnHeight)
				{
					ArrowHeight		= BtnHeight;
					ArrowWidth		= BtnHeight * 1000 / 1414;
					ArrowLeft		= BtnLeft + ((BtnWidth - ArrowWidth) / 2);

					ArrowRect		= TRect(ArrowLeft, BtnTop, ArrowLeft + ArrowWidth, BtnTop + ArrowHeight);
				}
				else
				{
					ArrowWidth		= BtnWidth;
					ArrowHeight		= BtnWidth * 1414 / 1000;
					ArrowTop			= BtnTop + ((BtnHeight - ArrowHeight) / 2);

					ArrowRect		= TRect(BtnLeft, ArrowTop, BtnLeft + ArrowWidth, ArrowTop + ArrowHeight);
				}
//				InflateRect(&ArrowRect, -ArrowWidth*32/100, -ArrowHeight*32/100);
//				InflateRect(&ArrowRect, -ArrowWidth*FArrowScale/112, -ArrowHeight*FArrowScale/112);
			}
			InflateRect(&ArrowRect, -(100-FArrowScale)*ArrowRect.Width()/200, -(100-FArrowScale)*ArrowRect.Height()/200);

			if (	(i == LEFT_BUTTON		&& FLeftButton->Highlighted) ||
					(i == RIGHT_BUTTON	&& FRightButton->Highlighted) ||
					(i == UP_BUTTON		&& FUpButton->Highlighted) ||
					(i == DOWN_BUTTON		&& FDownButton->Highlighted))
			{
				OffsetRect(&ArrowRect, 1, 1);
			}

			TPoint points[3];

			if (i == DOWN_BUTTON)
			{
				points[0] = Point(ArrowRect.Left, ArrowRect.Top);
				points[1] = Point(ArrowRect.Right, ArrowRect.Top);
				points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Bottom);
			}
			else if (i == UP_BUTTON)
			{
				points[0] = Point(ArrowRect.Left, ArrowRect.Bottom);
				points[1] = Point(ArrowRect.Right, ArrowRect.Bottom);
				points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Top);
			}
			else if (i == RIGHT_BUTTON)
			{
				points[0] = Point(ArrowRect.Left, ArrowRect.Top);
				points[1] = Point(ArrowRect.Right, (ArrowRect.Top + ArrowRect.Bottom) / 2);
				points[2] = Point(ArrowRect.Left, ArrowRect.Bottom);
			}
			else if (i == LEFT_BUTTON)
			{
				points[0] = Point(ArrowRect.Right, ArrowRect.Top);
				points[1] = Point(ArrowRect.Left, (ArrowRect.Top + ArrowRect.Bottom) / 2);
				points[2] = Point(ArrowRect.Right, ArrowRect.Bottom);
			}
			FImageBuffer->Canvas->Pen->Color		= FArrowBorderColor;
			FImageBuffer->Canvas->Brush->Color	= FArrowColor;
			FImageBuffer->Canvas->Polygon(points, 2);
		}
	}
*/
	HRGN hRgn = CreateRectRgnIndirect(&GridInnerRect);
	SelectClipRgn(FImageBuffer.get()->Canvas->Handle, hRgn);
	DeleteObject(hRgn);

	TRect ButtonsRect = GridInnerRect;
	InflateRect(&ButtonsRect, -FGridBorderWidth, -FGridBorderWidth);

	int ButtonY = -FVertPosition;
	for (int CurrentRow=0; CurrentRow<FRowCount; CurrentRow++)
	{
		int ButtonX			= -FHorizPosition;
		int ButtonHeight	= FRowHeights[CurrentRow].Size;

		for (int CurrentCol=0; CurrentCol<FColCount; CurrentCol++)
		{
			int ButtonWidth = FColWidths[CurrentCol].Size;

			TRect ButtonRect(ButtonX + ButtonsRect.Left,
								  ButtonY + ButtonsRect.Top,
								  ButtonX + ButtonsRect.Left + ButtonWidth,
								  ButtonY + ButtonsRect.Top + ButtonHeight);

			if (ButtonRect.Width() > 0 && ButtonRect.Height() > 0 &&
					ButtonRect.Left < GridInnerRect.Right && ButtonRect.Right > GridInnerRect.Left &&
					ButtonRect.Top < GridInnerRect.Bottom && ButtonRect.Bottom > GridInnerRect.Top)
			{
				TGridButton *Button = GetButton(CurrentRow, CurrentCol);
//				if (FVertScrollButtonPos != vsNone || FHorizScrollButtonPos != hsNone)
//				{
					PaintButton(Button, ButtonRect, GridInnerRect, FImageBuffer.get(), GridColor);
//				}
//				else
//				{
//					PaintButton(Button, ButtonRect, GridInnerRect, FImageBuffer.get(), Color);
//				}
			}
			ButtonX += ButtonWidth + FBtnGapWidth;
		}
		ButtonY += ButtonHeight + FBtnGapHeight;
	}
	SelectClipRgn(FImageBuffer.get()->Canvas->Handle, NULL);

	BitBlt(	Canvas->Handle,
				0,
				0,
				FImageBuffer->Width,
				FImageBuffer->Height,
				FImageBuffer->Canvas->Handle,
				0,
				0,
				SRCCOPY);
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::DefineProperties(TFiler *Filer)
{
	TComponent::DefineProperties(Filer);
	Filer->DefineBinaryProperty("Buttons", ReadButtons, WriteButtons, true);
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::ReadButtons(TStream* Stream)
{
	int Data;
	Stream->ReadBuffer(&Data, sizeof(Data));
	if (Data == 1) // dfm version
	{
		for (int y=0; y<FRowCount; y++)
		{
			Stream->ReadBuffer(&Data, sizeof(Data));
			FRowHeights[y].Size = Data;
			
			Stream->ReadBuffer(&FRowHeights[y].Ratio, sizeof(FRowHeights[y].Ratio));
		}
		for (int x=0; x<FColCount; x++)
		{
			Stream->ReadBuffer(&Data, sizeof(Data));
			FColWidths[x].Size = Data;

			Stream->ReadBuffer(&FColWidths[x].Ratio, sizeof(FColWidths[x].Ratio));
		}
		for (int y=0; y<FRowCount; y++)
		{
			for (int x=0; x<FColCount; x++)
			{
				TGridButton *GridButton = FButtons[(y*FColCount) + x];

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Name.SetLength(Data);
				Stream->ReadBuffer(GridButton->Font->Name.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Size = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsBold;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsItalic;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsUnderline;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsStrikeOut;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->FontColor = static_cast<TColor>(Data);

//
				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Name.SetLength(Data);
				Stream->ReadBuffer(GridButton->LatchedFont->Name.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Size = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsBold;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsItalic;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsUnderline;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsStrikeOut;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFontColor = static_cast<TColor>(Data);


//
				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Caption.SetLength(Data);
				Stream->ReadBuffer(GridButton->Caption.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Enabled = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Visible = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentFont = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentFontColor = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentLatchedFont = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentLatchedFontColor = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->GlyphAlpha = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->GlyphScale = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->TransparentGlyph = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->TransparentColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->KeepGlyphAspect = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data > 0)
				{
					GridButton->Glyph->LoadFromStream(Stream);
				}
			}
		}
	}
	else if (Data == 2) // dfm version
	{
		for (int y=0; y<FRowCount; y++)
		{
			Stream->ReadBuffer(&Data, sizeof(Data));
//			RowHeights[y] = Data;
			FRowHeights[y].Size = Data;
			FRowHeights[y].Ratio = 0;
		}
		for (int x=0; x<FColCount; x++)
		{
			Stream->ReadBuffer(&Data, sizeof(Data));
//			ColWidths[x] = Data;
			FColWidths[x].Size = Data;
			FColWidths[x].Ratio = 0;
		}
		for (int y=0; y<FRowCount; y++)
		{
			for (int x=0; x<FColCount; x++)
			{
				TGridButton *GridButton = FButtons[(y*FColCount) + x];

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Name.SetLength(Data);
				Stream->ReadBuffer(GridButton->Font->Name.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Size = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsBold;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsItalic;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsUnderline;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsStrikeOut;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->FontColor = static_cast<TColor>(Data);

//
				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Name.SetLength(Data);
				Stream->ReadBuffer(GridButton->LatchedFont->Name.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Size = Data;          

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsBold;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsItalic;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsUnderline;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsStrikeOut;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFontColor = static_cast<TColor>(Data);


//
				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Caption.SetLength(Data);
				Stream->ReadBuffer(GridButton->Caption.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Enabled = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Visible = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentFont = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentFontColor = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentLatchedFont = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentLatchedFontColor = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->GlyphAlpha = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->GlyphScale = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->TransparentGlyph = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->TransparentColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->KeepGlyphAspect = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data > 0)
				{
					GridButton->Glyph->LoadFromStream(Stream);
				}
			}
		}
	}
	else if (Data == 3) // dfm version
	{
		for (int y=0; y<FRowCount; y++)
		{
			Stream->ReadBuffer(&Data, sizeof(Data));
			FRowHeights[y].Size = Data;

			Stream->ReadBuffer(&FRowHeights[y].Ratio, sizeof(FRowHeights[y].Ratio));
		}
		for (int x=0; x<FColCount; x++)
		{
			Stream->ReadBuffer(&Data, sizeof(Data));
			FColWidths[x].Size = Data;

			Stream->ReadBuffer(&FColWidths[x].Ratio, sizeof(FColWidths[x].Ratio));
		}
		for (int y=0; y<FRowCount; y++)
		{
			for (int x=0; x<FColCount; x++)
			{
				TGridButton *GridButton = FButtons[(y*FColCount) + x];

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->DisabledColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Name.SetLength(Data);
				Stream->ReadBuffer(GridButton->Font->Name.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Size = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsBold;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsItalic;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsUnderline;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsStrikeOut;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->FontColor = static_cast<TColor>(Data);

//
				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Name.SetLength(Data);
				Stream->ReadBuffer(GridButton->LatchedFont->Name.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Size = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsBold;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsItalic;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsUnderline;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsStrikeOut;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFontColor = static_cast<TColor>(Data);


//
				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Caption.SetLength(Data);
				Stream->ReadBuffer(GridButton->Caption.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Enabled = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Visible = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentFont = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentFontColor = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentLatchedFont = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentLatchedFontColor = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->GlyphAlpha = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->GlyphScale = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->TransparentGlyph = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->TransparentColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->KeepGlyphAspect = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data > 0)
				{
					GridButton->Glyph->LoadFromStream(Stream);
				}
			}
		}
	}
	else if (Data == 4) // dfm version
	{
		for (int y=0; y<FRowCount; y++)
		{
			Stream->ReadBuffer(&Data, sizeof(Data));
			FRowHeights[y].Size = Data;

			Stream->ReadBuffer(&FRowHeights[y].Ratio, sizeof(FRowHeights[y].Ratio));
		}
		for (int x=0; x<FColCount; x++)
		{
			Stream->ReadBuffer(&Data, sizeof(Data));
			FColWidths[x].Size = Data;

			Stream->ReadBuffer(&FColWidths[x].Ratio, sizeof(FColWidths[x].Ratio));
		}
		for (int y=0; y<FRowCount; y++)
		{
			for (int x=0; x<FColCount; x++)
			{
				TGridButton *GridButton = FButtons[(y*FColCount) + x];

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->DisabledColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Name.SetLength(Data);
				Stream->ReadBuffer(GridButton->Font->Name.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Font->Size = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsBold;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsItalic;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsUnderline;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->Font->Style = GridButton->Font->Style << fsStrikeOut;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->FontColor = static_cast<TColor>(Data);

//
				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Name.SetLength(Data);
				Stream->ReadBuffer(GridButton->LatchedFont->Name.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Color = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFont->Size = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsBold;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsItalic;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsUnderline;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data) GridButton->LatchedFont->Style = GridButton->LatchedFont->Style << fsStrikeOut;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->LatchedFontColor = static_cast<TColor>(Data);


//
				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Caption.SetLength(Data);
				Stream->ReadBuffer(GridButton->Caption.c_str(), Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Enabled = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Visible = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentFont = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentFontColor = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentLatchedFont = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->ParentLatchedFontColor = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->GlyphAlpha = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->GlyphScale = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->TransparentGlyph = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->TransparentColor = static_cast<TColor>(Data);

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->KeepGlyphAspect = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				GridButton->Tag = Data;

				Stream->ReadBuffer(&Data, sizeof(Data));
				if (Data > 0)
				{
					GridButton->Glyph->LoadFromStream(Stream);
				}
			}
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::WriteButtons(TStream* Stream)
{
	int Data;

	Data = 4;	// dfm writer version
	Stream->WriteBuffer(&Data, sizeof(Data));

	for (int y=0; y<FRowCount; y++)
	{
//		Data = RowHeights[y];
		Data = FRowHeights[y].Size;
		Stream->WriteBuffer(&Data, sizeof(Data));

		Stream->WriteBuffer(&FRowHeights[y].Ratio, sizeof(FRowHeights[y].Ratio));
	}
	for (int x=0; x<FColCount; x++)
	{
		Data = FColWidths[x].Size;
		Stream->WriteBuffer(&Data, sizeof(Data));

		Stream->WriteBuffer(&FColWidths[x].Ratio, sizeof(FColWidths[x].Ratio));
	}
	for (int y=0; y<FRowCount; y++)
	{
		for (int x=0; x<FColCount; x++)
		{
			TGridButton *GridButton = FButtons[(y*FColCount) + x];

			Data = GridButton->Color;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->LatchedColor;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->DisabledColor;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->Font->Name.Length();
			Stream->WriteBuffer(&Data, sizeof(Data));
			Stream->WriteBuffer(GridButton->Font->Name.c_str(), Data);

			Data = GridButton->Font->Color;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->Font->Size;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->Font->Style.Contains(fsBold);
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->Font->Style.Contains(fsItalic);
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->Font->Style.Contains(fsUnderline);
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->Font->Style.Contains(fsStrikeOut);
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->FontColor;
			Stream->WriteBuffer(&Data, sizeof(Data));

//
			Data = GridButton->LatchedFont->Name.Length();
			Stream->WriteBuffer(&Data, sizeof(Data));
			Stream->WriteBuffer(GridButton->LatchedFont->Name.c_str(), Data);

			Data = GridButton->LatchedFont->Color;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->LatchedFont->Size;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->LatchedFont->Style.Contains(fsBold);
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->LatchedFont->Style.Contains(fsItalic);
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->LatchedFont->Style.Contains(fsUnderline);
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->LatchedFont->Style.Contains(fsStrikeOut);
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->LatchedFontColor;
			Stream->WriteBuffer(&Data, sizeof(Data));

//
			Data = GridButton->Caption.Length();
			Stream->WriteBuffer(&Data, sizeof(Data));
			Stream->WriteBuffer(GridButton->Caption.c_str(), Data);

			Data = GridButton->Enabled;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->Visible;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->ParentFont;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->ParentFontColor;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->ParentLatchedFont;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->ParentLatchedFontColor;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->GlyphAlpha;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->GlyphScale;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->TransparentGlyph;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->TransparentColor;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->KeepGlyphAspect;
			Stream->WriteBuffer(&Data, sizeof(Data));

			Data = GridButton->Tag;
			Stream->WriteBuffer(&Data, sizeof(Data));

			if (GridButton->Glyph->Empty)
			{
				Data = 0;
				Stream->WriteBuffer(&Data, sizeof(Data));
			}
			else
			{
				std::auto_ptr<TMemoryStream> Glyph(new TMemoryStream);
				GridButton->Glyph->SaveToStream(Glyph.get());
				Data = Glyph->Size;
				Stream->WriteBuffer(&Data, sizeof(Data));
				Stream->CopyFrom(Glyph.get(), 0);
			}
		}
	}
}
//---------------------------------------------------------------------------
/*bool __fastcall TTouchGrid::DoWrite(int Value)
{
	return (Value != -1);
}*/
//---------------------------------------------------------------------------
/*void __fastcall TTouchGrid::GetChildren(Classes::TGetChildProc Proc, Classes::TComponent* Root)
{
	TCustomTouchButtonControl::GetChildren(Proc, Root);
	for (unsigned i=0; i<FButtons.size(); i++)
	{
		Proc(FButtons.size());
		Proc(FButtons[i]);
	}
}
//---------------------------------------------------------------------------
TComponent* __fastcall TTouchGrid::GetChildOwner()
{
	return this; // This component is the owner of all children
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::ReadState(Classes::TReader* Reader)
{
	TComponentClass StreamUsedClasses[] = { __classid(TGridButton) };
	RegisterClasses(StreamUsedClasses,0); // Index is high position not count

	TCustomTouchButtonControl::ReadState(Reader);
}
//---------------------------------------------------------------------------
void __fastcall TTouchGrid::Loaded(void)
{
	TCustomTouchButtonControl::Loaded();

	FUpButton->Caption		= ((TTouchBtn *)Controls[0])->Caption;
	FUpButton->ParentColor	= false;
	FUpButton->Color			= ((TTouchBtn *)Controls[0])->Color;
	FUpButton->Font			= ((TTouchBtn *)Controls[0])->Font;

	FDownButton->Caption		= ((TTouchBtn *)Controls[1])->Caption;
	FDownButton->ParentColor	= false;
	FDownButton->Color		= ((TTouchBtn *)Controls[1])->Color;
	FDownButton->Font			= ((TTouchBtn *)Controls[1])->Font;

	FLeftButton->Caption		= ((TTouchBtn *)Controls[2])->Caption;
	FLeftButton->Color		= ((TTouchBtn *)Controls[2])->Color;
	FLeftButton->Font			= ((TTouchBtn *)Controls[2])->Font;

	FRightButton->Caption	= ((TTouchBtn *)Controls[3])->Caption;
	FRightButton->Color		= ((TTouchBtn *)Controls[3])->Color;
	FRightButton->Font		= ((TTouchBtn *)Controls[3])->Font;

	DestroyComponents();
}*/
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
__fastcall TGridScrollThread::TGridScrollThread(bool CreateSuspended): TThread(CreateSuspended)
{
	FreeOnTerminate	= false;
	Paused				= true;
	UseThread			= true;
	
	QueryPerformanceFrequency(&liClockFrequency);
	ptClockFrequency	= *(__int64*)&liClockFrequency;
}
//---------------------------------------------------------------------------
void TGridScrollThread::Scroll(TScrollDirection iDirection, int iDistance, int iTime)
{
/*	if (UseThread && !Suspended)
	{
		return;
	}*/
	DistanceLeft	= 0;
	
	FDistance		= iDistance;
	FTime				= iTime;
	FDirection		= iDirection;

	Paused			= false;
	if (!UseThread)
	{
		Execute();
	}
	else if (Suspended)
	{
		Resume();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridScrollThread::Execute()
{
	try
	{
		do
		{
			while (!Terminated && Paused && UseThread)
			{
				Suspend();
			}
			if (!Terminated)
			{
				Paused			= true;	// Will stop in a loop at completion of scroll

				Distance			= FDistance;	// Allow this scroll to complete while cue-ing up the next
				Time				= FTime;
				Direction		= FDirection;

				Jump				= 0;
				LastPos			= 0;
				DistanceLeft	= Distance;

				if (ptClockFrequency != 0)
				{
					QueryPerformanceCounter(&liStartTime);
					ptStartTime	= *(__int64*)&liStartTime;
					StartTime	= ptStartTime * 1000 / ptClockFrequency;
				}
				else
				{
					StartTime = GetTickCount();
				}
				CurrentTime = StartTime + 5;
				while(CurrentTime - Time < StartTime && DistanceLeft != 0 && Time > 0)
				{
					int Position = int(CurrentTime - StartTime) * Distance / int(Time);

					Jump = Position - LastPos;
					DistanceLeft -= Jump;
					LastPos = Position;

					if (Jump)
					{
						if (UseThread)
						{
							Synchronize(UpdateGrid);
						}
						else
						{
							UpdateGrid();
						}
					}
					if (ptClockFrequency != 0)
					{
						QueryPerformanceCounter(&liCurrentTime);
						ptCurrentTime	= *(__int64*)&liCurrentTime;
						CurrentTime		= ptCurrentTime * 1000 / ptClockFrequency;
					}
					else
					{
						CurrentTime = GetTickCount();
					}
					if (Terminated && UseThread)
					{
						break;
					}
					Sleep(0);
				}
				Jump = DistanceLeft;
				if (Jump)
				{
					if (UseThread)
					{
						Synchronize(UpdateGrid);
					}
					else
					{
						UpdateGrid();
					}
				}
				if (UseThread)
				{
					Synchronize(CompletedScroll);
				}
				else
				{
					CompletedScroll();
				}
			}
			Sleep(0);
		}
		while (!Terminated && UseThread);
	}
	catch(...)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridScrollThread::UpdateGrid(void)
{
	if (Direction == sdLeft || Direction == sdRight)
	{
		TouchGrid->FHorizPosition += Jump;
	}
	else if (Direction == sdUp || Direction == sdDown)
	{
		TouchGrid->FVertPosition += Jump;
	}
	TouchGrid->Invalidate();
	if (!UseThread)
	{
		TouchGrid->Update();
	}
}
//---------------------------------------------------------------------------
void __fastcall TGridScrollThread::CompletedScroll(void)
{
	if (!UseThread)
	{
		Application->ProcessMessages();
	}
	TouchGrid->GridCompleteScroll(FDirection, FDistance);
}
//---------------------------------------------------------------------------

