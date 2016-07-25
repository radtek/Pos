//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <Math.hpp>
#include "NumericEdit.h"
#include "MM_Math.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "NumericEdit.dcr"
//---------------------------------------------------------------------------
// ValidCtrCheck is used to assure that the components created do not have
// any pure virtual functions.
//
static inline void ValidCtrCheck(TNumericEdit *)
{
	new TNumericEdit(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::DefineProperties(TFiler *Filer)
{
	TComponent::DefineProperties(Filer);
	Filer->DefineProperty("DecimalPlaces", ReadProperty, WriteProperty, DoWrite(FDecimalPlaces));
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::ReadProperty(TReader *Reader)
{
	DecimalPlaces = Reader->ReadInteger();
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::WriteProperty(TWriter *Writer)
{
	Writer->WriteInteger(DecimalPlaces);
}
//---------------------------------------------------------------------------
bool __fastcall TNumericEdit::DoWrite(int Value)
{
	return (Value != -1);
}
//---------------------------------------------------------------------------
__fastcall TNumericEdit::TNumericEdit(TComponent* Owner)
	: TCustomEdit(Owner)
{
	FormattingText	= false;
	FAlignment		= taRightJustify;

	FValue			= 0;
	FPrecision		= 10;
	FDecimalPlaces	= -1;
	FNumericType	= ntGeneral;
	FFixedPoint		= false;

	if (Focused())
	{
		Value2EditText();
	}
	else
	{
		Value2FormattedText();
	}
}
//---------------------------------------------------------------------------
__fastcall TNumericEdit::~TNumericEdit()
{
	if (FCanvas)
	{
		delete FCanvas;
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::SetValue(double Value)
{
	if (FValue != Value)
	{
		FValue = Value;
		if (!Focused())
		{
			Value2FormattedText();
		}
		else
		{
			Value2EditText();
		}
		TCustomEdit::Change();
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::SetAlignment(TAlignment Value)
{
	if (FAlignment != Value)
	{
		FAlignment = Value;
//		RecreateWnd();
		Invalidate();
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::SetPrecision(int Value)
{
	if (Value > FDecimalPlaces)
	{
		FPrecision = Value;
	}
	else
	{
		FPrecision = FDecimalPlaces + 1;
	}
	if (Focused())
	{
		Value2EditText();
	}
	else
	{
		Value2FormattedText();
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::SetDecimalPlaces(int Value)
{
	FDecimalPlaces = Value;
	if (FPrecision <= FDecimalPlaces)
	{
		FPrecision = FDecimalPlaces + 1;
	}
	if (Focused())
	{
		Value2EditText();
	}
	else
	{
		Value2FormattedText();
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::SetNumericType(TNumericType Value)
{
	FNumericType = Value;
	if (Focused())
	{
		Value2EditText();
	}
	else
	{
		Value2FormattedText();
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::SetFixedPoint(bool Value)
{
	FFixedPoint = Value;
	if (Focused())
	{
		Value2EditText();
	}
	else
	{
		Value2FormattedText();
	}
}
//---------------------------------------------------------------------------
void TNumericEdit::Value2FormattedText()
{
	try
	{
		FormattingText = true;

		if (FNumericType == ntCurrency)
		{
			Text = MMMath::CurrencyString(FValue, FDecimalPlaces, true);
		}
		else
		{
			AnsiString Format;
			if (FDecimalPlaces <= 0)
			{
				Format = "0." + AnsiString::StringOfChar('#', FPrecision);
			}
			else
			{
				if (FFixedPoint)
				{
					Format = "0." + AnsiString::StringOfChar('0', FDecimalPlaces);
				}
				else
				{
					Format = "0." + AnsiString::StringOfChar('#', FDecimalPlaces);
				}
			}
			Text = FormatFloat(Format, MMMath::RoundFloat(FValue, FDecimalPlaces, FPrecision));
			if (FNumericType == ntPercent)
			{
				Text = Text + "%";
			}
		}
	}
	__finally
	{
		FormattingText = false;
	}
/*	try
	{
		FormattingText = true;

		switch (FNumericType)
		{
			case ntGeneral:
			{
				if (FDecimalPlaces > -1 && !FFixedPoint)
				{
//					Text = FloatToStrF(RoundTo(FValue, -FDecimalPlaces), ffGeneral, FPrecision, FDecimalPlaces);
					Text = FormatFloat("0." + AnsiString::StringOfChar('#', FDecimalPlaces), FValue);
				}
				else if (FDecimalPlaces == -1 || !FFixedPoint)
				{
//					Text = FloatToStrF(FValue, ffGeneral, FPrecision, FDecimalPlaces);
					Text = FormatFloat("0." + AnsiString::StringOfChar('#', FPrecision), FValue);
				}
				else
				{
//					Text = FloatToStrF(FValue, ffFixed, FPrecision, FDecimalPlaces);
					Text = FormatFloat("0." + AnsiString::StringOfChar('0', FPrecision), FValue);
				}
				break;
			}
			case ntCurrency:
			{
				Text = FloatToStrF(FValue, ffCurrency, FPrecision, FDecimalPlaces);
				break;
			}
			case ntPercent:
			{
				if (FDecimalPlaces > -1 && !FFixedPoint)
				{
//					Text = FloatToStrF(RoundTo(FValue, -FDecimalPlaces), ffGeneral, FPrecision, FDecimalPlaces) + "%";
					Text = FormatFloat("0." + AnsiString::StringOfChar('#', FDecimalPlaces), FValue) + "%";
				}
				else if (FDecimalPlaces == -1 || !FFixedPoint)
				{
//					Text = FloatToStrF(FValue, ffGeneral, FPrecision, FDecimalPlaces) + "%";
					Text = FormatFloat("0." + AnsiString::StringOfChar('#', FPrecision), FValue) + "%";
				}
				else
				{
//					Text = FloatToStrF(FValue, ffFixed, FPrecision, FDecimalPlaces) + "%";
					Text = FormatFloat("0." + AnsiString::StringOfChar('0', FPrecision), FValue) + "%";
				}
				break;
			}
		}
	}
	__finally
	{
		FormattingText = false;
	} */
}
//---------------------------------------------------------------------------
void TNumericEdit::Value2EditText()
{
	try
	{
		FormattingText = true;

		if (FValue == 0)
		{
			Text = "";
		}
		else
		{
			AnsiString Format;
			if (FDecimalPlaces <= 0)
			{
				Format = "0." + AnsiString::StringOfChar('#', FPrecision);
			}
			else
			{
				if (FFixedPoint)
				{
					Format = "0." + AnsiString::StringOfChar('0', FDecimalPlaces);
				}
				else
				{
					Format = "0." + AnsiString::StringOfChar('#', FDecimalPlaces);
				}
			}
			Text = FormatFloat(Format, MMMath::RoundFloat(FValue, FDecimalPlaces, FPrecision));
			if (FNumericType == ntPercent)
			{
				Text += "%";
			}
		}
	}
	__finally
	{
		FormattingText = false;
	}
/*	try
	{
		FormattingText = true;
		if (FValue == 0)
		{
			Text = "";
		}
		else
		{
			if (FDecimalPlaces > -1 && FFixedPoint)
			{
				Text = FloatToStrF(RoundTo(FValue, -FDecimalPlaces), ffFixed, FPrecision, FDecimalPlaces);
			}
			else
			{
				if (FDecimalPlaces > -1)
				{
					Text = FloatToStrF(RoundTo(FValue, -FDecimalPlaces), ffGeneral, FPrecision, FDecimalPlaces);
				}
				else
				{
					Text = FloatToStrF(FValue, ffGeneral, FPrecision, FDecimalPlaces);
				}
			}
		}
	}
	__finally
	{
		FormattingText = false;
	}*/
}
//---------------------------------------------------------------------------
void TNumericEdit::Text2Value()
{
	AnsiString TmpText;
	char Tmp;
	bool IsNeg;

	IsNeg = (Text.Pos("-") > 0) || (Text.Pos("(") > 0);

	TmpText = "";
	for (Tmp=1; Tmp<=Text.Length(); Tmp++)
	{
		if ((Text[Tmp] >= '0' && Text[Tmp] <='9') || Text[Tmp] == DecimalSeparator)
		{
			TmpText += Text[Tmp];
		}
	}
	try
	{
		if (TmpText == "")
		{
			TmpText = "0.00";
		}
		FValue = StrToFloat(TmpText);

		if (IsNeg) FValue = -FValue;
	}
	catch (Exception &E)
	{
		MessageBeep(MB_ICONASTERISK);
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::DoEnter()
{
	Value2EditText();
	SelectAll();
	TCustomEdit::DoEnter();
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::DoExit()
{
	Value2FormattedText();
	TCustomEdit::DoExit();
	Invalidate();
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::Change(void)
{
	if (!FormattingText)
	{
		Text2Value();
		TCustomEdit::Change();
	}
}
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::KeyPress(char &Key)
{
  AnsiString S;

  int i;
  Word wID;
  LongRec LParam;
//  {0x1A Ctl-Z}
	if (!((Key >= '0' && Key <= '9') || Key == '.' || Key == '-' || Key == VK_BACK || Key == VK_RETURN || Key == 0x1A))
	{
		Key = 0;
	}
	switch (Key)
	{
		case 13:   	Text2Value();
						break;
		case '.':	if (FDecimalPlaces == 0)
						{
							Key = 0;
						}
						break;
		case '-':	if (Text.Pos('-') || (SelStart > 0))
						{
							Key = 0;
						}
						break;
	}
	if (Key != VK_BACK && Key != 0x1A)
	{
//		S is the text excluding the highlighted text. Used to see if it will contain a decimal separator
		S = Text.SubString(1, SelStart) + Text.SubString(SelStart+SelLength+1, Text.Length());
		if (Key == DecimalSeparator && S.Pos(DecimalSeparator)) // only 1 decimal separator
		{
			Key = 0;
		}
		else
		{
//			S is a model of Text if we accept the keystroke.  Use SelStart and
//			SelLength to find the cursor (insert) position.
			S = Text.SubString(1, SelStart) + Key + Text.SubString(SelStart+SelLength+1, Text.Length());
			if (FDecimalPlaces > -1)
			{
				if (((S.Pos(DecimalSeparator) > 0) && (S.Length() - S.Pos(DecimalSeparator) > FDecimalPlaces))  //{too many decimal places}
					  || ((Key == '-') && (Text.Pos('-') != 0))     //{only one minus...}
					  || (S.Pos('-') > 1))
				{                           //{... and only at beginning}
					if (Key != VK_RETURN)
					{
						Key = 0;
					}
				}
			}
		}
	}
	if (Key != 0)
	{
		TCustomEdit::KeyPress(Key);
	}
}
//---------------------------------------------------------------------------
/*void __fastcall TNumericEdit::CreateParams(Controls::TCreateParams &Params)
{
	TCustomEdit::CreateParams(Params);
	Longint lStyle;
	switch (FAlignment)
	{
		case taLeftJustify:	lStyle = ES_LEFT;
									break;
		case taRightJustify:	lStyle = ES_RIGHT;
									break;
		case taCenter:			lStyle = ES_CENTER;
									break;
	}
	Params.Style = Params.Style | lStyle;
}*/
//---------------------------------------------------------------------------
void __fastcall TNumericEdit::WMPaint(TWMPaint& Message)
{
	int				Left;
	TPoint			Margins;
	TRect				R;
	HDC				DC;
	TPaintStruct	PS;
	AnsiString		S;

	if (FAlignment == taLeftJustify || Focused())
	{
		TCustomEdit::Dispatch(&Message);   
	}
	else
	{
		if (!FCanvas)
		{
			FCanvas = new TControlCanvas;
		}
		DC = Message.DC;
		if (!DC)
		{
			DC = BeginPaint(Handle, &PS);
		}
		FCanvas->Handle = DC;
		try
		{
			FCanvas->Font = Font;
			R = ClientRect;
			if (!(NewStyleControls && Ctl3D) && (BorderStyle == bsSingle))
			{
				FCanvas->Brush->Color = clWindowFrame;
				FCanvas->FrameRect(R);
				InflateRect(&R, -1, -1);
			}
			FCanvas->Brush->Color = Color;
			if (!Enabled)
			{
				FCanvas->Font->Color = clGrayText;
			}
			else
			{
				FCanvas->Font->Color = Font->Color;
			}
			S = Text;
			if (PasswordChar)
			{
				S = AnsiString::StringOfChar(PasswordChar, S.Length());
			}
			Margins = GetTextMargins();
			switch (FAlignment)
			{
				case taLeftJustify:	Left = Margins.x;
											break;
				case taRightJustify:	Left = ClientWidth - FCanvas->TextWidth(S) - Margins.x - 1;
											break;
				default:					Left = (ClientWidth - FCanvas->TextWidth(S)) / 2;
			}
			FCanvas->TextRect(R, Left, Margins.y, S);
		}
		__finally
		{
			FCanvas->Handle = 0;
			if (!Message.DC)
			{
				EndPaint(Handle, &PS);
			}
		}
	}
}
//---------------------------------------------------------------------------
TPoint __fastcall TNumericEdit::GetTextMargins()
{
	HDC			DC;
	HGDIOBJ		SaveFont;
	int			I;
	TTextMetric	SysMetrics, Metrics;
	TPoint		Result;

	if (NewStyleControls)
	{
		if (BorderStyle == bsNone)	I = 0;
		else if (Ctl3D)				I = 1;
		else								I = 2;
		Result.x = SendMessage(Handle, EM_GETMARGINS, 0, 0) & 0x0000FFFF + I;
		Result.y = I;
	}
	else
	{
		if (BorderStyle == bsNone)
		{
			I = 0;
		}
		else
		{
			DC = GetDC(0);
			GetTextMetrics(DC, &SysMetrics);
			SaveFont = SelectObject(DC, Font->Handle);
			GetTextMetrics(DC, &Metrics);
			SelectObject(DC, SaveFont);
			ReleaseDC(0, DC);
			I = SysMetrics.tmHeight;
			if (I > Metrics.tmHeight)
			{
				I = Metrics.tmHeight;
			}
			I = I / 4;
		}
		Result.x = I;
		Result.y = I;
	}
	return Result;
}
//---------------------------------------------------------------------------
/*  if NewStyleControls then
  begin
    if BorderStyle = bsNone then I := 0 else
      if Ctl3D then I := 1 else I := 2;
    Result.X := SendMessage(Handle, EM_GETMARGINS, 0, 0) and $0000FFFF + I;
    Result.Y := I;
  end else
  begin
    if BorderStyle = bsNone then I := 0 else
    begin
      DC := GetDC(0);
      GetTextMetrics(DC, SysMetrics);
      SaveFont := SelectObject(DC, Font.Handle);
      GetTextMetrics(DC, Metrics);
      SelectObject(DC, SaveFont);
      ReleaseDC(0, DC);
      I := SysMetrics.tmHeight;
      if I > Metrics.tmHeight then I := Metrics.tmHeight;
      I := I div 4;
    end;
    Result.X := I;
    Result.Y := I;
  end;
end;
*/
/*procedure TNumberEdit.WMPaint(var Message: TWMPaint);
var
  Left: Integer;
  Margins: TPoint;
  R: TRect;
  DC: HDC;
  PS: TPaintStruct;
  S: string;
begin
  if ((FAlignment = taLeftJustify) or FFocused) then
  begin
    inherited;
    Exit;
  end;
  if FCanvas = nil then
  begin
    FCanvas := TControlCanvas.Create;
    FCanvas.Control := Self;
  end;
  DC := Message.DC;
  if DC = 0 then DC := BeginPaint(Handle, PS);
  FCanvas.Handle := DC;
  try
	 FCanvas.Font := Font;
	 with FCanvas do
	 begin
		R := ClientRect;
		if not (NewStyleControls and Ctl3D) and (BorderStyle = bsSingle) then
		begin
		  Brush.Color := clWindowFrame;
		  FrameRect(R);
		  InflateRect(R, -1, -1);
		end;
		Brush.Color := Color;
		S := Text;
		if PasswordChar <> #0 then FillChar(S[1], Length(S), PasswordChar);
		Margins := GetTextMargins;
		case FAlignment of
		  taLeftJustify: Left := Margins.X;
		  taRightJustify: Left := ClientWidth - TextWidth(S) - Margins.X - 1;
		else
		  Left := (ClientWidth - TextWidth(S)) div 2;
		end;
		TextRect(R, Left, Margins.Y, S);
	 end;
  finally
	 FCanvas.Handle := 0;
    if Message.DC = 0 then EndPaint(Handle, PS);
  end;
end;*/
//---------------------------------------------------------------------------
namespace Numericedit
{
	void __fastcall PACKAGE Register()
	{
		TComponentClass classes[1] = {__classid(TNumericEdit)};
		RegisterComponents("Numeric Edit", classes, 0);
	}
}
//---------------------------------------------------------------------------

