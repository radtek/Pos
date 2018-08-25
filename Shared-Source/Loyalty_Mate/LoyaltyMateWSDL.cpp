// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8734/MenumateServices/LoyaltyMate/?wsdl
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?wsdl>0
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd0
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd2
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd3
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd4
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd1
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd5
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd6
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd7
// Encoding : utf-8
// Version  : 1.0
// (25/08/2018 12:05:46 a.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "LoyaltyMateWSDL.h"
#endif



namespace NS_LoyaltyMateWSDL {

_di_IWCFServiceLoyaltyMate GetIWCFServiceLoyaltyMate(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8734/MenumateServices/LoyaltyMate/?wsdl";
  static const char* defURL = "http://localhost:8734/MenumateServices/LoyaltyMate/";
  static const char* defSvc = "WCFServiceLoyaltyMate";
  static const char* defPrt = "basicHttpBinding_ServiceLoyaltyMate";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  THTTPRIO* rio = HTTPRIO ? HTTPRIO : new THTTPRIO(0);
  rio->HTTPWebNode->SendTimeout = 120000;
  rio->HTTPWebNode->ReceiveTimeout = 120000;
  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_IWCFServiceLoyaltyMate service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall MemberInfo::~MemberInfo()
{
  delete FBirthdayBenefitDate;
  delete FDateOfBirth;
  delete FLastModified;
  delete FLastVisitDate;
  delete FYearStartDate;
  for(int i=0; i<FMemberVouchers.Length; i++)
    if (FMemberVouchers[i])
      delete FMemberVouchers[i];
}

__fastcall LoyaltyMemberResponse::~LoyaltyMemberResponse()
{
  delete FMemberInfo;
}

__fastcall RequestInfo::~RequestInfo()
{
  delete FRequestTime;
}

__fastcall TransactionInfo::~TransactionInfo()
{
  delete FTransactionDate;
}

__fastcall LoyaltyCompanyResponse::~LoyaltyCompanyResponse()
{
  delete FCompanyInfo;
}

__fastcall CompanyInfo::~CompanyInfo()
{
  for(int i=0; i<FDiscounts.Length; i++)
    if (FDiscounts[i])
      delete FDiscounts[i];
  for(int i=0; i<FTierLevels.Length; i++)
    if (FTierLevels[i])
      delete FTierLevels[i];
}

__fastcall DiscountInfo::~DiscountInfo()
{
  delete FMaximumValue;
  delete FRoundToDecimalPlaces;
  delete FValue;
}

__fastcall LoyaltyGiftCardResponse::~LoyaltyGiftCardResponse()
{
  delete FGiftCardInfo;
}

__fastcall GiftCardInfo::~GiftCardInfo()
{
  delete FExpiryDate;
  delete FStartDate;
}

__fastcall LoyaltyVoucherResponse::~LoyaltyVoucherResponse()
{
  delete FVoucherInfo;
}

__fastcall VoucherTransactionInfo::~VoucherTransactionInfo()
{
  delete FTransactionDate;
  for(int i=0; i<FDiscountUsages.Length; i++)
    if (FDiscountUsages[i])
      delete FDiscountUsages[i];
}

__fastcall VoucherTransactionResponse::~VoucherTransactionResponse()
{
  delete FGiftCardExpiryDate;
}

__fastcall ReleasedVoucherInfo::~ReleasedVoucherInfo()
{
  for(int i=0; i<FDiscountCodes.Length; i++)
    if (FDiscountCodes[i])
      delete FDiscountCodes[i];
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
  for(int i=0; i<FSideGroup.Length; i++)
    if (FSideGroup[i])
      delete FSideGroup[i];
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

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IWCFServiceLoyaltyMate */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFServiceLoyaltyMate), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFServiceLoyaltyMate), L"http://tempuri.org/IWCFServiceLoyaltyMate/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFServiceLoyaltyMate), ioDocument);
  /* MMServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(MMServiceResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO", L"MMServiceResponse");
  /* MMServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(MMServiceResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO", L"MMServiceResponse2", L"MMServiceResponse");
  /* DiscountType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(DiscountType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"DiscountType");
  /* DisplayOption */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(DisplayOption_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"DisplayOption");
  /* ImplicationType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(ImplicationType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"ImplicationType");
  /* ProductPriority */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(ProductPriority_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"ProductPriority");
  /* DisplayOption */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(DisplayOption_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"DisplayOption");
  /* ProductPriority */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(ProductPriority_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"ProductPriority");
  /* TaxProfileType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(TaxProfileType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/OnlineOrdering.Enum", L"TaxProfileType");
  /* MenuType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(MenuType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/OnlineOrdering.Enum", L"MenuType");
  /* MenuType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(MenuType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/OnlineOrdering.Enum", L"MenuType");
  /* ArrayOfVoucherInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfVoucherInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfVoucherInfo");
  /* MemberInfo */
  RemClassRegistry()->RegisterXSClass(__classid(MemberInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"MemberInfo");
  /* VoucherInfo */
  RemClassRegistry()->RegisterXSClass(__classid(VoucherInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"VoucherInfo");
  /* LoyaltyResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(LoyaltyResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponseCode");
  /* LoyaltyMemberResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyMemberResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyMemberResponse");
  /* RequestInfo */
  RemClassRegistry()->RegisterXSClass(__classid(RequestInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"RequestInfo");
  /* LoyaltyResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponse");
  /* TransactionInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"TransactionInfo");
  /* LoyaltyCompanyResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyCompanyResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyCompanyResponse");
  /* ArrayOfDiscountInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscountInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfDiscountInfo");
  /* ArrayOfTierLevelInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTierLevelInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfTierLevelInfo");
  /* CompanyInfo */
  RemClassRegistry()->RegisterXSClass(__classid(CompanyInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"CompanyInfo");
  /* DiscountInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DiscountInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"DiscountInfo");
  /* TierLevelInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TierLevelInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"TierLevelInfo");
  /* LoyaltyGiftCardResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyGiftCardResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyGiftCardResponse");
  /* GiftCardInfo */
  RemClassRegistry()->RegisterXSClass(__classid(GiftCardInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"GiftCardInfo");
  /* LoyaltyVoucherResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyVoucherResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyVoucherResponse");
  /* ArrayOfDiscountUsageInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscountUsageInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfDiscountUsageInfo");
  /* VoucherTransactionInfo */
  RemClassRegistry()->RegisterXSClass(__classid(VoucherTransactionInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"VoucherTransactionInfo");
  /* DiscountUsageInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DiscountUsageInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"DiscountUsageInfo");
  /* VoucherTransactionResponse */
  RemClassRegistry()->RegisterXSClass(__classid(VoucherTransactionResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"VoucherTransactionResponse");
  /* ReleasedVoucherInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ReleasedVoucherInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ReleasedVoucherInfo");
  /* ArrayOfMenuConsumableInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfMenuConsumableInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfMenuConsumableInfo");
  /* SiteMenuInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteMenuInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"SiteMenuInfo");
  /* ArrayOfCourseInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfCourseInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfCourseInfo");
  /* MenuConsumableInfo */
  RemClassRegistry()->RegisterXSClass(__classid(MenuConsumableInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"MenuConsumableInfo");
  /* ArrayOfSiteItemInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSiteItemInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfSiteItemInfo");
  /* CourseInfo */
  RemClassRegistry()->RegisterXSClass(__classid(CourseInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"CourseInfo");
  /* ArrayOfItemSizeInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSizeInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfItemSizeInfo");
  /* ArrayOfSideGroupInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSideGroupInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfSideGroupInfo");
  /* SiteItemInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteItemInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"SiteItemInfo");
  /* ArrayOfItemSizeTaxProfileInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSizeTaxProfileInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfItemSizeTaxProfileInfo");
  /* ItemSizeInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSizeInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ItemSizeInfo");
  /* ItemSizeTaxProfileInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSizeTaxProfileInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ItemSizeTaxProfileInfo");
  /* ArrayOfItemSideInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSideInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfItemSideInfo");
  /* SideGroupInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SideGroupInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"SideGroupInfo");
  /* ItemSideInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSideInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ItemSideInfo");
  /* MemberInfo */
  RemClassRegistry()->RegisterXSClass(__classid(MemberInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"MemberInfo2", L"MemberInfo");
  /* VoucherInfo */
  RemClassRegistry()->RegisterXSClass(__classid(VoucherInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"VoucherInfo2", L"VoucherInfo");
  /* LoyaltyMemberResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyMemberResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyMemberResponse2", L"LoyaltyMemberResponse");
  /* RequestInfo */
  RemClassRegistry()->RegisterXSClass(__classid(RequestInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"RequestInfo2", L"RequestInfo");
  /* LoyaltyResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponse2", L"LoyaltyResponse");
  /* TransactionInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"TransactionInfo2", L"TransactionInfo");
  /* LoyaltyCompanyResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyCompanyResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyCompanyResponse2", L"LoyaltyCompanyResponse");
  /* CompanyInfo */
  RemClassRegistry()->RegisterXSClass(__classid(CompanyInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"CompanyInfo2", L"CompanyInfo");
  /* DiscountInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DiscountInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"DiscountInfo2", L"DiscountInfo");
  /* TierLevelInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TierLevelInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"TierLevelInfo2", L"TierLevelInfo");
  /* LoyaltyGiftCardResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyGiftCardResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyGiftCardResponse2", L"LoyaltyGiftCardResponse");
  /* GiftCardInfo */
  RemClassRegistry()->RegisterXSClass(__classid(GiftCardInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"GiftCardInfo2", L"GiftCardInfo");
  /* LoyaltyVoucherResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyVoucherResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyVoucherResponse2", L"LoyaltyVoucherResponse");
  /* VoucherTransactionInfo */
  RemClassRegistry()->RegisterXSClass(__classid(VoucherTransactionInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"VoucherTransactionInfo2", L"VoucherTransactionInfo");
  /* DiscountUsageInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DiscountUsageInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"DiscountUsageInfo2", L"DiscountUsageInfo");
  /* VoucherTransactionResponse */
  RemClassRegistry()->RegisterXSClass(__classid(VoucherTransactionResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"VoucherTransactionResponse2", L"VoucherTransactionResponse");
  /* ReleasedVoucherInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ReleasedVoucherInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ReleasedVoucherInfo2", L"ReleasedVoucherInfo");
  /* SiteMenuInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteMenuInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"SiteMenuInfo2", L"SiteMenuInfo");
  /* MenuConsumableInfo */
  RemClassRegistry()->RegisterXSClass(__classid(MenuConsumableInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"MenuConsumableInfo2", L"MenuConsumableInfo");
  /* CourseInfo */
  RemClassRegistry()->RegisterXSClass(__classid(CourseInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"CourseInfo2", L"CourseInfo");
  /* SiteItemInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteItemInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"SiteItemInfo2", L"SiteItemInfo");
  /* ItemSizeInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSizeInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ItemSizeInfo2", L"ItemSizeInfo");
  /* ItemSizeTaxProfileInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSizeTaxProfileInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ItemSizeTaxProfileInfo2", L"ItemSizeTaxProfileInfo");
  /* SideGroupInfo */
  RemClassRegistry()->RegisterXSClass(__classid(SideGroupInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"SideGroupInfo2", L"SideGroupInfo");
  /* ItemSideInfo */
  RemClassRegistry()->RegisterXSClass(__classid(ItemSideInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ItemSideInfo2", L"ItemSideInfo");
  /* TaxSettingType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(TaxSettingType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"TaxSettingType");
  /* ArrayOfTaxSettingsInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTaxSettingsInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering", L"ArrayOfTaxSettingsInfo");
  /* SiteTaxSettingsinfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteTaxSettingsinfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering", L"SiteTaxSettingsinfo");
  /* TaxSettingsInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TaxSettingsInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering", L"TaxSettingsInfo");
  /* SiteTaxSettingsinfo */
  RemClassRegistry()->RegisterXSClass(__classid(SiteTaxSettingsinfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering", L"SiteTaxSettingsinfo2", L"SiteTaxSettingsinfo");
  /* TaxSettingsInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TaxSettingsInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering", L"TaxSettingsInfo2", L"TaxSettingsInfo");
  /* DiscountType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(DiscountType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"DiscountType");
  /* TaxProfileType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(TaxProfileType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/OnlineOrdering.Enum", L"TaxProfileType");
  /* LoyaltyResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(LoyaltyResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponseCode");
  /* ArrayOfTierLevelInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTierLevelInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfTierLevelInfo");
  /* ArrayOfMenuConsumableInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfMenuConsumableInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfMenuConsumableInfo");
  /* ArrayOfSiteItemInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSiteItemInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfSiteItemInfo");
  /* ArrayOfItemSizeTaxProfileInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSizeTaxProfileInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfItemSizeTaxProfileInfo");
  /* ArrayOfItemSideInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSideInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfItemSideInfo");
  /* ArrayOfTaxSettingsInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTaxSettingsInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering", L"ArrayOfTaxSettingsInfo");
  /* ImplicationType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(ImplicationType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"ImplicationType");
  /* ArrayOfDiscountInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscountInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfDiscountInfo");
  /* ArrayOfCourseInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfCourseInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfCourseInfo");
  /* ArrayOfSideGroupInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSideGroupInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfSideGroupInfo");
  /* ArrayOfDiscountUsageInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscountUsageInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfDiscountUsageInfo");
  /* TaxSettingType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(TaxSettingType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum", L"TaxSettingType");
  /* ArrayOfItemSizeInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItemSizeInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfItemSizeInfo");
  /* ArrayOfVoucherInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfVoucherInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfVoucherInfo");
}
#pragma startup RegTypes 32

};     // NS_LoyaltyMateWSDL

