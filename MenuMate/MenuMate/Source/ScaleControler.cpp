//---------------------------------------------------------------------------


#pragma hdrstop

#include "ScaleControler.h"
#include "Scales.h"
#include "MMTouchNumpad.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TScaleController::TScaleController(TForm * inOwner, TWeighScale *inScales,TPoleDisplay *inPoleDisplay,TItemMinorComplete *inItem, bool inAutoConvertScalesWeight)
{
   Owner = inOwner;
   Scales = inScales;
   PoleDisplay = inPoleDisplay;
   Item = inItem;
   AutoConvertScalesWeight = inAutoConvertScalesWeight;
}

TModalResult TScaleController::GetWeight(TWeight &Weight)
{
   if (Item->PriceEach() == -999.99)
   {
      std::auto_ptr <TfrmTouchNumpad> frmTouchNumpad(TfrmTouchNumpad::Create <TfrmTouchNumpad> (Owner));
      frmTouchNumpad->Caption = "Enter Amount";
      frmTouchNumpad->btnSurcharge->Caption = "Ok";
      frmTouchNumpad->btnDiscount->Visible = false;
      frmTouchNumpad->btnSurcharge->Visible = true;
      frmTouchNumpad->Mode = pmCurrency;
      frmTouchNumpad->CURInitial = 0;
      if (frmTouchNumpad->ShowModal() == mrOk)
      {
         Item->SetPriceLevelCustom(frmTouchNumpad->CURResult);
      	Item->PriceLevel0 = frmTouchNumpad->CURResult;
         Item->PriceLevel1 = frmTouchNumpad->CURResult;
      }
       else
       {
           Item->SetPriceLevelCustom(0);
           Item->PriceLevel0 = 0;
           Item->PriceLevel1 = 0;
       }
   }

   std::auto_ptr<TfrmScales> frmScales (new TfrmScales(Owner, Scales, PoleDisplay, Weight, Item->WeightedPrice.TareWeight, AutoConvertScalesWeight, Item));
   if(Item->Size != "")
   {
      frmScales->lbItemName->Caption = Item->Size + " " + Item->Item;
   }
   else
   {
      frmScales->lbItemName->Caption = Item->Item;
   }
   frmScales->UnitPrice = Item->PriceEach();
   TModalResult result = frmScales->GetWeight();
   WeighedItem = frmScales->IsWeighed;
   return result;
}
