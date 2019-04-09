//---------------------------------------------------------------------------

#ifndef ItemMinorH
#define ItemMinorH

#include "MMPrice.h"
#include "PrintInfo.h"
#include "ServingCourse.h"
#include "GlobalSettings.h"
#include "TaxProfile.h"
#include <vector>
#include "MM_Menu.h"

enum eFreeSource {eFreeLocation, eFreeMember};

class TItemMinor
{
private:
	bool can_be_paid_for_using_points_;

	virtual double GetPointsPercent() const;
    virtual bool GetIsManuallyEnteredWeight() const;

protected:
	TMMPrice FPrice;
	double fPointsPercent;
    bool fIsManuallyEnteredWeight;

        //MM-6306 : double data type in place of currency
        __property double Qty = {
                read = GetQty,
                write = SetQty
        };

public:
        bool CanBePaidForUsingPoints(const bool);
        bool CanBePaidForUsingPoints() const;

	TPrintInfo FontInfo;

	TItemMinor();
   TItemMinor(const TItemMinor &);
	__fastcall virtual ~TItemMinor();
   TItemMinor &operator=(const TItemMinor &);

	UnicodeString Item;
	UnicodeString ItemKitchenName;
	UnicodeString Size;
	UnicodeString SizeKitchenName;
	UnicodeString MenuName;
	UnicodeString Course;
	UnicodeString CourseKitchenName;
	TServingCourse ServingCourse;

	int ItemKey;
	int ItemAppearanceOrder;

	int PLU;

	Currency Cost;
    Currency MaxRetailPrice;

	Currency Redeemed; // Value of Points Redeemd.
	UnicodeString DiscountReason;
	double PointsEarned; // Total points earned for this item, can be Zero
	// if the member does not earn points.

	Currency PriceLevelCustom; // Custom Price Level.

	Currency PriceLevel0; // RRP
	Currency PriceLevel1;

	double CostGSTPercent;
	double GSTPercent;
    std::vector<TaxProfile> TaxProfiles;
    std::map<int,Menu::TItemSizePriceLevel> PriceLevels;

	__property double PointsPercent =
	{
		read = GetPointsPercent,
		write = fPointsPercent
	};

	int MemberFreeSaleCount;
	double MemberFreeSaleDiscount;
	int LocationFreeSaleCount;
	double LocationFreeSaleDiscount;

	unsigned long SetMenuMask;
	bool SetMenuMaster;
	bool SetMenu; // Ordered via a master
	long SetMenuGroup;
	int SelectedItems;
	int PrevSelectedItems;
	int DealSelectedItems;
    int PrevDealSelectedItems;

	bool IsFreebe;
	eFreeSource FreeSource;

	long ThirdPartyKey;
	UnicodeString ThirdPartyCode;

	void virtual Assign(TItemMinor * Minor);

	Currency PriceEach();
	Currency Price() const;
	void ClearPrice();
	void InversePrice();
	void ResetQty();
	void CreditQty();
	void AddPrice(TMMPrice &Price);
	void SetPrice(TMMPrice &inPrice);

    double GetQty() const;  //MM-6306 : double data type in place of currency
    virtual void SetQty(const double new_qty);  //MM-6306 : double data type in place of currency

	bool IsPriceBarcodedItem;

      __property bool isManuallyEnteredWeight =
	{
		read = GetIsManuallyEnteredWeight,
		write = fIsManuallyEnteredWeight
	};

    Currency CostForPoints;
    //Currency CostForPoints;
    bool IsCanBePaidForUsingPoints;
    int RevenueCode;
    int ItemSizeIdentifierKey;
    UnicodeString ItemIdentifierKey;

private:
        double qty_;    //MM-6306 : double data type in place of currency
};

#endif
