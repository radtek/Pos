//---------------------------------------------------------------------------
#pragma hdrstop

#include "TouchNumpad.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TTouchNumpad *)
{
	new TTouchNumpad(NULL);
}
//---------------------------------------------------------------------------
__fastcall TTouchNumpad::TTouchNumpad(TComponent* Owner) : TCustomTouchButtonControl(Owner)
{
	for (int i=0; i<12; i++)
	{
		FButtons.push_back(new TGridButton(this));
	}
	FNumpadDisplay		= NULL;
	FCustomKeyCaption	= "00";
	SetCaptions();
	Width					= 201;
	Height				= 201;
//	BevelOuter			= bvLowered;

	BorderStyle			= bsSingle;
	FGridBorderWidth	= 8;
	FBtnGapWidth		= 2;
	FBtnGapHeight		= 2;
	FButtonColor		= FButtons[0]->Color;
	FNumberingType		= ntKeypadNumeric;
}
//---------------------------------------------------------------------------
void TTouchNumpad::SetCaptions()
{
	const AnsiString *KeyText = NULL;

	if (FNumberingType == ntKeypadNumeric)          KeyText = KeypadNumericKeyText;
	else if (FNumberingType == ntTelephoneNumeric)	KeyText = TelephoneNumericKeyText;

	for (int i=0; i<10; i++)
	{
		FButtons[i]->Caption = KeyText[i];
	}
	if (FCustomKey)
	{
		FButtons[10]->Caption = FCustomKeyCaption;
		FButtons[11]->Caption = "Clear";
	}
	else
	{
		FButtons[10]->Caption = "Clear";
	}
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::SetCustomKeyCaption(AnsiString Caption)
{
	FCustomKeyCaption = Caption;
	SetCaptions();
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::SetCustomKey(bool CustomKey)
{
	FCustomKey = CustomKey;
	SetCaptions();
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::Paint(void)
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

//	if (BevelOuter != bvNone)
//	if (BorderStyle == bsSingle)
//	{
//		DrawEdge(FImageBuffer->Canvas->Handle, &ImageRect, EDGE_SUNKEN, BF_ADJUST + BF_RECT);
//	}

	unsigned BtnCount = FButtons.size();
	if (!FCustomKey)//FNumberingType == ntKeypadNumeric || FNumberingType == ntTelephoneNumeric)
	{
		BtnCount -= 1;
	}
	for (unsigned i=0; i<BtnCount; i++)
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
TGridButton *TTouchNumpad::GetButtonXY(int X, int Y)
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
TRect TTouchNumpad::GetButtonRect(const TGridButton *Button)
{
	std::vector<TGridButton *>::iterator iButton = std::find(FButtons.begin(), FButtons.end(), Button);
	if (iButton != FButtons.end())
	{
		int ButtonIndex	= iButton - FButtons.begin();
		int RowIndex		= -1;
		int Top				= 0;
		int ButtonsHeight	= ClientHeight - FGridBorderWidth - FGridBorderWidth + FBtnGapHeight;
		if (ButtonIndex < 3)
		{
			RowIndex	= ButtonIndex;
			Top		= FGridBorderWidth;
		}
		else if (ButtonIndex < 6)
		{
			RowIndex	= ButtonIndex - 3;
			Top		= ButtonsHeight / 4 + FGridBorderWidth;
		}
		else if (ButtonIndex < 9)
		{
			RowIndex	= ButtonIndex - 6;
			Top		= ButtonsHeight * 2 / 4 + FGridBorderWidth;
		}
		else if (ButtonIndex < 12)
		{
			RowIndex	= ButtonIndex - 9;
			Top		= ButtonsHeight * 3 / 4 + FGridBorderWidth;
		}
		if (RowIndex > -1)
		{
			int WidthToGo = ClientWidth - FGridBorderWidth - FGridBorderWidth + FBtnGapWidth;
			int TotalFraction = 3;

			int Left = FGridBorderWidth;
			for (int i=0; i<RowIndex; i++)
			{
				int BtnWidth = min(WidthToGo / TotalFraction, ClientWidth-1);
				Left += BtnWidth;
				WidthToGo -= BtnWidth;
				TotalFraction --;
			}
			if (!FCustomKey && ButtonIndex == 10)
			{
				int Right	= min(Left + WidthToGo  - FBtnGapWidth, ClientWidth-1);
				int Bottom	= min(Top + (ButtonsHeight / 4) - FBtnGapHeight, ClientHeight-1);
				return TRect(Left, Top, Right, Bottom);
			}
			else
			{
				int Right	= min(Left + (WidthToGo / TotalFraction) - FBtnGapWidth, ClientWidth-1);
				int Bottom	= min(Top + (ButtonsHeight / 4) - FBtnGapHeight, ClientHeight-1);
				return TRect(Left, Top, Right, Bottom);
			}
		}
	}
	return TRect(0,0,0,0);
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::SetNumberingType(TNumberingType Value)
{
	FNumberingType = Value;
	SetCaptions();
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::SetGridBorderWidth(int GridBorderWidth)
{
	FGridBorderWidth	= GridBorderWidth;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::SetBtnGapWidth(int Width)
{
	FBtnGapWidth = Width;
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::SetBtnGapHeight(int Height)
{
	FBtnGapHeight = Height;
	Invalidate();
}
//---------------------------------------------------------------------------
void	__fastcall TTouchNumpad::SetButtonColor(TColor Value)
{
	FButtonColor = Value;
	for (unsigned i=0; i<FButtons.size(); i++)
	{
		FButtons[i]->Color = FButtonColor;
	}
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::MouseUp(TMouseButton Button, Classes::
	TShiftState Shift, int X, int Y)
{
	TGridButton *GridButton = GetButtonXY(X, Y);
	if (GridButton)
	{
		if (FOnClick || FNumpadDisplay)
		{
			std::vector<TGridButton *>::iterator iButton = std::find(FButtons.begin(), FButtons.end(), GridButton);
			if (iButton != FButtons.end())
			{
				int ButtonIndex = iButton - FButtons.begin();

				TNumpadKey NumpadKey;
				if (ButtonIndex < 10)
				{
					NumpadKey = static_cast<TNumpadKey>(StrToInt(GridButton->Caption));
				}
				else if (FCustomKey)
				{
					if (ButtonIndex == 10)
					{
						NumpadKey = nkCustom;
					}
					else if (ButtonIndex == 11)
					{
						NumpadKey = nkClear;
					}
				}
				else if (ButtonIndex == 10)
				{
					NumpadKey = nkClear;
				}
				else
				{
					throw Exception("Invalid button");
				}
				if (FNumpadDisplay)
				{
					FNumpadDisplay->KeyPressed(NumpadKey);
				}
				if (FOnClick)
				{
					FOnClick(this, NumpadKey);
				}
			}
		}
	}
	TCustomTouchButtonControl::MouseUp(Button, Shift, X, Y);
}
//---------------------------------------------------------------------------
void __fastcall TTouchNumpad::SetNumpadDisplay(TNumpadDisplay *NumpadDisplay)
{
	FNumpadDisplay = NumpadDisplay;
	if (FNumpadDisplay)
	{
		FNumpadDisplay->SetTouchNumpad(this);
	}
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
static inline void ValidCtrCheck(TNumpadDisplay *)
{
	new TNumpadDisplay(NULL);
}
//---------------------------------------------------------------------------
__fastcall TNumpadDisplay::TNumpadDisplay(TComponent* Owner) : TCustomPanel(Owner)
{
	BevelOuter			= bvNone;
	BorderStyle			= bsSingle;
	FText					= "000";
	FInitialText		= false;
	FTouchNumpad		= NULL;
	FOnGetNumericText	= NULL;
	FOnGetTimeText		= NULL;
	FDecimalPlaces		= 2;
}
//---------------------------------------------------------------------------
void __fastcall TNumpadDisplay::SetDecimalPlaces(int DecimalPlaces)
{
	if (NumpadDisplayMode == dmPIN)
	{
		if (DecimalPlaces <= 0)
		{
			throw Exception("DecimalPlaces must be greater then 0");
		}
	}
	else if (DecimalPlaces < 0 || DecimalPlaces > 4)
	{
		throw Exception("DecimalPlaces must be between 0 and 4");
	}
	if (FDecimalPlaces != DecimalPlaces)
	{
		FDecimalPlaces = DecimalPlaces;
		SetNumpadDisplayMode(FNumpadDisplayMode);
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumpadDisplay::SetNumpadDisplayMode(TNumpadDisplayMode Mode)
{
	FText = "";
	FNumpadDisplayMode = Mode;
	if (FTouchNumpad)
	{
		switch (Mode)
		{
			case dmCurrency:
			{
				FTouchNumpad->CustomKey				= true;
				FTouchNumpad->CustomKeyCaption	= "00";
				if (DecimalPlaces > 4)
				{
					DecimalPlaces = 4;
				}
				break;
			}
			case dmNumeric:
			{
				if (DecimalPlaces > 4)
				{
					DecimalPlaces = 4;
				}
				if (FDecimalPlaces != 0)
				{
					FTouchNumpad->CustomKey				= true;
					FTouchNumpad->CustomKeyCaption	= ".";
				}
				else
				{
					FTouchNumpad->CustomKey				= false;
				}
				break;
			}
			case dm24HrTime:
			{
				FTouchNumpad->CustomKey				= false;
				break;
			}
			case dm12HrTime:
			{
				FTouchNumpad->CustomKey				= true;
				FTouchNumpad->CustomKeyCaption	= "PM";
				break;
			}
			case dmPIN:
			{
				FTouchNumpad->CustomKey				= false;
				break;
			}
		}
	}
	KeyPressed(nkClear);
}
//---------------------------------------------------------------------------
void TNumpadDisplay::AdjustColors(TPanelBevel Bevel, TColor &TopColor, TColor &BottomColor)
{
	TopColor = clBtnHighlight;
	if (Bevel == bvLowered)
	{
		TopColor = clBtnShadow;
	}
	BottomColor = clBtnShadow;
	if (Bevel == bvLowered)
	{
		BottomColor = clBtnHighlight;
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumpadDisplay::Paint()
{
	TRect Rect = GetClientRect();
	TColor TopColor, BottomColor;

	if (BevelOuter != bvNone)
	{
		AdjustColors(BevelOuter, TopColor, BottomColor);
		Frame3D(Canvas, Rect, TopColor, BottomColor, BevelWidth);
	}
	Frame3D(Canvas, Rect, Color, Color, BorderWidth);
	if (BevelInner != bvNone)
	{
		AdjustColors(BevelInner, TopColor, BottomColor);
		Frame3D(Canvas, Rect, TopColor, BottomColor, BevelWidth);
	}
	Canvas->Brush->Color = Color;
	Canvas->FillRect(Rect);
	Canvas->Brush->Style = bsClear;
	Canvas->Font = Font;
	int FontHeight = Canvas->TextHeight("W");
	Rect.Top = ((Rect.Bottom + Rect.Top) - FontHeight) / 2;
	Rect.Bottom = Rect.Top + FontHeight;
	DrawText(Canvas->Handle, GetText().c_str(), -1, &Rect, DT_CENTER | DT_NOPREFIX);
}
//---------------------------------------------------------------------------
void TNumpadDisplay::SetTouchNumpad(TTouchNumpad *TouchNumpad)
{
	FTouchNumpad = TouchNumpad;
	if (FTouchNumpad)
	{
		SetNumpadDisplayMode(FNumpadDisplayMode);
	}
}
//---------------------------------------------------------------------------
void TNumpadDisplay::KeyPressed(TNumpadKey Key)
{
	if (FInitialText)
	{
		FInitialText = false;
		if (Key != nkClear)
		{
			KeyPressed(nkClear);
		}
	}
	switch (NumpadDisplayMode)
	{
		case dmCurrency:	UpdateCurrency(Key);	break;
		case dmNumeric:	UpdateNumeric(Key);	break;
		case dm12HrTime:	Update12HrTime(Key);	break;
		case dm24HrTime:	Update24HrTime(Key);	break;
		case dmPIN:			UpdatePIN(Key);	break;
	}
}
//---------------------------------------------------------------------------
AnsiString __fastcall TNumpadDisplay::GetText()
{
	AnsiString DisplayText;

	switch (NumpadDisplayMode)
	{
		case dmCurrency:
		{
			DisplayText = GetCurrencyText();
			if (FOnGetNumericText)
			{
				FOnGetNumericText(this, Numeric(), DisplayText);
			}
			break;
		}
		case dmNumeric:
		{
			DisplayText = GetNumericText();
			if (FOnGetNumericText)
			{
				FOnGetNumericText(this, Numeric(), DisplayText);
			}
			break;
		}
		case dm12HrTime:
		{
			DisplayText = Get12HrTimeText();
			if (FOnGetTimeText)
			{
				FOnGetTimeText(this, Time(), DisplayText);
			}
			break;
		}
		case dm24HrTime:
		{
			DisplayText = Get24HrTimeText();
			if (FOnGetTimeText)
			{
				FOnGetTimeText(this, Time(), DisplayText);
			}
			break;
		}
		case dmPIN:
		{
			DisplayText = GetPINText();
			if (FOnGetNumericText)
			{
				FOnGetNumericText(this, PIN(), DisplayText);
			}
			break;
		}
	}
	return DisplayText;
}
//---------------------------------------------------------------------------
AnsiString TNumpadDisplay::GetCurrencyText()
{
	int Pos = 1;
	while (FText[1] == '0' && Pos < FText.Length() - FDecimalPlaces)
	{
		FText.Delete(1, 1);
	}
	if (FDecimalPlaces == 0)
	{
		return FText;
	}
	else
	{
		return FText.SubString(1, FText.Length() - FDecimalPlaces) + "." +  
			FText.SubString(FText.Length() - FDecimalPlaces + 1, FDecimalPlaces);
	}
}
//---------------------------------------------------------------------------
void TNumpadDisplay::UpdateCurrency(TNumpadKey Key)
{
	switch (Key)
	{
		case nkCustom:
		{
			FText = FText + "00";
			break;
		}
		case nkClear:
		{
			FText = AnsiString::StringOfChar('0', FDecimalPlaces + 1);
			break;
		}
		default:
		{
			FText = FText + IntToStr(static_cast<int>(Key));
		}
	}
	Invalidate();
}
//---------------------------------------------------------------------------
AnsiString TNumpadDisplay::GetNumericText()
{
	// No leading '0's
	if (FText != "0" && FText != "0.")
	{
		int Pos = 1;
		while (FText[1] == '0' && Pos < FText.Length() - 2)
		{
			FText.Delete(1, 1);
		}
	}
	// Only 4dp
	int PointPos = FText.AnsiPos(".");
	if (PointPos > 0 && PointPos < FText.Length() - 4)
	{
		FText.SetLength(PointPos + 4);
	}
	// No trailng '.' (Only adjust display, not raw data)
	AnsiString DispalyText = FText;
	if (DispalyText[FText.Length()] == '.')
	{
		DispalyText.SetLength(FText.Length() - 1);
	}

	return FText;
}
//---------------------------------------------------------------------------
void TNumpadDisplay::UpdateNumeric(TNumpadKey Key)
{
	switch (Key)
	{
		case nkCustom:
		{
			if (FText.AnsiPos(".") == 0)
			{
				FText = FText + ".";
			}
			break;
		}
		case nkClear:
		{
			FText = "0";
			break;
		}
		default:
		{
			int KeyNumber = static_cast<int>(Key);
			if (KeyNumber == 0 && FText == "0")
			{
				return;
			}
			else if (FText == "0")
			{
				FText = IntToStr(KeyNumber);
			}
			else
			{
				int PointPos = FText.AnsiPos(".");
				if (PointPos > 0)
				{
					if (FText.Length() - PointPos >= FDecimalPlaces)
					{
						return;
					}
				}
				FText = FText + IntToStr(KeyNumber);
			}
		}
	}
	Invalidate();
}
//---------------------------------------------------------------------------
AnsiString TNumpadDisplay::Get12HrTimeText()
{
	AnsiString Output = "  :   ";
	switch (FText.Length())
	{
		case 0:	FText = "A";																					break;
		case 1:	break;
		case 2:	Output = AnsiString(&FText[2], 1) + " :  ";											break;
		case 3:	Output = AnsiString(&FText[2], 2) + ":  ";											break;
		case 4:	Output = AnsiString(&FText[2], 2) + ":" + AnsiString(&FText[4], 1) + " ";	break;
		case 5:	Output = AnsiString(&FText[2], 2) + ":" + AnsiString(&FText[4], 2);			break;
	}
	if (FText[1] == 'A')
	{
		Output += " AM";
	}
	else
	{
		Output += " PM";
	}
	return Output;
}
//---------------------------------------------------------------------------
void TNumpadDisplay::Update12HrTime(TNumpadKey Key)
{
	switch (Key)
	{
		case nkCustom:
		{
			if (FText[1] == 'A')
			{
				FText[1] = 'P';
				if (FTouchNumpad) FTouchNumpad->CustomKeyCaption = "AM";
			}
			else
			{
				FText[1] = 'A';
				if (FTouchNumpad) FTouchNumpad->CustomKeyCaption = "PM";
			}
			break;
		}
		case nkClear:
		{
			FText = "A";
			if (FTouchNumpad) FTouchNumpad->CustomKeyCaption = "PM";
			break;
		}
		default:
		{
			if (FText.Length() == 5)
			{
				break;
			}
			int KeyNumber = static_cast<int>(Key);

			if (FText.Length() == 0)
			{
				FText = "A";
				if (FTouchNumpad) FTouchNumpad->CustomKeyCaption = "PM";
			}
			if (FText.Length() == 1 && KeyNumber >= 2)
			{
				FText = FText + "0";
			}
			else if (FText.Length() == 2 && KeyNumber == 0 && FText[2] == '0')
			{
				break;
			}
			else if (FText.Length() == 2 && KeyNumber >= 3 && FText[2] == '1')
			{
				break;
			}
			else if (FText.Length() == 3 && KeyNumber >= 6)
			{
				break;
			}
			FText = FText + IntToStr(KeyNumber);
		}
	}
	Invalidate();
}
//---------------------------------------------------------------------------
AnsiString TNumpadDisplay::Get24HrTimeText()
{
	AnsiString Output = "  :  ";
	switch (FText.Length())
	{
		case 1:	Output = AnsiString(&FText[1], 1) + " :  ";											break;
		case 2:	Output = AnsiString(&FText[1], 2) + ":  ";											break;
		case 3:	Output = AnsiString(&FText[1], 2) + ":" + AnsiString(&FText[3], 1) + " ";	break;
		case 4:	Output = AnsiString(&FText[1], 2) + ":" + AnsiString(&FText[3], 2);			break;
	}
	return Output;
}
//---------------------------------------------------------------------------
void TNumpadDisplay::Update24HrTime(TNumpadKey Key)
{
	switch (Key)
	{
		case nkCustom:
		{
			break;
		}
		case nkClear:
		{
			FText = "";
			break;
		}
		default:
		{
			if (FText.Length() == 4)
			{
				break;
			}
			int KeyNumber = static_cast<int>(Key);

			if (FText.Length() == 0 && KeyNumber > 2)
			{
				break;
			}
			else if (FText.Length() == 1 && KeyNumber >= 4 && FText[1] == '2')
			{
				break;
			}
			else if (FText.Length() == 2 && KeyNumber >= 6)
			{
				break;
			}
			FText = FText + IntToStr(KeyNumber);
		}
	}
	Invalidate();
}
//---------------------------------------------------------------------------
AnsiString TNumpadDisplay::GetPINText()
{
	if (FInitialText)
	{
		return FText;
	}
	else
	{
		return AnsiString::StringOfChar('*', FText.Length());
	}
}
//---------------------------------------------------------------------------
void TNumpadDisplay::UpdatePIN(TNumpadKey Key)
{
	switch (Key)
	{
		case nkCustom:
		{
			break;
		}
		case nkClear:
		{
			FText = "";
			break;
		}
		default:
		{
			if (FText.Length() >= FDecimalPlaces)
			{
				break;
			}
			FText = FText + IntToStr(static_cast<int>(Key));
		}
	}
	Invalidate();
}
//---------------------------------------------------------------------------
TTime TNumpadDisplay::Time()
{
	if (FNumpadDisplayMode == dm12HrTime)
	{
		AnsiString timeText = FText;
		Word Hour, Min;
		if (timeText.Length() != 5)
		{
			timeText = timeText + AnsiString::StringOfChar('0', 5 - timeText.Length());
		}
		Hour = StrToInt(AnsiString(&timeText[2], 2));
		if (timeText[1] == 'P')
		{
			if (Hour != 12)
			{
				Hour += 12;
			}
		}
		else if (Hour == 12)
		{
			Hour = 0;
		}
		Min = StrToInt(AnsiString(&timeText[4], 2));
		return EncodeTime(Hour, Min, 0, 0);
	}
	else if (FNumpadDisplayMode == dm24HrTime)
	{
		AnsiString timeText = FText;
		Word Hour, Min;
		if (timeText.Length() != 4)
		{
			timeText = timeText + AnsiString::StringOfChar('0', 4 - timeText.Length());
		}
		Hour = StrToInt(AnsiString(&timeText[1], 2));
		Min = StrToInt(AnsiString(&timeText[3], 2));            
		return EncodeTime(Hour, Min, 0, 0);
	}
	else
	{
		throw Exception("Not in time mode.");
	}
}
//---------------------------------------------------------------------------
Currency TNumpadDisplay::Numeric()
{
	if (NumpadDisplayMode == dmCurrency)
	{
		return StrToCurr(GetCurrencyText());
	}
	else if (NumpadDisplayMode == dmNumeric)
	{
		return StrToCurr(GetNumericText());
	}
	else
	{
		throw Exception("Not in numeric mode.");
	}
}
//---------------------------------------------------------------------------
TPIN TNumpadDisplay::PIN()
{
	if (NumpadDisplayMode == dmPIN)
	{
		return FText;
	}
	else
	{
		throw Exception("Not in PIN mode.");
	}
}
//---------------------------------------------------------------------------
void TNumpadDisplay::SetTime(TTime Time)
{
	if (FNumpadDisplayMode != dm12HrTime && FNumpadDisplayMode != dm24HrTime)
	{
		throw Exception("Not in time mode.");
	}
	unsigned short Hour, Min, Sec, Msec;
	DecodeTime(Time, Hour, Min, Sec, Msec);
	bool AM = (Hour < 12);

	FText = "";
	if (FNumpadDisplayMode == dm12HrTime)
	{
		if (Hour > 12)
		{
			Hour -= 12;
		}
		if (Hour == 0)
		{
			Hour = 12;
		}
		if (AM) FText = "A"; else FText = "P";
	}
	if (Hour >= 10)
	{
		FText = FText + IntToStr(Hour);
	}
	else
	{
		FText = FText + "0" + IntToStr(Hour);
	}
	FText = FText + IntToStr(Min);
	FInitialText = true;
	Invalidate();
}
//---------------------------------------------------------------------------
void TNumpadDisplay::SetNumeric(TNumeric Numeric)
{
	int Whole = int(Numeric);
	int Part = (Numeric - Whole) * 10000;
	if (Part < 0) Part = -Part;
	AnsiString PartStr = IntToStr(Part);
	if (PartStr.Length() < 4)
	{
		PartStr = AnsiString::StringOfChar('0', 4 - PartStr.Length()) + PartStr;
	}
	if (PartStr.Length() > FDecimalPlaces)
	{
		PartStr.SetLength(FDecimalPlaces);
	}

	switch (FNumpadDisplayMode)
	{
		case dmCurrency:
		{
			if (PartStr.Length() < FDecimalPlaces)
			{
				PartStr = PartStr + AnsiString::StringOfChar('0', FDecimalPlaces - PartStr.Length());
			}
			FText = IntToStr(Whole) + PartStr;
			break;
		}
		case dmNumeric:
		{
			if (FDecimalPlaces == 0)
			{
				FText = IntToStr(Whole);
			}
			else
			{
				FText = IntToStr(Whole) + "." + PartStr;
			}
			break;
		}
		default:
		{
			throw Exception("Not in numeric mode.");
		}
	}
	FInitialText = true;
	Invalidate();
}
//---------------------------------------------------------------------------
void TNumpadDisplay::SetPIN(TPIN PIN)
{
	switch (FNumpadDisplayMode)
	{
		case dmPIN:
		{
			FText = PIN;
			break;
		}
		default:
		{
			throw Exception("Not in numeric mode.");
		}
	}
	FInitialText = true;
	Invalidate();
}
//---------------------------------------------------------------------------
void TNumpadDisplay::Clear()
{
	KeyPressed(nkClear);
}
//---------------------------------------------------------------------------

