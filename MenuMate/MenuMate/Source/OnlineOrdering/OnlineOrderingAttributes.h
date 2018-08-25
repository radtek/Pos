//---------------------------------------------------------------------------

#ifndef OnlineOrderingAttributesH
#define OnlineOrderingAttributesH
#include <Classes.hpp>
#include <list>
//---------------------------------------------------------------------------

enum eTaxProfileType{eSalesTax = 0, ePurchasetax, eServiceCharge, eServiceChargeTax, eLocalTax, eProfitTax };
enum eMenuType{ Food = 1, Beverage};
enum eTaxSettingType {ItemPriceIncludeTax = 3, ItemPriceIncludeServiceCharge, CalculateTaxBeforeDiscount, CalculateTaxAfterDiscount,
                        CalculateScPreDiscountedPrice, ReCalculateScAfterDiscount, ApplyServiceChargeTax, ServiceChargeTaxRate};

typedef int TMMTaxSettingType;

struct TItemSizeTaxProfileInfo
{
     int ItemSizeTaxProfileId;
     AnsiString Name;
     AnsiString Description;
     double Rate;
     eTaxProfileType TaxProfileType;
     int Priority;
};

struct  TItemSizeInfo
{
     int ItemSizeId;
     AnsiString Name;
     AnsiString Description;
     bool IsWeighted;
     double Price;
     bool IsFree;
     double PointsPercentage;
     double PointsPrice;
     bool CanBePaidUsingPoints;
     AnsiString OrderingUniqueId;
     AnsiString ThirdPartyId;
     int DefaultPatronCount;
     std::list<TItemSizeTaxProfileInfo> ItemSizeTaxProfiles;
};

struct  TItemSideInfo
{
     int SiteItemId;
     int SideGroupId;
     AnsiString Name;
     AnsiString Description;
     AnsiString ItemUniqueId;
     bool OnlyAsSide;
     int CompanyId;
     std::list<TItemSizeInfo> ItemSizes;
};

struct  TSideGroupInfo
{
     int SideGroupId;
     AnsiString Name;
     AnsiString Description;
     int MaxSelect;
     bool AllowSkip;
     std::list<TItemSideInfo> ItemSides;
};

struct  TSiteItemInfo
{
     int SiteItemId;
     AnsiString Name;
     AnsiString Description;
     AnsiString ItemUniqueId;
     bool OnlyAsSide;
     int CompanyId;
     std::list<TItemSizeInfo> ItemSizes;
     std::list<TSideGroupInfo> SideGroup;
};

struct  TCourseInfo
{
     int CourseId;
     AnsiString Name;
     AnsiString Description;
     AnsiString ServingCourseName;
     AnsiString ServingCourseDescription;
     std::list<TSiteItemInfo> Items;
};

struct TMenuConsumableInfo
{
     int MenuId;
     AnsiString Name;
     AnsiString Description;
     eMenuType MenuType;
     bool IsPalmable;
     std::list<TCourseInfo> SiteCourses;
};

struct TSiteMenuInfo
{
     int CompanyId;
     int SiteId;
     std::list<TMenuConsumableInfo> MenuConsumables;
};

struct TTaxSettingsInfo
{
    TMMTaxSettingType SettingType;
    AnsiString Value;
};

struct TSiteTaxSettingsInfo
{
    int SiteId;
    std::list<TTaxSettingsInfo> SiteTaxSettings;
};

#endif

