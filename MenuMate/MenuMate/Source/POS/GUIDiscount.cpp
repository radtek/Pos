//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#ifdef MenuMate
#include "DeviceRealTerminal.h"
#endif

#include "GUIDiscount.h"
#include "POSMain.h"
#include "math.h"
#include "GUIScale.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"

//---------------------------------------------------------------------------
__fastcall TfrmDiscount::TfrmDiscount(TComponent* Owner)
	: TZForm(Owner)
{
	ForcedType = avtNone;
}
//---------------------------------------------------------------------------

void __fastcall TfrmDiscount::FormCreate(TObject *Sender)
{
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
   CURInitial = 0;
   PERCInitial = 0;
   ForcedType = avtNone;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::FormShow(TObject *Sender)
{
	FormResize(NULL);
   ValueTypeBeforeSetPrice = avtDiscount;

	if (Mode == DiscModeCurrency)
	{
		pnl00->Caption = "00";
		wrkCurAmount = CURInitial;
		lbeEnteredValue->Caption = CurrToStrF(fabs(wrkCurAmount), ffCurrency, CurrencyDecimals);
		pnlToggle->Caption = "$ Mode";
	}
	else if (Mode == DiscModePercent)
	{
		pnl00->Caption = ".";
		wrkPercAmount = PERCInitial;
		PercentDecimal = false;
		lbeEnteredValue->Caption =  FloatToStrF(fabs(wrkPercAmount), ffNumber, 18, 1)  + " %";
		pnlToggle->Caption = "% Mode";
	}
	else if (Mode == DiscModeSetPrice)
	{
		pnl00->Caption = "00";
		wrkCurAmount = CURInitial;
		lbeEnteredValue->Caption = FormatFloat("$0.00",fabs(wrkCurAmount));
		pnlToggle->Caption = "Set Price";
	}
	else if (Mode == DiscModeCombo)
	{
		pnl00->Caption = "00";
		wrkCurAmount = CURInitial;
		lbeEnteredValue->Caption = FormatFloat("$0.00",fabs(wrkCurAmount));
		pnlToggle->Caption = "Combo";
	}
   else	if (Mode == DiscModeItem)
	{
		pnl00->Caption = "00";
		wrkCurAmount = CURInitial;
		lbeEnteredValue->Caption = CurrToStrF(fabs(wrkCurAmount), ffCurrency, CurrencyDecimals);
		pnlToggle->Caption = "Item Mode";
	}
    else if (Mode == DiscModePoints)
	{
		pnl00->Caption = "00";
		wrkCurAmount = CURInitial;
		lbeEnteredValue->Caption = CurrToStrF(fabs(wrkCurAmount), ffNumber, CurrencyDecimals);
		pnlToggle->Caption = "Point Mode";
    }


	if (Mode == DiscModeCurrency)
	{
		if(wrkCurAmount >= 0)
		{
			tbToggleAmount->Caption = "Applying Discount";
         	ValueType = avtDiscount;
		}
		else
		{
			tbToggleAmount->Caption = "Applying Surcharge";
         	ValueType = avtSurcharge;
		}
	}
	else if (Mode == DiscModePercent)
	{
		if(wrkPercAmount >= 0)
		{
			tbToggleAmount->Caption = "Applying Discount";
         	ValueType = avtDiscount;
		}
		else
		{
			tbToggleAmount->Caption = "Applying Surcharge";
         	ValueType = avtSurcharge;
		}
	}
	else if (Mode == DiscModeSetPrice || Mode == DiscModeCombo)
	{
		tbToggleAmount->Caption = "Set Price";
      	ValueType = avtNone;
	}
    else if (Mode == DiscModeItem)
	{
		if(wrkCurAmount >= 0)
		{
			tbToggleAmount->Caption = "Applying Discount";
         	ValueType = avtDiscount;
		}
		else
		{
			tbToggleAmount->Caption = "Applying Surcharge";
         	ValueType = avtSurcharge;
		}
	}
    else if(Mode == DiscModePoints)
    {
       	tbToggleAmount->Caption = "Set Points";
        ValueType = avtPoints;
    }

      //************4278*********************/
    if(ForcedType != avtNone)
    {
		ValueType = ForcedType;
        tbToggleAmount->OnMouseClick = NULL;
		if(ValueType == avtDiscount)
		{
			tbToggleAmount->Caption = "Applying Discount";
		}
		else
		{
			tbToggleAmount->Caption = "Applying Surcharge";
		}
    }

	lbeEnteredValue->Color = clHighlight;
	lbeEnteredValue->Font->Color = clHighlightText;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::FormHide(TObject *Sender)
{
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
   CURInitial = 0;
	PERCInitial = 0;
	tbToggleAmount->Visible = true;
	this->Caption = "Discount / Surcharge / Set Price";
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::WMDisplayChange(TWMDisplayChange& Message)
{
	FormResize(NULL);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
   {
      int Temp = Tag;
		Tag = Screen->Width;
		if ((double)Screen->Width / Screen->Height < 1.4)
		{
			ScaleBy(Screen->Width, Temp);
		}
		else
		{
			GUIScale::ChildrenTop(Panel11, 0.95);
		}
   }
   Panel5->Width = Panel2->BorderWidth;
   ClientWidth = Panel3->Left + Panel3->Width + Panel5->Width + btnCancel->Width + (btnCancel->Left * 2) + (Panel2->BorderWidth * 2);
   ClientHeight = Panel26->Top + Panel26->Height + Panel26->Left + (Panel2->BorderWidth * 2);
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::NumberMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	TLabel *Btn = (TLabel *)Sender;

	if (Mode == DiscModeCurrency || Mode == DiscModeSetPrice || Mode == DiscModeCombo ||Mode == DiscModeItem)
   {
      if (wrkCurAmount > 999999999 )
      {
			return;
      }
      if (lbeEnteredValue->Color == clHighlight)
      {
         if (Btn->Caption == "00")
         {
            wrkCurAmount = 0;
            lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
         }
         else
         {
            wrkCurAmount = StrToCurr(Btn->Caption) / 100;
			lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
         }
      }
      else
      {
         if (Btn->Caption == "00")
         {
            wrkCurAmount = double(wrkCurAmount) * 100;
				lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
         }
		 else
		 {
				wrkCurAmount = double(wrkCurAmount) * 10;
				wrkCurAmount += double(StrToCurr(Btn->Caption) / 100);
				lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
         }
      }
   }
  else if (Mode == DiscModePercent)
   {
   	if (Btn->Caption == ".")
      {
         if (!PercentDecimal)
         {
            PercentDecimal = true;
            if (lbeEnteredValue->Color == clHighlight)
            {
               wrkPercAmount = 0;
					lbeEnteredValue->Caption = "0. %";
            }
            else
            {
                lbeEnteredValue->Caption = FloatToStrF(wrkPercAmount, ffNumber, 18, 1)  + " %";
            }
         }
      }
      else
      {
         if (lbeEnteredValue->Color == clHighlight)
         {
				wrkPercAmount = StrToFloat(Btn->Caption);
                PercentDecimal = false;
				lbeEnteredValue->Caption = FloatToStrF(wrkPercAmount, ffNumber	, 18, 1) + " %";
         }
         else
         {
         	if (PercentDecimal)
            {
                if (int(wrkPercAmount * 10) - (int(wrkPercAmount) * 10) == 0)
                {
                  if (IntToStr(int(wrkPercAmount * 10)).Length() < 4 &&
                     wrkPercAmount + (double(StrToInt(Btn->Caption)) / 10) <= 100)
                  {
                            wrkPercAmount += double(StrToFloat(Btn->Caption)) / 10;
                     lbeEnteredValue->Caption = FloatToStr(wrkPercAmount) + " %";
                  }
                }
            }
            else
            {
               if (wrkPercAmount * 10 + StrToInt(Btn->Caption) <= 100)
               {
						wrkPercAmount = wrkPercAmount * 10 + StrToFloat(Btn->Caption);
						lbeEnteredValue->Caption = FloatToStrF(wrkPercAmount, ffNumber	, 18, 1) + " %";
               }
            }
         }
   	}
   }
   else	if (Mode == DiscModePoints)
   {
      if (wrkCurAmount > 999999999 )
      {
			return;
      }
      if (lbeEnteredValue->Color == clHighlight)
      {
         if (Btn->Caption == "00")
         {
            wrkCurAmount = 0;
            lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffNumber, CurrencyDecimals);
         }
         else
         {
            wrkCurAmount = StrToCurr(Btn->Caption) / 100;
			lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffNumber, CurrencyDecimals);
         }
      }
      else
      {
         if (Btn->Caption == "00")
         {
            wrkCurAmount = double(wrkCurAmount) * 100;
            lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffNumber, CurrencyDecimals);
         }
		 else
		 {
			wrkCurAmount = double(wrkCurAmount) * 10;
			wrkCurAmount += double(StrToCurr(Btn->Caption) / 100);
			lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffNumber, CurrencyDecimals);
         }
      }
   }
	lbeEnteredValue->ParentColor = true;
	lbeEnteredValue->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::btnCLRMouseUp(TObject *Sender,
      TMouseButton Button, TShiftState Shift, int X, int Y)
{
	if (Mode == DiscModeCurrency)
	{
		wrkCurAmount = 0;
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
	}
	else if (Mode == DiscModeSetPrice)
	{
		wrkCurAmount = 0;
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
	}
	else if (Mode == DiscModePercent)
   {
		wrkPercAmount = 0;
		lbeEnteredValue->Caption = FloatToStrF(wrkPercAmount, ffNumber	, 18, 1) + " %";
   }
	else if (Mode == DiscModeCombo)
	{
		wrkCurAmount = 0;
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
	}
    else if (Mode == DiscModeItem)
	{
		wrkCurAmount = 0;
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
	}
    else if (Mode == DiscModePoints)
	{
		wrkCurAmount = 0;
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffNumber, CurrencyDecimals);
	}
   PercentDecimal = false;
	lbeEnteredValue->ParentColor = true;
	lbeEnteredValue->Font->Color = clBlack;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::btnCancelClick(TObject *Sender)
{
	ModalResult = mrCancel;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::pnlOkClick(TObject *Sender)
{
   if (Mode == DiscModeCurrency)
	{
      if(ValueType ==  avtDiscount)
      {
         wrkCurAmount = fabs(wrkCurAmount);
      }
      else if(ValueType ==  avtSurcharge)
      {
         wrkCurAmount = -fabs(wrkCurAmount);
      }
   }
	else if (Mode == DiscModePercent)
   {
      if(ValueType ==  avtDiscount)
      {
         wrkPercAmount = fabs(wrkPercAmount);
      }
      else if(ValueType ==  avtSurcharge)
      {
         wrkPercAmount = -fabs(wrkPercAmount);
      }
   }
    if (Mode == DiscModeItem)
	{
      if(ValueType ==  avtDiscount)
      {
         wrkCurAmount = fabs(wrkCurAmount);
      }
      else if(ValueType ==  avtSurcharge)
      {
         wrkCurAmount = -fabs(wrkCurAmount);
      }
   }
      //************4278*********************/
	if (Mode == DiscModeCurrency || Mode == DiscModeSetPrice || Mode == DiscModeCombo|| Mode == DiscModeItem || Mode == DiscModePoints)
   {
		CURResult = wrkCurAmount;
   }
   else if (Mode == DiscModePercent)
   {
		PERCResult = wrkPercAmount;
   }
	ModalResult = mrOk;
}
//---------------------------------------------------------------------------
void __fastcall TfrmDiscount::pnlToggleClick(TObject *Sender)
{
	if (Mode == DiscModeCurrency)
	{
		Mode = DiscModeCombo;
		pnl00->Caption = "00";
		if (TotalValue != 0)
		{
			wrkCurAmount = RoundToNearest(double(wrkPercAmount) * TotalValue / 100,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
		}
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
		pnlToggle->Caption = "Combo";
        ValueTypeBeforeSetPrice = ValueType;
        ValueType = avtNone;
	}
	else if (Mode == DiscModeCombo)
	{
		Mode = DiscModePercent;
		PercentDecimal = false;
		pnl00->Caption = ".";
		if (TotalValue != 0)
		{
			wrkPercAmount = double(int(1000 * wrkCurAmount / TotalValue)) / 10;
		}
		lbeEnteredValue->Caption = FloatToStr(wrkPercAmount) + " %";
		pnlToggle->Caption = "% Mode";
        ValueType = ValueTypeBeforeSetPrice;
	}
	else if (Mode == DiscModePercent)
	{
		Mode = DiscModeSetPrice;
		pnl00->Caption = "00";
		if (TotalValue != 0)
		{
			wrkCurAmount = RoundToNearest(double(wrkPercAmount) * TotalValue / 100,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
		}
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
		pnlToggle->Caption = "Set Price";
        ValueTypeBeforeSetPrice = ValueType;
        ValueType = avtNone;
	}
	else if (Mode == DiscModeSetPrice)
	{
		Mode = DiscModeItem;
		pnl00->Caption = "00";
		if (TotalValue != 0)
		{
			wrkCurAmount = RoundToNearest(double(wrkPercAmount) * TotalValue / 100,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
		}
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
		pnlToggle->Caption = "Item Mode";
        ValueType = ValueTypeBeforeSetPrice;
	}
   	else if (Mode == DiscModeItem)
	{
    	Mode = DiscModePoints;
		pnl00->Caption = "00";
		if (TotalValue != 0)
		{
			wrkCurAmount = RoundToNearest(double(wrkPercAmount) * TotalValue / 100,MIN_CURRENCY_VALUE,TGlobalSettings::Instance().MidPointRoundsDown);
		}
		lbeEnteredValue->Caption = FloatToStrF(wrkCurAmount, ffNumber	, 18, 1);
		pnlToggle->Caption = "Point Mode";
        ValueType = avtPoints;
    }
    else if (Mode == DiscModePoints)
	{
    	Mode = DiscModeCurrency;
		pnl00->Caption = "00";
		lbeEnteredValue->Caption = CurrToStrF(wrkCurAmount, ffCurrency, CurrencyDecimals);
		pnlToggle->Caption = "$ Mode";
        ValueType = ValueTypeBeforeSetPrice;
    }

       if(ValueType == avtDiscount)
       {
        tbToggleAmount->Caption = "Applying Discount";
       }
       else if(ValueType == avtSurcharge)
       {
          tbToggleAmount->Caption = "Applying Surcharge";
       }
       else if(ValueType == avtPoints)
       {
         tbToggleAmount->Caption = "Set Points";
       }
       else
       {
          tbToggleAmount->Caption = "Set Price";
       }

	lbeEnteredValue->Color = clHighlight;
	lbeEnteredValue->Font->Color = clHighlightText;
}
//---------------------------------------------------------------------------
void TfrmDiscount::ForceType(TAdjustmentValueType inValueType)
{
	ForcedType = inValueType;
}

void __fastcall TfrmDiscount::tbToggleAmountClick(TObject *Sender)
{
   if(ValueType ==  avtDiscount)
   {
      ValueType = avtSurcharge;
      tbToggleAmount->Caption = "Applying Surcharge";
   }
   else if(ValueType ==  avtSurcharge)
   {
      ValueType = avtDiscount;
   	tbToggleAmount->Caption = "Applying Discount";
   }

   if (Mode == DiscModeCurrency)
   {
      lbeEnteredValue->Caption = CurrToStrF(fabs(wrkCurAmount), ffCurrency, CurrencyDecimals);
   }
  else  if (Mode == DiscModeItem)
   {
      lbeEnteredValue->Caption = CurrToStrF(fabs(wrkCurAmount), ffCurrency, CurrencyDecimals);
   }
   else if (Mode == DiscModePercent)
   {
      lbeEnteredValue->Caption = FloatToStr(fabs(wrkPercAmount)) + " %";
   }
}
//---------------------------------------------------------------------------


