//---------------------------------------------------------------------------


#pragma hdrstop

#include "Payment.h"
#include <Graphics.hpp>
#include "Rounding.h"
#include "GlobalSettings.h"
#include "PaymentTransaction.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

TPayment::TPayment(TPaymentTransaction *inOwner) : Owner(inOwner)
{
    Result = eProcessing;
    FPay = 0;
    FPayRounding = 0;
    FCashOut = 0;
    FCashOutRounding = 0;
    DisplayOrder = 0;
    GroupNumber = 0;
    RefundPointsVal = 0;
    AdjustmentReason = "";
    AmountAdjust = 0;
    PercentAdjust = 0;
    CSVNumber = 0;
    Name = "";
    NameOveride = "";
    ReferenceNumber = "";
    ExternalReferenceNumber = "";
    VoucherCode = "";
    FixedVoucherCode = "";
    VoucherMerchantID = "";
    SysNameOveride = "";
    TabCreditKey = 0;
    TaxRate = 0;
    Colour = clWhite;
    Visible = true;
    CreditTransaction = false;
    SuppressEftPosReceipt = false;
    PaymentThirdPartyID = "";
    SecondaryPMSIPAddress = "";
    SecondaryPMSPortNumber = 0;
    RoundTo = MIN_CURRENCY_VALUE;
    UniVoucherURL = "";
    UniVoucherUser = "";
    UniVoucherPass = "";
    UniVoucherToken = "";
    CardType = "";
    RefundPoints = false;
    TabKey=0;
    TabName="";
    GLCode = "";
    TipAmount = 0;
    AutoPopulateBlindBalance = false;
    _assignedPaymentGroups.clear();
    Properties.clear();
}

void TPayment:: operator = (const TPayment & Data)
{
   FPay = Data.FPay;
   FCashOut = Data.FCashOut;
   FPayRounding = Data.FPayRounding;
   FCashOutRounding = Data.FCashOutRounding;
   FAdjustment = Data.FAdjustment;
   FAdjustmentRounding = Data.FAdjustmentRounding;
   AdjustmentReason = Data.AdjustmentReason;
   AmountAdjust = Data.AmountAdjust;
   Colour = Data.Colour;
   CreditTransaction = Data.CreditTransaction;
   CSVNumber = Data.CSVNumber;
   DisplayOrder = Data.DisplayOrder;
   Result = Data.Result;
   GroupNumber = Data.GroupNumber;
   Name = Data.Name;
   NameOveride = Data.NameOveride;
   Note = Data.Note;
   VoucherCode = Data.VoucherCode;
   FixedVoucherCode = Data.FixedVoucherCode;
   VoucherMerchantID = Data.VoucherMerchantID;
   PaymentThirdPartyID = Data.PaymentThirdPartyID;
   PercentAdjust = Data.PercentAdjust;
   ReferenceNumber = Data.ReferenceNumber;
   ExternalReferenceNumber = Data.ExternalReferenceNumber;
   Result = Data.Result;
   SuppressEftPosReceipt = Data.SuppressEftPosReceipt;
   SysNameOveride = Data.SysNameOveride;
   TabCreditKey = Data.TabCreditKey;
   TaxRate = Data.TaxRate;
   Visible = Data.Visible;
   UniVoucherURL = Data.UniVoucherURL;
   UniVoucherUser = Data.UniVoucherUser;
   UniVoucherPass = Data.UniVoucherPass;
   UniVoucherToken = Data.UniVoucherToken;
   CardType = Data.CardType;
   TabKey=Data.TabKey;
   TabName= Data.TabName;
   GLCode = Data.GLCode;
   TipAmount = Data.TipAmount;
   SetAssignedGroups( Data.GetAssignedGroups() );
   Properties = Data.Properties;
}

void TPayment::Reset()
{
   Result = eProcessing;
   if (!GetPaymentAttribute(ePayTypeCredit))
   {
	  SetPay(0);
	  SetChange(0);
	  NameOveride = "";
   }
   SetCashOut(0);
   SetAdjustment(0);
   CSVNumber = 0;
   ReferenceNumber = "";
   ExternalReferenceNumber = "";
   VoucherCode = "";
   CreditTransaction = false;
   SuppressEftPosReceipt = false;
   TipAmount = 0;
}

void TPayment::Failed()
{
   if (Name != CREDIT)
   {
	  SetPay(0);
	  SetChange(0);
   }
   SetCashOut(0);
   SetAdjustment(0);
   CSVNumber = 0;
   NameOveride = "";
   ReferenceNumber = "";
   ExternalReferenceNumber = "";
   TipAmount = 0;
}

void __fastcall TPayment::SetPay(Currency value)
{
   if (FPay != value)
   {
	  FPay = value;

	  Currency RoundedVal = RoundToNearest(value, RoundTo, TGlobalSettings::Instance().MidPointRoundsDown);
	  FPayRounding = RoundedVal - FPay;
	  SetChange(0);
	  if (Owner != NULL)
	  {
		 Owner->Money.Recalc(*Owner);
	  }
   }
}

void __fastcall TPayment::SetChange(Currency value)
{
   if (FChange != value)
   {
	  FChange = value;
   }
}

void __fastcall TPayment::SetCashOut(Currency value)
{
   if (FCashOut != value)
   {
	  FCashOut = value;

	  Currency RoundedVal = RoundToNearest(value, RoundTo, TGlobalSettings::Instance().MidPointRoundsDown);
	  FCashOutRounding = RoundedVal - FCashOut;
	  if (Owner != NULL)
	  {
		 Owner->Money.Recalc(*Owner);
	  }
   }
}

void __fastcall TPayment::SetRefundPointsValue(Currency value)
{
    RefundPointsVal = value;
}

Currency __fastcall TPayment::GetPay()
{
   return FPay;
}

Currency __fastcall TPayment::GetRefundPointsValue()
{
   return RefundPointsVal;
}

Currency __fastcall TPayment::GetCashOut()
{
   return FCashOut;
}

Currency __fastcall TPayment::GetPayTotal()
{
   // Total Paid by this Payment Type.
   // This needs to calculate in the change but it dosn't at the moment.
   return FPay + FPayRounding - FChange;
}

Currency __fastcall TPayment::GetAdjustment()
{
   return FAdjustment;
}

Currency __fastcall TPayment::GetAdjustmentRounding()
{
   // Total Paid by this Payment Type.
   return FAdjustmentRounding;
}

Currency __fastcall TPayment::GetSurcharge()
{
   if(FAdjustment > 0)
   {
   	return FAdjustment;
   }
   else
   {
      return 0;
   }
}

Currency __fastcall TPayment::GetSurchargeRounding()
{
   if(FAdjustment > 0)
   {
   	return FAdjustmentRounding;
   }
   else
   {
      return 0;
   }
}

Currency __fastcall TPayment::GetDiscount()
{
   if(FAdjustment < 0)
   {
   	return FAdjustment;
   }
   else
   {
      return 0;
   }
}

Currency __fastcall TPayment::GetDiscountRounding()
{
   if(FAdjustment < 0)
   {
   	return FAdjustmentRounding;
   }
   else
   {
      return 0;
   }
}

Currency __fastcall TPayment::GetSurchargeTotal()
{
   if(FAdjustment > 0)
   {
   	return FAdjustment + FAdjustmentRounding;
   }
   else
   {
      return 0;
   }
}

Currency __fastcall TPayment::GetDiscountTotal()
{
   if(FAdjustment < 0)
   {
   	return FAdjustment + FAdjustmentRounding;
   }
   else
   {
      return 0;
   }
}

Currency __fastcall TPayment::GetSurchargeExcGST()
{
	Currency RetVal = 0;
	Currency Total = GetSurchargeTotal();
	if (Total != 0)
	{
		RetVal += ((Total * 100) / (TaxRate + 100.0));
	}
	return RetVal;
}

Currency __fastcall TPayment::GetSurchargeGSTContent()
{
	Currency RetVal = 0;
	Currency Total = GetSurchargeTotal();
	if (Total != 0)
	{
		RetVal += Total - ((Total * 100) / (TaxRate + 100.0));
	}
	return RetVal;
}

Currency __fastcall TPayment::GetDiscountExcGST()
{
	Currency RetVal = 0;
	Currency Total = GetDiscountTotal();
	if (Total != 0)
	{
		RetVal += ((Total * 100) / (TaxRate + 100.0));
	}
	return RetVal;
}

Currency __fastcall TPayment::GetDiscountGSTContent()
{
	Currency RetVal = 0;
	Currency Total = GetDiscountTotal();
	if (Total != 0)
	{
		RetVal += Total - ((Total * 100) / (TaxRate + 100.0));
	}
	return RetVal;
}

void __fastcall TPayment::SetAdjustment(Currency value)
{
   if (FAdjustment != value)
   {
	  FAdjustment = value;

	  Currency RoundedVal = RoundToNearest(value, RoundTo,TGlobalSettings::Instance().MidPointRoundsDown);
	  FAdjustmentRounding = RoundedVal - FAdjustment;
	  if (Owner != NULL)
	  {
		 Owner->Money.Recalc(*Owner);
	  }
   }
}

Currency __fastcall TPayment::GetPayTendered()
{
   // Total Tendered by this Payment Type.
   return FPay + FPayRounding;
}

Currency __fastcall TPayment::GetPayRounding()
{
   // Total Paid by this Payment Type.
   return FPayRounding;
}

Currency __fastcall TPayment::GetCashOutRounding()
{
   // Total Paid by this Payment Type.
   return FCashOutRounding;
}

Currency __fastcall TPayment::GetRoundingTotal()
{
   // Total Paid by this Payment Type.
   return GetPayRounding() + GetCashOutRounding() + GetAdjustmentRounding();
}

Currency __fastcall TPayment::GetPayCashOutTotal()
{
   // Total Paid by this Payment Type.
   return GetPayTotal() + GetCashOutTotal();
}

Currency __fastcall TPayment::GetCashOutTotal()
{
   // Total Paid by this Payment Type.
   return FCashOutRounding + FCashOut;
}

Currency __fastcall TPayment::GetChange()
{
   // Total Change in this Payment Type.
   return FChange;
}

std::vector<TPaymentTypeGroup> TPayment::GetAssignedGroups() const
{
    return _assignedPaymentGroups;
}

TPaymentTypeGroup TPayment::GetFirstAssignedGroup()
{
    TPaymentTypeGroup group(-1);

    if(_assignedPaymentGroups.size() > 0 )
    {
        group = _assignedPaymentGroups.front();
    }

    return group;
}

void TPayment::SetAssignedGroups( std::vector<TPaymentTypeGroup> groups )
{
   _assignedPaymentGroups.clear();
   _assignedPaymentGroups.assign( groups.begin(), groups.end() );
}

void TPayment::SetAssignedGroups( TPaymentTypeGroup group )
{
   _assignedPaymentGroups.clear();
   _assignedPaymentGroups.push_back( group );
}

void TPayment::SetPaymentAttribute(ePaymentAttribute attributeIndex,bool attributeValue)
{
  if(attributeValue && !(Properties.find(attributeIndex) != Properties.end()))
    Properties.insert(attributeIndex);
}

void TPayment::AssignPaymentAttribute(TPayment &payment)
{
  Properties = payment.Properties;
}

void TPayment::ClearPaymentAttribute()
{
  Properties.clear();
}

bool TPayment::GetPaymentAttribute(ePaymentAttribute attributeIndex)
{
  bool retVal = false;
  if(Properties.find(attributeIndex) != Properties.end())
     retVal = true;
  return false;
}

AnsiString TPayment::GetPropertyString()
{
   AnsiString propStr = "-";
   for(std::set<int>::iterator it = Properties.begin() ; it != Properties.end() ;advance(it,1))
   {
        int attribute = *it;
        propStr = propStr + IntToStr(attribute) + "-";
   }
   return propStr;
}

bool TPayment::IsLoyaltyVoucher()
{
   return GetPaymentAttribute(ePayTypeGetVoucherDetails) && (Name == "Gift Card" || Name == "Voucher" );
}

bool TPayment::IsLoyaltyGiftCard()
{
   return GetPaymentAttribute(ePayTypeGetVoucherDetails) && (Name == "Gift Card");
}
