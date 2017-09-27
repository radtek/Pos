//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "Scales.h"
#include "MMMessageBox.h"
#include "MMTouchNumpad.h"
#include "DeviceRealTerminal.h"
#include "MMPrice.h"
#include "Rounding.h"

//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "TouchBtn"
#pragma link "TouchControls"
#pragma resource "*.dfm"
//---------------------------------------------------------------------------
__fastcall TfrmScales::TfrmScales(TComponent* Owner,TWeighScale *inScales,TPoleDisplay *inPoleDisplay, TWeight &inWeight,TWeight &inTareWeight,bool inAutoConvertScalesWeight,TItemMinorComplete *inItem)
   : TZForm(Owner) , Weight(inWeight), TareWeight(inTareWeight)
{
   Scales = inScales;
   PoleDisplay = inPoleDisplay;
   Item = inItem;
   AutoConvertScalesWeight = inAutoConvertScalesWeight;
   if(Scales)
   {
      Scales->OnWeightChanged.RegisterForEvent(OnWeightChanged);
      Scales->OnStabilityChanged.RegisterForEvent(OnStabilityChanged);
      tbtnOk->Enabled = Scales->IsStable();
   }
   UnitPrice = 0;
   lbUnitPrice->Caption = FormatFloat("0.00",UnitPrice);
   lbPackagingWeight->Caption = TareWeight.ToStringWithUnit();
   TDeviceRealTerminal::Instance().Scales->Counter = 0;
}
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------

void __fastcall TfrmScales::FormShow(TObject *Sender)
{
    FormResize(Sender);
    tbtnAmount->Visible = TGlobalSettings::Instance().AllowToEnterPrice;
    tbtnWeight->Visible = TGlobalSettings::Instance().AllowToEnterWeight;

    if(TGlobalSettings::Instance().ShowCurrencySymbol)
    {
       Label4->Caption = "Total (" + CurrencyString + ")";
    }
    else
    {
      Label4->Caption = "Total";
    }
    IsWeighed= false;
}
//---------------------------------------------------------------------------

void __fastcall TfrmScales::FormResize(TObject *Sender)
{
	if (Tag != Screen->Width)
	{
      int Temp = Tag;
		Tag = Screen->Width;
                 if((double)Screen->Width / Screen->Height < 1.4)
                {
		 ScaleBy(Screen->Width, Temp);
                }
	}
//	ClientWidth = Panel1->Width + Panel2->Width + Panel3->Width + (BorderWidth * 3);
//	ClientHeight = Panel3->Height + (BorderWidth * 2);
	Left = (Screen->Width - Width) / 2;
	Top = (Screen->Height - Height) / 2;
}
//---------------------------------------------------------------------------

__fastcall TfrmScales::~TfrmScales()
{
   if(Scales)
   {
      Scales->OnWeightChanged.DeregisterForEvent(OnWeightChanged);
      Scales->OnStabilityChanged.DeregisterForEvent(OnStabilityChanged);
   }
}

TModalResult TfrmScales::GetWeight()
{
   return ShowModal();
}

void TfrmScales::OnWeightChanged(TSystemEvents *Sender)
{
   if(Scales->Weight.Unit != ewuKg && AutoConvertScalesWeight == false)
   {
      MessageBox("The Scales are configured for the wrong unit. "
                 "They must report their weights in kilograms as that is how the items are priced in Office.\r"
                 "You can override this in [Maintenace] [Misc Settings] "
                 "by ticking [Auto Convert Scales Weights] "
                 "in which case the system will try and automatically convert the weight."
                 , "Sacles Error", MB_OK + MB_ICONERROR);
      ModalResult = mrCancel;
   }
   else
   {
      IsWeighed = true;
      Weight.Assign(Scales->Weight);
      lbScalesWeight->Caption = Weight.ToStringWithUnit();
      Weight = Weight - TareWeight;
      WeightLabel->Caption = Weight.ToStringWithUnit();
      lbUnitPrice->Caption = FormatFloat("$0.00",UnitPrice);
      if(Weight.Valid())
      {
         if(Weight.GetWeight() < 0)
         {
            PoleDisplay->UpdatePoleDisplay(lbItemName->Caption,"",Weight.ToStringWithUnit(),"Invalid");
            lbTotalPrice->Caption = "Invalid";
            tbtnOk->Enabled = false;
         }
         else
         {
            lbTotalPrice->Caption = FormatFloat("$0.00",UnitPrice * Weight.AsKiloGrams());
            PoleDisplay->UpdatePoleDisplay(lbItemName->Caption,"",Weight.ToStringWithUnit(),lbTotalPrice->Caption);
         }
      }
      else
      {
         lbTotalPrice->Caption = "0.0000";
         PoleDisplay->UpdatePoleDisplay(lbItemName->Caption,"","Invalid Weight","");
      }

   }
}

void TfrmScales::OnStabilityChanged(TSystemEvents *Sender)
{
   if(Scales->IsStable())
   {
      if(TGlobalSettings::Instance().WeightLimit > 0)
      {
          if(TGlobalSettings::Instance().WeightLimit < Scales->Weight.GetWeight())
          {
              Weight.SetWeightIn_Kg(0);
              lbScalesWeight->Caption = Weight.ToStringWithUnit();
              WeightLabel->Caption = Weight.ToStringWithUnit();
              lbTotalPrice->Caption = FormatFloat("$0.00",UnitPrice * Weight.AsKiloGrams());
              PoleDisplay->UpdatePoleDisplay(lbItemName->Caption,"",Weight.ToStringWithUnit(),lbTotalPrice->Caption);
              MessageBox("Weight exceeds maximum value of " + TGlobalSettings::Instance().WeightLimit + " kg. Unable to proceed ", "Weight Exceeds", MB_OK + MB_ICONINFORMATION);
          }
          else if(Weight.GetWeight() > 0)
          {
             tbtnOk->Enabled = true;
          }
      }
      else if(Weight.GetWeight() > 0)
      {
         tbtnOk->Enabled = true;
      }
   }
   else
   {
      tbtnOk->Enabled = false;
   }
}

void __fastcall TfrmScales::tbtnOkMouseClick(TObject *Sender)
{
   ModalResult = mrOk;
}
//---------------------------------------------------------------------------

void __fastcall TfrmScales::tbtnCancelMouseClick(TObject *Sender)
{
   ModalResult = mrCancel;
}

//---------------------------------------------------------------------------

void __fastcall TfrmScales::tbtnAmountMouseClick(TObject *Sender)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad = OpenTouchNumPad("Enter Amount", pmDecimal);

    if(Item->PriceEach()==0.00)
       {

      MessageBox("Items price is already zero \nyou can't set price for such an item","info",MB_OK);
      }

    else if (frmTouchNumpad->ShowModal() == mrOk)
    {
        Currency enteredAmount = frmTouchNumpad->CURResult;
        IsWeighed = false;
        SetWeight(enteredAmount, amount);
    }
}
//---------------------------------------------------------------------------


void __fastcall TfrmScales::tbtnWeightMouseClick(TObject *Sender)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad = OpenTouchNumPad("Enter Weight", pmWeight);

    if (frmTouchNumpad->ShowModal() == mrOk)
    {
        Currency enteredWeight = frmTouchNumpad->CURResult;
        IsWeighed = false;
        SetWeight(enteredWeight, weight);
    }
}

std::auto_ptr <TfrmTouchNumpad> TfrmScales::OpenTouchNumPad(AnsiString caption, TPadMode padMode)
{
    std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad>(Screen->ActiveForm));
    frmTouchNumpad->Caption = caption;
    frmTouchNumpad->btnOk->Visible = true;
    frmTouchNumpad->btnCancel->Visible = true;
    frmTouchNumpad->btnCLR->Visible = true;
    frmTouchNumpad->btnDiscount->Visible = false;
    frmTouchNumpad->btnSurcharge->Visible = false;
    frmTouchNumpad->Mode = padMode;
    frmTouchNumpad->CURInitial = 0;

    return frmTouchNumpad;
}

bool TfrmScales::SetWeight(Currency enteredValue, eEntryMode entryMode)
{
    Currency enteredWeight;
    if(entryMode == amount)
    {
        enteredWeight = (enteredValue/(Item->PriceEach()));
    }
    else
    {
        enteredWeight = enteredValue;
    }

    //check entered weight or price doesn't exceed the value of maximum weight
    if(TGlobalSettings::Instance().WeightLimit >= (enteredWeight + TareWeight.GetWeight())|| TGlobalSettings::Instance().WeightLimit == 0.000)
    {

        //Create a new wieght and assign the entered value to it.
        TWeight tWeight;
        tWeight.Unit = ewuKg;
        tWeight.State = ewuValid;
        tWeight.SetWeightIn_Kg(enteredWeight);

        //Assign the entered value to the Weight, this value will be used in Select Dish to calculate price.
        Weight.SetWeightIn_Kg(enteredWeight);

        //Now add the tare weight for display purpose.
        //We are adding tare weight since the user has manually entered the weight it implies customer want to buy the entered weight so total weight will be the weight entered plus the tare weight.
        //All caculation will be done on the weight entered by user.
        tWeight.SetWeightIn_Kg(tWeight.AsKiloGrams() + TareWeight.AsKiloGrams());

        lbScalesWeight->Caption = Weight.ToStringWithUnit();
        WeightLabel->Caption = tWeight.ToStringWithUnit();
        lbUnitPrice->Caption = FormatFloat("0.00", UnitPrice);

        Currency price = entryMode == amount ? enteredValue : RoundToNearest((UnitPrice * Weight.AsKiloGrams()), MIN_CURRENCY_VALUE, !TGlobalSettings::Instance().MidPointRoundsDown);

        if(Weight.Valid())
        {
            if(Weight.GetWeight() < 0)
            {
                PoleDisplay->UpdatePoleDisplay(lbItemName->Caption, "", tWeight.ToStringWithUnit(), "Invalid");
                lbTotalPrice->Caption = "Invalid";
                tbtnOk->Enabled = false;
                return false;
            }
            else
            {
                lbTotalPrice->Caption = FormatFloat("0.00", price);
                PoleDisplay->UpdatePoleDisplay(lbItemName->Caption, "", Weight.ToStringWithUnit(), lbTotalPrice->Caption);
                tbtnOk->Enabled = true;
                return true;
            }
        }
        else
        {
            lbTotalPrice->Caption = "0.0000";
            PoleDisplay->UpdatePoleDisplay(lbItemName->Caption, "", "Invalid Weight","");
            return false;
        }
    }

    else
    {
        MessageBox("Weight exceeds maximum value of " + TGlobalSettings::Instance().WeightLimit + " kg. Unable to proceed ", "Weight Exceeds", MB_OK + MB_ICONINFORMATION);
    }
}
//---------------------------------------------------------------------------

