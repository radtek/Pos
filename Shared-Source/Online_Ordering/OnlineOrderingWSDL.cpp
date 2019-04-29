// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8750/MenumateServices/OnlineOrdering/?wsdl
//  >Import : http://localhost:8750/MenumateServices/OnlineOrdering/?wsdl>0
//  >Import : http://localhost:8750/MenumateServices/OnlineOrdering/?xsd=xsd0
//  >Import : http://localhost:8750/MenumateServices/OnlineOrdering/?xsd=xsd2
//  >Import : http://localhost:8750/MenumateServices/OnlineOrdering/?xsd=xsd3
//  >Import : http://localhost:8750/MenumateServices/OnlineOrdering/?xsd=xsd4
//  >Import : http://localhost:8750/MenumateServices/OnlineOrdering/?xsd=xsd5
//  >Import : http://localhost:8750/MenumateServices/OnlineOrdering/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (29/04/2019 5:40:32 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "OnlineOrderingWSDL.h"
#endif



namespace NS__OnlineOrderingWSDL {

_di_IWCFServiceOnlineOrdering GetIWCFServiceOnlineOrdering(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8750/MenumateServices/OnlineOrdering/?wsdl";
  static const char* defURL = "http://localhost:8750/MenumateServices/OnlineOrdering/";
  static const char* defSvc = "WCFServiceOnlineOrdering";
  static const char* defPrt = "basicHttpBinding_ServiceOnlineOrdering";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  THTTPRIO* rio = HTTPRIO ? HTTPRIO : new THTTPRIO(0);
  rio->HTTPWebNode->SendTimeout = 100000;
  rio->HTTPWebNode->ReceiveTimeout = 100000;
  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_IWCFServiceOnlineOrdering service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall SiteMenuInfo::~SiteMenuInfo()
{
  for(int i=0; i<FMenuConsumables.Length; i++)
    if (FMenuConsumables[i])
      delete FMenuConsumables[i];
}

__fastcall MenuConsumableInfo::~MenuConsumableInfo()
{
  for(int i=0; i<FSiteCourses.Length; i++)
    if (FSiteCourses[i])
      delete FSiteCourses[i];
}

__fastcall CourseInfo::~CourseInfo()
{
  for(int i=0; i<FItems.Length; i++)
    if (FItems[i])
      delete FItems[i];
}

__fastcall SiteItemInfo::~SiteItemInfo()
{
  for(int i=0; i<FItemSizes.Length; i++)
    if (FItemSizes[i])
      delete FItemSizes[i];
  for(int i=0; i<FSideGroups.Length; i++)
    if (FSideGroups[i])
      delete FSideGroups[i];
}

__fastcall ItemSizeInfo::~ItemSizeInfo()
{
  for(int i=0; i<FItemSizeTaxProfiles.Length; i++)
    if (FItemSizeTaxProfiles[i])
      delete FItemSizeTaxProfiles[i];
}

__fastcall SideGroupInfo::~SideGroupInfo()
{
  for(int i=0; i<FItemSides.Length; i++)
    if (FItemSides[i])
      delete FItemSides[i];
}

__fastcall ItemSideInfo::~ItemSideInfo()
{
  for(int i=0; i<FItemSizes.Length; i++)
    if (FItemSizes[i])
      delete FItemSizes[i];
}

__fastcall SiteTaxSettingsinfo::~SiteTaxSettingsinfo()
{
  for(int i=0; i<FSiteTaxSettings.Length; i++)
    if (FSiteTaxSettings[i])
      delete FSiteTaxSettings[i];
}

__fastcall OrderInvoiceTransactionModel::~OrderInvoiceTransactionModel()
{
  delete FInvoiceTransaction;
}

__fastcall SiteOrderModel::~SiteOrderModel()
{
  delete FOrderInvoiceTransaction;
  delete FTransactionDate;
  for(int i=0; i<FOrderItems.Length; i++)
    if (FOrderItems[i])
      delete FOrderItems[i];
}

__fastcall InvoiceTransactionModel::~InvoiceTransactionModel()
{
  delete FTransactionDate;
}

__fastcall OrderItemModel::~OrderItemModel()
{
  for(int i=0; i<FOrderItemSizes.Length; i++)
    if (FOrderItemSizes[i])
      delete FOrderItemSizes[i];
}

__fastcall OrderItemSizeModel::~OrderItemSizeModel()
{
  for(int i=0; i<FOrderItemSizeDiscounts.Length; i++)
    if (FOrderItemSizeDiscounts[i])
      delete FOrderItemSizeDiscounts[i];
  for(int i=0; i<FOrderItemSizeTaxProfiles.Length; i++)
    if (FOrderItemSizeTaxProfiles[i])
      delete FOrderItemSizeTaxProfiles[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IWCFServiceOnlineOrdering */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFServiceOnlineOrdering), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFServiceOnlineOrdering), L"http://tempuri.org/IWCFServiceOnlineOrdering/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFServiceOnlineOrdering), ioDocument);
  /* TaxProfileType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(TaxProfileType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"TaxProfileType");
  /* MenuType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(MenuType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"MenuType");
  /* SiteSettingType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(SiteSettingType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"SiteSettingType");
  /* OrderContainerType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(OrderContainerType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"OrderContainerType");
  /* UserType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(UserType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"UserType");
  /* OrderType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(OrderType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"OrderType");
  /* MenuType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(MenuType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"MenuType");
  /* OrderContainerType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(OrderContainerType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"OrderContainerType");
  /* OrderType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(OrderType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"OrderType");
  /* ArrayOfMenuConsumableInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfMenuConsumableInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfMenuConsumableInfo");
  /* SiteMenuInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteMenuInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"SiteMenuInfo");
  /* ArrayOfCourseInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfCourseInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfCourseInfo");
  /* MenuConsumableInfo */
  RemClassRegistry()->RegisterXSClass(__classid(MenuConsumableInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"MenuConsumableInfo");
  /* ArrayOfSiteItemInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSiteItemInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfSiteItemInfo");
  /* CourseInfo */
  RemClassRegistry()->RegisterXSClass(__classid(CourseInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"CourseInfo");
  /* ArrayOfItemSizeInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSizeInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfItemSizeInfo");
  /* ArrayOfSideGroupInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSideGroupInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfSideGroupInfo");
  /* SiteItemInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteItemInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"SiteItemInfo");
  /* ArrayOfItemSizeTaxProfileInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSizeTaxProfileInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfItemSizeTaxProfileInfo");
  /* ItemSizeInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSizeInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ItemSizeInfo");
  /* ItemSizeTaxProfileInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSizeTaxProfileInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ItemSizeTaxProfileInfo");
  /* ArrayOfItemSideInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSideInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfItemSideInfo");
  /* SideGroupInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SideGroupInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"SideGroupInfo");
  /* ItemSideInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSideInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ItemSideInfo");
  /* ArrayOfTaxSettingsInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTaxSettingsInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfTaxSettingsInfo");
  /* SiteTaxSettingsinfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteTaxSettingsinfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"SiteTaxSettingsinfo");
  /* TaxSettingsInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TaxSettingsInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"TaxSettingsInfo");
  /* SiteMenuInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteMenuInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"SiteMenuInfo2", L"SiteMenuInfo");
  /* MenuConsumableInfo */
  RemClassRegistry()->RegisterXSClass(__classid(MenuConsumableInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"MenuConsumableInfo2", L"MenuConsumableInfo");
  /* CourseInfo */
  RemClassRegistry()->RegisterXSClass(__classid(CourseInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"CourseInfo2", L"CourseInfo");
  /* SiteItemInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteItemInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"SiteItemInfo2", L"SiteItemInfo");
  /* ItemSizeInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSizeInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ItemSizeInfo2", L"ItemSizeInfo");
  /* ItemSizeTaxProfileInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSizeTaxProfileInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ItemSizeTaxProfileInfo2", L"ItemSizeTaxProfileInfo");
  /* SideGroupInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SideGroupInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"SideGroupInfo2", L"SideGroupInfo");
  /* ItemSideInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSideInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ItemSideInfo2", L"ItemSideInfo");
  /* SiteTaxSettingsinfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteTaxSettingsinfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"SiteTaxSettingsinfo2", L"SiteTaxSettingsinfo");
  /* TaxSettingsInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TaxSettingsInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"TaxSettingsInfo2", L"TaxSettingsInfo");
  /* OOServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(OOServiceResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"OOServiceResponse");
  /* OOLoyaltyResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(OOLoyaltyResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"OOLoyaltyResponseCode");
  /* OOLoyaltyResponse */
  RemClassRegistry()->RegisterXSClass(__classid(OOLoyaltyResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"OOLoyaltyResponse");
  /* LoyaltyOOResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyOOResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"LoyaltyOOResponse");
  /* OnlineOrderingDetails */
  RemClassRegistry()->RegisterXSClass(__classid(OnlineOrderingDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"OnlineOrderingDetails");
  /* OOLoyaltyResponse */
  RemClassRegistry()->RegisterXSClass(__classid(OOLoyaltyResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"OOLoyaltyResponse2", L"OOLoyaltyResponse");
  /* OOServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(OOServiceResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"OOServiceResponse2", L"OOServiceResponse");
  /* LoyaltyOOResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyOOResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"LoyaltyOOResponse2", L"LoyaltyOOResponse");
  /* OnlineOrderingDetails */
  RemClassRegistry()->RegisterXSClass(__classid(OnlineOrderingDetails2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"OnlineOrderingDetails2", L"OnlineOrderingDetails");
  /* OrderInvoiceTransactionModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderInvoiceTransactionModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderInvoiceTransactionModel");
  /* ArrayOfOrderItemModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOrderItemModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"ArrayOfOrderItemModel");
  /* SiteOrderModel */
  RemClassRegistry()->RegisterXSClass(__classid(SiteOrderModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"SiteOrderModel");
  /* InvoiceTransactionModel */
  RemClassRegistry()->RegisterXSClass(__classid(InvoiceTransactionModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"InvoiceTransactionModel");
  /* ArrayOfOrderItemSizeModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOrderItemSizeModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"ArrayOfOrderItemSizeModel");
  /* OrderItemModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderItemModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderItemModel");
  /* ArrayOfOrderItemSizeDiscountModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOrderItemSizeDiscountModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"ArrayOfOrderItemSizeDiscountModel");
  /* ArrayOfOrderItemSizeTaxProfileModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOrderItemSizeTaxProfileModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"ArrayOfOrderItemSizeTaxProfileModel");
  /* OrderItemSizeModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderItemSizeModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderItemSizeModel");
  /* OrderItemSizeDiscountModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderItemSizeDiscountModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderItemSizeDiscountModel");
  /* OrderItemSizeTaxProfileModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderItemSizeTaxProfileModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderItemSizeTaxProfileModel");
  /* SiteOrderModel */
  RemClassRegistry()->RegisterXSClass(__classid(SiteOrderModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"SiteOrderModel2", L"SiteOrderModel");
  /* OrderInvoiceTransactionModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderInvoiceTransactionModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderInvoiceTransactionModel2", L"OrderInvoiceTransactionModel");
  /* InvoiceTransactionModel */
  RemClassRegistry()->RegisterXSClass(__classid(InvoiceTransactionModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"InvoiceTransactionModel2", L"InvoiceTransactionModel");
  /* OrderItemModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderItemModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderItemModel2", L"OrderItemModel");
  /* OrderItemSizeModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderItemSizeModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderItemSizeModel2", L"OrderItemSizeModel");
  /* OrderItemSizeDiscountModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderItemSizeDiscountModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderItemSizeDiscountModel2", L"OrderItemSizeDiscountModel");
  /* OrderItemSizeTaxProfileModel */
  RemClassRegistry()->RegisterXSClass(__classid(OrderItemSizeTaxProfileModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"OrderItemSizeTaxProfileModel2", L"OrderItemSizeTaxProfileModel");
  /* SiteSettingType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(SiteSettingType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"SiteSettingType");
  /* ArrayOfMenuConsumableInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfMenuConsumableInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfMenuConsumableInfo");
  /* ArrayOfSiteItemInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSiteItemInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfSiteItemInfo");
  /* ArrayOfItemSizeTaxProfileInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSizeTaxProfileInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfItemSizeTaxProfileInfo");
  /* ArrayOfItemSideInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSideInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfItemSideInfo");
  /* OOLoyaltyResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(OOLoyaltyResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering", L"OOLoyaltyResponseCode");
  /* ArrayOfOrderItemModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOrderItemModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"ArrayOfOrderItemModel");
  /* ArrayOfOrderItemSizeDiscountModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOrderItemSizeDiscountModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"ArrayOfOrderItemSizeDiscountModel");
  /* UserType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(UserType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"UserType");
  /* ArrayOfItemSizeInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSizeInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfItemSizeInfo");
  /* ArrayOfTaxSettingsInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTaxSettingsInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfTaxSettingsInfo");
  /* ArrayOfOrderItemSizeModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOrderItemSizeModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"ArrayOfOrderItemSizeModel");
  /* TaxProfileType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(TaxProfileType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"TaxProfileType");
  /* ArrayOfSideGroupInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSideGroupInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfSideGroupInfo");
  /* ArrayOfCourseInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfCourseInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels", L"ArrayOfCourseInfo");
  /* ArrayOfOrderItemSizeTaxProfileModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOrderItemSizeTaxProfileModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels", L"ArrayOfOrderItemSizeTaxProfileModel");
}
#pragma startup RegTypes 32

};     // NS__

