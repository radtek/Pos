//---------------------------------------------------------------------------


#pragma hdrstop

#include "ChitNumber.h"
#include <StrUtils.hpp>

//---------------------------------------------------------------------------

#pragma package(smart_init)

TChitNumber::TChitNumber(int inChitNumberKey,  UnicodeString inName, UnicodeString inMask,
                         EChitPromptType inType,EChitFormatType inFormatType, int inAppearenceOrder,bool inDefault,
			 EChitTriggerType inTriggerType, UnicodeString inTriggerSelection,
			 const UnicodeString &default_menu_name,bool isHoldAndSendDisabled,bool isNonChargableChit,
                         bool isDisplayTablesEnabled,bool IsCaptureCustomerNameAndPhone, bool IsAutoSaveOnTab,
                         bool IsCaptureCustomerDetails, bool promptForPickUpDeliveryTime, int inAddMinutes, bool isSaveCustomerName,
                         bool isSaveMemberName, bool isSaveMemberAddress, bool isOnlinePickupOrder, bool isOnlineDeliveryOrder) :
      default_menu_name_(default_menu_name),
      active_option_(L""),
      ChitNumberKey(inChitNumberKey),
      Name(inName) ,
      Mask(inMask),
      Type(inType),
      Format(inFormatType),
      TriggerType(inTriggerType),
      TriggerSelection(inTriggerSelection),
      AppearenceOrder(inAppearenceOrder),
      Default(inDefault),
      IsHoldAndSendDisabled(isHoldAndSendDisabled),
      IsNonChargableChit(isNonChargableChit),
      IsDisplayTablesEnabled(isDisplayTablesEnabled),
      IsCaptureCustomerNameAndPhone(IsCaptureCustomerNameAndPhone),
      IsAutoSaveOnTab(IsAutoSaveOnTab),
      IsCaptureCustomerDetails(IsCaptureCustomerDetails),
      PromptForPickUpDeliveryTime(promptForPickUpDeliveryTime),
      AddMinutes(inAddMinutes),
      SaveCustomerName(isSaveCustomerName),
      SaveMemberName(isSaveMemberName),
      SaveMemberAddress(isSaveMemberAddress),
      OnlinePickupOrder(isOnlinePickupOrder),
      OnlineDeliveryOrder(isOnlineDeliveryOrder)
{
   RemovedTaxList = new TStringList;
   RemovedServiceChargeList = new TStringList;
   ApplyDiscountsList = new TStringList;
}

const bool
TChitNumber::HasAnOption()
const
{
    return active_option_.Length() != 0;
}


TChitNumber::TChitNumber()
{
   Clear();
}

bool TChitNumber::Valid()
const
{
   return (ChitNumberKey > 0);
}

bool TChitNumber::Assigned()
const
{
   return (ChitNumber != "");
}

bool TChitNumber::Enabled()
const
{
   return (Type != ectOff);
}

bool TChitNumber::Clear()
{
   ChitNumberKey = 0;
   ChitNumber = "";
   Type = ectOff;
   Name = "Chit Number";
   Mask = "";
   Default = false;
   TriggerType = ecttOff;
   Format = ectNumeric;
   TriggerSelection = "";
   default_menu_name_ = L"";
   active_option_ = L"";
   IsHoldAndSendDisabled = false;
   IsNonChargableChit = false;
   IsDisplayTablesEnabled = false;
   IsCaptureCustomerNameAndPhone = false;
   IsAutoSaveOnTab = false;
   IsCaptureCustomerDetails = false;
   RemovedTaxList = new TStringList;
   RemovedServiceChargeList = new TStringList ;
   PromptForPickUpDeliveryTime = false;
   DeliveryTime = 0;
   AddMinutes = 0;
   ApplyDiscountsList = new TStringList;
   SaveCustomerName = false;
   SaveMemberName = false;
   SaveMemberAddress = false;
   OnlinePickupOrder = false;
   OnlineDeliveryOrder = false;
}

const UnicodeString &
TChitNumber::ActiveOption()
const
{
    return active_option_;
}

const UnicodeString &
TChitNumber::ActiveOption(const UnicodeString &name)
{
    return active_option_ = name;
}

UnicodeString TChitNumber::GetChitNumber()
{
    if (Format == ectList) {
        if (active_option_.Length())
            return active_option_;
        else if (ChitNumber.Length())
            return ChitNumber;
    }

   UnicodeString Chit = Mask + ChitNumber;
   if(Mask.Pos("%") != 0)
   {
      Chit = AnsiReplaceStr(Mask, "%", ChitNumber);
   }
   else if(Mask != "")
   {
      Chit = Mask + " " + ChitNumber;
   }
   else
   {
      if(Assigned())
      {
         Chit = Name + " " + ChitNumber; //this to get chit name
      }
      else
      {
         Chit = Name;
      }
   }
   return Chit;
}

const UnicodeString &
TChitNumber::DefaultMenu()
const
{
    return default_menu_name_;
}

bool
TChitNumber::DefaultMenu(const UnicodeString &default_menu)
{
    return (default_menu_name_ = default_menu).Length() != 0;
}

bool
TChitNumber::HasDefaultMenu()
const
{
    return default_menu_name_.Length() != 0;
}

