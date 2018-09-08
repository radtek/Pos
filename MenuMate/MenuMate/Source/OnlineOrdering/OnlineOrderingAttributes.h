//---------------------------------------------------------------------------

#ifndef OnlineOrderingAttributesH
#define OnlineOrderingAttributesH
#include <Classes.hpp>
#include <list>
//---------------------------------------------------------------------------

enum eTaxProfileType {eSalesTax, ePurchasetax, eServiceCharge, eServiceChargeTax, eLocalTax, eProfitTax };
enum eMenuType { Food, Beverage};
enum eSiteSettingType {PickUp, DineIn, TakeAway, ItemPriceIncludeTax, ItemPriceIncludeServiceCharge, CalculateTaxBeforeDiscount, CalculateTaxAfterDiscount,
                        CalculateScPreDiscountedPrice, ReCalculateScAfterDiscount, ApplyServiceChargeTax, ServiceChargeTaxRate};
enum eOrderContainerType {OnlineTab, OnlineTable };
enum eUserType {Member, Staff};
enum eOrderType {eNormalOrder, eUnused, eCanceledOrder, eCreditNonExistingOrder};



//Menu models
struct TItemSizeTaxProfileInfo
{
     int ItemSizeTaxProfileId;
     AnsiString Name;
     AnsiString Description;
     double Rate;
     eTaxProfileType TaxProfileType;
     int Priority;
};

struct  TItemSizeInf
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
     std::list<TItemSizeInf> ItemSizes;
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
     std::list<TItemSizeInf> ItemSizes;
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

//Tax Setting Models
struct TTaxSettingsInfo
{
    eSiteSettingType SettingType;
    AnsiString Value;
};

struct TSiteTaxSettingsInfo
{
    int SiteId;
    std::list<TTaxSettingsInfo> SiteTaxSettings;
};

//Order Models

struct TOrderItemSizeTaxProfileModel
{
    int OrderItemSizeTaxProfileId;
    int OrderItemSizeId;
    eTaxProfileType TaxProfileType;
    double Percentage;
    double Value;
    int ItemSizeTaxProfileId;
    AnsiString Name;
    AnsiString Description;
    double Rate;
    int Priority;
    int CompanyId;
};

struct TOrderItemSizeDiscountModel
{
    int OrderItemSizeDiscountId;
    int OrderItemSizeId;
    AnsiString Name;
    double Value;
    AnsiString Code;
};

struct TOrderItemSizeModel
{
    int OrderItemSizeId;
    int OrderItemId;
    AnsiString Name;
    int ItemSizeId;
    double Quantity;
    double MenuPrice;
    double Price;
    double PriceInclusive;
    double BasePrice;
    int ReferenceOrderItemSizeId;
    std::list<TOrderItemSizeDiscountModel> OrderItemSizeDiscounts;
    std::list<TOrderItemSizeTaxProfileModel> OrderItemSizeTaxProfiles;
};

struct TOrderItemModel
{
	int OrderItemId;
	int OrderId;
	AnsiString Name;
	AnsiString Description;
	int SiteItemId;
	double Price;
	std::list<TOrderItemSizeModel> OrderItemSizes;
};

struct TSiteOrderModel
{
	 int CompanyId;
	 int SiteId;
	 int OrderId;
	 AnsiString Location;
	 double TotalAmount;
	 int ContainerNumber;
	 eOrderContainerType ContainerType;
	 AnsiString ContainerName;
	 AnsiString OrderGuid;
	 int UserReferenceId;
	 eUserType UserType;
	 AnsiString TerminalName;
	 TDateTime TransactionDate;
	 eOrderType OrderType;
	 std::list<TOrderItemModel> OrderItems;
	 eSiteSettingType TransactionType;
	 bool IsConfirmed;
};

#endif

