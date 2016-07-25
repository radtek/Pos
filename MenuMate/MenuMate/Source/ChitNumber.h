//---------------------------------------------------------------------------

#ifndef ChitNumberH
#define ChitNumberH
//---------------------------------------------------------------------------

#include <Classes.hpp>
#include <set>
#include <vector>

#include "EnumChit.h"

class TChitNumber
{
    private:
        UnicodeString default_menu_name_;
		UnicodeString active_option_;


   public :
   TChitNumber(int inChitNumberKey,
               UnicodeString inName,
               UnicodeString inMask,
               EChitPromptType inType,
               EChitFormatType inFormatType,
               int inAppearenceOrder,
               bool inDefault,
               EChitTriggerType inTriggerType,
               UnicodeString inSelectionName,
	       const UnicodeString &default_menu_name,
	       bool isHoldAndSendDisabled,
               bool isNonChargableChit,
               bool isDisplayTablesEnabled,
               bool IsCaptureCustomerNameAndPhone,
               bool IsAutoSaveOnTab,
               bool IsCaptureCustomerDetails,
               bool promptForPickUpDeliveryTime,
               int inAddMinutes,
               bool isSaveCustomerName,
               bool isSaveMemberName,
               bool isSaveMemberAddress,
               bool isOnlinePickupOrder,
               bool isOnlineDeliveryOrder);

   TChitNumber();
   int ChitNumberKey;
   UnicodeString ChitNumber;

   const UnicodeString &DefaultMenu() const;
   bool DefaultMenu(const UnicodeString &default_menu);
   bool HasDefaultMenu() const;

    const bool HasAnOption() const;
    const UnicodeString &ActiveOption() const;
    const UnicodeString &ActiveOption(const UnicodeString &name);

   UnicodeString Name;
   UnicodeString Mask;
   EChitPromptType Type;
   EChitTriggerType TriggerType;
   EChitFormatType Format;
   UnicodeString TriggerSelection;
   UnicodeString GetChitNumber();
   bool IsHoldAndSendDisabled;

   //MM-1647 Added a new setting to chit to show table display if chit is selected.
   bool IsDisplayTablesEnabled;
   bool IsNonChargableChit;
   bool IsCaptureCustomerNameAndPhone;
   bool IsAutoSaveOnTab;
   bool IsCaptureCustomerDetails;
   int  AppearenceOrder;
   bool Default;
   bool Valid() const;
   bool Assigned() const;
   bool Clear();
   bool Enabled() const;
   TStringList* RemovedTaxList;
   TStringList* RemovedServiceChargeList;
   TStringList* ApplyDiscountsList;


   bool PromptForPickUpDeliveryTime;
   TDateTime DeliveryTime;
   int AddMinutes;
   bool SaveCustomerName;
   bool SaveMemberName;
   bool SaveMemberAddress;
   bool OnlinePickupOrder;
   bool OnlineDeliveryOrder;
};
#endif
