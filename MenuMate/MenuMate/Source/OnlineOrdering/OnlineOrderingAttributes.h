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
     TItemSizeTaxProfileInfo(): ItemSizeTaxProfileId(0), Name(""), Description(""), Rate(0.0), TaxProfileType(eSalesTax), Priority(0) {}
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
     TItemSizeInf(): ItemSizeId(0), Name(""), Description(""), IsWeighted(false), Price(0.0), IsFree(false), PointsPercentage(0.0), PointsPrice(0),
                     CanBePaidUsingPoints(false), OrderingUniqueId(""), ThirdPartyId(""), DefaultPatronCount(1) { }
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
     TItemSideInfo(): SiteItemId(0), SideGroupId(0), Name(""), Description(""), ItemUniqueId(""), OnlyAsSide(false), CompanyId(0) {}
};

struct  TSideGroupInfo
{
     int SideGroupId;
     AnsiString Name;
     AnsiString Description;
     int MaxSelect;
     bool AllowSkip;
     std::list<TItemSideInfo> ItemSides;
     TSideGroupInfo(): SideGroupId(0), Name(""), Description(""), MaxSelect(0), AllowSkip(false) {}
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
     TSiteItemInfo():  SiteItemId(0), Name(""),  Description(""), ItemUniqueId(""),  OnlyAsSide(false), CompanyId(0) {}
};

struct  TCourseInfo
{
     int CourseId;
     AnsiString Name;
     AnsiString Description;
     AnsiString ServingCourseName;
     AnsiString ServingCourseDescription;
     std::list<TSiteItemInfo> Items;
     TCourseInfo(): CourseId(0), Name(""), Description(""), ServingCourseName(""), ServingCourseDescription("") {}
};

struct TMenuConsumableInfo
{
     int MenuId;
     AnsiString Name;
     AnsiString Description;
     eMenuType MenuType;
     bool IsPalmable;
     std::list<TCourseInfo> SiteCourses;
     TMenuConsumableInfo(): MenuId(0), Name(""),  Description(""), MenuType(Food), IsPalmable(true) {}
};

struct TSiteMenuInfo
{
     int CompanyId;
     int SiteId;
     std::list<TMenuConsumableInfo> MenuConsumables;
     TSiteMenuInfo(): CompanyId(0), SiteId(0) {}
};

//Tax Setting Models
struct TTaxSettingsInfo
{
    eSiteSettingType SettingType;
    AnsiString Value;
    TTaxSettingsInfo(): SettingType(PickUp), Value(0) {}
};

struct TSiteTaxSettingsInfo
{
    int SiteId;
    std::list<TTaxSettingsInfo> SiteTaxSettings;
    TSiteTaxSettingsInfo(): SiteId(0) {}
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
    TOrderItemSizeTaxProfileModel(): OrderItemSizeTaxProfileId(0), OrderItemSizeId(0), TaxProfileType(eSalesTax), Percentage(0.0), Value(0.0),
                                     ItemSizeTaxProfileId(0), Name(""), Description(""), Rate(0.0), Priority(0), CompanyId(0) {}
};

struct TOrderItemSizeDiscountModel
{
    int OrderItemSizeDiscountId;
    int OrderItemSizeId;
    AnsiString Name;
    double Value;
    AnsiString Code;
    TOrderItemSizeDiscountModel(): OrderItemSizeDiscountId(0), OrderItemSizeId(0), Name(""),  Value(0.0), Code("") {}
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
    AnsiString ItemSizeUniqueId;
    std::list<TOrderItemSizeDiscountModel> OrderItemSizeDiscounts;
    std::list<TOrderItemSizeTaxProfileModel> OrderItemSizeTaxProfiles;
    TOrderItemSizeModel(): OrderItemSizeId(0), OrderItemId(0), Name(""), ItemSizeId(0), Quantity(0.0), MenuPrice(0.0), Price(0.0), PriceInclusive(0.0),
                            BasePrice(0.0), ReferenceOrderItemSizeId(0), ItemSizeUniqueId("") {}
};

struct TOrderItemModel
{
	int OrderItemId;
	int OrderId;
	AnsiString Name;
	AnsiString Description;
	int SiteItemId;
	double Price;
    AnsiString ItemUniqueId;
	std::list<TOrderItemSizeModel> OrderItemSizes;
    TOrderItemModel(): OrderItemId(0), OrderId(0), Name(""), Description(""), SiteItemId(0), Price(0.0), ItemUniqueId("") {}
};

struct TInvoiceTransactionModel
{
	 int InvoiceTransactionId;
	 AnsiString InvoiceNumber;
	 double TotalSaleAmount;
	 TDateTime TransactionDate;
	 int SiteId;
	 AnsiString TerminalName;
	 TByteDynArray Receipt;
	 AnsiString ReceiptPath;
	 double Rounding;
	 int UserReferenceId;
	 eUserType UserType;
     TInvoiceTransactionModel():InvoiceTransactionId(0), InvoiceNumber(""), TotalSaleAmount(0.0), TransactionDate(Now()), SiteId(0), TerminalName(""),
                                ReceiptPath(""), Rounding(0.0),UserReferenceId(0), UserType(Member) {}
};

struct TOrderInvoiceTransactionModel
{
    int OrderInvoiceTransactionId;
    int  OrderId;
    int InvoiceTransactionId;
    TInvoiceTransactionModel InvoiceTransaction;
    TOrderInvoiceTransactionModel():OrderInvoiceTransactionId(0), OrderId(0), InvoiceTransactionId(0){}
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
     AnsiString UserEmailId;
     AnsiString MemberGuid;
     TOrderInvoiceTransactionModel OrderInvoiceTransaction;
     TSiteOrderModel():CompanyId(0), SiteId(0), OrderId(0), Location(""), TotalAmount(0.0), ContainerNumber(0), ContainerType(OnlineTab), ContainerName(""),
                       OrderGuid(""), UserReferenceId(0), UserType(Member), TerminalName(""), TransactionDate(Now()), OrderType(eNormalOrder),
                       TransactionType(PickUp), IsConfirmed(true), UserEmailId(""), MemberGuid("") {}
};

#endif

