//---------------------------------------------------------------------------


#pragma hdrstop

#include "ItemMinor.h"
#include "math.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

void
TItemMinor::SetQty(
    const double new_qty)   //MM-6306 : double data type in place of currency
{
        qty_ = new_qty;
}

double
TItemMinor::GetQty()    //MM-6306 : double data type in place of currency
const
{
        return qty_;
}

TItemMinor::TItemMinor()
{
   ItemKey = 0;
   SetMenuMask = 0;
   SetMenuMaster = false;
   SetMenu = false;
   SetMenuGroup = 0;
   Qty = 1;
   FPrice = TMMPrice();
   Cost = 0;
   CostGSTPercent = 0;
   //GSTPercent = 0;
   PointsPercent = 0;
   Redeemed = 0;
   PointsEarned = 0;
   ItemAppearanceOrder = 0;
   ItemKitchenName = "";
   CourseKitchenName = "";
   SizeKitchenName = "";
   ThirdPartyCode = "";
	ServingCourse.Clear();
	SelectedItems = 0;
	PrevSelectedItems = 0;
	DealSelectedItems = 0;
    PrevDealSelectedItems = 0;

   MemberFreeSaleCount = 0;
   MemberFreeSaleDiscount = 0;
   LocationFreeSaleCount = 0;
   LocationFreeSaleDiscount = 0;
   PriceLevelCustom = 0;
   IsFreebe = false;
   FreeSource = eFreeLocation;
   PLU = -1;

   can_be_paid_for_using_points_ = 0;
   IsPriceBarcodedItem = false;

   isManuallyEnteredWeight=false;
   CostForPoints = 0;
   //CostForPoints = 0;
   IsCanBePaidForUsingPoints = false;  // check can be paid by points..
   ItemSizeIdentifierKey = 0;
}

TItemMinor::TItemMinor(
  const TItemMinor &initializer)
{
   ItemKey = initializer.ItemKey;
   Qty = initializer.Qty;
   Cost = initializer.Cost;
   MaxRetailPrice = initializer.MaxRetailPrice;
   CostGSTPercent = initializer.CostGSTPercent;
   Course = initializer.Course;
   //GSTPercent = initializer.GSTPercent;
   TaxProfiles = initializer.TaxProfiles;
   Item = initializer.Item;
   MenuName = initializer.MenuName;
   FPrice = initializer.FPrice;
   Redeemed = initializer.Redeemed;
   PriceLevel0 = initializer.PriceLevel0;
   PriceLevel1 = initializer.PriceLevel1;
   PointsEarned = initializer.PointsEarned;
   PointsPercent = initializer.PointsPercent;
   SetMenu = initializer.SetMenu;
   SetMenuGroup = initializer.SetMenuGroup;
   SetMenuMask = initializer.SetMenuMask;
   SetMenuMaster = initializer.SetMenuMaster;
   Size = initializer.Size;
   ThirdPartyKey = initializer.ThirdPartyKey;
   ThirdPartyCode = initializer.ThirdPartyCode;
   ItemKitchenName = initializer.ItemKitchenName;
   SizeKitchenName = initializer.SizeKitchenName;
   CourseKitchenName = initializer.CourseKitchenName;
   ServingCourse = initializer.ServingCourse;
   DiscountReason = initializer.DiscountReason;
   IsFreebe = initializer.IsFreebe;
   MemberFreeSaleCount = initializer.MemberFreeSaleCount;
   LocationFreeSaleCount = initializer.LocationFreeSaleCount;
   ItemAppearanceOrder = initializer.ItemAppearanceOrder;
   MemberFreeSaleDiscount = initializer.MemberFreeSaleDiscount;
   LocationFreeSaleDiscount = initializer.LocationFreeSaleDiscount;
   PriceLevelCustom = initializer.PriceLevelCustom;
   PLU = initializer.PLU;
   can_be_paid_for_using_points_ = initializer.can_be_paid_for_using_points_;
   IsPriceBarcodedItem = initializer.IsPriceBarcodedItem;
   PriceLevels=initializer.PriceLevels;
   isManuallyEnteredWeight =initializer.isManuallyEnteredWeight;
   CostForPoints = initializer.CostForPoints;
   IsCanBePaidForUsingPoints = initializer.IsCanBePaidForUsingPoints;  // check can be paid by points..
   ItemSizeIdentifierKey = initializer.ItemSizeIdentifierKey;
}

TItemMinor &
TItemMinor::operator=(
  const TItemMinor &rhs)
{
   if (this == &rhs)
      return *this;

   ItemKey = rhs.ItemKey;
   Qty = rhs.Qty;
   Cost = rhs.Cost;
   MaxRetailPrice = rhs.MaxRetailPrice;
   CostGSTPercent = rhs.CostGSTPercent;
   Course = rhs.Course;
   //GSTPercent = rhs.GSTPercent;
   TaxProfiles = rhs.TaxProfiles;
   Item = rhs.Item;
   MenuName = rhs.MenuName;
   FPrice = rhs.FPrice;
   Redeemed = rhs.Redeemed;
   PriceLevel0 = rhs.PriceLevel0;
   PriceLevel1 = rhs.PriceLevel1;
   PointsEarned = rhs.PointsEarned;
   PointsPercent = rhs.PointsPercent;
   SetMenu = rhs.SetMenu;
   SetMenuGroup = rhs.SetMenuGroup;
   SetMenuMask = rhs.SetMenuMask;
   SetMenuMaster = rhs.SetMenuMaster;
   Size = rhs.Size;
   ThirdPartyKey = rhs.ThirdPartyKey;
   ThirdPartyCode = rhs.ThirdPartyCode;
   ItemKitchenName = rhs.ItemKitchenName;
   SizeKitchenName = rhs.SizeKitchenName;
   CourseKitchenName = rhs.CourseKitchenName;
   ServingCourse = rhs.ServingCourse;
   DiscountReason = rhs.DiscountReason;
   IsFreebe = rhs.IsFreebe;
   MemberFreeSaleCount = rhs.MemberFreeSaleCount;
   LocationFreeSaleCount = rhs.LocationFreeSaleCount;
   ItemAppearanceOrder = rhs.ItemAppearanceOrder;
   MemberFreeSaleDiscount = rhs.MemberFreeSaleDiscount;
   LocationFreeSaleDiscount = rhs.LocationFreeSaleDiscount;
   PriceLevelCustom = rhs.PriceLevelCustom;
   PLU = rhs.PLU;
   can_be_paid_for_using_points_ = rhs.can_be_paid_for_using_points_;
   IsPriceBarcodedItem = rhs.IsPriceBarcodedItem;
   PriceLevels= rhs.PriceLevels;
   isManuallyEnteredWeight= rhs.isManuallyEnteredWeight;
   CostForPoints = rhs.CostForPoints;
   //CostForPoints = rhs.CostForPoints;
   IsCanBePaidForUsingPoints = rhs.IsCanBePaidForUsingPoints;  // check can be paid by points..
   ItemSizeIdentifierKey = rhs.ItemSizeIdentifierKey;

}

bool
TItemMinor::CanBePaidForUsingPoints()
const
{
        return can_be_paid_for_using_points_;
}

bool
TItemMinor::CanBePaidForUsingPoints(const bool yes_or_no)
{
        return can_be_paid_for_using_points_ = yes_or_no;
}

__fastcall TItemMinor::~TItemMinor()
{
}

void TItemMinor::Assign(TItemMinor * Minor)
{
   Minor->ItemKey = ItemKey;
   Minor->Qty = Qty;
   Minor->Cost = Cost;
   Minor->MaxRetailPrice = MaxRetailPrice;
   Minor->CostGSTPercent = CostGSTPercent;
   Minor->Course = Course;
   //Minor->GSTPercent = GSTPercent;
   Minor->TaxProfiles = TaxProfiles;
   Minor->Item = Item;
   Minor->MenuName = MenuName;
   Minor->FPrice = FPrice;
   Minor->Redeemed = Redeemed;
   Minor->PriceLevel0 = PriceLevel0;
   Minor->PriceLevel1 = PriceLevel1;
   Minor->PointsEarned = PointsEarned;
   Minor->PointsPercent = PointsPercent;
   Minor->SetMenu = SetMenu;
   Minor->SetMenuGroup = SetMenuGroup;
   Minor->SetMenuMask = SetMenuMask;
   Minor->SetMenuMaster = SetMenuMaster;
   Minor->Size = Size;
   Minor->ThirdPartyKey = ThirdPartyKey;
   Minor->ThirdPartyCode = ThirdPartyCode;
   Minor->ItemKitchenName = ItemKitchenName;
   Minor->SizeKitchenName = SizeKitchenName;
   Minor->CourseKitchenName = CourseKitchenName;
   Minor->ServingCourse = ServingCourse;
   Minor->DiscountReason = DiscountReason;

   Minor->IsFreebe = IsFreebe;
   Minor->MemberFreeSaleCount = MemberFreeSaleCount;
   Minor->LocationFreeSaleCount = LocationFreeSaleCount;

   Minor->ItemAppearanceOrder = ItemAppearanceOrder;

   Minor->MemberFreeSaleDiscount = MemberFreeSaleDiscount;
   Minor->LocationFreeSaleDiscount = LocationFreeSaleDiscount;
   Minor->PriceLevelCustom = PriceLevelCustom;
   Minor->PLU = PLU;

   Minor->can_be_paid_for_using_points_ = can_be_paid_for_using_points_;
   Minor->IsPriceBarcodedItem = IsPriceBarcodedItem;
   Minor->PriceLevels = PriceLevels;
   Minor->isManuallyEnteredWeight = isManuallyEnteredWeight;
   Minor->CostForPoints = CostForPoints;
   //Minor->CostForPoints = CostForPoints;
   Minor->IsCanBePaidForUsingPoints = IsCanBePaidForUsingPoints;  // check can be paid by points..
   Minor->ItemSizeIdentifierKey = ItemSizeIdentifierKey;

}

void TItemMinor::ClearPrice()
{
   FPrice = TMMPrice();
   // ItemPrice.clear();
}

void TItemMinor::SetPrice(TMMPrice &inPrice)
{
   FPrice = inPrice;
}

void TItemMinor::InversePrice()
{
   FPrice.Inverse();
}

void TItemMinor::ResetQty()
{
   Qty = fabs(Qty);
}

void TItemMinor::CreditQty()
{
   Qty = -fabs(Qty);
}


Currency TItemMinor::PriceEach()
{
   return FPrice.Total();
}

Currency TItemMinor::Price()
const
{
   double total = double(FPrice.Total().Val);   //MM-6306
   return total/10000 * Qty;    //MM-6306
}

double TItemMinor::GetPointsPercent()
const
{
	return fPointsPercent;
}

bool  TItemMinor::GetIsManuallyEnteredWeight()
const
{
	return fIsManuallyEnteredWeight;
}
