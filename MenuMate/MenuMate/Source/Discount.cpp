//---------------------------------------------------------------------------


#pragma hdrstop

#include <cassert>
#include "Discount.h"
#include "ItemSizeCategory.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

#include "ItemMinorComplete.h"
#include "TaxManagement.h"

TDiscount::TDiscount()
{
	Clear();
}

void TDiscount::Clear()
{
   Mode = DiscModeCurrency;
   Source = dsMMUser;
   Type = dtFixed;
   Amount = 0;
   PercentAmount = 0;
   Rounding = 0.01;
   Name = "";
   Description = "";
   DiscountKey = 0;
   DBKey = 0;
   Priority = 0;
   DiscountCode = "";
   AppearanceOrder = 0;
   MembersExempt = false;
   MembersOnly = false;
   ExcludeTax = false;
   MaximumValue = 0;
   ComboAmount = 0;
   Group = 0;
   IsThorBill = false;
   CategoryFilterKeys.clear();
   IsClipp = false;
   MaxItemAffected = 0;
   MinItemRequired = 0;
   IsCloudDiscount = false;
   DailyUsageAllowedPerMember = 0;
   ProductPriority = ppCheapest;
   IsMembershipDiscount = false;
   OriginalAmount = 0;
   OriginalPercentAmount = 0;
   DiscountAppliedTime = 0;
   //IsInitiallyPrompt = false;
}

bool
TDiscount::operator==(TDiscount &rhs)
{
	return Mode == rhs.Mode
          && Source == rhs.Source
          && Type == rhs.Type
          && Amount == rhs.Amount
          && PercentAmount == rhs.PercentAmount
          && Rounding == rhs.Rounding
          && Name == rhs.Name
          && Description == rhs.Description
          && DiscountKey == rhs.DiscountKey
          && DBKey == rhs.DBKey
          && Priority == rhs.Priority
          && AppearanceOrder == rhs.AppearanceOrder
          && MembersExempt == rhs.MembersExempt
          && MembersOnly == rhs.MembersOnly
          && Group == rhs.Group
          && CategoryFilterKeys == rhs.CategoryFilterKeys
          && MaxItemAffected == rhs.MaxItemAffected
          && MinItemRequired == rhs.MinItemRequired
          && IsCloudDiscount == rhs.IsCloudDiscount
          && DailyUsageAllowedPerMember == rhs.DailyUsageAllowedPerMember
          && OriginalAmount == rhs.OriginalAmount
          && OriginalPercentAmount == rhs.OriginalPercentAmount;
}

UnicodeString TDiscount::GetDescription() const
{
   if(Description == "")
   {
      return Name;
   }
   else
   {
      return Description;
   }
}

Currency TDiscount::DiscountValue(Currency value_to_be_discounted, OrderStatus OrderType) const
{
	switch (Mode)
    {
	case DiscModePercent:
    {
		value_to_be_discounted = value_to_be_discounted * PercentAmount / 100.0;
        return -value_to_be_discounted;
	}
	case DiscModeSetPrice:
		return Amount - value_to_be_discounted;
	case DiscModeCombo:
		return Amount - value_to_be_discounted;
    case DiscModeDeal:
		return Amount - value_to_be_discounted;
	case DiscModeCurrency:
		if (Amount > value_to_be_discounted && OrderType != CreditNonExistingOrder)
			return -value_to_be_discounted;
		return -Amount;
       //************4278*********************/
    case DiscModeItem:
		if (Amount > value_to_be_discounted )
			return -value_to_be_discounted;
		return -Amount;  
          //************4278*********************/
	}

	/* This epilogue is unreachable. But, knowing Borland, it's possible it'll
	 * be executed somehow. To catch this case, I've defined the assertion.
	 */
	assert(0);
	return 0;
}

Currency TDiscount::setAmount(Currency inAmount)
{
    if(Source == dsMMMebersPoints && inAmount > MaximumValue)
    {
    	amount = MaximumValue;
    }
    else
    {
		amount = inAmount;
    }
    return amount;
}

Currency TDiscount::setOriginalAmount(Currency inAmount)
{
    if(Source == dsMMMebersPoints && inAmount > MaximumValue)
    {
    	originalamount = MaximumValue;
    }
    else
    {
		originalamount = inAmount;
    }
    return originalamount;
}

bool TDiscount::ContainsCatKey(TListCategoryContainer *Categories) const
{
    bool found = false;
    for (int i = 0; i < Categories->Count && !found ; i++)
    {
        TItemSizeCategory *ItemCategory = Categories->CategoryGet(i);
        if(CategoryFilterKeys.find(ItemCategory->CategoryKey) != CategoryFilterKeys.end())
        {
            found = true;
        }
    }
	return found;
}

bool TDiscount::IsSeniorCitizensDiscount()
{
    DiscountGroupList.size() == 1 && DiscountGroupList[0].Name == SCD_DISCOUNT_GROUP;
}

bool TDiscount::IsPersonWithDisabilityDiscount()
{
    bool isPersonWithDisability = false;

    for(int i = 0; i < DiscountGroupList.size(); i++)
    {
        isPersonWithDisability = DiscountGroupList[i].Name == PWD_DISCOUNT_GROUP;
        if(isPersonWithDisability)
          break;
    }

    return isPersonWithDisability;
}

bool TDiscount::IsComplimentaryDiscount()
{
    DiscountGroupList.size() == 1 && DiscountGroupList[0].Name == COMPLIMENTARY_GROUP;
}

bool TDiscount::IsNonChargableDiscount()
{
    DiscountGroupList.size() == 1 && DiscountGroupList[0].Name == NONCHARGABLE_GROUP;
}
