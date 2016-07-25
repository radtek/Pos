//---------------------------------------------------------------------------
#pragma hdrstop

#include "TouchBtn.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TTouchBtn *)
{
	new TTouchBtn(NULL);
}             
//---------------------------------------------------------------------------
__fastcall TTouchBtn::TTouchBtn(TComponent* Owner) : TCustomTouchButtonControl(Owner)
{
	Width			  				= 185;
	Height						= 57;

	ParentFont					= true;
	ParentColor					= true;

	FButtonBuffer				= new DWORD[Width * Height];

	TGridButton *Button		= new TGridButton(this);
	Button->Color				= FDefaultBtnColor;
	Button->DisabledColor	= FDefaultBtnColor;
	Button->LatchedColor		= FDefaultBtnLatchedColor;
	Button->Font->Assign(Font);
	Button->FontColor			= Font->Color;
	Button->ParentFont		= true;
	Button->ParentFontColor	= true;
	FButtons.push_back(Button);

	FArrowType					= atNone;
	FArrowColor					= clBlack;
	FArrowBorderColor			= clBlack;
	FArrowScale					= 30;

   FInitialDelay				= 500;
   FRepeatDelay				= 50;
   FAccelSteps					= 1;
	FAutoRepeatCount			= 1;	// Start at 1 cos the first 1 has already gone with the first push.
	FBtnOnAutoRepeat			= NULL;

	FOnMouseClick				= BtnMouseClick;
	FOnMouseDown				= BtnMouseDown;
	FOnMouseUp					= BtnMouseUp;
	FOnMouseMove				= BtnMouseMove;
}
//---------------------------------------------------------------------------
TGridButton *TTouchBtn::GetButtonXY(int X, int Y)
{
	if (PtInRect(&GetButtonRect(FButtons[0]), TPoint(X, Y)))
	{
		return FButtons[0];
	}
	else
	{
		return NULL;
	}
}
//---------------------------------------------------------------------------
TRect TTouchBtn::GetButtonRect(const TGridButton *Button)
{
	return TRect(BorderWidth, BorderWidth, Width - BorderWidth, Height - BorderWidth);
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::WMSize(TWMSize& Message)
{
	TCustomPanel::Dispatch(&Message);
	
	delete [] FButtonBuffer;
	FButtonBuffer = new DWORD[Width * Height];
}
//---------------------------------------------------------------------------
UnicodeString __fastcall TTouchBtn::GetCaption()
{
	return FButtons[0]->Caption;
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetCaption(UnicodeString Caption)
{
	FButtons[0]->Caption = Caption;
	Invalidate();
}
//---------------------------------------------------------------------------
bool __fastcall TTouchBtn::GetEnabled()
{
	return FButtons[0]->Enabled;
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetEnabled(bool Value)
{
	FButtons[0]->Enabled = Value;
	if (FTimer)
   {
   	FTimer->Enabled = false;
      delete FTimer;
      FTimer = NULL;
	}
	TCustomTouchButtonControl::Enabled = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TTouchBtn::GetBtnColor()
{
	return FButtons[0]->Color;
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetBtnColor(TColor Color)
{
	FButtons[0]->Color = Color;
	Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TTouchBtn::GetBtnLatchedColor()
{
	return FButtons[0]->LatchedColor;
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetBtnLatchedColor(TColor Color)
{
	FButtons[0]->LatchedColor = Color;
	Invalidate();
}
//---------------------------------------------------------------------------
bool __fastcall TTouchBtn::GetHighlighted()
{
	return FButtons[0]->Highlighted;
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetHighlighted(bool Highlighted)
{
	FButtons[0]->Highlighted = Highlighted;
	Invalidate();
}
//---------------------------------------------------------------------------
bool __fastcall TTouchBtn::GetLatched()
{
	return FButtons[0]->Latched;
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetLatched(bool Latched)
{
	FButtons[0]->Latched = Latched;
	Invalidate();
}
//---------------------------------------------------------------------------
TColor __fastcall TTouchBtn::GetDisabledBtnColor()
{
	return FButtons[0]->DisabledColor;
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetDisabledBtnColor(TColor Color)
{
	FButtons[0]->DisabledColor = Color;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetArrowType(Touchbtn::TArrowType Value)
{
	FArrowType = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetArrowColor(TColor Value)
{
	FArrowColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetArrowBorderColor(TColor Value)
{
	FArrowBorderColor = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::SetArrowScale(int Value)
{
	if (Value < 0 || Value > 100)
	{
		throw Exception("Scale must be between 0 and 100");
	}
	FArrowScale = Value;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::Paint(void)
{
	TRect FullRect								= GetClientRect();
	FImageBuffer->Width						= FullRect.Width();
	FImageBuffer->Height						= FullRect.Height();
	FImageBuffer->Canvas->Brush->Color	= Color;
	FImageBuffer->Canvas->FillRect(FullRect);

	TRect Rect = GetButtonRect(FButtons[0]);

	PaintButton(FButtons[0], Rect, Rect, FImageBuffer.get(), Color);

	if (BevelOuter != bvNone)
	{
		if (BevelOuter == bvLowered)
		{
			Frame3D(FImageBuffer->Canvas, FullRect, clBtnShadow, clBtnHighlight, BevelWidth);
		}
		else
		{
			Frame3D(FImageBuffer->Canvas, FullRect, clBtnHighlight, clBtnShadow, BevelWidth);
		}
	}
	if (BevelInner != bvNone)
	{
		Frame3D(FImageBuffer->Canvas, FullRect, Color, Color, BevelWidth);
		if (BevelOuter == bvLowered)
		{
			Frame3D(FImageBuffer->Canvas, FullRect, clBtnHighlight, clBtnShadow, BevelWidth);
		}
		else
		{
			Frame3D(FImageBuffer->Canvas, FullRect, clBtnShadow, clBtnHighlight, BevelWidth);
		}
	}
	if (FArrowType != atNone)
	{
		int MinDimention = 0;
		if (Rect.Width() > 0 && Rect.Height())
		{
			MinDimention = Rect.Height();
		}
		else
		{
			MinDimention = Rect.Width();
		}
		int BtnLeft			= Rect.Left + ((Rect.Width() - MinDimention) / 2);
		int BtnTop			= Rect.Top + ((Rect.Height() - MinDimention) / 2);
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

		if (FButtons[0]->Highlighted)
		{
			OffsetRect(&ArrowRect, 1, 1);
		}

		TPoint points[3];

		if (FArrowType == atDown)
		{
			points[0] = Point(ArrowRect.Left, ArrowRect.Top);
			points[1] = Point(ArrowRect.Right, ArrowRect.Top);
			points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Bottom);
		}
		else if (FArrowType == atUp)
		{
			points[0] = Point(ArrowRect.Left, ArrowRect.Bottom);
			points[1] = Point(ArrowRect.Right, ArrowRect.Bottom);
			points[2] = Point((ArrowRect.Left + ArrowRect.Right) / 2, ArrowRect.Top);
		}
		else if (FArrowType == atRight)
		{
			points[0] = Point(ArrowRect.Left, ArrowRect.Top);
			points[1] = Point(ArrowRect.Right, (ArrowRect.Top + ArrowRect.Bottom) / 2);
			points[2] = Point(ArrowRect.Left, ArrowRect.Bottom);
		}
		else if (FArrowType == atLeft)
		{
			points[0] = Point(ArrowRect.Right, ArrowRect.Top);
			points[1] = Point(ArrowRect.Left, (ArrowRect.Top + ArrowRect.Bottom) / 2);
			points[2] = Point(ArrowRect.Right, ArrowRect.Bottom);
		}
		FImageBuffer->Canvas->Pen->Color		= FArrowBorderColor;
		FImageBuffer->Canvas->Brush->Color	= FArrowColor;
		FImageBuffer->Canvas->Polygon(points, 2);
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
void __fastcall TTouchBtn::BtnMouseClick(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton)
{
	if (FBtnOnMouseClick)
	{
		FBtnOnMouseClick(this);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::BtnMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton, int X, int Y)
{
	if (FTimer)
	{
		FTimer->Enabled = false;
		delete FTimer;
		FTimer = NULL;
	}
	if (FBtnOnMouseDown)
	{
		FBtnOnMouseDown(this, Button, Shift, X, Y);
	}
	if (Button == mbLeft && FBtnOnAutoRepeat)
	{
		FBtnOnAutoRepeat(this);
		
		FTimer				= new TTimer(this);
		FTimer->Enabled	= false;
		FTimer->Interval	= FInitialDelay;
		FTimer->OnTimer	= FTimerAutoRepeat;
		FAutoRepeatCount	= 1;
		FTimer->Enabled	= true;
   }
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::BtnMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, TGridButton *GridButton, int X, int Y)
{
	if (FTimer)
   {
		FTimer->Enabled = false;
		delete FTimer;
		FTimer = NULL;
	}
	if (FBtnOnMouseUp)
	{
		FBtnOnMouseUp(this, Button, Shift, X, Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::BtnMouseMove(TObject *Sender, TShiftState Shift, TGridButton *GridButton, int X, int Y)
{
	if (FBtnOnMouseMove)
	{
		FBtnOnMouseMove(this, Shift, X, Y);
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchBtn::FTimerAutoRepeat(TObject *Sender)
{
	if (FButtons[0]->Highlighted)
	{
		FTimer->Enabled = false;
		if (FAutoRepeatCount < FAccelSteps)
		{
			FTimer->Interval = FInitialDelay - ((FInitialDelay - FRepeatDelay) * FAutoRepeatCount / FAccelSteps);
			++FAutoRepeatCount;
		}
		else
		{
			FTimer->Interval = FRepeatDelay;
		}
		FBtnOnAutoRepeat(this);
		if (FTimer)
		{
			FTimer->Enabled = true;
		}
	}
}
//---------------------------------------------------------------------------
