//---------------------------------------------------------------------------
#pragma hdrstop

#include "TouchKeyboard.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
enum TTouchButtonKey {

	 TKB_TILD = 0,
	 TKB_1,
	 TKB_2,
	 TKB_3,
	 TKB_4,
	 TKB_5,
	 TKB_6,
	 TKB_7,
	 TKB_8,
	 TKB_9,
	 TKB_0,

	 TKB_MINUS,
	 TKB_PLUS,
	 TKB_BACK,

	 TKB_TAB,

	 TKB_Q,
	 TKB_W,
	 TKB_E,
	 TKB_R,
	 TKB_T,
	 TKB_Y,
	 TKB_U,
	 TKB_I,
	 TKB_O,
	 TKB_P,

	 TKB_SQRL,				// [{
	 TKB_SQRR,				//	]{
	 TKB_BKSLASH,			//	\|

	 TKB_CAPS,

	 TKB_A,
	 TKB_S,
	 TKB_D,
	 TKB_F,
	 TKB_G,
	 TKB_H,
	 TKB_J,
	 TKB_K,
	 TKB_L,

	 TKB_COLON,			 // ,:
	 TKB_QUOTE,			 // '"
	 TKB_ENTER,			 // '"

	 TKB_LSHIFT,

	 TKB_Z,
	 TKB_X,
	 TKB_C,
	 TKB_V,
	 TKB_B,
	 TKB_N,
	 TKB_M,

	 TKB_COMMA,
	 TKB_DOT,
	 TKB_SLASH,
	 TKB_RSHIFT,

	 TKB_SPACE,

	 TKB_LEFT,
	 TKB_RIGHT,
	 TKB_UP,
	 TKB_DOWN,
	 TKB_NONE
};
//---------------------------------------------------------------------------
const int Row1Ratio[] = { 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 660, 1420 };
const int Row2Ratio[] = { 1004, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 1004 };
const int Row3Ratio[] = { 1100, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 1574 };
const int Row4Ratio[] = { 1670, 666, 666, 666, 666, 666, 666, 666, 666, 666, 666, 1670 };
const int Row5Ratio[] = { 10000};
const int Row6Ratio[] = { 100, 100, 100, 100 };
//---------------------------------------------------------------------------
const TBtnText Row1Text[] = {
	TBtnText("`", '`', "~", '~', false),
	TBtnText("1", '1', "!", '!', false),
	TBtnText("2", '2', "@", '@', false),
	TBtnText("3", '3', "#", '#', false),
	TBtnText("4", '4', "$", '$', false),
	TBtnText("5", '5', "%", '%', false),
	TBtnText("6", '6', "^", '^', false),
	TBtnText("7", '7', "&", '&', false),
	TBtnText("8", '8', "*", '*', false),
	TBtnText("9", '9', "(", '(', false),
	TBtnText("0", '0', ")", ')', false),
	TBtnText("-", '-', "_", '_', false),
	TBtnText("=", '=', "+", '+', false),
	TBtnText("Backspace", '\b', "Backspace", '\b')
};
const TBtnText Row2Text[] = {
	TBtnText("Tab", '\t', "Tab", '\t'),
	TBtnText("q", 'q', "Q", 'Q'),
	TBtnText("w", 'w', "W", 'W'),
	TBtnText("e", 'e', "E", 'E'),
	TBtnText("r", 'r', "R", 'R'),
	TBtnText("t", 't', "T", 'T'),
	TBtnText("y", 'y', "Y", 'Y'),
	TBtnText("u", 'u', "U", 'U'),
	TBtnText("i", 'i', "I", 'I'),
	TBtnText("o", 'o', "O", 'O'),
	TBtnText("p", 'p', "P", 'P'),
	TBtnText("[", '[', "{", '{', false),
	TBtnText("]", ']', "}", '}', false),
	TBtnText("\\", '\\', "|", '|', false)
};
const TBtnText Row3Text[] = {
	TBtnText("Caps Lock", 0, "Caps Lock", 0),
	TBtnText("a", 'a', "A", 'A'),
	TBtnText("s", 's', "S", 'S'),
	TBtnText("d", 'd', "D", 'D'),
	TBtnText("f", 'f', "F", 'F'),
	TBtnText("g", 'g', "G", 'G'),
	TBtnText("h", 'h', "H", 'H'),
	TBtnText("j", 'j', "J", 'J'),
	TBtnText("k", 'k', "K", 'K'),
	TBtnText("l", 'l', "L", 'L'),
	TBtnText(";", ';', ":", ':', false),
	TBtnText("'", '\'', "\"", '\"', false),
	TBtnText("Enter", '\r', "Enter", '\r')
};
const TBtnText Row4Text[] = {
	TBtnText("Shift", 0, "Shift", 0),
	TBtnText("z", 'z', "Z", 'Z'),
	TBtnText("x", 'x', "X", 'X'),
	TBtnText("c", 'c', "C", 'C'),
	TBtnText("v", 'v', "V", 'V'),
	TBtnText("b", 'b', "B", 'B'),
	TBtnText("n", 'n', "N", 'N'),
	TBtnText("m", 'm', "M", 'M'),
	TBtnText(",", ',', "<", '<', false),
	TBtnText(".", '.', ">", '>', false),
	TBtnText("/", '/', "?", '?', false),
	TBtnText("Shift", 0, "Shift", 0)
};
const TBtnText Row5Text[] = {
	TBtnText("", ' ', " ", ' ')
};
const TBtnText Row6Text[] = {
	TBtnText("", ' ', " ", ' '),
	TBtnText("", ' ', " ", ' '),
	TBtnText("", ' ', " ", ' '),
	TBtnText("", ' ', " ", ' ')
};
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TTouchKeyboard *)
{
	new TTouchKeyboard(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTouchKeyboard::TTouchKeyboard(TComponent* Owner) : TCustomTouchButtonControl(Owner)
{
	for (int i=0; i<TKB_NONE; i++)
	{
		FButtons.push_back(new TGridButton(this));
	}
	SetCaptions();
	Width					= 505;
	Height				= 201;
	BevelOuter			= bvLowered;

	FShift				= false;
	FGridBorderWidth	= 8;
	FBtnGapWidth		= 2;
	FBtnGapHeight		= 2;
	FButtonColor		= FButtons[0]->Color;
}
//---------------------------------------------------------------------------
void TTouchKeyboard::SetCaptions()
{
	for (unsigned i=0; i<FButtons.size(); i++)
	{
		TGridButton *GridButton = FButtons[i];
		
		TBtnText BtnText = GetBtnText(i);
		if (BtnText.UseShiftedWhenLocked)
		{
			if ((FShift || FCapsLock) && !(FShift && FCapsLock))
			{
				GridButton->Caption = BtnText.ShiftedCaption;
			}
			else
			{
				GridButton->Caption = BtnText.UnshiftedCaption;
			}
		}
		else
		{
			if (FShift)
			{
				GridButton->Caption = BtnText.ShiftedCaption;
			}
			else
			{
				GridButton->Caption = BtnText.UnshiftedCaption;
			}
		}
	}
}
//---------------------------------------------------------------------------
const TBtnText &TTouchKeyboard::GetBtnText(int ButtonIndex)
{
	const TBtnText *RowText = NULL;
	int RowIndex = -1;
	if (ButtonIndex < TKB_TAB)
	{
		RowText	= Row1Text;
		RowIndex	= ButtonIndex;
	}
	else if (ButtonIndex < TKB_CAPS)
	{
		RowText	= Row2Text;
		RowIndex	= ButtonIndex - TKB_TAB;
	}
	else if (ButtonIndex < TKB_LSHIFT)
	{
		RowText	= Row3Text;
		RowIndex	= ButtonIndex - TKB_CAPS;
	}
	else if (ButtonIndex < TKB_SPACE)
	{
		RowText	= Row4Text;
		RowIndex	= ButtonIndex - TKB_LSHIFT;
	}
	else if (ButtonIndex < TKB_LEFT)
	{
		RowText	= Row5Text;
		RowIndex	= ButtonIndex - TKB_SPACE;
	}
	else if (ButtonIndex < TKB_NONE)
	{
		RowText	= Row6Text;
		RowIndex	= ButtonIndex - TKB_LEFT;
	}
	if (RowText == NULL || RowIndex == -1)
	{
		throw Exception ("No text for button index " + IntToStr(ButtonIndex));
	}
	return RowText[RowIndex];
}
//---------------------------------------------------------------------------
void __fastcall TTouchKeyboard::Paint(void)
{
	TRect ImageRect							= GetClientRect();

	FImageBuffer->Width						= ImageRect.Width();
	FImageBuffer->Height						= ImageRect.Height();
	FImageBuffer->Canvas->Brush->Color	= Color;
	FImageBuffer->Canvas->FillRect(ImageRect);

	if (FButtonBuffer)
	{
		delete [] FButtonBuffer;
	}
	FButtonBuffer = new DWORD[ImageRect.Width() * ImageRect.Height()];

	if (BevelOuter != bvNone)
	{
		DrawEdge(FImageBuffer->Canvas->Handle, &ImageRect, EDGE_SUNKEN, BF_ADJUST + BF_RECT);
	}

	for (int i=0; i<=TKB_SPACE; i++)
	{
		TRect Rect = GetButtonRect(FButtons[i]);

		PaintButton(FButtons[i], Rect, ImageRect, FImageBuffer.get(), Color);
	}

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
TGridButton *TTouchKeyboard::GetButtonXY(int X, int Y)
{
	for (unsigned i=0; i<FButtons.size(); i++)
	{
		if (PtInRect(&GetButtonRect(FButtons[i]), TPoint(X, Y)))
		{
			return FButtons[i];
		}
	}
	return NULL;
}
//---------------------------------------------------------------------------
TRect TTouchKeyboard::GetButtonRect(const TGridButton *Button)
{
	std::vector<TGridButton *>::iterator iButton = std::find(FButtons.begin(), FButtons.end(), Button);
	if (iButton != FButtons.end())
	{
		int ButtonIndex	= iButton - FButtons.begin();
		const int *Row		= NULL;
		int RowIndex		= -1;
		int Top				= 0;
		int ButtonsHeight	= ClientHeight - FGridBorderWidth - FGridBorderWidth + FBtnGapHeight;
		if (ButtonIndex < TKB_TAB)
		{
			Row		= Row1Ratio;
			RowIndex	= ButtonIndex;
			Top		= FGridBorderWidth;
		}
		else if (ButtonIndex < TKB_CAPS)
		{
			Row		= Row2Ratio;
			RowIndex	= ButtonIndex - TKB_TAB;
			Top		= ButtonsHeight / 5 + FGridBorderWidth;
		}
		else if (ButtonIndex < TKB_LSHIFT)
		{
			Row		= Row3Ratio;
			RowIndex	= ButtonIndex - TKB_CAPS;
			Top		= ButtonsHeight * 2 / 5 + FGridBorderWidth;
		}
		else if (ButtonIndex < TKB_SPACE)
		{
			Row		= Row4Ratio;
			RowIndex	= ButtonIndex - TKB_LSHIFT;
			Top		= ButtonsHeight * 3 / 5 + FGridBorderWidth;
		}
		else if (ButtonIndex < TKB_LEFT)
		{
			Row		= Row5Ratio;
			RowIndex	= ButtonIndex - TKB_SPACE;
			Top		= ButtonsHeight * 4 / 5 + FGridBorderWidth;
		}
		else if (ButtonIndex < TKB_NONE)
		{
			Row		= Row6Ratio;
			RowIndex	= ButtonIndex - TKB_LEFT;
		}
		if (Row && RowIndex > -1)
		{
			int WidthToGo = ClientWidth - FGridBorderWidth - FGridBorderWidth + FBtnGapWidth;
			int TotalFraction = 10000;

			int Left = FGridBorderWidth;
			for (int i=0; i<RowIndex; i++)
			{
				int BtnWidth = min(Row[i] * WidthToGo / TotalFraction, ClientWidth-1);
				Left += BtnWidth;
				WidthToGo -= BtnWidth;
				TotalFraction -= Row[i];
			}
			int Right	= min(Left + (Row[RowIndex] * WidthToGo / TotalFraction) - FBtnGapWidth, ClientWidth-1);
			int Bottom	= min(Top + (ButtonsHeight / 5) - FBtnGapHeight, ClientHeight-1);
			return TRect(Left, Top, Right, Bottom);
		}
	}
	return TRect(0,0,0,0);
}
//---------------------------------------------------------------------------
void __fastcall TTouchKeyboard::SetGridBorderWidth(int GridBorderWidth)
{
	FGridBorderWidth	= GridBorderWidth;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchKeyboard::SetBtnGapWidth(int Width)
{
	FBtnGapWidth = Width;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchKeyboard::SetBtnGapHeight(int Height)
{
	FBtnGapHeight = Height;
	Invalidate();
}
//---------------------------------------------------------------------------
void	__fastcall TTouchKeyboard::SetButtonColor(TColor Value)
{
	FButtonColor = Value;
	for (unsigned i=0; i<FButtons.size(); i++)
	{
		FButtons[i]->Color			= FButtonColor;
		FButtons[i]->LatchedColor	= FButtonColor;
	}
	Invalidate();
}
//---------------------------------------------------------------------------
void	__fastcall TTouchKeyboard::SetShift(bool Value)
{
	FShift = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void	__fastcall TTouchKeyboard::SetCapsLock(bool Value)
{
	FCapsLock = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchKeyboard::MouseDown(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	bool CallDefaultHandler = false;
	TGridButton *GridButton = GetButtonXY(X, Y);
	if (GridButton)
	{
		std::vector<TGridButton *>::iterator iButton = std::find(FButtons.begin(), FButtons.end(), GridButton);
		if (iButton != FButtons.end())
		{
			int ButtonIndex = iButton - FButtons.begin();
			switch (ButtonIndex)
			{
				case TKB_CAPS:
				{
					break;
				}
				case TKB_LSHIFT:
				case TKB_RSHIFT:
				{
					break;
				}
				case TKB_LEFT:
				{
					break;
				}
				case TKB_RIGHT:
				{
					break;
				}
				case TKB_UP:
				{
					break;
				}
				case TKB_DOWN:
				{
					break;
				}
				default:
				{
					CallDefaultHandler = true;
					break;
				}
			}
		}
	}
	if (CallDefaultHandler)
	{
		TCustomTouchButtonControl::MouseDown(Button, Shift, X, Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchKeyboard::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	bool CallDefaultHandler = false;
	TGridButton *GridButton = GetButtonXY(X, Y);
	if (GridButton)
	{
		std::vector<TGridButton *>::iterator iButton = std::find(FButtons.begin(), FButtons.end(), GridButton);
		if (iButton != FButtons.end())
		{
			int ButtonIndex = iButton - FButtons.begin();
			switch (ButtonIndex)
			{
				case TKB_CAPS:
				{
					FCapsLock = !FCapsLock;
					GridButton->Latched = FCapsLock;
					break;
				}
				case TKB_LSHIFT:
				case TKB_RSHIFT:
				{
					FShift = !FShift;
					FButtons[TKB_LSHIFT]->Latched = FShift;
					FButtons[TKB_RSHIFT]->Latched = FShift;
					break;
				}
				case TKB_LEFT:
				{
					break;
				}
				case TKB_RIGHT:
				{
					break;
				}
				case TKB_UP:
				{
					break;
				}
				case TKB_DOWN:
				{
					break;
				}
				default:
				{
					if (FOnClick)
					{
						char Char = 0;
						TBtnText BtnText = GetBtnText(ButtonIndex);
						if (BtnText.UseShiftedWhenLocked)
						{
							if ((FShift || FCapsLock) && !(FShift && FCapsLock))
							{
								Char = BtnText.ShiftedChar;
							}
							else
							{
								Char = BtnText.UnshiftedChar;
							}
						}
						else
						{
							if (FShift)
							{
								Char = BtnText.ShiftedChar;
							}
							else
							{
								Char = BtnText.UnshiftedChar;
							}
						}
						FOnClick(this, Char);
					}
					FShift = false;
					FButtons[TKB_LSHIFT]->Latched = false;
					FButtons[TKB_RSHIFT]->Latched = false;
					CallDefaultHandler = true;
					break;
				}
			}
		}
	}
	if (CallDefaultHandler)
	{
		TCustomTouchButtonControl::MouseUp(Button, Shift, X, Y);
	}
	SetCaptions();
}
//---------------------------------------------------------------------------

