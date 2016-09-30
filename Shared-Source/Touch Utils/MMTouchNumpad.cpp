// ---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "MMTouchNumpad.h"
 #include <sstream>
 #include <string>
// ---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma link "TouchNumpad"
#pragma link "TouchPages"
#pragma resource "*.dfm"

// ---------------------------------------------------------------------------
__fastcall TfrmTouchNumpad::TfrmTouchNumpad(TComponent* Owner) : TZForm(Owner)
{
   PreSelect = true;
   Format = pmCurrency;
   MaxLength = 9; // Default length is 9
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::FormShow(TObject *Sender)
{
   FormResize(NULL);
   BtnExit = -1;
   if(View == viewQuantity)
   {
       pcItemModify->ActivePage = tsQuantity;
   }
   else
   {
       pcItemModify->ActivePage = tsGeneral;
   }
   // Set the Foramt for currency and decimal formatting.
   TFloatFormat Format = (Mode == pmCurrency) ? ffCurrency : ffFixed;

   if(pcItemModify->ActivePage != tsQuantity)
   {
   if (Mode == pmCurrency || Mode == pmDecimal || Mode == pmWeight)
   {
	  pnl00->Visible = true;
	  btnCLR->Left = pnl3->Left;
	  btnCLR->Width = pnl3->Width;
	  wrkPayAmount = CURInitial;
	  UpdateDisplay();
	  if (CURInitial == 0 || PreSelect == false)
	  {
		 lbeEnteredValue->ParentColor = true;
		 lbeEnteredValue->Font->Color = clBlack;
	  }
	  else
	  {
		 lbeEnteredValue->Color = clHighlight;
		 lbeEnteredValue->Font->Color = clHighlightText;
	  }
   }
   else if (Mode == pmNumber)
   {
	  pnl00->Visible = false;
	  btnCLR->Left = pnl2->Left;
	  btnCLR->Width = pnl3->Left + pnl3->Width - pnl2->Left;
	  wrkIntAmount = INTInitial;
	  UpdateDisplay();
	  if (INTInitial == 0 || PreSelect == false)
	  {
		 lbeEnteredValue->ParentColor = true;
		 lbeEnteredValue->Font->Color = clBlack;
	  }
	  else
	  {
		 lbeEnteredValue->Color = clHighlight;
		 lbeEnteredValue->Font->Color = clHighlightText;
	  }
   }
   else if (Mode == pmPIN)
   {
	  pnl00->Visible = false;
	  btnCLR->Left = pnl2->Left;
	  btnCLR->Width = pnl3->Left + pnl3->Width - pnl2->Left;
	  wrkStrAmount = StrInitial;
	  lbeEnteredValue->Caption = "";
	  lbeEnteredValue->ParentColor = true;
	  lbeEnteredValue->Font->Color = clBlack;
   }
   else if (Mode == pmSTR)
   {
	  pnl00->Visible = false;
	  btnCLR->Left = pnl2->Left;
	  btnCLR->Width = pnl3->Left + pnl3->Width - pnl2->Left;
	  wrkNumStrAmount = NUMSTRInitial;
	  UpdateDisplay();
      AnsiString NUMInitial = NUMSTRInitial;
	  if (atoi(NUMInitial.c_str()) == 0 || PreSelect == false)
	  {
		 lbeEnteredValue->ParentColor = true;
		 lbeEnteredValue->Font->Color = clBlack;
	  }
	  else
	  {
		 lbeEnteredValue->Color = clHighlight;
		 lbeEnteredValue->Font->Color = clHighlightText;
	  }
   }  }
}

void __fastcall TfrmTouchNumpad::UpdateDisplay()
{
    if (Mode == pmCurrency)
    {
        lbeEnteredValue->Caption = UnicodeString::CurrToStrF(wrkPayAmount, UnicodeString::sffFixed, 2);
    }
    else if (Mode == pmDecimal)
    {
        lbeEnteredValue->Caption = FormatFloat("0.00",wrkPayAmount);
    }
    else if (Mode == pmWeight)
    {
        lbeEnteredValue->Caption = FormatFloat("0.000", wrkPayAmount);
    }
    else if (Mode == pmNumber)
    {
        lbeEnteredValue->Caption = IntToStr(wrkIntAmount);
    }
    else if (Mode == pmPIN)
    {
        lbeEnteredValue->Caption = UnicodeString::StringOfChar('*', wrkStrAmount.Length());
    }
    else if (Mode == pmSTR)
    {
        lbeEnteredValue->Caption = wrkNumStrAmount;
    }
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::FormHide(TObject *Sender)
{
   Left = (Screen->Width - Width) / 2;
   Top = (Screen->Height - Height) / 2;
   CURInitial = 0;
   INTInitial = 0;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::btnNumberMouseDown(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   TLabel *Btn = (TLabel*)Sender;

   if (Mode == pmCurrency || Mode == pmDecimal)
   {
	  if (wrkPayAmount > 999999999 != 0)
	  {
		 return;
	  }
	  if (lbeEnteredValue->Color == clHighlight)
	  {
		 if (Btn->Caption == "00")
		 {
			wrkPayAmount = 0;
			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
		 else
		 {
			wrkPayAmount = double(StrToFloat(Btn->Caption)) / double(100.0);
			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
	  }
	  else
	  {
		 if (Btn->Caption == "00")
		 {
			wrkPayAmount = wrkPayAmount * 100.0;
			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
		 else
		 {
			if (ForcedNegitive && wrkPayAmount < 0) // Do the Math on + Numbers
			{
			   wrkPayAmount = -wrkPayAmount;
			}

			wrkPayAmount = wrkPayAmount * 10.0;
			wrkPayAmount += double(StrToFloat(Btn->Caption)) / double(100.0);

			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
	  }
   }
    else if(Mode == pmWeight)
    {
        if (wrkPayAmount > 999 != 0)
	  {
		 return;
	  }
	  if (lbeEnteredValue->Color == clHighlight)
	  {
		 if (Btn->Caption == "00")
		 {
			wrkPayAmount = 0;
			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
		 else
		 {
			wrkPayAmount = double(StrToFloat(Btn->Caption)) / double(1000.0);
			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
	  }
	  else
	  {
		 if (Btn->Caption == "00")
		 {
			wrkPayAmount = wrkPayAmount * 100.0;
			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
		 else
		 {
			if (ForcedNegitive && wrkPayAmount < 0) // Do the Math on + Numbers
			{
			   wrkPayAmount = -wrkPayAmount;
			}

			wrkPayAmount = wrkPayAmount * 10.0;
			wrkPayAmount += double(StrToFloat(Btn->Caption)) / double(1000.0);

			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
	  }
    }
   else if (Mode == pmNumber)
   {
	  if (lbeEnteredValue->Color == clHighlight)
	  {
		 wrkIntAmount = StrToInt(Btn->Caption);
		 if (ForcedNegitive)
		 {
			if (wrkIntAmount > 0)
			{
			   wrkIntAmount = -wrkIntAmount;
			}
		 }
		 UpdateDisplay();
	  }
	  else
	  {
		 if (IntToStr(wrkIntAmount).Length() < MaxLength)
		 {
			if (ForcedNegitive && wrkIntAmount < 0) // Do the Math on + Numbers
			{
			   wrkIntAmount = -wrkIntAmount;
			}
			wrkIntAmount = wrkIntAmount * 10 + StrToInt(Btn->Caption);
			if (ForcedNegitive)
			{
			   if (wrkIntAmount > 0)
			   {
				  wrkIntAmount = -wrkIntAmount;
			   }
			}
			UpdateDisplay();
		 }
	  }
   }
   else if (Mode == pmPIN)
   {
	  wrkStrAmount = wrkStrAmount + Btn->Caption;
	  UpdateDisplay();
   }
   else if (Mode == pmSTR)
   {
	  if (lbeEnteredValue->Color == clHighlight)
	  {
         wrkNumStrAmount = Btn->Caption;
		 UpdateDisplay();
	  }
	  else
	  {
		 if (wrkNumStrAmount.Length() < MaxLength)
		 {
			wrkNumStrAmount = wrkNumStrAmount+Btn->Caption;
			UpdateDisplay();
		 }
	  }
   }
   lbeEnteredValue->ParentColor = true;
   lbeEnteredValue->Font->Color = clBlack;

}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::btnCLRMouseUp(TObject *Sender, TMouseButton Button, TShiftState Shift, int X, int Y)
{
   if (Mode == pmCurrency || Mode == pmDecimal || Mode == pmWeight)
   {
	  wrkPayAmount = 0;
	  UpdateDisplay();
   }
   else if (Mode == pmNumber)
   {
	  wrkIntAmount = 0;
	  UpdateDisplay();
   }
   else if (Mode == pmPIN)
   {
	  wrkStrAmount = "";
	  lbeEnteredValue->Caption = "";
   }
   else if (Mode == pmSTR)
   {
      wrkNumStrAmount = "";
      lbeEnteredValue->Caption = "";
   }
   lbeEnteredValue->ParentColor = true;
   lbeEnteredValue->Font->Color = clBlack;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::WMDisplayChange(TWMDisplayChange& Message)
{
   FormResize(NULL);
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::FormResize(TObject *Sender)
{
   SetWindowPos(Handle, HWND_TOP, Left, Top, Width, Height, 0);
   if (Tag != Screen->Width)
   {
	  int Temp = Tag;
	  Tag = Screen->Width;
          if((double)Screen->Width / Screen->Height)
          {
	  	ScaleBy(Screen->Width, Temp);
          }
   }

   Panel5->Width = Panel2->BorderWidth;
   ClientWidth = Panel2->Width;
   ClientHeight = Panel2->Height;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::btnCancelClick(TObject *Sender)
{
   BtnExit = 0;
   ModalResult = mrCancel;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::btnDiscountClick(TObject *Sender)
{
   if (Mode == pmCurrency || Mode == pmDecimal || Mode == pmWeight)
   {
	  CURResult = -wrkPayAmount;
   }
   else if (Mode == pmNumber)
   {
	  INTResult = -wrkIntAmount;
   }
   BtnExit = 2;
   ModalResult = mrOk;
}

// ---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::btnSurchargeClick(TObject *Sender)
{
    if(View == viewGeneral)
    {
       if (Mode == pmCurrency || Mode == pmDecimal || Mode == pmWeight)
       {
          CURResult = wrkPayAmount;
       }
       else if (Mode == pmNumber)
       {
          INTResult = wrkIntAmount;
       }
       else if (Mode == pmPIN)
       {
          STRResult = wrkStrAmount;
       }
   }
   else
   {
        CURResult = splitValue;
   }
   BtnExit = 1;
   ModalResult = mrOk;
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTouchNumpad::FormKeyDown(TObject *Sender, WORD &Key, TShiftState Shift)
{
   if (Key == VK_RETURN || Key == VK_SPACE)
   {
	  btnSurchargeClick(Sender);
   }
   if ((Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9) || (char(Key) >= '0' && char(Key) <= '9'))
   {
	  int KeyPressed = 0;
	  if (Key >= VK_NUMPAD0 && Key <= VK_NUMPAD9)
	  {
		 KeyPressed = Key - VK_NUMPAD0;
	  }
	  else if (char(Key) >= '0' && char(Key) <= '9')
	  {
		 KeyPressed = char(Key) - 0x30;
	  }

	  if (Mode == pmCurrency || Mode == pmDecimal)
	  {
		 if (long(double(wrkPayAmount)) / 100000 != 0)
		 {
			return;
		 }
		 if (lbeEnteredValue->Color == clHighlight)
		 {
			wrkPayAmount = double(KeyPressed) / double(100.0);
			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
			lbeEnteredValue->ParentColor = true;
			lbeEnteredValue->Font->Color = clBlack;

		 }
		 else
		 {
			if (ForcedNegitive && wrkPayAmount < 0) // Do the Math on + Numbers
			{
			   wrkPayAmount = -wrkPayAmount;
			}

			wrkPayAmount = wrkPayAmount * 10.0;
			wrkPayAmount += double(KeyPressed) / double(100.0);

			if (ForcedNegitive)
			{
			   if (wrkPayAmount > 0)
			   {
				  wrkPayAmount = -wrkPayAmount;
			   }
			}
			UpdateDisplay();
		 }
	  }
	  else if (Mode == pmNumber)
	  {
		 if (lbeEnteredValue->Color == clHighlight)
		 {
			wrkIntAmount = KeyPressed;
			if (ForcedNegitive)
			{
			   if (wrkIntAmount > 0)
			   {
				  wrkIntAmount = -wrkIntAmount;
			   }
			}
			UpdateDisplay();
			lbeEnteredValue->ParentColor = true;
			lbeEnteredValue->Font->Color = clBlack;
		 }
		 else
		 {
			if (IntToStr(wrkIntAmount).Length() < MaxLength)
			{
			   if (ForcedNegitive && wrkIntAmount < 0) // Do the Math on + Numbers
			   {
				  wrkIntAmount = -wrkIntAmount;
			   }
			   wrkIntAmount = wrkIntAmount * 10 + KeyPressed;
			   if (ForcedNegitive)
			   {
				  if (wrkIntAmount > 0)
				  {
					 wrkIntAmount = -wrkIntAmount;
				  }
			   }
			   UpdateDisplay();
			}
		 }
	  }
	  else if (Mode == pmPIN)
	  {
		 wrkStrAmount = wrkStrAmount + IntToStr(KeyPressed);
		 UpdateDisplay();
	  }
      else if (Mode == pmSTR)
      {
		 if (lbeEnteredValue->Color == clHighlight)
		 {
			wrkNumStrAmount = KeyPressed;
			UpdateDisplay();
			lbeEnteredValue->ParentColor = true;
			lbeEnteredValue->Font->Color = clBlack;
		 }
		 else
		 {
			if (wrkNumStrAmount.Length() < MaxLength)
			{
			   wrkNumStrAmount = wrkNumStrAmount+KeyPressed;
			   UpdateDisplay();
			}
		 }
	  }
   }
}
// ---------------------------------------------------------------------------

void __fastcall TfrmTouchNumpad::btnOkMouseClick(TObject *Sender)
{
   if (Mode == pmCurrency || Mode == pmDecimal || Mode == pmWeight)
   {
      CURResult = wrkPayAmount;
   }
   else if (Mode == pmNumber)
   {
      INTResult = wrkIntAmount;
   }
   else if (Mode == pmPIN)
   {
      STRResult = wrkStrAmount;
   }
   else if (Mode == pmSTR)
   {
      NUMSTRResult = wrkNumStrAmount;
   }
   BtnExit = 1;
   ModalResult = mrOk;
}
// ---------------------------------------------------------------------------

void TfrmTouchNumpad::SetMaxLengthValue(int Length)
{
    MaxLength = Length;
}
//---------------------------------------------------------------------------
void __fastcall TfrmTouchNumpad::tnpQuantityClick(TObject *Sender, TNumpadKey Key)
{
   static const double maximum_quantity = 9999.0;
   if (QtyDisplay->Numeric() > maximum_quantity)
      QtyDisplay->SetNumeric(static_cast<int>(QtyDisplay->Numeric()) / 10);
   splitValue = QtyDisplay->Numeric();
}
