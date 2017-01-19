// ---------------------------------------------------------------------------

#ifndef DiscountH
#define DiscountH

#include <vcl.h>
#include <set>
#include <vector>
#include "EnumOrderStatus.h"
#include "ListCategoryContainer.h"

#include "DiscountGroup.h"
/* Key used for system generated members points discount*/
/* TODO 1 -o Michael -c Kludge : This code is less than ideal
we are fudging the creation of a Discount here and the  Manager Dicount will
recoginse the Key and supply an appropriate system discount when queryied.
In an ideal world the membership discount around points would be a system
discount and these 'system discounts' would live in the DB and
show where appropriate. */
#define dsMMMebersPointsKEY -2
#define dsMMDealKey -3
// ---------------------------------------------------------------------------
enum TDiscountMode
{
	DiscModeCurrency, DiscModePercent, DiscModeSetPrice, DiscModeCombo, DiscModeDeal , DiscModeItem,DiscModePoints
};

enum TDiscountType
{
	dtFixed, dtPromptDescription, dtPromptAmount, dtPromptDescriptionAmount
};

enum TProductPriority
{
	ppNone, ppCheapest, ppHighest
};

/* I have had to move the system Discount to 100, Discount that appear after the system discount
are not remvoed by the standard xxxxx.Clear() type calls. Currently discounts of type dsMMSystem are not used*/
enum TDiscountSource
{
	dsMMUser, dsMMMembership, dsMMMembershipReward, dsMMLocationReward, dsMMMebersPoints,dsMMSystem=100
};

/* TODO :
Add begin and end times array.
Add a priority for Ordering the Discounts so Holiday surcharges can be applied last. */
class TDiscount
{
private :
	Currency amount;
    Currency originalamount;
public:
	bool operator==(TDiscount &rhs);

	TDiscountMode Mode;
	TDiscountType Type;
	UnicodeString DiscountCode;
	TDiscountSource Source;
	Currency ComboAmount;
	Currency Rounding;
	double PercentAmount;
	UnicodeString Name;
	UnicodeString Description;
    UnicodeString VoucherCode;
	int DBKey;
	int DiscountKey;
	int Priority;
	int AppearanceOrder;
    int Group;
    bool IsThorBill;
    bool MembersExempt;
    bool MembersOnly;
    bool ExcludeTax;
    bool IsClipp;
    int MaxItemAffected;
    int MinItemRequired;
    bool IsCloudDiscount;
	Currency MaximumValue;
    int DailyUsageAllowedPerMember;
    TProductPriority ProductPriority;
    bool IsMembershipDiscount;

    std::set<int> CategoryFilterKeys;
    std::vector<TDiscountGroup> DiscountGroupList;
	void Clear();
	Currency setAmount(Currency amount);
	TDiscount();
	UnicodeString GetDescription()const;
    __property Currency Amount = {read=amount, write=setAmount};
	Currency DiscountValue(Currency value_to_be_discounted, OrderStatus OrderType) const;
	bool ContainsCatKey(TListCategoryContainer *Categories) const;
    bool IsSeniorCitizensDiscount();
    bool IsPersonWithDisabilityDiscount();
    bool IsComplimentaryDiscount();
    bool IsNonChargableDiscount();
	bool operator < (const TDiscount & f1)
	{
		bool Retval = false;
		if (Priority < f1.Priority)
		{
			Retval = true;
		}
		else if (Priority == f1.Priority)
		{
			if (DiscountKey < f1.DiscountKey)
			{
				Retval = true;
			}
		}
		return Retval;
	}friend bool operator < (const TDiscount & f1, const TDiscount & f2);
    Currency setOriginalAmount(Currency originalamount);
    __property Currency OriginalAmount = {read=originalamount, write=setOriginalAmount};
};

bool operator < (const TDiscount & f1, const TDiscount & f2)
{
	bool Retval = false;
	if (f1.Priority < f2.Priority)
	{
		Retval = true;
	}
	else if (f1.Priority == f2.Priority)
	{
		if (f1.DiscountKey < f2.DiscountKey)
		{
			Retval = true;
		}
	}
	return Retval;
};
#endif
