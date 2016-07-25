//---------------------------------------------------------------------------
#pragma hdrstop

#include "TouchTime.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
const int HOUR_UP_BUTTON		= 0;
const int HOUR_DOWN_BUTTON		= 1;
const int MINUTE_UP_BUTTON		= 2;
const int MINUTE_DOWN_BUTTON	= 3;
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TTouchTime *)
{
	new TTouchTime(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTouchTime::TTouchTime(TComponent* Owner) : TCustomTouchButtonControl(Owner),
		FHourUpButton(new TGridButton(this)),
		FHourDownButton(new TGridButton(this)),
		FMinuteUpButton(new TGridButton(this)),
		FMinuteDownButton(new TGridButton(this)),
		FScrollTimer(new TTimer(NULL)),
		FImageRect(0, 0, 0, 0)
{
	FOnTimeClick						= NULL;
	FOnChange							= NULL;

	Width									= 273;
	Height								= 121;
	BorderWidth							= 4;
	BevelOuter							= bvLowered;

	IndicatorDelay						= 100;

	FButtonWidth						= 60;
	FBtnGapWidth						= 4;
	FBtnGapHeight						= 4;
	ParentFont							= false;
	Font->Size							= 26;
	Font->Style							= TFontStyles() << fsBold;
	FArrowColor							= clBlack;
	FArrowBorderColor					= clBlack;
	FArrowScale							= 30;

	FScrollTimer->OnTimer			= ScrollTimeout;
	FInitialDelay						= 300;
	FRepeatDelay						= 100;

	FScrollButtonRatio				= 0;

	FDisplayFormat						= "HH:nn";
}
//---------------------------------------------------------------------------
__fastcall TTouchTime::~TTouchTime()
{
	try
	{
		delete FHourUpButton;
		delete FHourDownButton;
		delete FMinuteUpButton;
		delete FMinuteDownButton;
	}
	catch(...)
	{
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetTime(TTime Time)
{
	FTime = Time - static_cast<int>(Time);
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetDisplayFormat(AnsiString DisplayFormat)
{
	FDisplayFormat = DisplayFormat;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetButtonWidth(int ButtonWidth)
{
	FButtonWidth = ButtonWidth;
	FScrollButtonRatio = static_cast<Extended>(FButtonWidth) / static_cast<Extended>(Width);
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetArrowColor(TColor Value)
{
	FArrowColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetArrowBorderColor(TColor Value)
{
	FArrowBorderColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetArrowScale(int Value)
{
	if (Value < 0 || Value > 100)
	{
		throw Exception("Scale must be between 0 and 100");
	}
	FArrowScale = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::ScrollTimeout(TObject *Sender)
{
	FScrollTimer->Enabled = false;
	if (FMouseInCurrentScrollButton)
	{
		ApplyScroll();
		FScrollTimer->Interval = FRepeatDelay;
		FScrollTimer->Enabled = true;
	}
}
//---------------------------------------------------------------------------
int TTouchTime::GetMaxButtonSize()
{
	int MaxWidth = 0;
	int MaxHeight = 0;
	for (int i=0; i<4; i++)	MaxWidth = max(MaxWidth, ScrollButtonsRect[i].Width());
	for (int i=0; i<4; i++)	MaxHeight = max(MaxHeight, ScrollButtonsRect[i].Height());
	return MaxWidth * MaxHeight;
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::Paint(void)
{
	FImageRect									= GetClientRect();

	FImageBuffer->Width						= FImageRect.Width();
	FImageBuffer->Height						= FImageRect.Height();
	FImageBuffer->Canvas->Brush->Color	= Color;
	FImageBuffer->Canvas->FillRect(FImageRect);

	if (BevelOuter != bvNone)
	{
		DrawEdge(FImageBuffer->Canvas->Handle, &FImageRect, EDGE_SUNKEN, BF_ADJUST + BF_RECT);
	}
	if ((FScrollButtonRatio == 0 || !FAutoScale ) && Width > 0)
	{
		FScrollButtonRatio = static_cast<Extended>(FButtonWidth) / Width;
	}

	FImageBuffer->Canvas->Font = Font;//FTimeFont.get();

	TRect CenterRect = FImageRect;
	AnsiString TimeStr = FTime.FormatString(FDisplayFormat);

	int TextHeight = TimeText(CenterRect, FImageBuffer->Canvas->Handle, TimeStr, false);
	OffsetRect(&CenterRect, (FImageRect.Width() - CenterRect.Width()) / 2, (FImageRect.Height() - TextHeight) / 2);
	TimeText(CenterRect, FImageBuffer->Canvas->Handle, TimeStr, true);

	CalculateScrollButtonPos();
	if (FButtonBuffer)
	{
		delete [] FButtonBuffer;
	}
	FButtonBuffer = new DWORD[GetMaxButtonSize()];

	PaintButton(FHourUpButton,			ScrollButtonsRect[HOUR_UP_BUTTON], FImageRect, FImageBuffer.get(), Color);
	PaintButton(FHourDownButton,		ScrollButtonsRect[HOUR_DOWN_BUTTON], FImageRect, FImageBuffer.get(), Color);
	PaintButton(FMinuteUpButton,		ScrollButtonsRect[MINUTE_UP_BUTTON], FImageRect, FImageBuffer.get(), Color);
	PaintButton(FMinuteDownButton,	ScrollButtonsRect[MINUTE_DOWN_BUTTON], FImageRect, FImageBuffer.get(), Color);

/*	for (int i=0; i<4; i++)
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
		InflateRect(&ArrowRect, -ArrowWidth/4, -ArrowHeight/4);
		if (	(i == HOUR_UP_BUTTON			&& FHourUpButton->Highlighted) ||
				(i == HOUR_DOWN_BUTTON		&& FHourDownButton->Highlighted) ||
				(i == MINUTE_UP_BUTTON		&& FMinuteUpButton->Highlighted) ||
				(i == MINUTE_DOWN_BUTTON	&& FMinuteDownButton->Highlighted))
		{
			OffsetRect(&ArrowRect, 1, 1);
		}

		TPoint points[3];

		if (i % 2)
		{
			points[0] = Point(ArrowRect.Left, ArrowRect.Top);
			points[1] = Point(ArrowRect.Right, ArrowRect.Top);
			points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Bottom);
		}
		else
		{
			points[0] = Point(ArrowRect.Left, ArrowRect.Bottom);
			points[1] = Point(ArrowRect.Right, ArrowRect.Bottom);
			points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Top);
		}
		FImageBuffer->Canvas->Pen->Color		= FArrowBorderColor;
		FImageBuffer->Canvas->Brush->Color	= FArrowColor;
		FImageBuffer->Canvas->Polygon(points, 2);
	} */
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
		if ((i==HOUR_UP_BUTTON		&& FHourUpButton->Caption		== "") ||
			 (i==HOUR_DOWN_BUTTON	&& FHourDownButton->Caption	== "") ||
			 (i==MINUTE_UP_BUTTON	&& FMinuteUpButton->Caption	== "") ||
			 (i==MINUTE_DOWN_BUTTON	&& FMinuteDownButton->Caption	== ""))
		{
			int BtnLeft			= ScrollButtonsRect[i].Left + ((ScrollButtonsRect[i].Width() - MinDimention) / 2);
			int BtnTop			= ScrollButtonsRect[i].Top + ((ScrollButtonsRect[i].Height() - MinDimention) / 2);
			int BtnWidth		= MinDimention;
			int BtnHeight		= MinDimention;

			int ArrowHeight;
			int ArrowWidth;
			int ArrowLeft;
			int ArrowTop;

			TRect ArrowRect;
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
			InflateRect(&ArrowRect, -(100-FArrowScale)*ArrowRect.Width()/200, -(100-FArrowScale)*ArrowRect.Height()/200);

			if (	(i == HOUR_UP_BUTTON			&& FHourUpButton->Highlighted) ||
					(i == HOUR_DOWN_BUTTON		&& FHourDownButton->Highlighted) ||
					(i == MINUTE_UP_BUTTON		&& FMinuteUpButton->Highlighted) ||
					(i == MINUTE_DOWN_BUTTON	&& FMinuteDownButton->Highlighted))
			{
				OffsetRect(&ArrowRect, 1, 1);
			}

			TPoint points[3];

			if (i % 2)
			{
				points[0] = Point(ArrowRect.Left, ArrowRect.Top);
				points[1] = Point(ArrowRect.Right, ArrowRect.Top);
				points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Bottom);
			}
			else
			{
				points[0] = Point(ArrowRect.Left, ArrowRect.Bottom);
				points[1] = Point(ArrowRect.Right, ArrowRect.Bottom);
				points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Top);
			}
			FImageBuffer->Canvas->Pen->Color		= FArrowBorderColor;
			FImageBuffer->Canvas->Brush->Color	= FArrowColor;
			FImageBuffer->Canvas->Polygon(points, 2);
		}
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
int TTouchTime::TimeText(TRect &Rect, HDC Handle, AnsiString Str, bool Draw)
{
	int Options = DT_CENTER | DT_NOPREFIX;
	if (!Draw)
	{
		Options |= DT_CALCRECT;
	}
	return DrawTextEx( Handle,
							 Str.c_str(),
							 Str.Length(),
							 &Rect,
							 Options,
							 NULL);
}
//---------------------------------------------------------------------------
void TTouchTime::CalculateScrollButtonPos()
{
	if (FAutoScale)
	{
		FButtonWidth = Width * FScrollButtonRatio;
	}
	ScrollButtonsRect[HOUR_UP_BUTTON].Left			= FImageRect.Left + ButtonGapWidth;
	ScrollButtonsRect[HOUR_UP_BUTTON].Top			= FImageRect.Top + ButtonGapHeight;
	ScrollButtonsRect[HOUR_UP_BUTTON].Right		= FImageRect.Left + FButtonWidth + ButtonGapWidth;
	ScrollButtonsRect[HOUR_UP_BUTTON].Bottom		= FImageRect.Top + ((FImageRect.Height() - ButtonGapHeight) / 2);

	ScrollButtonsRect[HOUR_DOWN_BUTTON].Left		= FImageRect.Left + ButtonGapWidth;
	ScrollButtonsRect[HOUR_DOWN_BUTTON].Top		= ScrollButtonsRect[HOUR_UP_BUTTON].Bottom + ButtonGapHeight;
	ScrollButtonsRect[HOUR_DOWN_BUTTON].Right		= FImageRect.Left + FButtonWidth + ButtonGapWidth;
	ScrollButtonsRect[HOUR_DOWN_BUTTON].Bottom	= FImageRect.Bottom - ButtonGapHeight;

	ScrollButtonsRect[MINUTE_UP_BUTTON].Left		= FImageRect.Right - ButtonGapWidth - FButtonWidth;
	ScrollButtonsRect[MINUTE_UP_BUTTON].Top		= FImageRect.Top + ButtonGapHeight;
	ScrollButtonsRect[MINUTE_UP_BUTTON].Right		= FImageRect.Right - ButtonGapWidth;
	ScrollButtonsRect[MINUTE_UP_BUTTON].Bottom	= FImageRect.Top + ((FImageRect.Height() - ButtonGapHeight) / 2);

	ScrollButtonsRect[MINUTE_DOWN_BUTTON].Left	= FImageRect.Right - ButtonGapWidth - FButtonWidth;
	ScrollButtonsRect[MINUTE_DOWN_BUTTON].Top		= ScrollButtonsRect[MINUTE_UP_BUTTON].Bottom + ButtonGapHeight;
	ScrollButtonsRect[MINUTE_DOWN_BUTTON].Right	= FImageRect.Right - ButtonGapWidth;
	ScrollButtonsRect[MINUTE_DOWN_BUTTON].Bottom	= FImageRect.Bottom - ButtonGapHeight;
}
//---------------------------------------------------------------------------
TGridButton *TTouchTime::GetScrollButton(int X, int Y)
{
	if (PtInRect(ScrollButtonsRect[HOUR_UP_BUTTON], TPoint(X, Y)))
	{
		return FHourUpButton;
	} else
	if (PtInRect(ScrollButtonsRect[HOUR_DOWN_BUTTON], TPoint(X, Y)))
	{
		return FHourDownButton;
	} else
	if (PtInRect(ScrollButtonsRect[MINUTE_UP_BUTTON], TPoint(X, Y)))
	{
		return FMinuteUpButton;
	} else
	if (PtInRect(ScrollButtonsRect[MINUTE_DOWN_BUTTON], TPoint(X, Y)))
	{
		return FMinuteDownButton;
	}
	return NULL;
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetGridBorderWidth(int GridBorderWidth)
{
	FGridBorderWidth	= GridBorderWidth;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetBtnGapWidth(int Width)
{
	FBtnGapWidth = Width;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::SetBtnGapHeight(int Height)
{
	FBtnGapHeight = Height;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::MouseDown(TMouseButton Button, Classes::
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
			FScrollTimer->Interval = FInitialDelay;
			FScrollTimer->Enabled = true;
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
void __fastcall TTouchTime::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	FScrollTimer->Enabled = false;
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
		if (FOnTimeClick)
		{
			FOnTimeClick(this);
		}
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::MouseMove(Classes::TShiftState Shift, int X, int Y)
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
		FScroll = IsCurrentButton;

		if (FScroll && FCurrentScrollButton)
		{
			SetButtonDown(FCurrentScrollButton);
			FScrollTimer->Enabled = true;
			Invalidate();
		}
	}
	else
	{
		TCustomTouchButtonControl::MouseMove(Shift, X, Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchTime::WMCancelMode(TWMCancelMode& Message)
{
	FScrollTimer->Enabled = false;
	TCustomTouchButtonControl::WMCancelMode(Message);
	if (FCurrentClickedScrollButton)
	{
		SetButtonUp(FCurrentClickedScrollButton);
		FCurrentClickedScrollButton = NULL;
		Invalidate();
		FScroll					= false;
		FCurrentScrollButton	= NULL;
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void TTouchTime::ApplyScroll()
{
	Word Hour, Min, Sec, MSec;
	DecodeTime(FTime, Hour, Min, Sec, MSec);

	if (FCurrentClickedScrollButton == FHourUpButton)
	{
		FScroll					= true;
		FCurrentScrollButton	= FHourUpButton;

		if (++Hour >= 24) Hour = 0;
	}
	else if (FCurrentClickedScrollButton == FHourDownButton)
	{
		FScroll					= true;
		FCurrentScrollButton	= FHourDownButton;
		if (short(--Hour) < 0) Hour = 23;
	}
	else if (FCurrentClickedScrollButton == FMinuteUpButton)
	{
		FScroll					= true;
		FCurrentScrollButton	= FMinuteUpButton;
		if (++Min >= 59) Min = 0;
	}
	else if (FCurrentClickedScrollButton == FMinuteDownButton)
	{
		FScroll					= true;
		FCurrentScrollButton	= FMinuteDownButton;
		if (short(--Min) < 0) Min = 59;
	}
	FTime = EncodeTime(Hour, Min, Sec, MSec);
	if (FOnChange)
	{
		FOnChange(this);
	}
	Invalidate();
}
//---------------------------------------------------------------------------

