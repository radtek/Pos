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
// Encoding : utf-8
// Version  : 1.0
// (23/09/2017 9:16:13 p.m. - - $Rev: 25127 $)
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
  /* DiscountType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(DiscountType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"DiscountType");
  /* ArrayOfVoucherInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfVoucherInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfVoucherInfo");
  /* ArrayOfDiscountInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscountInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfDiscountInfo");
  /* ArrayOfDiscountUsageInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscountUsageInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfDiscountUsageInfo");
  /* ImplicationType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(ImplicationType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/Loyaltymate.Enum", L"ImplicationType");
  /* ArrayOfTierLevelInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTierLevelInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfTierLevelInfo");
  /* LoyaltyResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(LoyaltyResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponseCode");
}
#pragma startup RegTypes 32

};     // NS__

