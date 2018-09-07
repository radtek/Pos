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
// (7/09/2018 10:32:54 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   LoyaltyMateWSDLH
#define   LoyaltyMateWSDLH

#include <System.hpp>
#include <InvokeRegistry.hpp>
#include <XSBuiltIns.hpp>
#include <winsock2.h>
#include <SOAPHTTPClient.hpp>

#if !defined(SOAP_REMOTABLE_CLASS)
#define SOAP_REMOTABLE_CLASS __declspec(delphiclass)
#endif
#if !defined(IS_OPTN)
#define IS_OPTN 0x0001
#endif
#if !defined(IS_UNBD)
#define IS_UNBD 0x0002
#endif
#if !defined(IS_NLBL)
#define IS_NLBL 0x0004
#endif
#if !defined(IS_REF)
#define IS_REF 0x0080
#endif


namespace NS__LoyaltyMateWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:dateTime        - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:decimal         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:long            - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS MMServiceResponse;
class SOAP_REMOTABLE_CLASS MMServiceResponse2;
class SOAP_REMOTABLE_CLASS MemberInfo;
class SOAP_REMOTABLE_CLASS VoucherInfo;
class SOAP_REMOTABLE_CLASS LoyaltyMemberResponse;
class SOAP_REMOTABLE_CLASS RequestInfo;
class SOAP_REMOTABLE_CLASS LoyaltyResponse;
class SOAP_REMOTABLE_CLASS TransactionInfo;
class SOAP_REMOTABLE_CLASS LoyaltyCompanyResponse;
class SOAP_REMOTABLE_CLASS CompanyInfo;
class SOAP_REMOTABLE_CLASS DiscountInfo;
class SOAP_REMOTABLE_CLASS TierLevelInfo;
class SOAP_REMOTABLE_CLASS LoyaltyGiftCardResponse;
class SOAP_REMOTABLE_CLASS GiftCardInfo;
class SOAP_REMOTABLE_CLASS LoyaltyVoucherResponse;
class SOAP_REMOTABLE_CLASS VoucherTransactionInfo;
class SOAP_REMOTABLE_CLASS DiscountUsageInfo;
class SOAP_REMOTABLE_CLASS VoucherTransactionResponse;
class SOAP_REMOTABLE_CLASS ReleasedVoucherInfo;
class SOAP_REMOTABLE_CLASS LoyaltyOnlineOrderingResponse;
class SOAP_REMOTABLE_CLASS MemberInfo2;
class SOAP_REMOTABLE_CLASS VoucherInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyMemberResponse2;
class SOAP_REMOTABLE_CLASS RequestInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyResponse2;
class SOAP_REMOTABLE_CLASS TransactionInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyCompanyResponse2;
class SOAP_REMOTABLE_CLASS CompanyInfo2;
class SOAP_REMOTABLE_CLASS DiscountInfo2;
class SOAP_REMOTABLE_CLASS TierLevelInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyGiftCardResponse2;
class SOAP_REMOTABLE_CLASS GiftCardInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyVoucherResponse2;
class SOAP_REMOTABLE_CLASS VoucherTransactionInfo2;
class SOAP_REMOTABLE_CLASS DiscountUsageInfo2;
class SOAP_REMOTABLE_CLASS VoucherTransactionResponse2;
class SOAP_REMOTABLE_CLASS ReleasedVoucherInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyOnlineOrderingResponse2;
class SOAP_REMOTABLE_CLASS SiteMenuInfo;
class SOAP_REMOTABLE_CLASS MenuConsumableInfo;
class SOAP_REMOTABLE_CLASS CourseInfo;
class SOAP_REMOTABLE_CLASS SiteItemInfo;
class SOAP_REMOTABLE_CLASS ItemSizeInfo;
class SOAP_REMOTABLE_CLASS ItemSizeTaxProfileInfo;
class SOAP_REMOTABLE_CLASS SideGroupInfo;
class SOAP_REMOTABLE_CLASS ItemSideInfo;
class SOAP_REMOTABLE_CLASS SiteTaxSettingsinfo;
class SOAP_REMOTABLE_CLASS TaxSettingsInfo;
class SOAP_REMOTABLE_CLASS SiteMenuInfo2;
class SOAP_REMOTABLE_CLASS MenuConsumableInfo2;
class SOAP_REMOTABLE_CLASS CourseInfo2;
class SOAP_REMOTABLE_CLASS SiteItemInfo2;
class SOAP_REMOTABLE_CLASS ItemSizeInfo2;
class SOAP_REMOTABLE_CLASS ItemSizeTaxProfileInfo2;
class SOAP_REMOTABLE_CLASS SideGroupInfo2;
class SOAP_REMOTABLE_CLASS ItemSideInfo2;
class SOAP_REMOTABLE_CLASS SiteTaxSettingsinfo2;
class SOAP_REMOTABLE_CLASS TaxSettingsInfo2;
class SOAP_REMOTABLE_CLASS SiteOrderModel;
class SOAP_REMOTABLE_CLASS OrderItemModel;
class SOAP_REMOTABLE_CLASS OrderItemSizeModel;
class SOAP_REMOTABLE_CLASS OrderItemSizeDiscountModel;
class SOAP_REMOTABLE_CLASS OrderItemSizeTaxProfileModel;
class SOAP_REMOTABLE_CLASS SiteOrderModel2;
class SOAP_REMOTABLE_CLASS OrderItemModel2;
class SOAP_REMOTABLE_CLASS OrderItemSizeModel2;
class SOAP_REMOTABLE_CLASS OrderItemSizeDiscountModel2;
class SOAP_REMOTABLE_CLASS OrderItemSizeTaxProfileModel2;

enum class DiscountType   /* "http://schemas.datacontract.org/2004/07/Loyaltymate.Enum"[GblSmpl] */
{
  Percent, 
  Dollar, 
  Combo, 
  ItemMode, 
  SetPrice
};

class DiscountType_TypeInfoHolder : public TObject {
  DiscountType __instanceType;
public:
__published:
  __property DiscountType __propType = { read=__instanceType };
};

enum class DisplayOption   /* "http://schemas.datacontract.org/2004/07/Loyaltymate.Enum"[GblSmpl] */
{
  FixedDescriptionAndAmount, 
  PromptForDescription, 
  PromptForAmount, 
  PromptForDescriptionAndAmount
};

class DisplayOption_TypeInfoHolder : public TObject {
  DisplayOption __instanceType;
public:
__published:
  __property DisplayOption __propType = { read=__instanceType };
};

enum class ImplicationType   /* "http://schemas.datacontract.org/2004/07/Loyaltymate.Enum"[GblSmpl] */
{
  Discount, 
  Surcharge, 
  Point
};

class ImplicationType_TypeInfoHolder : public TObject {
  ImplicationType __instanceType;
public:
__published:
  __property ImplicationType __propType = { read=__instanceType };
};

enum class ProductPriority   /* "http://schemas.datacontract.org/2004/07/Loyaltymate.Enum"[GblSmpl] */
{
  LowestPriceFirst, 
  HighestPriceFirst, 
  NotApllicable
};

class ProductPriority_TypeInfoHolder : public TObject {
  ProductPriority __instanceType;
public:
__published:
  __property ProductPriority __propType = { read=__instanceType };
};

enum class LoyaltyResponseCode   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate"[GblSmpl] */
{
  Successful, 
  AuthenticationFailed, 
  CreateMemberFailed, 
  UpdateMemberFailed, 
  DeleteMemberFailed, 
  GetMemberFailed, 
  PostTransactionFailed, 
  MemberNotExist, 
  CompanySyncFailed, 
  InvalidGiftVoucher, 
  InvalidPocketVoucher, 
  TransactionFailed, 
  GetGiftCardFailed, 
  GetPocketVoucherFailed, 
  MenuSyncingFailed, 
  TaxSettingSyncingFailed, 
  UpdateOnlineOrderStatusFailed, 
  PostOnlineOrderInvoiceInfoFailed
};

class LoyaltyResponseCode_TypeInfoHolder : public TObject {
  LoyaltyResponseCode __instanceType;
public:
__published:
  __property LoyaltyResponseCode __propType = { read=__instanceType };
};

enum class TaxProfileType   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum"[GblSmpl] */
{
  SalesTax, 
  Purchasetax, 
  ServiceCharge, 
  ServiceChargeTax, 
  LocalTax, 
  ProfitTax
};

class TaxProfileType_TypeInfoHolder : public TObject {
  TaxProfileType __instanceType;
public:
__published:
  __property TaxProfileType __propType = { read=__instanceType };
};

enum class MenuType   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum"[GblSmpl] */
{
  Food, 
  Beverage
};

class MenuType_TypeInfoHolder : public TObject {
  MenuType __instanceType;
public:
__published:
  __property MenuType __propType = { read=__instanceType };
};

enum class SiteSettingType   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum"[GblSmpl] */
{
  PickUp, 
  DineIn, 
  TakeAway, 
  ItemPriceIncludeTax, 
  ItemPriceIncludeServiceCharge, 
  CalculateTaxBeforeDiscount, 
  CalculateTaxAfterDiscount, 
  CalculateScPreDiscountedPrice, 
  ReCalculateScAfterDiscount, 
  ApplyServiceChargeTax, 
  ServiceChargeTaxRate
};

class SiteSettingType_TypeInfoHolder : public TObject {
  SiteSettingType __instanceType;
public:
__published:
  __property SiteSettingType __propType = { read=__instanceType };
};

enum class OrderContainerType   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum"[GblSmpl] */
{
  Tab, 
  Table
};

class OrderContainerType_TypeInfoHolder : public TObject {
  OrderContainerType __instanceType;
public:
__published:
  __property OrderContainerType __propType = { read=__instanceType };
};

enum class OrderType   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum"[GblSmpl] */
{
  NormalOrder, 
  Unused, 
  CanceledOrder, 
  CreditNonExistingOrder
};

class OrderType_TypeInfoHolder : public TObject {
  OrderType __instanceType;
public:
__published:
  __property OrderType __propType = { read=__instanceType };
};

enum class UserType   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Enum"[GblSmpl] */
{
  Member, 
  Staff
};

class UserType_TypeInfoHolder : public TObject {
  UserType __instanceType;
public:
__published:
  __property UserType __propType = { read=__instanceType };
};



// ************************************************************************ //
// XML       : MMServiceResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO
// ************************************************************************ //
class MMServiceResponse : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FMessage;
  bool            FMessage_Specified;
  bool            FSuccessful;
  bool            FSuccessful_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetMessage(int Index, UnicodeString _prop_val)
  {  FMessage = _prop_val; FMessage_Specified = true;  }
  bool __fastcall Message_Specified(int Index)
  {  return FMessage_Specified;  } 
  void __fastcall SetSuccessful(int Index, bool _prop_val)
  {  FSuccessful = _prop_val; FSuccessful_Specified = true;  }
  bool __fastcall Successful_Specified(int Index)
  {  return FSuccessful_Specified;  } 
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString    Message = { index=(IS_OPTN|IS_NLBL), read=FMessage, write=SetMessage, stored = Message_Specified };
  __property bool       Successful = { index=(IS_OPTN), read=FSuccessful, write=SetSuccessful, stored = Successful_Specified };
};




// ************************************************************************ //
// XML       : MMServiceResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO
// ************************************************************************ //
class MMServiceResponse2 : public MMServiceResponse {
private:
__published:
};


typedef DynamicArray<VoucherInfo*> ArrayOfVoucherInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate"[GblCplx] */


// ************************************************************************ //
// XML       : MemberInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class MemberInfo : public TRemotable {
private:
  bool            FActivated;
  bool            FActivated_Specified;
  UnicodeString   FAddress1;
  bool            FAddress1_Specified;
  UnicodeString   FAddress2;
  bool            FAddress2_Specified;
  double          FAvailableBirthDayPoint;
  bool            FAvailableBirthDayPoint_Specified;
  double          FAvailableFirstVisitPoint;
  bool            FAvailableFirstVisitPoint_Specified;
  TXSDateTime*    FBirthdayBenefitDate;
  bool            FBirthdayBenefitDate_Specified;
  UnicodeString   FCardNumber;
  bool            FCardNumber_Specified;
  UnicodeString   FCity;
  bool            FCity_Specified;
  UnicodeString   FCountry;
  bool            FCountry_Specified;
  double          FCurrentYearPoint;
  bool            FCurrentYearPoint_Specified;
  TXSDateTime*    FDateOfBirth;
  bool            FDateOfBirth_Specified;
  double          FEarnedPoints;
  bool            FEarnedPoints_Specified;
  UnicodeString   FEmail;
  bool            FEmail_Specified;
  UnicodeString   FFirstName;
  bool            FFirstName_Specified;
  int             FGender;
  bool            FGender_Specified;
  bool            FHasTransactions;
  bool            FHasTransactions_Specified;
  __int64         FHomeSiteId;
  bool            FHomeSiteId_Specified;
  bool            FIsFirstVisitRewarded;
  bool            FIsFirstVisitRewarded_Specified;
  TXSDateTime*    FLastModified;
  bool            FLastModified_Specified;
  UnicodeString   FLastName;
  bool            FLastName_Specified;
  TXSDateTime*    FLastVisitDate;
  bool            FLastVisitDate_Specified;
  double          FLoadedPoints;
  bool            FLoadedPoints_Specified;
  UnicodeString   FMemberCardCode;
  bool            FMemberCardCode_Specified;
  int             FMemberType;
  bool            FMemberType_Specified;
  ArrayOfVoucherInfo FMemberVouchers;
  bool            FMemberVouchers_Specified;
  UnicodeString   FMembershipNumber;
  bool            FMembershipNumber_Specified;
  UnicodeString   FMiddleName;
  bool            FMiddleName_Specified;
  UnicodeString   FMobile;
  bool            FMobile_Specified;
  UnicodeString   FPhoneNumber;
  bool            FPhoneNumber_Specified;
  int             FPointRule;
  bool            FPointRule_Specified;
  double          FPreviousYearPoint;
  bool            FPreviousYearPoint_Specified;
  UnicodeString   FState;
  bool            FState_Specified;
  __int64         FTierId;
  bool            FTierId_Specified;
  UnicodeString   FTitle;
  bool            FTitle_Specified;
  UnicodeString   FUniqueId;
  bool            FUniqueId_Specified;
  TXSDateTime*    FYearStartDate;
  bool            FYearStartDate_Specified;
  UnicodeString   FZipCode;
  bool            FZipCode_Specified;
  void __fastcall SetActivated(int Index, bool _prop_val)
  {  FActivated = _prop_val; FActivated_Specified = true;  }
  bool __fastcall Activated_Specified(int Index)
  {  return FActivated_Specified;  } 
  void __fastcall SetAddress1(int Index, UnicodeString _prop_val)
  {  FAddress1 = _prop_val; FAddress1_Specified = true;  }
  bool __fastcall Address1_Specified(int Index)
  {  return FAddress1_Specified;  } 
  void __fastcall SetAddress2(int Index, UnicodeString _prop_val)
  {  FAddress2 = _prop_val; FAddress2_Specified = true;  }
  bool __fastcall Address2_Specified(int Index)
  {  return FAddress2_Specified;  } 
  void __fastcall SetAvailableBirthDayPoint(int Index, double _prop_val)
  {  FAvailableBirthDayPoint = _prop_val; FAvailableBirthDayPoint_Specified = true;  }
  bool __fastcall AvailableBirthDayPoint_Specified(int Index)
  {  return FAvailableBirthDayPoint_Specified;  } 
  void __fastcall SetAvailableFirstVisitPoint(int Index, double _prop_val)
  {  FAvailableFirstVisitPoint = _prop_val; FAvailableFirstVisitPoint_Specified = true;  }
  bool __fastcall AvailableFirstVisitPoint_Specified(int Index)
  {  return FAvailableFirstVisitPoint_Specified;  } 
  void __fastcall SetBirthdayBenefitDate(int Index, TXSDateTime* _prop_val)
  {  FBirthdayBenefitDate = _prop_val; FBirthdayBenefitDate_Specified = true;  }
  bool __fastcall BirthdayBenefitDate_Specified(int Index)
  {  return FBirthdayBenefitDate_Specified;  } 
  void __fastcall SetCardNumber(int Index, UnicodeString _prop_val)
  {  FCardNumber = _prop_val; FCardNumber_Specified = true;  }
  bool __fastcall CardNumber_Specified(int Index)
  {  return FCardNumber_Specified;  } 
  void __fastcall SetCity(int Index, UnicodeString _prop_val)
  {  FCity = _prop_val; FCity_Specified = true;  }
  bool __fastcall City_Specified(int Index)
  {  return FCity_Specified;  } 
  void __fastcall SetCountry(int Index, UnicodeString _prop_val)
  {  FCountry = _prop_val; FCountry_Specified = true;  }
  bool __fastcall Country_Specified(int Index)
  {  return FCountry_Specified;  } 
  void __fastcall SetCurrentYearPoint(int Index, double _prop_val)
  {  FCurrentYearPoint = _prop_val; FCurrentYearPoint_Specified = true;  }
  bool __fastcall CurrentYearPoint_Specified(int Index)
  {  return FCurrentYearPoint_Specified;  } 
  void __fastcall SetDateOfBirth(int Index, TXSDateTime* _prop_val)
  {  FDateOfBirth = _prop_val; FDateOfBirth_Specified = true;  }
  bool __fastcall DateOfBirth_Specified(int Index)
  {  return FDateOfBirth_Specified;  } 
  void __fastcall SetEarnedPoints(int Index, double _prop_val)
  {  FEarnedPoints = _prop_val; FEarnedPoints_Specified = true;  }
  bool __fastcall EarnedPoints_Specified(int Index)
  {  return FEarnedPoints_Specified;  } 
  void __fastcall SetEmail(int Index, UnicodeString _prop_val)
  {  FEmail = _prop_val; FEmail_Specified = true;  }
  bool __fastcall Email_Specified(int Index)
  {  return FEmail_Specified;  } 
  void __fastcall SetFirstName(int Index, UnicodeString _prop_val)
  {  FFirstName = _prop_val; FFirstName_Specified = true;  }
  bool __fastcall FirstName_Specified(int Index)
  {  return FFirstName_Specified;  } 
  void __fastcall SetGender(int Index, int _prop_val)
  {  FGender = _prop_val; FGender_Specified = true;  }
  bool __fastcall Gender_Specified(int Index)
  {  return FGender_Specified;  } 
  void __fastcall SetHasTransactions(int Index, bool _prop_val)
  {  FHasTransactions = _prop_val; FHasTransactions_Specified = true;  }
  bool __fastcall HasTransactions_Specified(int Index)
  {  return FHasTransactions_Specified;  } 
  void __fastcall SetHomeSiteId(int Index, __int64 _prop_val)
  {  FHomeSiteId = _prop_val; FHomeSiteId_Specified = true;  }
  bool __fastcall HomeSiteId_Specified(int Index)
  {  return FHomeSiteId_Specified;  } 
  void __fastcall SetIsFirstVisitRewarded(int Index, bool _prop_val)
  {  FIsFirstVisitRewarded = _prop_val; FIsFirstVisitRewarded_Specified = true;  }
  bool __fastcall IsFirstVisitRewarded_Specified(int Index)
  {  return FIsFirstVisitRewarded_Specified;  } 
  void __fastcall SetLastModified(int Index, TXSDateTime* _prop_val)
  {  FLastModified = _prop_val; FLastModified_Specified = true;  }
  bool __fastcall LastModified_Specified(int Index)
  {  return FLastModified_Specified;  } 
  void __fastcall SetLastName(int Index, UnicodeString _prop_val)
  {  FLastName = _prop_val; FLastName_Specified = true;  }
  bool __fastcall LastName_Specified(int Index)
  {  return FLastName_Specified;  } 
  void __fastcall SetLastVisitDate(int Index, TXSDateTime* _prop_val)
  {  FLastVisitDate = _prop_val; FLastVisitDate_Specified = true;  }
  bool __fastcall LastVisitDate_Specified(int Index)
  {  return FLastVisitDate_Specified;  } 
  void __fastcall SetLoadedPoints(int Index, double _prop_val)
  {  FLoadedPoints = _prop_val; FLoadedPoints_Specified = true;  }
  bool __fastcall LoadedPoints_Specified(int Index)
  {  return FLoadedPoints_Specified;  } 
  void __fastcall SetMemberCardCode(int Index, UnicodeString _prop_val)
  {  FMemberCardCode = _prop_val; FMemberCardCode_Specified = true;  }
  bool __fastcall MemberCardCode_Specified(int Index)
  {  return FMemberCardCode_Specified;  } 
  void __fastcall SetMemberType(int Index, int _prop_val)
  {  FMemberType = _prop_val; FMemberType_Specified = true;  }
  bool __fastcall MemberType_Specified(int Index)
  {  return FMemberType_Specified;  } 
  void __fastcall SetMemberVouchers(int Index, ArrayOfVoucherInfo _prop_val)
  {  FMemberVouchers = _prop_val; FMemberVouchers_Specified = true;  }
  bool __fastcall MemberVouchers_Specified(int Index)
  {  return FMemberVouchers_Specified;  } 
  void __fastcall SetMembershipNumber(int Index, UnicodeString _prop_val)
  {  FMembershipNumber = _prop_val; FMembershipNumber_Specified = true;  }
  bool __fastcall MembershipNumber_Specified(int Index)
  {  return FMembershipNumber_Specified;  } 
  void __fastcall SetMiddleName(int Index, UnicodeString _prop_val)
  {  FMiddleName = _prop_val; FMiddleName_Specified = true;  }
  bool __fastcall MiddleName_Specified(int Index)
  {  return FMiddleName_Specified;  } 
  void __fastcall SetMobile(int Index, UnicodeString _prop_val)
  {  FMobile = _prop_val; FMobile_Specified = true;  }
  bool __fastcall Mobile_Specified(int Index)
  {  return FMobile_Specified;  } 
  void __fastcall SetPhoneNumber(int Index, UnicodeString _prop_val)
  {  FPhoneNumber = _prop_val; FPhoneNumber_Specified = true;  }
  bool __fastcall PhoneNumber_Specified(int Index)
  {  return FPhoneNumber_Specified;  } 
  void __fastcall SetPointRule(int Index, int _prop_val)
  {  FPointRule = _prop_val; FPointRule_Specified = true;  }
  bool __fastcall PointRule_Specified(int Index)
  {  return FPointRule_Specified;  } 
  void __fastcall SetPreviousYearPoint(int Index, double _prop_val)
  {  FPreviousYearPoint = _prop_val; FPreviousYearPoint_Specified = true;  }
  bool __fastcall PreviousYearPoint_Specified(int Index)
  {  return FPreviousYearPoint_Specified;  } 
  void __fastcall SetState(int Index, UnicodeString _prop_val)
  {  FState = _prop_val; FState_Specified = true;  }
  bool __fastcall State_Specified(int Index)
  {  return FState_Specified;  } 
  void __fastcall SetTierId(int Index, __int64 _prop_val)
  {  FTierId = _prop_val; FTierId_Specified = true;  }
  bool __fastcall TierId_Specified(int Index)
  {  return FTierId_Specified;  } 
  void __fastcall SetTitle(int Index, UnicodeString _prop_val)
  {  FTitle = _prop_val; FTitle_Specified = true;  }
  bool __fastcall Title_Specified(int Index)
  {  return FTitle_Specified;  } 
  void __fastcall SetUniqueId(int Index, UnicodeString _prop_val)
  {  FUniqueId = _prop_val; FUniqueId_Specified = true;  }
  bool __fastcall UniqueId_Specified(int Index)
  {  return FUniqueId_Specified;  } 
  void __fastcall SetYearStartDate(int Index, TXSDateTime* _prop_val)
  {  FYearStartDate = _prop_val; FYearStartDate_Specified = true;  }
  bool __fastcall YearStartDate_Specified(int Index)
  {  return FYearStartDate_Specified;  } 
  void __fastcall SetZipCode(int Index, UnicodeString _prop_val)
  {  FZipCode = _prop_val; FZipCode_Specified = true;  }
  bool __fastcall ZipCode_Specified(int Index)
  {  return FZipCode_Specified;  } 

public:
  __fastcall ~MemberInfo();
__published:
  __property bool        Activated = { index=(IS_OPTN), read=FActivated, write=SetActivated, stored = Activated_Specified };
  __property UnicodeString   Address1 = { index=(IS_OPTN|IS_NLBL), read=FAddress1, write=SetAddress1, stored = Address1_Specified };
  __property UnicodeString   Address2 = { index=(IS_OPTN|IS_NLBL), read=FAddress2, write=SetAddress2, stored = Address2_Specified };
  __property double     AvailableBirthDayPoint = { index=(IS_OPTN), read=FAvailableBirthDayPoint, write=SetAvailableBirthDayPoint, stored = AvailableBirthDayPoint_Specified };
  __property double     AvailableFirstVisitPoint = { index=(IS_OPTN), read=FAvailableFirstVisitPoint, write=SetAvailableFirstVisitPoint, stored = AvailableFirstVisitPoint_Specified };
  __property TXSDateTime* BirthdayBenefitDate = { index=(IS_OPTN|IS_NLBL), read=FBirthdayBenefitDate, write=SetBirthdayBenefitDate, stored = BirthdayBenefitDate_Specified };
  __property UnicodeString CardNumber = { index=(IS_OPTN|IS_NLBL), read=FCardNumber, write=SetCardNumber, stored = CardNumber_Specified };
  __property UnicodeString       City = { index=(IS_OPTN|IS_NLBL), read=FCity, write=SetCity, stored = City_Specified };
  __property UnicodeString    Country = { index=(IS_OPTN|IS_NLBL), read=FCountry, write=SetCountry, stored = Country_Specified };
  __property double     CurrentYearPoint = { index=(IS_OPTN), read=FCurrentYearPoint, write=SetCurrentYearPoint, stored = CurrentYearPoint_Specified };
  __property TXSDateTime* DateOfBirth = { index=(IS_OPTN|IS_NLBL), read=FDateOfBirth, write=SetDateOfBirth, stored = DateOfBirth_Specified };
  __property double     EarnedPoints = { index=(IS_OPTN), read=FEarnedPoints, write=SetEarnedPoints, stored = EarnedPoints_Specified };
  __property UnicodeString      Email = { index=(IS_OPTN|IS_NLBL), read=FEmail, write=SetEmail, stored = Email_Specified };
  __property UnicodeString  FirstName = { index=(IS_OPTN|IS_NLBL), read=FFirstName, write=SetFirstName, stored = FirstName_Specified };
  __property int            Gender = { index=(IS_OPTN), read=FGender, write=SetGender, stored = Gender_Specified };
  __property bool       HasTransactions = { index=(IS_OPTN), read=FHasTransactions, write=SetHasTransactions, stored = HasTransactions_Specified };
  __property __int64    HomeSiteId = { index=(IS_OPTN), read=FHomeSiteId, write=SetHomeSiteId, stored = HomeSiteId_Specified };
  __property bool       IsFirstVisitRewarded = { index=(IS_OPTN), read=FIsFirstVisitRewarded, write=SetIsFirstVisitRewarded, stored = IsFirstVisitRewarded_Specified };
  __property TXSDateTime* LastModified = { index=(IS_OPTN), read=FLastModified, write=SetLastModified, stored = LastModified_Specified };
  __property UnicodeString   LastName = { index=(IS_OPTN|IS_NLBL), read=FLastName, write=SetLastName, stored = LastName_Specified };
  __property TXSDateTime* LastVisitDate = { index=(IS_OPTN|IS_NLBL), read=FLastVisitDate, write=SetLastVisitDate, stored = LastVisitDate_Specified };
  __property double     LoadedPoints = { index=(IS_OPTN), read=FLoadedPoints, write=SetLoadedPoints, stored = LoadedPoints_Specified };
  __property UnicodeString MemberCardCode = { index=(IS_OPTN|IS_NLBL), read=FMemberCardCode, write=SetMemberCardCode, stored = MemberCardCode_Specified };
  __property int        MemberType = { index=(IS_OPTN), read=FMemberType, write=SetMemberType, stored = MemberType_Specified };
  __property ArrayOfVoucherInfo MemberVouchers = { index=(IS_OPTN|IS_NLBL), read=FMemberVouchers, write=SetMemberVouchers, stored = MemberVouchers_Specified };
  __property UnicodeString MembershipNumber = { index=(IS_OPTN|IS_NLBL), read=FMembershipNumber, write=SetMembershipNumber, stored = MembershipNumber_Specified };
  __property UnicodeString MiddleName = { index=(IS_OPTN|IS_NLBL), read=FMiddleName, write=SetMiddleName, stored = MiddleName_Specified };
  __property UnicodeString     Mobile = { index=(IS_OPTN|IS_NLBL), read=FMobile, write=SetMobile, stored = Mobile_Specified };
  __property UnicodeString PhoneNumber = { index=(IS_OPTN|IS_NLBL), read=FPhoneNumber, write=SetPhoneNumber, stored = PhoneNumber_Specified };
  __property int         PointRule = { index=(IS_OPTN), read=FPointRule, write=SetPointRule, stored = PointRule_Specified };
  __property double     PreviousYearPoint = { index=(IS_OPTN), read=FPreviousYearPoint, write=SetPreviousYearPoint, stored = PreviousYearPoint_Specified };
  __property UnicodeString      State = { index=(IS_OPTN|IS_NLBL), read=FState, write=SetState, stored = State_Specified };
  __property __int64        TierId = { index=(IS_OPTN), read=FTierId, write=SetTierId, stored = TierId_Specified };
  __property UnicodeString      Title = { index=(IS_OPTN|IS_NLBL), read=FTitle, write=SetTitle, stored = Title_Specified };
  __property UnicodeString   UniqueId = { index=(IS_OPTN|IS_NLBL), read=FUniqueId, write=SetUniqueId, stored = UniqueId_Specified };
  __property TXSDateTime* YearStartDate = { index=(IS_OPTN|IS_NLBL), read=FYearStartDate, write=SetYearStartDate, stored = YearStartDate_Specified };
  __property UnicodeString    ZipCode = { index=(IS_OPTN|IS_NLBL), read=FZipCode, write=SetZipCode, stored = ZipCode_Specified };
};




// ************************************************************************ //
// XML       : VoucherInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class VoucherInfo : public TRemotable {
private:
  UnicodeString   FDiscountCode;
  bool            FDiscountCode_Specified;
  int             FNumberOfUsesRemaining;
  bool            FNumberOfUsesRemaining_Specified;
  UnicodeString   FVoucherDescription;
  bool            FVoucherDescription_Specified;
  UnicodeString   FVoucherName;
  bool            FVoucherName_Specified;
  void __fastcall SetDiscountCode(int Index, UnicodeString _prop_val)
  {  FDiscountCode = _prop_val; FDiscountCode_Specified = true;  }
  bool __fastcall DiscountCode_Specified(int Index)
  {  return FDiscountCode_Specified;  } 
  void __fastcall SetNumberOfUsesRemaining(int Index, int _prop_val)
  {  FNumberOfUsesRemaining = _prop_val; FNumberOfUsesRemaining_Specified = true;  }
  bool __fastcall NumberOfUsesRemaining_Specified(int Index)
  {  return FNumberOfUsesRemaining_Specified;  } 
  void __fastcall SetVoucherDescription(int Index, UnicodeString _prop_val)
  {  FVoucherDescription = _prop_val; FVoucherDescription_Specified = true;  }
  bool __fastcall VoucherDescription_Specified(int Index)
  {  return FVoucherDescription_Specified;  } 
  void __fastcall SetVoucherName(int Index, UnicodeString _prop_val)
  {  FVoucherName = _prop_val; FVoucherName_Specified = true;  }
  bool __fastcall VoucherName_Specified(int Index)
  {  return FVoucherName_Specified;  } 
__published:
  __property UnicodeString DiscountCode = { index=(IS_OPTN|IS_NLBL), read=FDiscountCode, write=SetDiscountCode, stored = DiscountCode_Specified };
  __property int        NumberOfUsesRemaining = { index=(IS_OPTN), read=FNumberOfUsesRemaining, write=SetNumberOfUsesRemaining, stored = NumberOfUsesRemaining_Specified };
  __property UnicodeString VoucherDescription = { index=(IS_OPTN|IS_NLBL), read=FVoucherDescription, write=SetVoucherDescription, stored = VoucherDescription_Specified };
  __property UnicodeString VoucherName = { index=(IS_OPTN|IS_NLBL), read=FVoucherName, write=SetVoucherName, stored = VoucherName_Specified };
};




// ************************************************************************ //
// XML       : LoyaltyMemberResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyMemberResponse : public MMServiceResponse {
private:
  MemberInfo*     FMemberInfo;
  bool            FMemberInfo_Specified;
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetMemberInfo(int Index, MemberInfo* _prop_val)
  {  FMemberInfo = _prop_val; FMemberInfo_Specified = true;  }
  bool __fastcall MemberInfo_Specified(int Index)
  {  return FMemberInfo_Specified;  } 
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 

public:
  __fastcall ~LoyaltyMemberResponse();
__published:
  __property MemberInfo* MemberInfo = { index=(IS_OPTN|IS_NLBL), read=FMemberInfo, write=SetMemberInfo, stored = MemberInfo_Specified };
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
};




// ************************************************************************ //
// XML       : RequestInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class RequestInfo : public TRemotable {
private:
  UnicodeString   FRequestKey;
  bool            FRequestKey_Specified;
  TXSDateTime*    FRequestTime;
  bool            FRequestTime_Specified;
  __int64         FSiteCode;
  bool            FSiteCode_Specified;
  void __fastcall SetRequestKey(int Index, UnicodeString _prop_val)
  {  FRequestKey = _prop_val; FRequestKey_Specified = true;  }
  bool __fastcall RequestKey_Specified(int Index)
  {  return FRequestKey_Specified;  } 
  void __fastcall SetRequestTime(int Index, TXSDateTime* _prop_val)
  {  FRequestTime = _prop_val; FRequestTime_Specified = true;  }
  bool __fastcall RequestTime_Specified(int Index)
  {  return FRequestTime_Specified;  } 
  void __fastcall SetSiteCode(int Index, __int64 _prop_val)
  {  FSiteCode = _prop_val; FSiteCode_Specified = true;  }
  bool __fastcall SiteCode_Specified(int Index)
  {  return FSiteCode_Specified;  } 

public:
  __fastcall ~RequestInfo();
__published:
  __property UnicodeString RequestKey = { index=(IS_OPTN|IS_NLBL), read=FRequestKey, write=SetRequestKey, stored = RequestKey_Specified };
  __property TXSDateTime* RequestTime = { index=(IS_OPTN), read=FRequestTime, write=SetRequestTime, stored = RequestTime_Specified };
  __property __int64      SiteCode = { index=(IS_OPTN), read=FSiteCode, write=SetSiteCode, stored = SiteCode_Specified };
};




// ************************************************************************ //
// XML       : LoyaltyResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyResponse : public MMServiceResponse {
private:
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 
__published:
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
};




// ************************************************************************ //
// XML       : TransactionInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class TransactionInfo : public TRemotable {
private:
  UnicodeString   FInvoiceNumber;
  bool            FInvoiceNumber_Specified;
  double          FPointsDelta;
  bool            FPointsDelta_Specified;
  int             FPointsType;
  bool            FPointsType_Specified;
  __int64         FSiteCode;
  bool            FSiteCode_Specified;
  TXSDateTime*    FTransactionDate;
  bool            FTransactionDate_Specified;
  UnicodeString   FUniqueId;
  bool            FUniqueId_Specified;
  void __fastcall SetInvoiceNumber(int Index, UnicodeString _prop_val)
  {  FInvoiceNumber = _prop_val; FInvoiceNumber_Specified = true;  }
  bool __fastcall InvoiceNumber_Specified(int Index)
  {  return FInvoiceNumber_Specified;  } 
  void __fastcall SetPointsDelta(int Index, double _prop_val)
  {  FPointsDelta = _prop_val; FPointsDelta_Specified = true;  }
  bool __fastcall PointsDelta_Specified(int Index)
  {  return FPointsDelta_Specified;  } 
  void __fastcall SetPointsType(int Index, int _prop_val)
  {  FPointsType = _prop_val; FPointsType_Specified = true;  }
  bool __fastcall PointsType_Specified(int Index)
  {  return FPointsType_Specified;  } 
  void __fastcall SetSiteCode(int Index, __int64 _prop_val)
  {  FSiteCode = _prop_val; FSiteCode_Specified = true;  }
  bool __fastcall SiteCode_Specified(int Index)
  {  return FSiteCode_Specified;  } 
  void __fastcall SetTransactionDate(int Index, TXSDateTime* _prop_val)
  {  FTransactionDate = _prop_val; FTransactionDate_Specified = true;  }
  bool __fastcall TransactionDate_Specified(int Index)
  {  return FTransactionDate_Specified;  } 
  void __fastcall SetUniqueId(int Index, UnicodeString _prop_val)
  {  FUniqueId = _prop_val; FUniqueId_Specified = true;  }
  bool __fastcall UniqueId_Specified(int Index)
  {  return FUniqueId_Specified;  } 

public:
  __fastcall ~TransactionInfo();
__published:
  __property UnicodeString InvoiceNumber = { index=(IS_OPTN|IS_NLBL), read=FInvoiceNumber, write=SetInvoiceNumber, stored = InvoiceNumber_Specified };
  __property double     PointsDelta = { index=(IS_OPTN), read=FPointsDelta, write=SetPointsDelta, stored = PointsDelta_Specified };
  __property int        PointsType = { index=(IS_OPTN), read=FPointsType, write=SetPointsType, stored = PointsType_Specified };
  __property __int64      SiteCode = { index=(IS_OPTN), read=FSiteCode, write=SetSiteCode, stored = SiteCode_Specified };
  __property TXSDateTime* TransactionDate = { index=(IS_OPTN), read=FTransactionDate, write=SetTransactionDate, stored = TransactionDate_Specified };
  __property UnicodeString   UniqueId = { index=(IS_OPTN|IS_NLBL), read=FUniqueId, write=SetUniqueId, stored = UniqueId_Specified };
};




// ************************************************************************ //
// XML       : LoyaltyCompanyResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyCompanyResponse : public MMServiceResponse {
private:
  CompanyInfo*    FCompanyInfo;
  bool            FCompanyInfo_Specified;
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetCompanyInfo(int Index, CompanyInfo* _prop_val)
  {  FCompanyInfo = _prop_val; FCompanyInfo_Specified = true;  }
  bool __fastcall CompanyInfo_Specified(int Index)
  {  return FCompanyInfo_Specified;  } 
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 

public:
  __fastcall ~LoyaltyCompanyResponse();
__published:
  __property CompanyInfo* CompanyInfo = { index=(IS_OPTN|IS_NLBL), read=FCompanyInfo, write=SetCompanyInfo, stored = CompanyInfo_Specified };
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
};


typedef DynamicArray<DiscountInfo*> ArrayOfDiscountInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate"[GblCplx] */
typedef DynamicArray<TierLevelInfo*> ArrayOfTierLevelInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate"[GblCplx] */


// ************************************************************************ //
// XML       : CompanyInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class CompanyInfo : public TRemotable {
private:
  ArrayOfDiscountInfo FDiscounts;
  bool            FDiscounts_Specified;
  bool            FHasGiftCardsAvailable;
  bool            FHasGiftCardsAvailable_Specified;
  bool            FHasPocketVouchersAvailable;
  bool            FHasPocketVouchersAvailable_Specified;
  ArrayOfTierLevelInfo FTierLevels;
  bool            FTierLevels_Specified;
  void __fastcall SetDiscounts(int Index, ArrayOfDiscountInfo _prop_val)
  {  FDiscounts = _prop_val; FDiscounts_Specified = true;  }
  bool __fastcall Discounts_Specified(int Index)
  {  return FDiscounts_Specified;  } 
  void __fastcall SetHasGiftCardsAvailable(int Index, bool _prop_val)
  {  FHasGiftCardsAvailable = _prop_val; FHasGiftCardsAvailable_Specified = true;  }
  bool __fastcall HasGiftCardsAvailable_Specified(int Index)
  {  return FHasGiftCardsAvailable_Specified;  } 
  void __fastcall SetHasPocketVouchersAvailable(int Index, bool _prop_val)
  {  FHasPocketVouchersAvailable = _prop_val; FHasPocketVouchersAvailable_Specified = true;  }
  bool __fastcall HasPocketVouchersAvailable_Specified(int Index)
  {  return FHasPocketVouchersAvailable_Specified;  } 
  void __fastcall SetTierLevels(int Index, ArrayOfTierLevelInfo _prop_val)
  {  FTierLevels = _prop_val; FTierLevels_Specified = true;  }
  bool __fastcall TierLevels_Specified(int Index)
  {  return FTierLevels_Specified;  } 

public:
  __fastcall ~CompanyInfo();
__published:
  __property ArrayOfDiscountInfo  Discounts = { index=(IS_OPTN|IS_NLBL), read=FDiscounts, write=SetDiscounts, stored = Discounts_Specified };
  __property bool       HasGiftCardsAvailable = { index=(IS_OPTN), read=FHasGiftCardsAvailable, write=SetHasGiftCardsAvailable, stored = HasGiftCardsAvailable_Specified };
  __property bool       HasPocketVouchersAvailable = { index=(IS_OPTN), read=FHasPocketVouchersAvailable, write=SetHasPocketVouchersAvailable, stored = HasPocketVouchersAvailable_Specified };
  __property ArrayOfTierLevelInfo TierLevels = { index=(IS_OPTN|IS_NLBL), read=FTierLevels, write=SetTierLevels, stored = TierLevels_Specified };
};




// ************************************************************************ //
// XML       : DiscountInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class DiscountInfo : public TRemotable {
private:
  int             FAppearanceOrder;
  bool            FAppearanceOrder_Specified;
  UnicodeString   FCode;
  bool            FCode_Specified;
  int             FDailyUsageAllowedPerMember;
  bool            FDailyUsageAllowedPerMember_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  int             FDiscountGroup;
  bool            FDiscountGroup_Specified;
  __int64         FDiscountId;
  bool            FDiscountId_Specified;
  DiscountType    FDiscountType;
  bool            FDiscountType_Specified;
  DisplayOption   FDisplayAs;
  bool            FDisplayAs_Specified;
  ImplicationType FImplicationType;
  bool            FImplicationType_Specified;
  bool            FIsAutoMembersDiscount;
  bool            FIsAutoMembersDiscount_Specified;
  bool            FIsCategoryFilterApplicable;
  bool            FIsCategoryFilterApplicable_Specified;
  bool            FIsMemberExemptDiscount;
  bool            FIsMemberExemptDiscount_Specified;
  bool            FIsMembersOnlyDiscount;
  bool            FIsMembersOnlyDiscount_Specified;
  int             FMaximumNumberOfItemsAllowed;
  bool            FMaximumNumberOfItemsAllowed_Specified;
  TXSDecimal*     FMaximumValue;
  bool            FMaximumValue_Specified;
  int             FMinimumNumberOfItemsAllowed;
  bool            FMinimumNumberOfItemsAllowed_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  int             FPriorityOrder;
  bool            FPriorityOrder_Specified;
  ProductPriority FProductPriority;
  bool            FProductPriority_Specified;
  TXSDecimal*     FRoundToDecimalPlaces;
  bool            FRoundToDecimalPlaces_Specified;
  TXSDecimal*     FValue;
  bool            FValue_Specified;
  void __fastcall SetAppearanceOrder(int Index, int _prop_val)
  {  FAppearanceOrder = _prop_val; FAppearanceOrder_Specified = true;  }
  bool __fastcall AppearanceOrder_Specified(int Index)
  {  return FAppearanceOrder_Specified;  } 
  void __fastcall SetCode(int Index, UnicodeString _prop_val)
  {  FCode = _prop_val; FCode_Specified = true;  }
  bool __fastcall Code_Specified(int Index)
  {  return FCode_Specified;  } 
  void __fastcall SetDailyUsageAllowedPerMember(int Index, int _prop_val)
  {  FDailyUsageAllowedPerMember = _prop_val; FDailyUsageAllowedPerMember_Specified = true;  }
  bool __fastcall DailyUsageAllowedPerMember_Specified(int Index)
  {  return FDailyUsageAllowedPerMember_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetDiscountGroup(int Index, int _prop_val)
  {  FDiscountGroup = _prop_val; FDiscountGroup_Specified = true;  }
  bool __fastcall DiscountGroup_Specified(int Index)
  {  return FDiscountGroup_Specified;  } 
  void __fastcall SetDiscountId(int Index, __int64 _prop_val)
  {  FDiscountId = _prop_val; FDiscountId_Specified = true;  }
  bool __fastcall DiscountId_Specified(int Index)
  {  return FDiscountId_Specified;  } 
  void __fastcall SetDiscountType(int Index, DiscountType _prop_val)
  {  FDiscountType = _prop_val; FDiscountType_Specified = true;  }
  bool __fastcall DiscountType_Specified(int Index)
  {  return FDiscountType_Specified;  } 
  void __fastcall SetDisplayAs(int Index, DisplayOption _prop_val)
  {  FDisplayAs = _prop_val; FDisplayAs_Specified = true;  }
  bool __fastcall DisplayAs_Specified(int Index)
  {  return FDisplayAs_Specified;  } 
  void __fastcall SetImplicationType(int Index, ImplicationType _prop_val)
  {  FImplicationType = _prop_val; FImplicationType_Specified = true;  }
  bool __fastcall ImplicationType_Specified(int Index)
  {  return FImplicationType_Specified;  } 
  void __fastcall SetIsAutoMembersDiscount(int Index, bool _prop_val)
  {  FIsAutoMembersDiscount = _prop_val; FIsAutoMembersDiscount_Specified = true;  }
  bool __fastcall IsAutoMembersDiscount_Specified(int Index)
  {  return FIsAutoMembersDiscount_Specified;  } 
  void __fastcall SetIsCategoryFilterApplicable(int Index, bool _prop_val)
  {  FIsCategoryFilterApplicable = _prop_val; FIsCategoryFilterApplicable_Specified = true;  }
  bool __fastcall IsCategoryFilterApplicable_Specified(int Index)
  {  return FIsCategoryFilterApplicable_Specified;  } 
  void __fastcall SetIsMemberExemptDiscount(int Index, bool _prop_val)
  {  FIsMemberExemptDiscount = _prop_val; FIsMemberExemptDiscount_Specified = true;  }
  bool __fastcall IsMemberExemptDiscount_Specified(int Index)
  {  return FIsMemberExemptDiscount_Specified;  } 
  void __fastcall SetIsMembersOnlyDiscount(int Index, bool _prop_val)
  {  FIsMembersOnlyDiscount = _prop_val; FIsMembersOnlyDiscount_Specified = true;  }
  bool __fastcall IsMembersOnlyDiscount_Specified(int Index)
  {  return FIsMembersOnlyDiscount_Specified;  } 
  void __fastcall SetMaximumNumberOfItemsAllowed(int Index, int _prop_val)
  {  FMaximumNumberOfItemsAllowed = _prop_val; FMaximumNumberOfItemsAllowed_Specified = true;  }
  bool __fastcall MaximumNumberOfItemsAllowed_Specified(int Index)
  {  return FMaximumNumberOfItemsAllowed_Specified;  } 
  void __fastcall SetMaximumValue(int Index, TXSDecimal* _prop_val)
  {  FMaximumValue = _prop_val; FMaximumValue_Specified = true;  }
  bool __fastcall MaximumValue_Specified(int Index)
  {  return FMaximumValue_Specified;  } 
  void __fastcall SetMinimumNumberOfItemsAllowed(int Index, int _prop_val)
  {  FMinimumNumberOfItemsAllowed = _prop_val; FMinimumNumberOfItemsAllowed_Specified = true;  }
  bool __fastcall MinimumNumberOfItemsAllowed_Specified(int Index)
  {  return FMinimumNumberOfItemsAllowed_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetPriorityOrder(int Index, int _prop_val)
  {  FPriorityOrder = _prop_val; FPriorityOrder_Specified = true;  }
  bool __fastcall PriorityOrder_Specified(int Index)
  {  return FPriorityOrder_Specified;  } 
  void __fastcall SetProductPriority(int Index, ProductPriority _prop_val)
  {  FProductPriority = _prop_val; FProductPriority_Specified = true;  }
  bool __fastcall ProductPriority_Specified(int Index)
  {  return FProductPriority_Specified;  } 
  void __fastcall SetRoundToDecimalPlaces(int Index, TXSDecimal* _prop_val)
  {  FRoundToDecimalPlaces = _prop_val; FRoundToDecimalPlaces_Specified = true;  }
  bool __fastcall RoundToDecimalPlaces_Specified(int Index)
  {  return FRoundToDecimalPlaces_Specified;  } 
  void __fastcall SetValue(int Index, TXSDecimal* _prop_val)
  {  FValue = _prop_val; FValue_Specified = true;  }
  bool __fastcall Value_Specified(int Index)
  {  return FValue_Specified;  } 

public:
  __fastcall ~DiscountInfo();
__published:
  __property int        AppearanceOrder = { index=(IS_OPTN|IS_NLBL), read=FAppearanceOrder, write=SetAppearanceOrder, stored = AppearanceOrder_Specified };
  __property UnicodeString       Code = { index=(IS_OPTN|IS_NLBL), read=FCode, write=SetCode, stored = Code_Specified };
  __property int        DailyUsageAllowedPerMember = { index=(IS_OPTN), read=FDailyUsageAllowedPerMember, write=SetDailyUsageAllowedPerMember, stored = DailyUsageAllowedPerMember_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property int        DiscountGroup = { index=(IS_OPTN), read=FDiscountGroup, write=SetDiscountGroup, stored = DiscountGroup_Specified };
  __property __int64    DiscountId = { index=(IS_OPTN), read=FDiscountId, write=SetDiscountId, stored = DiscountId_Specified };
  __property DiscountType DiscountType = { index=(IS_OPTN), read=FDiscountType, write=SetDiscountType, stored = DiscountType_Specified };
  __property DisplayOption  DisplayAs = { index=(IS_OPTN), read=FDisplayAs, write=SetDisplayAs, stored = DisplayAs_Specified };
  __property ImplicationType ImplicationType = { index=(IS_OPTN), read=FImplicationType, write=SetImplicationType, stored = ImplicationType_Specified };
  __property bool       IsAutoMembersDiscount = { index=(IS_OPTN), read=FIsAutoMembersDiscount, write=SetIsAutoMembersDiscount, stored = IsAutoMembersDiscount_Specified };
  __property bool       IsCategoryFilterApplicable = { index=(IS_OPTN), read=FIsCategoryFilterApplicable, write=SetIsCategoryFilterApplicable, stored = IsCategoryFilterApplicable_Specified };
  __property bool       IsMemberExemptDiscount = { index=(IS_OPTN), read=FIsMemberExemptDiscount, write=SetIsMemberExemptDiscount, stored = IsMemberExemptDiscount_Specified };
  __property bool       IsMembersOnlyDiscount = { index=(IS_OPTN), read=FIsMembersOnlyDiscount, write=SetIsMembersOnlyDiscount, stored = IsMembersOnlyDiscount_Specified };
  __property int        MaximumNumberOfItemsAllowed = { index=(IS_OPTN), read=FMaximumNumberOfItemsAllowed, write=SetMaximumNumberOfItemsAllowed, stored = MaximumNumberOfItemsAllowed_Specified };
  __property TXSDecimal* MaximumValue = { index=(IS_OPTN), read=FMaximumValue, write=SetMaximumValue, stored = MaximumValue_Specified };
  __property int        MinimumNumberOfItemsAllowed = { index=(IS_OPTN), read=FMinimumNumberOfItemsAllowed, write=SetMinimumNumberOfItemsAllowed, stored = MinimumNumberOfItemsAllowed_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property int        PriorityOrder = { index=(IS_OPTN|IS_NLBL), read=FPriorityOrder, write=SetPriorityOrder, stored = PriorityOrder_Specified };
  __property ProductPriority ProductPriority = { index=(IS_OPTN), read=FProductPriority, write=SetProductPriority, stored = ProductPriority_Specified };
  __property TXSDecimal* RoundToDecimalPlaces = { index=(IS_OPTN), read=FRoundToDecimalPlaces, write=SetRoundToDecimalPlaces, stored = RoundToDecimalPlaces_Specified };
  __property TXSDecimal*      Value = { index=(IS_OPTN), read=FValue, write=SetValue, stored = Value_Specified };
};




// ************************************************************************ //
// XML       : TierLevelInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class TierLevelInfo : public TRemotable {
private:
  bool            FAllowEarntLoyaltyRedemption;
  bool            FAllowEarntLoyaltyRedemption_Specified;
  double          FBirthdayBonus;
  bool            FBirthdayBonus_Specified;
  bool            FChangeCard;
  bool            FChangeCard_Specified;
  int             FLevel;
  bool            FLevel_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  double          FPointsRequired;
  bool            FPointsRequired_Specified;
  double          FPricedRedemptionRate;
  bool            FPricedRedemptionRate_Specified;
  bool            FSendMailToHeadOffice;
  bool            FSendMailToHeadOffice_Specified;
  bool            FSendMailToMember;
  bool            FSendMailToMember_Specified;
  int             FTierId;
  bool            FTierId_Specified;
  double          FWeighedRedemptionPoints;
  bool            FWeighedRedemptionPoints_Specified;
  double          FWeighedRedemptionWeight;
  bool            FWeighedRedemptionWeight_Specified;
  void __fastcall SetAllowEarntLoyaltyRedemption(int Index, bool _prop_val)
  {  FAllowEarntLoyaltyRedemption = _prop_val; FAllowEarntLoyaltyRedemption_Specified = true;  }
  bool __fastcall AllowEarntLoyaltyRedemption_Specified(int Index)
  {  return FAllowEarntLoyaltyRedemption_Specified;  } 
  void __fastcall SetBirthdayBonus(int Index, double _prop_val)
  {  FBirthdayBonus = _prop_val; FBirthdayBonus_Specified = true;  }
  bool __fastcall BirthdayBonus_Specified(int Index)
  {  return FBirthdayBonus_Specified;  } 
  void __fastcall SetChangeCard(int Index, bool _prop_val)
  {  FChangeCard = _prop_val; FChangeCard_Specified = true;  }
  bool __fastcall ChangeCard_Specified(int Index)
  {  return FChangeCard_Specified;  } 
  void __fastcall SetLevel(int Index, int _prop_val)
  {  FLevel = _prop_val; FLevel_Specified = true;  }
  bool __fastcall Level_Specified(int Index)
  {  return FLevel_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetPointsRequired(int Index, double _prop_val)
  {  FPointsRequired = _prop_val; FPointsRequired_Specified = true;  }
  bool __fastcall PointsRequired_Specified(int Index)
  {  return FPointsRequired_Specified;  } 
  void __fastcall SetPricedRedemptionRate(int Index, double _prop_val)
  {  FPricedRedemptionRate = _prop_val; FPricedRedemptionRate_Specified = true;  }
  bool __fastcall PricedRedemptionRate_Specified(int Index)
  {  return FPricedRedemptionRate_Specified;  } 
  void __fastcall SetSendMailToHeadOffice(int Index, bool _prop_val)
  {  FSendMailToHeadOffice = _prop_val; FSendMailToHeadOffice_Specified = true;  }
  bool __fastcall SendMailToHeadOffice_Specified(int Index)
  {  return FSendMailToHeadOffice_Specified;  } 
  void __fastcall SetSendMailToMember(int Index, bool _prop_val)
  {  FSendMailToMember = _prop_val; FSendMailToMember_Specified = true;  }
  bool __fastcall SendMailToMember_Specified(int Index)
  {  return FSendMailToMember_Specified;  } 
  void __fastcall SetTierId(int Index, int _prop_val)
  {  FTierId = _prop_val; FTierId_Specified = true;  }
  bool __fastcall TierId_Specified(int Index)
  {  return FTierId_Specified;  } 
  void __fastcall SetWeighedRedemptionPoints(int Index, double _prop_val)
  {  FWeighedRedemptionPoints = _prop_val; FWeighedRedemptionPoints_Specified = true;  }
  bool __fastcall WeighedRedemptionPoints_Specified(int Index)
  {  return FWeighedRedemptionPoints_Specified;  } 
  void __fastcall SetWeighedRedemptionWeight(int Index, double _prop_val)
  {  FWeighedRedemptionWeight = _prop_val; FWeighedRedemptionWeight_Specified = true;  }
  bool __fastcall WeighedRedemptionWeight_Specified(int Index)
  {  return FWeighedRedemptionWeight_Specified;  } 
__published:
  __property bool       AllowEarntLoyaltyRedemption = { index=(IS_OPTN), read=FAllowEarntLoyaltyRedemption, write=SetAllowEarntLoyaltyRedemption, stored = AllowEarntLoyaltyRedemption_Specified };
  __property double     BirthdayBonus = { index=(IS_OPTN), read=FBirthdayBonus, write=SetBirthdayBonus, stored = BirthdayBonus_Specified };
  __property bool       ChangeCard = { index=(IS_OPTN), read=FChangeCard, write=SetChangeCard, stored = ChangeCard_Specified };
  __property int             Level = { index=(IS_OPTN), read=FLevel, write=SetLevel, stored = Level_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property double     PointsRequired = { index=(IS_OPTN), read=FPointsRequired, write=SetPointsRequired, stored = PointsRequired_Specified };
  __property double     PricedRedemptionRate = { index=(IS_OPTN), read=FPricedRedemptionRate, write=SetPricedRedemptionRate, stored = PricedRedemptionRate_Specified };
  __property bool       SendMailToHeadOffice = { index=(IS_OPTN), read=FSendMailToHeadOffice, write=SetSendMailToHeadOffice, stored = SendMailToHeadOffice_Specified };
  __property bool       SendMailToMember = { index=(IS_OPTN), read=FSendMailToMember, write=SetSendMailToMember, stored = SendMailToMember_Specified };
  __property int            TierId = { index=(IS_OPTN), read=FTierId, write=SetTierId, stored = TierId_Specified };
  __property double     WeighedRedemptionPoints = { index=(IS_OPTN), read=FWeighedRedemptionPoints, write=SetWeighedRedemptionPoints, stored = WeighedRedemptionPoints_Specified };
  __property double     WeighedRedemptionWeight = { index=(IS_OPTN), read=FWeighedRedemptionWeight, write=SetWeighedRedemptionWeight, stored = WeighedRedemptionWeight_Specified };
};




// ************************************************************************ //
// XML       : LoyaltyGiftCardResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyGiftCardResponse : public MMServiceResponse {
private:
  GiftCardInfo*   FGiftCardInfo;
  bool            FGiftCardInfo_Specified;
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetGiftCardInfo(int Index, GiftCardInfo* _prop_val)
  {  FGiftCardInfo = _prop_val; FGiftCardInfo_Specified = true;  }
  bool __fastcall GiftCardInfo_Specified(int Index)
  {  return FGiftCardInfo_Specified;  } 
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 

public:
  __fastcall ~LoyaltyGiftCardResponse();
__published:
  __property GiftCardInfo* GiftCardInfo = { index=(IS_OPTN|IS_NLBL), read=FGiftCardInfo, write=SetGiftCardInfo, stored = GiftCardInfo_Specified };
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
};




// ************************************************************************ //
// XML       : GiftCardInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class GiftCardInfo : public TRemotable {
private:
  TXSDateTime*    FExpiryDate;
  bool            FExpiryDate_Specified;
  UnicodeString   FGiftCardNumber;
  bool            FGiftCardNumber_Specified;
  double          FPointBalance;
  bool            FPointBalance_Specified;
  UnicodeString   FResponseMessage;
  bool            FResponseMessage_Specified;
  TXSDateTime*    FStartDate;
  bool            FStartDate_Specified;
  int             FStatusCode;
  bool            FStatusCode_Specified;
  void __fastcall SetExpiryDate(int Index, TXSDateTime* _prop_val)
  {  FExpiryDate = _prop_val; FExpiryDate_Specified = true;  }
  bool __fastcall ExpiryDate_Specified(int Index)
  {  return FExpiryDate_Specified;  } 
  void __fastcall SetGiftCardNumber(int Index, UnicodeString _prop_val)
  {  FGiftCardNumber = _prop_val; FGiftCardNumber_Specified = true;  }
  bool __fastcall GiftCardNumber_Specified(int Index)
  {  return FGiftCardNumber_Specified;  } 
  void __fastcall SetPointBalance(int Index, double _prop_val)
  {  FPointBalance = _prop_val; FPointBalance_Specified = true;  }
  bool __fastcall PointBalance_Specified(int Index)
  {  return FPointBalance_Specified;  } 
  void __fastcall SetResponseMessage(int Index, UnicodeString _prop_val)
  {  FResponseMessage = _prop_val; FResponseMessage_Specified = true;  }
  bool __fastcall ResponseMessage_Specified(int Index)
  {  return FResponseMessage_Specified;  } 
  void __fastcall SetStartDate(int Index, TXSDateTime* _prop_val)
  {  FStartDate = _prop_val; FStartDate_Specified = true;  }
  bool __fastcall StartDate_Specified(int Index)
  {  return FStartDate_Specified;  } 
  void __fastcall SetStatusCode(int Index, int _prop_val)
  {  FStatusCode = _prop_val; FStatusCode_Specified = true;  }
  bool __fastcall StatusCode_Specified(int Index)
  {  return FStatusCode_Specified;  } 

public:
  __fastcall ~GiftCardInfo();
__published:
  __property TXSDateTime* ExpiryDate = { index=(IS_OPTN), read=FExpiryDate, write=SetExpiryDate, stored = ExpiryDate_Specified };
  __property UnicodeString GiftCardNumber = { index=(IS_OPTN|IS_NLBL), read=FGiftCardNumber, write=SetGiftCardNumber, stored = GiftCardNumber_Specified };
  __property double     PointBalance = { index=(IS_OPTN), read=FPointBalance, write=SetPointBalance, stored = PointBalance_Specified };
  __property UnicodeString ResponseMessage = { index=(IS_OPTN|IS_NLBL), read=FResponseMessage, write=SetResponseMessage, stored = ResponseMessage_Specified };
  __property TXSDateTime*  StartDate = { index=(IS_OPTN), read=FStartDate, write=SetStartDate, stored = StartDate_Specified };
  __property int        StatusCode = { index=(IS_OPTN), read=FStatusCode, write=SetStatusCode, stored = StatusCode_Specified };
};




// ************************************************************************ //
// XML       : LoyaltyVoucherResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyVoucherResponse : public MMServiceResponse {
private:
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  VoucherInfo*    FVoucherInfo;
  bool            FVoucherInfo_Specified;
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 
  void __fastcall SetVoucherInfo(int Index, VoucherInfo* _prop_val)
  {  FVoucherInfo = _prop_val; FVoucherInfo_Specified = true;  }
  bool __fastcall VoucherInfo_Specified(int Index)
  {  return FVoucherInfo_Specified;  } 

public:
  __fastcall ~LoyaltyVoucherResponse();
__published:
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
  __property VoucherInfo* VoucherInfo = { index=(IS_OPTN|IS_NLBL), read=FVoucherInfo, write=SetVoucherInfo, stored = VoucherInfo_Specified };
};


typedef DynamicArray<DiscountUsageInfo*> ArrayOfDiscountUsageInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate"[GblCplx] */


// ************************************************************************ //
// XML       : VoucherTransactionInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class VoucherTransactionInfo : public TRemotable {
private:
  ArrayOfDiscountUsageInfo FDiscountUsages;
  bool            FDiscountUsages_Specified;
  UnicodeString   FGiftCardNumber;
  bool            FGiftCardNumber_Specified;
  UnicodeString   FInvoiceNumber;
  bool            FInvoiceNumber_Specified;
  UnicodeString   FMemberUniqueId;
  bool            FMemberUniqueId_Specified;
  double          FMemberVoucherDiscountAmount;
  bool            FMemberVoucherDiscountAmount_Specified;
  double          FPocketVoucherDiscountAmount;
  bool            FPocketVoucherDiscountAmount_Specified;
  UnicodeString   FPocketVoucherNumber;
  bool            FPocketVoucherNumber_Specified;
  double          FPointsPurchased;
  bool            FPointsPurchased_Specified;
  double          FPointsRedeemed;
  bool            FPointsRedeemed_Specified;
  UnicodeString   FPurchasedGiftCardNumber;
  bool            FPurchasedGiftCardNumber_Specified;
  __int64         FSiteCode;
  bool            FSiteCode_Specified;
  double          FTotalSaleAmount;
  bool            FTotalSaleAmount_Specified;
  TXSDateTime*    FTransactionDate;
  bool            FTransactionDate_Specified;
  UnicodeString   FTransactionReferenceNumber;
  bool            FTransactionReferenceNumber_Specified;
  UnicodeString   FVoucherName;
  bool            FVoucherName_Specified;
  void __fastcall SetDiscountUsages(int Index, ArrayOfDiscountUsageInfo _prop_val)
  {  FDiscountUsages = _prop_val; FDiscountUsages_Specified = true;  }
  bool __fastcall DiscountUsages_Specified(int Index)
  {  return FDiscountUsages_Specified;  } 
  void __fastcall SetGiftCardNumber(int Index, UnicodeString _prop_val)
  {  FGiftCardNumber = _prop_val; FGiftCardNumber_Specified = true;  }
  bool __fastcall GiftCardNumber_Specified(int Index)
  {  return FGiftCardNumber_Specified;  } 
  void __fastcall SetInvoiceNumber(int Index, UnicodeString _prop_val)
  {  FInvoiceNumber = _prop_val; FInvoiceNumber_Specified = true;  }
  bool __fastcall InvoiceNumber_Specified(int Index)
  {  return FInvoiceNumber_Specified;  } 
  void __fastcall SetMemberUniqueId(int Index, UnicodeString _prop_val)
  {  FMemberUniqueId = _prop_val; FMemberUniqueId_Specified = true;  }
  bool __fastcall MemberUniqueId_Specified(int Index)
  {  return FMemberUniqueId_Specified;  } 
  void __fastcall SetMemberVoucherDiscountAmount(int Index, double _prop_val)
  {  FMemberVoucherDiscountAmount = _prop_val; FMemberVoucherDiscountAmount_Specified = true;  }
  bool __fastcall MemberVoucherDiscountAmount_Specified(int Index)
  {  return FMemberVoucherDiscountAmount_Specified;  } 
  void __fastcall SetPocketVoucherDiscountAmount(int Index, double _prop_val)
  {  FPocketVoucherDiscountAmount = _prop_val; FPocketVoucherDiscountAmount_Specified = true;  }
  bool __fastcall PocketVoucherDiscountAmount_Specified(int Index)
  {  return FPocketVoucherDiscountAmount_Specified;  } 
  void __fastcall SetPocketVoucherNumber(int Index, UnicodeString _prop_val)
  {  FPocketVoucherNumber = _prop_val; FPocketVoucherNumber_Specified = true;  }
  bool __fastcall PocketVoucherNumber_Specified(int Index)
  {  return FPocketVoucherNumber_Specified;  } 
  void __fastcall SetPointsPurchased(int Index, double _prop_val)
  {  FPointsPurchased = _prop_val; FPointsPurchased_Specified = true;  }
  bool __fastcall PointsPurchased_Specified(int Index)
  {  return FPointsPurchased_Specified;  } 
  void __fastcall SetPointsRedeemed(int Index, double _prop_val)
  {  FPointsRedeemed = _prop_val; FPointsRedeemed_Specified = true;  }
  bool __fastcall PointsRedeemed_Specified(int Index)
  {  return FPointsRedeemed_Specified;  } 
  void __fastcall SetPurchasedGiftCardNumber(int Index, UnicodeString _prop_val)
  {  FPurchasedGiftCardNumber = _prop_val; FPurchasedGiftCardNumber_Specified = true;  }
  bool __fastcall PurchasedGiftCardNumber_Specified(int Index)
  {  return FPurchasedGiftCardNumber_Specified;  } 
  void __fastcall SetSiteCode(int Index, __int64 _prop_val)
  {  FSiteCode = _prop_val; FSiteCode_Specified = true;  }
  bool __fastcall SiteCode_Specified(int Index)
  {  return FSiteCode_Specified;  } 
  void __fastcall SetTotalSaleAmount(int Index, double _prop_val)
  {  FTotalSaleAmount = _prop_val; FTotalSaleAmount_Specified = true;  }
  bool __fastcall TotalSaleAmount_Specified(int Index)
  {  return FTotalSaleAmount_Specified;  } 
  void __fastcall SetTransactionDate(int Index, TXSDateTime* _prop_val)
  {  FTransactionDate = _prop_val; FTransactionDate_Specified = true;  }
  bool __fastcall TransactionDate_Specified(int Index)
  {  return FTransactionDate_Specified;  } 
  void __fastcall SetTransactionReferenceNumber(int Index, UnicodeString _prop_val)
  {  FTransactionReferenceNumber = _prop_val; FTransactionReferenceNumber_Specified = true;  }
  bool __fastcall TransactionReferenceNumber_Specified(int Index)
  {  return FTransactionReferenceNumber_Specified;  } 
  void __fastcall SetVoucherName(int Index, UnicodeString _prop_val)
  {  FVoucherName = _prop_val; FVoucherName_Specified = true;  }
  bool __fastcall VoucherName_Specified(int Index)
  {  return FVoucherName_Specified;  } 

public:
  __fastcall ~VoucherTransactionInfo();
__published:
  __property ArrayOfDiscountUsageInfo DiscountUsages = { index=(IS_OPTN|IS_NLBL), read=FDiscountUsages, write=SetDiscountUsages, stored = DiscountUsages_Specified };
  __property UnicodeString GiftCardNumber = { index=(IS_OPTN|IS_NLBL), read=FGiftCardNumber, write=SetGiftCardNumber, stored = GiftCardNumber_Specified };
  __property UnicodeString InvoiceNumber = { index=(IS_OPTN|IS_NLBL), read=FInvoiceNumber, write=SetInvoiceNumber, stored = InvoiceNumber_Specified };
  __property UnicodeString MemberUniqueId = { index=(IS_OPTN|IS_NLBL), read=FMemberUniqueId, write=SetMemberUniqueId, stored = MemberUniqueId_Specified };
  __property double     MemberVoucherDiscountAmount = { index=(IS_OPTN), read=FMemberVoucherDiscountAmount, write=SetMemberVoucherDiscountAmount, stored = MemberVoucherDiscountAmount_Specified };
  __property double     PocketVoucherDiscountAmount = { index=(IS_OPTN), read=FPocketVoucherDiscountAmount, write=SetPocketVoucherDiscountAmount, stored = PocketVoucherDiscountAmount_Specified };
  __property UnicodeString PocketVoucherNumber = { index=(IS_OPTN|IS_NLBL), read=FPocketVoucherNumber, write=SetPocketVoucherNumber, stored = PocketVoucherNumber_Specified };
  __property double     PointsPurchased = { index=(IS_OPTN), read=FPointsPurchased, write=SetPointsPurchased, stored = PointsPurchased_Specified };
  __property double     PointsRedeemed = { index=(IS_OPTN), read=FPointsRedeemed, write=SetPointsRedeemed, stored = PointsRedeemed_Specified };
  __property UnicodeString PurchasedGiftCardNumber = { index=(IS_OPTN|IS_NLBL), read=FPurchasedGiftCardNumber, write=SetPurchasedGiftCardNumber, stored = PurchasedGiftCardNumber_Specified };
  __property __int64      SiteCode = { index=(IS_OPTN), read=FSiteCode, write=SetSiteCode, stored = SiteCode_Specified };
  __property double     TotalSaleAmount = { index=(IS_OPTN), read=FTotalSaleAmount, write=SetTotalSaleAmount, stored = TotalSaleAmount_Specified };
  __property TXSDateTime* TransactionDate = { index=(IS_OPTN), read=FTransactionDate, write=SetTransactionDate, stored = TransactionDate_Specified };
  __property UnicodeString TransactionReferenceNumber = { index=(IS_OPTN|IS_NLBL), read=FTransactionReferenceNumber, write=SetTransactionReferenceNumber, stored = TransactionReferenceNumber_Specified };
  __property UnicodeString VoucherName = { index=(IS_OPTN|IS_NLBL), read=FVoucherName, write=SetVoucherName, stored = VoucherName_Specified };
};




// ************************************************************************ //
// XML       : DiscountUsageInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class DiscountUsageInfo : public TRemotable {
private:
  double          FDiscountAmount;
  bool            FDiscountAmount_Specified;
  UnicodeString   FDiscountCode;
  bool            FDiscountCode_Specified;
  void __fastcall SetDiscountAmount(int Index, double _prop_val)
  {  FDiscountAmount = _prop_val; FDiscountAmount_Specified = true;  }
  bool __fastcall DiscountAmount_Specified(int Index)
  {  return FDiscountAmount_Specified;  } 
  void __fastcall SetDiscountCode(int Index, UnicodeString _prop_val)
  {  FDiscountCode = _prop_val; FDiscountCode_Specified = true;  }
  bool __fastcall DiscountCode_Specified(int Index)
  {  return FDiscountCode_Specified;  } 
__published:
  __property double     DiscountAmount = { index=(IS_OPTN), read=FDiscountAmount, write=SetDiscountAmount, stored = DiscountAmount_Specified };
  __property UnicodeString DiscountCode = { index=(IS_OPTN|IS_NLBL), read=FDiscountCode, write=SetDiscountCode, stored = DiscountCode_Specified };
};




// ************************************************************************ //
// XML       : VoucherTransactionResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class VoucherTransactionResponse : public MMServiceResponse {
private:
  TXSDateTime*    FGiftCardExpiryDate;
  bool            FGiftCardExpiryDate_Specified;
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetGiftCardExpiryDate(int Index, TXSDateTime* _prop_val)
  {  FGiftCardExpiryDate = _prop_val; FGiftCardExpiryDate_Specified = true;  }
  bool __fastcall GiftCardExpiryDate_Specified(int Index)
  {  return FGiftCardExpiryDate_Specified;  } 
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 

public:
  __fastcall ~VoucherTransactionResponse();
__published:
  __property TXSDateTime* GiftCardExpiryDate = { index=(IS_OPTN|IS_NLBL), read=FGiftCardExpiryDate, write=SetGiftCardExpiryDate, stored = GiftCardExpiryDate_Specified };
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
};




// ************************************************************************ //
// XML       : ReleasedVoucherInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class ReleasedVoucherInfo : public TRemotable {
private:
  ArrayOfDiscountUsageInfo FDiscountCodes;
  bool            FDiscountCodes_Specified;
  UnicodeString   FGiftCardNumber;
  bool            FGiftCardNumber_Specified;
  UnicodeString   FPocketVoucherNumber;
  bool            FPocketVoucherNumber_Specified;
  UnicodeString   FTransactionReferenceNumber;
  bool            FTransactionReferenceNumber_Specified;
  UnicodeString   FVoucherName;
  bool            FVoucherName_Specified;
  void __fastcall SetDiscountCodes(int Index, ArrayOfDiscountUsageInfo _prop_val)
  {  FDiscountCodes = _prop_val; FDiscountCodes_Specified = true;  }
  bool __fastcall DiscountCodes_Specified(int Index)
  {  return FDiscountCodes_Specified;  } 
  void __fastcall SetGiftCardNumber(int Index, UnicodeString _prop_val)
  {  FGiftCardNumber = _prop_val; FGiftCardNumber_Specified = true;  }
  bool __fastcall GiftCardNumber_Specified(int Index)
  {  return FGiftCardNumber_Specified;  } 
  void __fastcall SetPocketVoucherNumber(int Index, UnicodeString _prop_val)
  {  FPocketVoucherNumber = _prop_val; FPocketVoucherNumber_Specified = true;  }
  bool __fastcall PocketVoucherNumber_Specified(int Index)
  {  return FPocketVoucherNumber_Specified;  } 
  void __fastcall SetTransactionReferenceNumber(int Index, UnicodeString _prop_val)
  {  FTransactionReferenceNumber = _prop_val; FTransactionReferenceNumber_Specified = true;  }
  bool __fastcall TransactionReferenceNumber_Specified(int Index)
  {  return FTransactionReferenceNumber_Specified;  } 
  void __fastcall SetVoucherName(int Index, UnicodeString _prop_val)
  {  FVoucherName = _prop_val; FVoucherName_Specified = true;  }
  bool __fastcall VoucherName_Specified(int Index)
  {  return FVoucherName_Specified;  } 

public:
  __fastcall ~ReleasedVoucherInfo();
__published:
  __property ArrayOfDiscountUsageInfo DiscountCodes = { index=(IS_OPTN|IS_NLBL), read=FDiscountCodes, write=SetDiscountCodes, stored = DiscountCodes_Specified };
  __property UnicodeString GiftCardNumber = { index=(IS_OPTN|IS_NLBL), read=FGiftCardNumber, write=SetGiftCardNumber, stored = GiftCardNumber_Specified };
  __property UnicodeString PocketVoucherNumber = { index=(IS_OPTN|IS_NLBL), read=FPocketVoucherNumber, write=SetPocketVoucherNumber, stored = PocketVoucherNumber_Specified };
  __property UnicodeString TransactionReferenceNumber = { index=(IS_OPTN|IS_NLBL), read=FTransactionReferenceNumber, write=SetTransactionReferenceNumber, stored = TransactionReferenceNumber_Specified };
  __property UnicodeString VoucherName = { index=(IS_OPTN|IS_NLBL), read=FVoucherName, write=SetVoucherName, stored = VoucherName_Specified };
};




// ************************************************************************ //
// XML       : LoyaltyOnlineOrderingResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyOnlineOrderingResponse : public TRemotable {
private:
  bool            FIsSuccessful;
  bool            FIsSuccessful_Specified;
  UnicodeString   FResponseText;
  bool            FResponseText_Specified;
  void __fastcall SetIsSuccessful(int Index, bool _prop_val)
  {  FIsSuccessful = _prop_val; FIsSuccessful_Specified = true;  }
  bool __fastcall IsSuccessful_Specified(int Index)
  {  return FIsSuccessful_Specified;  } 
  void __fastcall SetResponseText(int Index, UnicodeString _prop_val)
  {  FResponseText = _prop_val; FResponseText_Specified = true;  }
  bool __fastcall ResponseText_Specified(int Index)
  {  return FResponseText_Specified;  } 
__published:
  __property bool       IsSuccessful = { index=(IS_OPTN), read=FIsSuccessful, write=SetIsSuccessful, stored = IsSuccessful_Specified };
  __property UnicodeString ResponseText = { index=(IS_OPTN|IS_NLBL), read=FResponseText, write=SetResponseText, stored = ResponseText_Specified };
};




// ************************************************************************ //
// XML       : MemberInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class MemberInfo2 : public MemberInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : VoucherInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class VoucherInfo2 : public VoucherInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyMemberResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyMemberResponse2 : public LoyaltyMemberResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : RequestInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class RequestInfo2 : public RequestInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyResponse2 : public LoyaltyResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : TransactionInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class TransactionInfo2 : public TransactionInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyCompanyResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyCompanyResponse2 : public LoyaltyCompanyResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : CompanyInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class CompanyInfo2 : public CompanyInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : DiscountInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class DiscountInfo2 : public DiscountInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : TierLevelInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class TierLevelInfo2 : public TierLevelInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyGiftCardResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyGiftCardResponse2 : public LoyaltyGiftCardResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : GiftCardInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class GiftCardInfo2 : public GiftCardInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyVoucherResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyVoucherResponse2 : public LoyaltyVoucherResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : VoucherTransactionInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class VoucherTransactionInfo2 : public VoucherTransactionInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : DiscountUsageInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class DiscountUsageInfo2 : public DiscountUsageInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : VoucherTransactionResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class VoucherTransactionResponse2 : public VoucherTransactionResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : ReleasedVoucherInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class ReleasedVoucherInfo2 : public ReleasedVoucherInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyOnlineOrderingResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyOnlineOrderingResponse2 : public LoyaltyOnlineOrderingResponse {
private:
__published:
};


typedef DynamicArray<MenuConsumableInfo*> ArrayOfMenuConsumableInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : SiteMenuInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class SiteMenuInfo : public TRemotable {
private:
  __int64         FCompanyId;
  bool            FCompanyId_Specified;
  ArrayOfMenuConsumableInfo FMenuConsumables;
  bool            FMenuConsumables_Specified;
  __int64         FSiteId;
  bool            FSiteId_Specified;
  void __fastcall SetCompanyId(int Index, __int64 _prop_val)
  {  FCompanyId = _prop_val; FCompanyId_Specified = true;  }
  bool __fastcall CompanyId_Specified(int Index)
  {  return FCompanyId_Specified;  } 
  void __fastcall SetMenuConsumables(int Index, ArrayOfMenuConsumableInfo _prop_val)
  {  FMenuConsumables = _prop_val; FMenuConsumables_Specified = true;  }
  bool __fastcall MenuConsumables_Specified(int Index)
  {  return FMenuConsumables_Specified;  } 
  void __fastcall SetSiteId(int Index, __int64 _prop_val)
  {  FSiteId = _prop_val; FSiteId_Specified = true;  }
  bool __fastcall SiteId_Specified(int Index)
  {  return FSiteId_Specified;  } 

public:
  __fastcall ~SiteMenuInfo();
__published:
  __property __int64     CompanyId = { index=(IS_OPTN), read=FCompanyId, write=SetCompanyId, stored = CompanyId_Specified };
  __property ArrayOfMenuConsumableInfo MenuConsumables = { index=(IS_OPTN|IS_NLBL), read=FMenuConsumables, write=SetMenuConsumables, stored = MenuConsumables_Specified };
  __property __int64        SiteId = { index=(IS_OPTN), read=FSiteId, write=SetSiteId, stored = SiteId_Specified };
};


typedef DynamicArray<CourseInfo*> ArrayOfCourseInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : MenuConsumableInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class MenuConsumableInfo : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  bool            FIsPalmable;
  bool            FIsPalmable_Specified;
  __int64         FMenuId;
  bool            FMenuId_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  ArrayOfCourseInfo FSiteCourses;
  bool            FSiteCourses_Specified;
  __int64         FSiteMenuCourseId;
  bool            FSiteMenuCourseId_Specified;
  MenuType        FType;
  bool            FType_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetIsPalmable(int Index, bool _prop_val)
  {  FIsPalmable = _prop_val; FIsPalmable_Specified = true;  }
  bool __fastcall IsPalmable_Specified(int Index)
  {  return FIsPalmable_Specified;  } 
  void __fastcall SetMenuId(int Index, __int64 _prop_val)
  {  FMenuId = _prop_val; FMenuId_Specified = true;  }
  bool __fastcall MenuId_Specified(int Index)
  {  return FMenuId_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetSiteCourses(int Index, ArrayOfCourseInfo _prop_val)
  {  FSiteCourses = _prop_val; FSiteCourses_Specified = true;  }
  bool __fastcall SiteCourses_Specified(int Index)
  {  return FSiteCourses_Specified;  } 
  void __fastcall SetSiteMenuCourseId(int Index, __int64 _prop_val)
  {  FSiteMenuCourseId = _prop_val; FSiteMenuCourseId_Specified = true;  }
  bool __fastcall SiteMenuCourseId_Specified(int Index)
  {  return FSiteMenuCourseId_Specified;  } 
  void __fastcall SetType(int Index, MenuType _prop_val)
  {  FType = _prop_val; FType_Specified = true;  }
  bool __fastcall Type_Specified(int Index)
  {  return FType_Specified;  } 

public:
  __fastcall ~MenuConsumableInfo();
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property bool       IsPalmable = { index=(IS_OPTN), read=FIsPalmable, write=SetIsPalmable, stored = IsPalmable_Specified };
  __property __int64        MenuId = { index=(IS_OPTN), read=FMenuId, write=SetMenuId, stored = MenuId_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property ArrayOfCourseInfo SiteCourses = { index=(IS_OPTN|IS_NLBL), read=FSiteCourses, write=SetSiteCourses, stored = SiteCourses_Specified };
  __property __int64    SiteMenuCourseId = { index=(IS_OPTN), read=FSiteMenuCourseId, write=SetSiteMenuCourseId, stored = SiteMenuCourseId_Specified };
  __property MenuType         Type = { index=(IS_OPTN), read=FType, write=SetType, stored = Type_Specified };
};


typedef DynamicArray<SiteItemInfo*> ArrayOfSiteItemInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : CourseInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class CourseInfo : public TRemotable {
private:
  __int64         FCourseId;
  bool            FCourseId_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  ArrayOfSiteItemInfo FItems;
  bool            FItems_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  UnicodeString   FServingCourseDescription;
  bool            FServingCourseDescription_Specified;
  UnicodeString   FServingCourseName;
  bool            FServingCourseName_Specified;
  __int64         FSiteMenuCourseId;
  bool            FSiteMenuCourseId_Specified;
  void __fastcall SetCourseId(int Index, __int64 _prop_val)
  {  FCourseId = _prop_val; FCourseId_Specified = true;  }
  bool __fastcall CourseId_Specified(int Index)
  {  return FCourseId_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetItems(int Index, ArrayOfSiteItemInfo _prop_val)
  {  FItems = _prop_val; FItems_Specified = true;  }
  bool __fastcall Items_Specified(int Index)
  {  return FItems_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetServingCourseDescription(int Index, UnicodeString _prop_val)
  {  FServingCourseDescription = _prop_val; FServingCourseDescription_Specified = true;  }
  bool __fastcall ServingCourseDescription_Specified(int Index)
  {  return FServingCourseDescription_Specified;  } 
  void __fastcall SetServingCourseName(int Index, UnicodeString _prop_val)
  {  FServingCourseName = _prop_val; FServingCourseName_Specified = true;  }
  bool __fastcall ServingCourseName_Specified(int Index)
  {  return FServingCourseName_Specified;  } 
  void __fastcall SetSiteMenuCourseId(int Index, __int64 _prop_val)
  {  FSiteMenuCourseId = _prop_val; FSiteMenuCourseId_Specified = true;  }
  bool __fastcall SiteMenuCourseId_Specified(int Index)
  {  return FSiteMenuCourseId_Specified;  } 

public:
  __fastcall ~CourseInfo();
__published:
  __property __int64      CourseId = { index=(IS_OPTN), read=FCourseId, write=SetCourseId, stored = CourseId_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property ArrayOfSiteItemInfo      Items = { index=(IS_OPTN|IS_NLBL), read=FItems, write=SetItems, stored = Items_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property UnicodeString ServingCourseDescription = { index=(IS_OPTN|IS_NLBL), read=FServingCourseDescription, write=SetServingCourseDescription, stored = ServingCourseDescription_Specified };
  __property UnicodeString ServingCourseName = { index=(IS_OPTN|IS_NLBL), read=FServingCourseName, write=SetServingCourseName, stored = ServingCourseName_Specified };
  __property __int64    SiteMenuCourseId = { index=(IS_OPTN), read=FSiteMenuCourseId, write=SetSiteMenuCourseId, stored = SiteMenuCourseId_Specified };
};


typedef DynamicArray<ItemSizeInfo*> ArrayOfItemSizeInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels"[GblCplx] */
typedef DynamicArray<SideGroupInfo*> ArrayOfSideGroupInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : SiteItemInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class SiteItemInfo : public TRemotable {
private:
  __int64         FCompanyId;
  bool            FCompanyId_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FImagePath;
  bool            FImagePath_Specified;
  ArrayOfItemSizeInfo FItemSizes;
  bool            FItemSizes_Specified;
  UnicodeString   FItemUniqueId;
  bool            FItemUniqueId_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  bool            FOnlyAsSide;
  bool            FOnlyAsSide_Specified;
  ArrayOfSideGroupInfo FSideGroups;
  bool            FSideGroups_Specified;
  __int64         FSiteItemId;
  bool            FSiteItemId_Specified;
  __int64         FSiteMenuCourseId;
  bool            FSiteMenuCourseId_Specified;
  void __fastcall SetCompanyId(int Index, __int64 _prop_val)
  {  FCompanyId = _prop_val; FCompanyId_Specified = true;  }
  bool __fastcall CompanyId_Specified(int Index)
  {  return FCompanyId_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetImagePath(int Index, UnicodeString _prop_val)
  {  FImagePath = _prop_val; FImagePath_Specified = true;  }
  bool __fastcall ImagePath_Specified(int Index)
  {  return FImagePath_Specified;  } 
  void __fastcall SetItemSizes(int Index, ArrayOfItemSizeInfo _prop_val)
  {  FItemSizes = _prop_val; FItemSizes_Specified = true;  }
  bool __fastcall ItemSizes_Specified(int Index)
  {  return FItemSizes_Specified;  } 
  void __fastcall SetItemUniqueId(int Index, UnicodeString _prop_val)
  {  FItemUniqueId = _prop_val; FItemUniqueId_Specified = true;  }
  bool __fastcall ItemUniqueId_Specified(int Index)
  {  return FItemUniqueId_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOnlyAsSide(int Index, bool _prop_val)
  {  FOnlyAsSide = _prop_val; FOnlyAsSide_Specified = true;  }
  bool __fastcall OnlyAsSide_Specified(int Index)
  {  return FOnlyAsSide_Specified;  } 
  void __fastcall SetSideGroups(int Index, ArrayOfSideGroupInfo _prop_val)
  {  FSideGroups = _prop_val; FSideGroups_Specified = true;  }
  bool __fastcall SideGroups_Specified(int Index)
  {  return FSideGroups_Specified;  } 
  void __fastcall SetSiteItemId(int Index, __int64 _prop_val)
  {  FSiteItemId = _prop_val; FSiteItemId_Specified = true;  }
  bool __fastcall SiteItemId_Specified(int Index)
  {  return FSiteItemId_Specified;  } 
  void __fastcall SetSiteMenuCourseId(int Index, __int64 _prop_val)
  {  FSiteMenuCourseId = _prop_val; FSiteMenuCourseId_Specified = true;  }
  bool __fastcall SiteMenuCourseId_Specified(int Index)
  {  return FSiteMenuCourseId_Specified;  } 

public:
  __fastcall ~SiteItemInfo();
__published:
  __property __int64     CompanyId = { index=(IS_OPTN), read=FCompanyId, write=SetCompanyId, stored = CompanyId_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString  ImagePath = { index=(IS_OPTN|IS_NLBL), read=FImagePath, write=SetImagePath, stored = ImagePath_Specified };
  __property ArrayOfItemSizeInfo  ItemSizes = { index=(IS_OPTN|IS_NLBL), read=FItemSizes, write=SetItemSizes, stored = ItemSizes_Specified };
  __property UnicodeString ItemUniqueId = { index=(IS_OPTN|IS_NLBL), read=FItemUniqueId, write=SetItemUniqueId, stored = ItemUniqueId_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property bool       OnlyAsSide = { index=(IS_OPTN), read=FOnlyAsSide, write=SetOnlyAsSide, stored = OnlyAsSide_Specified };
  __property ArrayOfSideGroupInfo SideGroups = { index=(IS_OPTN|IS_NLBL), read=FSideGroups, write=SetSideGroups, stored = SideGroups_Specified };
  __property __int64    SiteItemId = { index=(IS_OPTN), read=FSiteItemId, write=SetSiteItemId, stored = SiteItemId_Specified };
  __property __int64    SiteMenuCourseId = { index=(IS_OPTN), read=FSiteMenuCourseId, write=SetSiteMenuCourseId, stored = SiteMenuCourseId_Specified };
};


typedef DynamicArray<ItemSizeTaxProfileInfo*> ArrayOfItemSizeTaxProfileInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : ItemSizeInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSizeInfo : public TRemotable {
private:
  bool            FCanBePaidUsingPoints;
  bool            FCanBePaidUsingPoints_Specified;
  int             FDefaultPatronCount;
  bool            FDefaultPatronCount_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  bool            FIsFree;
  bool            FIsFree_Specified;
  bool            FIsWeighted;
  bool            FIsWeighted_Specified;
  __int64         FItemSizeId;
  bool            FItemSizeId_Specified;
  ArrayOfItemSizeTaxProfileInfo FItemSizeTaxProfiles;
  bool            FItemSizeTaxProfiles_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  UnicodeString   FOrderingUniqueId;
  bool            FOrderingUniqueId_Specified;
  double          FPointsPercentage;
  bool            FPointsPercentage_Specified;
  double          FPointsPrice;
  bool            FPointsPrice_Specified;
  double          FPrice;
  bool            FPrice_Specified;
  __int64         FSiteItemId;
  bool            FSiteItemId_Specified;
  UnicodeString   FThirdPartyId;
  bool            FThirdPartyId_Specified;
  void __fastcall SetCanBePaidUsingPoints(int Index, bool _prop_val)
  {  FCanBePaidUsingPoints = _prop_val; FCanBePaidUsingPoints_Specified = true;  }
  bool __fastcall CanBePaidUsingPoints_Specified(int Index)
  {  return FCanBePaidUsingPoints_Specified;  } 
  void __fastcall SetDefaultPatronCount(int Index, int _prop_val)
  {  FDefaultPatronCount = _prop_val; FDefaultPatronCount_Specified = true;  }
  bool __fastcall DefaultPatronCount_Specified(int Index)
  {  return FDefaultPatronCount_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetIsFree(int Index, bool _prop_val)
  {  FIsFree = _prop_val; FIsFree_Specified = true;  }
  bool __fastcall IsFree_Specified(int Index)
  {  return FIsFree_Specified;  } 
  void __fastcall SetIsWeighted(int Index, bool _prop_val)
  {  FIsWeighted = _prop_val; FIsWeighted_Specified = true;  }
  bool __fastcall IsWeighted_Specified(int Index)
  {  return FIsWeighted_Specified;  } 
  void __fastcall SetItemSizeId(int Index, __int64 _prop_val)
  {  FItemSizeId = _prop_val; FItemSizeId_Specified = true;  }
  bool __fastcall ItemSizeId_Specified(int Index)
  {  return FItemSizeId_Specified;  } 
  void __fastcall SetItemSizeTaxProfiles(int Index, ArrayOfItemSizeTaxProfileInfo _prop_val)
  {  FItemSizeTaxProfiles = _prop_val; FItemSizeTaxProfiles_Specified = true;  }
  bool __fastcall ItemSizeTaxProfiles_Specified(int Index)
  {  return FItemSizeTaxProfiles_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOrderingUniqueId(int Index, UnicodeString _prop_val)
  {  FOrderingUniqueId = _prop_val; FOrderingUniqueId_Specified = true;  }
  bool __fastcall OrderingUniqueId_Specified(int Index)
  {  return FOrderingUniqueId_Specified;  } 
  void __fastcall SetPointsPercentage(int Index, double _prop_val)
  {  FPointsPercentage = _prop_val; FPointsPercentage_Specified = true;  }
  bool __fastcall PointsPercentage_Specified(int Index)
  {  return FPointsPercentage_Specified;  } 
  void __fastcall SetPointsPrice(int Index, double _prop_val)
  {  FPointsPrice = _prop_val; FPointsPrice_Specified = true;  }
  bool __fastcall PointsPrice_Specified(int Index)
  {  return FPointsPrice_Specified;  } 
  void __fastcall SetPrice(int Index, double _prop_val)
  {  FPrice = _prop_val; FPrice_Specified = true;  }
  bool __fastcall Price_Specified(int Index)
  {  return FPrice_Specified;  } 
  void __fastcall SetSiteItemId(int Index, __int64 _prop_val)
  {  FSiteItemId = _prop_val; FSiteItemId_Specified = true;  }
  bool __fastcall SiteItemId_Specified(int Index)
  {  return FSiteItemId_Specified;  } 
  void __fastcall SetThirdPartyId(int Index, UnicodeString _prop_val)
  {  FThirdPartyId = _prop_val; FThirdPartyId_Specified = true;  }
  bool __fastcall ThirdPartyId_Specified(int Index)
  {  return FThirdPartyId_Specified;  } 

public:
  __fastcall ~ItemSizeInfo();
__published:
  __property bool       CanBePaidUsingPoints = { index=(IS_OPTN), read=FCanBePaidUsingPoints, write=SetCanBePaidUsingPoints, stored = CanBePaidUsingPoints_Specified };
  __property int        DefaultPatronCount = { index=(IS_OPTN), read=FDefaultPatronCount, write=SetDefaultPatronCount, stored = DefaultPatronCount_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property bool           IsFree = { index=(IS_OPTN), read=FIsFree, write=SetIsFree, stored = IsFree_Specified };
  __property bool       IsWeighted = { index=(IS_OPTN), read=FIsWeighted, write=SetIsWeighted, stored = IsWeighted_Specified };
  __property __int64    ItemSizeId = { index=(IS_OPTN), read=FItemSizeId, write=SetItemSizeId, stored = ItemSizeId_Specified };
  __property ArrayOfItemSizeTaxProfileInfo ItemSizeTaxProfiles = { index=(IS_OPTN|IS_NLBL), read=FItemSizeTaxProfiles, write=SetItemSizeTaxProfiles, stored = ItemSizeTaxProfiles_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property UnicodeString OrderingUniqueId = { index=(IS_OPTN|IS_NLBL), read=FOrderingUniqueId, write=SetOrderingUniqueId, stored = OrderingUniqueId_Specified };
  __property double     PointsPercentage = { index=(IS_OPTN), read=FPointsPercentage, write=SetPointsPercentage, stored = PointsPercentage_Specified };
  __property double     PointsPrice = { index=(IS_OPTN), read=FPointsPrice, write=SetPointsPrice, stored = PointsPrice_Specified };
  __property double          Price = { index=(IS_OPTN), read=FPrice, write=SetPrice, stored = Price_Specified };
  __property __int64    SiteItemId = { index=(IS_OPTN), read=FSiteItemId, write=SetSiteItemId, stored = SiteItemId_Specified };
  __property UnicodeString ThirdPartyId = { index=(IS_OPTN|IS_NLBL), read=FThirdPartyId, write=SetThirdPartyId, stored = ThirdPartyId_Specified };
};




// ************************************************************************ //
// XML       : ItemSizeTaxProfileInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSizeTaxProfileInfo : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  __int64         FItemSizeTaxProfileId;
  bool            FItemSizeTaxProfileId_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  int             FPriority;
  bool            FPriority_Specified;
  double          FRate;
  bool            FRate_Specified;
  __int64         FTaxProfileId;
  bool            FTaxProfileId_Specified;
  TaxProfileType  FType;
  bool            FType_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetItemSizeTaxProfileId(int Index, __int64 _prop_val)
  {  FItemSizeTaxProfileId = _prop_val; FItemSizeTaxProfileId_Specified = true;  }
  bool __fastcall ItemSizeTaxProfileId_Specified(int Index)
  {  return FItemSizeTaxProfileId_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetPriority(int Index, int _prop_val)
  {  FPriority = _prop_val; FPriority_Specified = true;  }
  bool __fastcall Priority_Specified(int Index)
  {  return FPriority_Specified;  } 
  void __fastcall SetRate(int Index, double _prop_val)
  {  FRate = _prop_val; FRate_Specified = true;  }
  bool __fastcall Rate_Specified(int Index)
  {  return FRate_Specified;  } 
  void __fastcall SetTaxProfileId(int Index, __int64 _prop_val)
  {  FTaxProfileId = _prop_val; FTaxProfileId_Specified = true;  }
  bool __fastcall TaxProfileId_Specified(int Index)
  {  return FTaxProfileId_Specified;  } 
  void __fastcall SetType(int Index, TaxProfileType _prop_val)
  {  FType = _prop_val; FType_Specified = true;  }
  bool __fastcall Type_Specified(int Index)
  {  return FType_Specified;  } 
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property __int64    ItemSizeTaxProfileId = { index=(IS_OPTN), read=FItemSizeTaxProfileId, write=SetItemSizeTaxProfileId, stored = ItemSizeTaxProfileId_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property int          Priority = { index=(IS_OPTN), read=FPriority, write=SetPriority, stored = Priority_Specified };
  __property double           Rate = { index=(IS_OPTN), read=FRate, write=SetRate, stored = Rate_Specified };
  __property __int64    TaxProfileId = { index=(IS_OPTN), read=FTaxProfileId, write=SetTaxProfileId, stored = TaxProfileId_Specified };
  __property TaxProfileType       Type = { index=(IS_OPTN), read=FType, write=SetType, stored = Type_Specified };
};


typedef DynamicArray<ItemSideInfo*> ArrayOfItemSideInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : SideGroupInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class SideGroupInfo : public TRemotable {
private:
  bool            FAllowSkip;
  bool            FAllowSkip_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  ArrayOfItemSideInfo FItemSides;
  bool            FItemSides_Specified;
  int             FMaxSelect;
  bool            FMaxSelect_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  __int64         FSideGroupId;
  bool            FSideGroupId_Specified;
  __int64         FSiteItemId;
  bool            FSiteItemId_Specified;
  void __fastcall SetAllowSkip(int Index, bool _prop_val)
  {  FAllowSkip = _prop_val; FAllowSkip_Specified = true;  }
  bool __fastcall AllowSkip_Specified(int Index)
  {  return FAllowSkip_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetItemSides(int Index, ArrayOfItemSideInfo _prop_val)
  {  FItemSides = _prop_val; FItemSides_Specified = true;  }
  bool __fastcall ItemSides_Specified(int Index)
  {  return FItemSides_Specified;  } 
  void __fastcall SetMaxSelect(int Index, int _prop_val)
  {  FMaxSelect = _prop_val; FMaxSelect_Specified = true;  }
  bool __fastcall MaxSelect_Specified(int Index)
  {  return FMaxSelect_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetSideGroupId(int Index, __int64 _prop_val)
  {  FSideGroupId = _prop_val; FSideGroupId_Specified = true;  }
  bool __fastcall SideGroupId_Specified(int Index)
  {  return FSideGroupId_Specified;  } 
  void __fastcall SetSiteItemId(int Index, __int64 _prop_val)
  {  FSiteItemId = _prop_val; FSiteItemId_Specified = true;  }
  bool __fastcall SiteItemId_Specified(int Index)
  {  return FSiteItemId_Specified;  } 

public:
  __fastcall ~SideGroupInfo();
__published:
  __property bool        AllowSkip = { index=(IS_OPTN), read=FAllowSkip, write=SetAllowSkip, stored = AllowSkip_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property ArrayOfItemSideInfo  ItemSides = { index=(IS_OPTN|IS_NLBL), read=FItemSides, write=SetItemSides, stored = ItemSides_Specified };
  __property int         MaxSelect = { index=(IS_OPTN), read=FMaxSelect, write=SetMaxSelect, stored = MaxSelect_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property __int64    SideGroupId = { index=(IS_OPTN), read=FSideGroupId, write=SetSideGroupId, stored = SideGroupId_Specified };
  __property __int64    SiteItemId = { index=(IS_OPTN), read=FSiteItemId, write=SetSiteItemId, stored = SiteItemId_Specified };
};




// ************************************************************************ //
// XML       : ItemSideInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSideInfo : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  ArrayOfItemSizeInfo FItemSizes;
  bool            FItemSizes_Specified;
  UnicodeString   FItemUniqueId;
  bool            FItemUniqueId_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  bool            FOnlyAsSide;
  bool            FOnlyAsSide_Specified;
  __int64         FSiteItemId;
  bool            FSiteItemId_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetItemSizes(int Index, ArrayOfItemSizeInfo _prop_val)
  {  FItemSizes = _prop_val; FItemSizes_Specified = true;  }
  bool __fastcall ItemSizes_Specified(int Index)
  {  return FItemSizes_Specified;  } 
  void __fastcall SetItemUniqueId(int Index, UnicodeString _prop_val)
  {  FItemUniqueId = _prop_val; FItemUniqueId_Specified = true;  }
  bool __fastcall ItemUniqueId_Specified(int Index)
  {  return FItemUniqueId_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOnlyAsSide(int Index, bool _prop_val)
  {  FOnlyAsSide = _prop_val; FOnlyAsSide_Specified = true;  }
  bool __fastcall OnlyAsSide_Specified(int Index)
  {  return FOnlyAsSide_Specified;  } 
  void __fastcall SetSiteItemId(int Index, __int64 _prop_val)
  {  FSiteItemId = _prop_val; FSiteItemId_Specified = true;  }
  bool __fastcall SiteItemId_Specified(int Index)
  {  return FSiteItemId_Specified;  } 

public:
  __fastcall ~ItemSideInfo();
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property ArrayOfItemSizeInfo  ItemSizes = { index=(IS_OPTN|IS_NLBL), read=FItemSizes, write=SetItemSizes, stored = ItemSizes_Specified };
  __property UnicodeString ItemUniqueId = { index=(IS_OPTN|IS_NLBL), read=FItemUniqueId, write=SetItemUniqueId, stored = ItemUniqueId_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property bool       OnlyAsSide = { index=(IS_OPTN), read=FOnlyAsSide, write=SetOnlyAsSide, stored = OnlyAsSide_Specified };
  __property __int64    SiteItemId = { index=(IS_OPTN), read=FSiteItemId, write=SetSiteItemId, stored = SiteItemId_Specified };
};


typedef DynamicArray<TaxSettingsInfo*> ArrayOfTaxSettingsInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : SiteTaxSettingsinfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class SiteTaxSettingsinfo : public TRemotable {
private:
  __int64         FSiteId;
  bool            FSiteId_Specified;
  ArrayOfTaxSettingsInfo FSiteTaxSettings;
  bool            FSiteTaxSettings_Specified;
  void __fastcall SetSiteId(int Index, __int64 _prop_val)
  {  FSiteId = _prop_val; FSiteId_Specified = true;  }
  bool __fastcall SiteId_Specified(int Index)
  {  return FSiteId_Specified;  } 
  void __fastcall SetSiteTaxSettings(int Index, ArrayOfTaxSettingsInfo _prop_val)
  {  FSiteTaxSettings = _prop_val; FSiteTaxSettings_Specified = true;  }
  bool __fastcall SiteTaxSettings_Specified(int Index)
  {  return FSiteTaxSettings_Specified;  } 

public:
  __fastcall ~SiteTaxSettingsinfo();
__published:
  __property __int64        SiteId = { index=(IS_OPTN), read=FSiteId, write=SetSiteId, stored = SiteId_Specified };
  __property ArrayOfTaxSettingsInfo SiteTaxSettings = { index=(IS_OPTN|IS_NLBL), read=FSiteTaxSettings, write=SetSiteTaxSettings, stored = SiteTaxSettings_Specified };
};




// ************************************************************************ //
// XML       : TaxSettingsInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class TaxSettingsInfo : public TRemotable {
private:
  SiteSettingType FSettingType;
  bool            FSettingType_Specified;
  UnicodeString   FValue;
  bool            FValue_Specified;
  void __fastcall SetSettingType(int Index, SiteSettingType _prop_val)
  {  FSettingType = _prop_val; FSettingType_Specified = true;  }
  bool __fastcall SettingType_Specified(int Index)
  {  return FSettingType_Specified;  } 
  void __fastcall SetValue(int Index, UnicodeString _prop_val)
  {  FValue = _prop_val; FValue_Specified = true;  }
  bool __fastcall Value_Specified(int Index)
  {  return FValue_Specified;  } 
__published:
  __property SiteSettingType SettingType = { index=(IS_OPTN), read=FSettingType, write=SetSettingType, stored = SettingType_Specified };
  __property UnicodeString      Value = { index=(IS_OPTN|IS_NLBL), read=FValue, write=SetValue, stored = Value_Specified };
};




// ************************************************************************ //
// XML       : SiteMenuInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class SiteMenuInfo2 : public SiteMenuInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : MenuConsumableInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class MenuConsumableInfo2 : public MenuConsumableInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : CourseInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class CourseInfo2 : public CourseInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : SiteItemInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class SiteItemInfo2 : public SiteItemInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : ItemSizeInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSizeInfo2 : public ItemSizeInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : ItemSizeTaxProfileInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSizeTaxProfileInfo2 : public ItemSizeTaxProfileInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : SideGroupInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class SideGroupInfo2 : public SideGroupInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : ItemSideInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSideInfo2 : public ItemSideInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : SiteTaxSettingsinfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class SiteTaxSettingsinfo2 : public SiteTaxSettingsinfo {
private:
__published:
};




// ************************************************************************ //
// XML       : TaxSettingsInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.MenuModels
// ************************************************************************ //
class TaxSettingsInfo2 : public TaxSettingsInfo {
private:
__published:
};


typedef DynamicArray<OrderItemModel*> ArrayOfOrderItemModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels"[GblCplx] */


// ************************************************************************ //
// XML       : SiteOrderModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class SiteOrderModel : public TRemotable {
private:
  __int64         FCompanyId;
  bool            FCompanyId_Specified;
  UnicodeString   FContainerName;
  bool            FContainerName_Specified;
  int             FContainerNumber;
  bool            FContainerNumber_Specified;
  OrderContainerType FContainerType;
  bool            FContainerType_Specified;
  bool            FIsConfirmed;
  bool            FIsConfirmed_Specified;
  UnicodeString   FLocation;
  bool            FLocation_Specified;
  UnicodeString   FOrderGuid;
  bool            FOrderGuid_Specified;
  __int64         FOrderId;
  bool            FOrderId_Specified;
  ArrayOfOrderItemModel FOrderItems;
  bool            FOrderItems_Specified;
  OrderType       FOrderType;
  bool            FOrderType_Specified;
  __int64         FSiteId;
  bool            FSiteId_Specified;
  UnicodeString   FTerminalName;
  bool            FTerminalName_Specified;
  double          FTotalAmount;
  bool            FTotalAmount_Specified;
  TXSDateTime*    FTransactionDate;
  bool            FTransactionDate_Specified;
  SiteSettingType FTransactionType;
  bool            FTransactionType_Specified;
  __int64         FUserReferenceId;
  bool            FUserReferenceId_Specified;
  UserType        FUserType;
  bool            FUserType_Specified;
  void __fastcall SetCompanyId(int Index, __int64 _prop_val)
  {  FCompanyId = _prop_val; FCompanyId_Specified = true;  }
  bool __fastcall CompanyId_Specified(int Index)
  {  return FCompanyId_Specified;  } 
  void __fastcall SetContainerName(int Index, UnicodeString _prop_val)
  {  FContainerName = _prop_val; FContainerName_Specified = true;  }
  bool __fastcall ContainerName_Specified(int Index)
  {  return FContainerName_Specified;  } 
  void __fastcall SetContainerNumber(int Index, int _prop_val)
  {  FContainerNumber = _prop_val; FContainerNumber_Specified = true;  }
  bool __fastcall ContainerNumber_Specified(int Index)
  {  return FContainerNumber_Specified;  } 
  void __fastcall SetContainerType(int Index, OrderContainerType _prop_val)
  {  FContainerType = _prop_val; FContainerType_Specified = true;  }
  bool __fastcall ContainerType_Specified(int Index)
  {  return FContainerType_Specified;  } 
  void __fastcall SetIsConfirmed(int Index, bool _prop_val)
  {  FIsConfirmed = _prop_val; FIsConfirmed_Specified = true;  }
  bool __fastcall IsConfirmed_Specified(int Index)
  {  return FIsConfirmed_Specified;  } 
  void __fastcall SetLocation(int Index, UnicodeString _prop_val)
  {  FLocation = _prop_val; FLocation_Specified = true;  }
  bool __fastcall Location_Specified(int Index)
  {  return FLocation_Specified;  } 
  void __fastcall SetOrderGuid(int Index, UnicodeString _prop_val)
  {  FOrderGuid = _prop_val; FOrderGuid_Specified = true;  }
  bool __fastcall OrderGuid_Specified(int Index)
  {  return FOrderGuid_Specified;  } 
  void __fastcall SetOrderId(int Index, __int64 _prop_val)
  {  FOrderId = _prop_val; FOrderId_Specified = true;  }
  bool __fastcall OrderId_Specified(int Index)
  {  return FOrderId_Specified;  } 
  void __fastcall SetOrderItems(int Index, ArrayOfOrderItemModel _prop_val)
  {  FOrderItems = _prop_val; FOrderItems_Specified = true;  }
  bool __fastcall OrderItems_Specified(int Index)
  {  return FOrderItems_Specified;  } 
  void __fastcall SetOrderType(int Index, OrderType _prop_val)
  {  FOrderType = _prop_val; FOrderType_Specified = true;  }
  bool __fastcall OrderType_Specified(int Index)
  {  return FOrderType_Specified;  } 
  void __fastcall SetSiteId(int Index, __int64 _prop_val)
  {  FSiteId = _prop_val; FSiteId_Specified = true;  }
  bool __fastcall SiteId_Specified(int Index)
  {  return FSiteId_Specified;  } 
  void __fastcall SetTerminalName(int Index, UnicodeString _prop_val)
  {  FTerminalName = _prop_val; FTerminalName_Specified = true;  }
  bool __fastcall TerminalName_Specified(int Index)
  {  return FTerminalName_Specified;  } 
  void __fastcall SetTotalAmount(int Index, double _prop_val)
  {  FTotalAmount = _prop_val; FTotalAmount_Specified = true;  }
  bool __fastcall TotalAmount_Specified(int Index)
  {  return FTotalAmount_Specified;  } 
  void __fastcall SetTransactionDate(int Index, TXSDateTime* _prop_val)
  {  FTransactionDate = _prop_val; FTransactionDate_Specified = true;  }
  bool __fastcall TransactionDate_Specified(int Index)
  {  return FTransactionDate_Specified;  } 
  void __fastcall SetTransactionType(int Index, SiteSettingType _prop_val)
  {  FTransactionType = _prop_val; FTransactionType_Specified = true;  }
  bool __fastcall TransactionType_Specified(int Index)
  {  return FTransactionType_Specified;  } 
  void __fastcall SetUserReferenceId(int Index, __int64 _prop_val)
  {  FUserReferenceId = _prop_val; FUserReferenceId_Specified = true;  }
  bool __fastcall UserReferenceId_Specified(int Index)
  {  return FUserReferenceId_Specified;  } 
  void __fastcall SetUserType(int Index, UserType _prop_val)
  {  FUserType = _prop_val; FUserType_Specified = true;  }
  bool __fastcall UserType_Specified(int Index)
  {  return FUserType_Specified;  } 

public:
  __fastcall ~SiteOrderModel();
__published:
  __property __int64     CompanyId = { index=(IS_OPTN), read=FCompanyId, write=SetCompanyId, stored = CompanyId_Specified };
  __property UnicodeString ContainerName = { index=(IS_OPTN|IS_NLBL), read=FContainerName, write=SetContainerName, stored = ContainerName_Specified };
  __property int        ContainerNumber = { index=(IS_OPTN), read=FContainerNumber, write=SetContainerNumber, stored = ContainerNumber_Specified };
  __property OrderContainerType ContainerType = { index=(IS_OPTN), read=FContainerType, write=SetContainerType, stored = ContainerType_Specified };
  __property bool       IsConfirmed = { index=(IS_OPTN), read=FIsConfirmed, write=SetIsConfirmed, stored = IsConfirmed_Specified };
  __property UnicodeString   Location = { index=(IS_OPTN|IS_NLBL), read=FLocation, write=SetLocation, stored = Location_Specified };
  __property UnicodeString  OrderGuid = { index=(IS_OPTN|IS_NLBL), read=FOrderGuid, write=SetOrderGuid, stored = OrderGuid_Specified };
  __property __int64       OrderId = { index=(IS_OPTN), read=FOrderId, write=SetOrderId, stored = OrderId_Specified };
  __property ArrayOfOrderItemModel OrderItems = { index=(IS_OPTN|IS_NLBL), read=FOrderItems, write=SetOrderItems, stored = OrderItems_Specified };
  __property OrderType   OrderType = { index=(IS_OPTN), read=FOrderType, write=SetOrderType, stored = OrderType_Specified };
  __property __int64        SiteId = { index=(IS_OPTN), read=FSiteId, write=SetSiteId, stored = SiteId_Specified };
  __property UnicodeString TerminalName = { index=(IS_OPTN|IS_NLBL), read=FTerminalName, write=SetTerminalName, stored = TerminalName_Specified };
  __property double     TotalAmount = { index=(IS_OPTN), read=FTotalAmount, write=SetTotalAmount, stored = TotalAmount_Specified };
  __property TXSDateTime* TransactionDate = { index=(IS_OPTN), read=FTransactionDate, write=SetTransactionDate, stored = TransactionDate_Specified };
  __property SiteSettingType TransactionType = { index=(IS_OPTN), read=FTransactionType, write=SetTransactionType, stored = TransactionType_Specified };
  __property __int64    UserReferenceId = { index=(IS_OPTN), read=FUserReferenceId, write=SetUserReferenceId, stored = UserReferenceId_Specified };
  __property UserType     UserType = { index=(IS_OPTN), read=FUserType, write=SetUserType, stored = UserType_Specified };
};


typedef DynamicArray<OrderItemSizeModel*> ArrayOfOrderItemSizeModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels"[GblCplx] */


// ************************************************************************ //
// XML       : OrderItemModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemModel : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  __int64         FOrderId;
  bool            FOrderId_Specified;
  __int64         FOrderItemId;
  bool            FOrderItemId_Specified;
  ArrayOfOrderItemSizeModel FOrderItemSizes;
  bool            FOrderItemSizes_Specified;
  double          FPrice;
  bool            FPrice_Specified;
  __int64         FSiteItemId;
  bool            FSiteItemId_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOrderId(int Index, __int64 _prop_val)
  {  FOrderId = _prop_val; FOrderId_Specified = true;  }
  bool __fastcall OrderId_Specified(int Index)
  {  return FOrderId_Specified;  } 
  void __fastcall SetOrderItemId(int Index, __int64 _prop_val)
  {  FOrderItemId = _prop_val; FOrderItemId_Specified = true;  }
  bool __fastcall OrderItemId_Specified(int Index)
  {  return FOrderItemId_Specified;  } 
  void __fastcall SetOrderItemSizes(int Index, ArrayOfOrderItemSizeModel _prop_val)
  {  FOrderItemSizes = _prop_val; FOrderItemSizes_Specified = true;  }
  bool __fastcall OrderItemSizes_Specified(int Index)
  {  return FOrderItemSizes_Specified;  } 
  void __fastcall SetPrice(int Index, double _prop_val)
  {  FPrice = _prop_val; FPrice_Specified = true;  }
  bool __fastcall Price_Specified(int Index)
  {  return FPrice_Specified;  } 
  void __fastcall SetSiteItemId(int Index, __int64 _prop_val)
  {  FSiteItemId = _prop_val; FSiteItemId_Specified = true;  }
  bool __fastcall SiteItemId_Specified(int Index)
  {  return FSiteItemId_Specified;  } 

public:
  __fastcall ~OrderItemModel();
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property __int64       OrderId = { index=(IS_OPTN), read=FOrderId, write=SetOrderId, stored = OrderId_Specified };
  __property __int64    OrderItemId = { index=(IS_OPTN), read=FOrderItemId, write=SetOrderItemId, stored = OrderItemId_Specified };
  __property ArrayOfOrderItemSizeModel OrderItemSizes = { index=(IS_OPTN|IS_NLBL), read=FOrderItemSizes, write=SetOrderItemSizes, stored = OrderItemSizes_Specified };
  __property double          Price = { index=(IS_OPTN), read=FPrice, write=SetPrice, stored = Price_Specified };
  __property __int64    SiteItemId = { index=(IS_OPTN), read=FSiteItemId, write=SetSiteItemId, stored = SiteItemId_Specified };
};


typedef DynamicArray<OrderItemSizeDiscountModel*> ArrayOfOrderItemSizeDiscountModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels"[GblCplx] */
typedef DynamicArray<OrderItemSizeTaxProfileModel*> ArrayOfOrderItemSizeTaxProfileModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels"[GblCplx] */


// ************************************************************************ //
// XML       : OrderItemSizeModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeModel : public TRemotable {
private:
  double          FBasePrice;
  bool            FBasePrice_Specified;
  __int64         FItemSizeId;
  bool            FItemSizeId_Specified;
  double          FMenuPrice;
  bool            FMenuPrice_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  __int64         FOrderItemId;
  bool            FOrderItemId_Specified;
  ArrayOfOrderItemSizeDiscountModel FOrderItemSizeDiscounts;
  bool            FOrderItemSizeDiscounts_Specified;
  __int64         FOrderItemSizeId;
  bool            FOrderItemSizeId_Specified;
  ArrayOfOrderItemSizeTaxProfileModel FOrderItemSizeTaxProfiles;
  bool            FOrderItemSizeTaxProfiles_Specified;
  double          FPrice;
  bool            FPrice_Specified;
  double          FPriceInclusive;
  bool            FPriceInclusive_Specified;
  double          FQuantity;
  bool            FQuantity_Specified;
  __int64         FReferenceOrderItemSizeId;
  bool            FReferenceOrderItemSizeId_Specified;
  void __fastcall SetBasePrice(int Index, double _prop_val)
  {  FBasePrice = _prop_val; FBasePrice_Specified = true;  }
  bool __fastcall BasePrice_Specified(int Index)
  {  return FBasePrice_Specified;  } 
  void __fastcall SetItemSizeId(int Index, __int64 _prop_val)
  {  FItemSizeId = _prop_val; FItemSizeId_Specified = true;  }
  bool __fastcall ItemSizeId_Specified(int Index)
  {  return FItemSizeId_Specified;  } 
  void __fastcall SetMenuPrice(int Index, double _prop_val)
  {  FMenuPrice = _prop_val; FMenuPrice_Specified = true;  }
  bool __fastcall MenuPrice_Specified(int Index)
  {  return FMenuPrice_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOrderItemId(int Index, __int64 _prop_val)
  {  FOrderItemId = _prop_val; FOrderItemId_Specified = true;  }
  bool __fastcall OrderItemId_Specified(int Index)
  {  return FOrderItemId_Specified;  } 
  void __fastcall SetOrderItemSizeDiscounts(int Index, ArrayOfOrderItemSizeDiscountModel _prop_val)
  {  FOrderItemSizeDiscounts = _prop_val; FOrderItemSizeDiscounts_Specified = true;  }
  bool __fastcall OrderItemSizeDiscounts_Specified(int Index)
  {  return FOrderItemSizeDiscounts_Specified;  } 
  void __fastcall SetOrderItemSizeId(int Index, __int64 _prop_val)
  {  FOrderItemSizeId = _prop_val; FOrderItemSizeId_Specified = true;  }
  bool __fastcall OrderItemSizeId_Specified(int Index)
  {  return FOrderItemSizeId_Specified;  } 
  void __fastcall SetOrderItemSizeTaxProfiles(int Index, ArrayOfOrderItemSizeTaxProfileModel _prop_val)
  {  FOrderItemSizeTaxProfiles = _prop_val; FOrderItemSizeTaxProfiles_Specified = true;  }
  bool __fastcall OrderItemSizeTaxProfiles_Specified(int Index)
  {  return FOrderItemSizeTaxProfiles_Specified;  } 
  void __fastcall SetPrice(int Index, double _prop_val)
  {  FPrice = _prop_val; FPrice_Specified = true;  }
  bool __fastcall Price_Specified(int Index)
  {  return FPrice_Specified;  } 
  void __fastcall SetPriceInclusive(int Index, double _prop_val)
  {  FPriceInclusive = _prop_val; FPriceInclusive_Specified = true;  }
  bool __fastcall PriceInclusive_Specified(int Index)
  {  return FPriceInclusive_Specified;  } 
  void __fastcall SetQuantity(int Index, double _prop_val)
  {  FQuantity = _prop_val; FQuantity_Specified = true;  }
  bool __fastcall Quantity_Specified(int Index)
  {  return FQuantity_Specified;  } 
  void __fastcall SetReferenceOrderItemSizeId(int Index, __int64 _prop_val)
  {  FReferenceOrderItemSizeId = _prop_val; FReferenceOrderItemSizeId_Specified = true;  }
  bool __fastcall ReferenceOrderItemSizeId_Specified(int Index)
  {  return FReferenceOrderItemSizeId_Specified;  } 

public:
  __fastcall ~OrderItemSizeModel();
__published:
  __property double      BasePrice = { index=(IS_OPTN), read=FBasePrice, write=SetBasePrice, stored = BasePrice_Specified };
  __property __int64    ItemSizeId = { index=(IS_OPTN), read=FItemSizeId, write=SetItemSizeId, stored = ItemSizeId_Specified };
  __property double      MenuPrice = { index=(IS_OPTN), read=FMenuPrice, write=SetMenuPrice, stored = MenuPrice_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property __int64    OrderItemId = { index=(IS_OPTN), read=FOrderItemId, write=SetOrderItemId, stored = OrderItemId_Specified };
  __property ArrayOfOrderItemSizeDiscountModel OrderItemSizeDiscounts = { index=(IS_OPTN|IS_NLBL), read=FOrderItemSizeDiscounts, write=SetOrderItemSizeDiscounts, stored = OrderItemSizeDiscounts_Specified };
  __property __int64    OrderItemSizeId = { index=(IS_OPTN), read=FOrderItemSizeId, write=SetOrderItemSizeId, stored = OrderItemSizeId_Specified };
  __property ArrayOfOrderItemSizeTaxProfileModel OrderItemSizeTaxProfiles = { index=(IS_OPTN|IS_NLBL), read=FOrderItemSizeTaxProfiles, write=SetOrderItemSizeTaxProfiles, stored = OrderItemSizeTaxProfiles_Specified };
  __property double          Price = { index=(IS_OPTN), read=FPrice, write=SetPrice, stored = Price_Specified };
  __property double     PriceInclusive = { index=(IS_OPTN), read=FPriceInclusive, write=SetPriceInclusive, stored = PriceInclusive_Specified };
  __property double       Quantity = { index=(IS_OPTN), read=FQuantity, write=SetQuantity, stored = Quantity_Specified };
  __property __int64    ReferenceOrderItemSizeId = { index=(IS_OPTN), read=FReferenceOrderItemSizeId, write=SetReferenceOrderItemSizeId, stored = ReferenceOrderItemSizeId_Specified };
};




// ************************************************************************ //
// XML       : OrderItemSizeDiscountModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeDiscountModel : public TRemotable {
private:
  UnicodeString   FCode;
  bool            FCode_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  __int64         FOrderItemSizeDiscountId;
  bool            FOrderItemSizeDiscountId_Specified;
  __int64         FOrderItemSizeId;
  bool            FOrderItemSizeId_Specified;
  double          FValue;
  bool            FValue_Specified;
  void __fastcall SetCode(int Index, UnicodeString _prop_val)
  {  FCode = _prop_val; FCode_Specified = true;  }
  bool __fastcall Code_Specified(int Index)
  {  return FCode_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOrderItemSizeDiscountId(int Index, __int64 _prop_val)
  {  FOrderItemSizeDiscountId = _prop_val; FOrderItemSizeDiscountId_Specified = true;  }
  bool __fastcall OrderItemSizeDiscountId_Specified(int Index)
  {  return FOrderItemSizeDiscountId_Specified;  } 
  void __fastcall SetOrderItemSizeId(int Index, __int64 _prop_val)
  {  FOrderItemSizeId = _prop_val; FOrderItemSizeId_Specified = true;  }
  bool __fastcall OrderItemSizeId_Specified(int Index)
  {  return FOrderItemSizeId_Specified;  } 
  void __fastcall SetValue(int Index, double _prop_val)
  {  FValue = _prop_val; FValue_Specified = true;  }
  bool __fastcall Value_Specified(int Index)
  {  return FValue_Specified;  } 
__published:
  __property UnicodeString       Code = { index=(IS_OPTN|IS_NLBL), read=FCode, write=SetCode, stored = Code_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property __int64    OrderItemSizeDiscountId = { index=(IS_OPTN), read=FOrderItemSizeDiscountId, write=SetOrderItemSizeDiscountId, stored = OrderItemSizeDiscountId_Specified };
  __property __int64    OrderItemSizeId = { index=(IS_OPTN), read=FOrderItemSizeId, write=SetOrderItemSizeId, stored = OrderItemSizeId_Specified };
  __property double          Value = { index=(IS_OPTN), read=FValue, write=SetValue, stored = Value_Specified };
};




// ************************************************************************ //
// XML       : OrderItemSizeTaxProfileModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeTaxProfileModel : public TRemotable {
private:
  __int64         FCompanyId;
  bool            FCompanyId_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  __int64         FItemSizeTaxProfileId;
  bool            FItemSizeTaxProfileId_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  __int64         FOrderItemSizeId;
  bool            FOrderItemSizeId_Specified;
  __int64         FOrderItemSizeTaxProfileId;
  bool            FOrderItemSizeTaxProfileId_Specified;
  double          FPercentage;
  bool            FPercentage_Specified;
  int             FPriority;
  bool            FPriority_Specified;
  double          FRate;
  bool            FRate_Specified;
  TaxProfileType  FTaxProfileType;
  bool            FTaxProfileType_Specified;
  double          FValue;
  bool            FValue_Specified;
  void __fastcall SetCompanyId(int Index, __int64 _prop_val)
  {  FCompanyId = _prop_val; FCompanyId_Specified = true;  }
  bool __fastcall CompanyId_Specified(int Index)
  {  return FCompanyId_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetItemSizeTaxProfileId(int Index, __int64 _prop_val)
  {  FItemSizeTaxProfileId = _prop_val; FItemSizeTaxProfileId_Specified = true;  }
  bool __fastcall ItemSizeTaxProfileId_Specified(int Index)
  {  return FItemSizeTaxProfileId_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOrderItemSizeId(int Index, __int64 _prop_val)
  {  FOrderItemSizeId = _prop_val; FOrderItemSizeId_Specified = true;  }
  bool __fastcall OrderItemSizeId_Specified(int Index)
  {  return FOrderItemSizeId_Specified;  } 
  void __fastcall SetOrderItemSizeTaxProfileId(int Index, __int64 _prop_val)
  {  FOrderItemSizeTaxProfileId = _prop_val; FOrderItemSizeTaxProfileId_Specified = true;  }
  bool __fastcall OrderItemSizeTaxProfileId_Specified(int Index)
  {  return FOrderItemSizeTaxProfileId_Specified;  } 
  void __fastcall SetPercentage(int Index, double _prop_val)
  {  FPercentage = _prop_val; FPercentage_Specified = true;  }
  bool __fastcall Percentage_Specified(int Index)
  {  return FPercentage_Specified;  } 
  void __fastcall SetPriority(int Index, int _prop_val)
  {  FPriority = _prop_val; FPriority_Specified = true;  }
  bool __fastcall Priority_Specified(int Index)
  {  return FPriority_Specified;  } 
  void __fastcall SetRate(int Index, double _prop_val)
  {  FRate = _prop_val; FRate_Specified = true;  }
  bool __fastcall Rate_Specified(int Index)
  {  return FRate_Specified;  } 
  void __fastcall SetTaxProfileType(int Index, TaxProfileType _prop_val)
  {  FTaxProfileType = _prop_val; FTaxProfileType_Specified = true;  }
  bool __fastcall TaxProfileType_Specified(int Index)
  {  return FTaxProfileType_Specified;  } 
  void __fastcall SetValue(int Index, double _prop_val)
  {  FValue = _prop_val; FValue_Specified = true;  }
  bool __fastcall Value_Specified(int Index)
  {  return FValue_Specified;  } 
__published:
  __property __int64     CompanyId = { index=(IS_OPTN), read=FCompanyId, write=SetCompanyId, stored = CompanyId_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property __int64    ItemSizeTaxProfileId = { index=(IS_OPTN), read=FItemSizeTaxProfileId, write=SetItemSizeTaxProfileId, stored = ItemSizeTaxProfileId_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property __int64    OrderItemSizeId = { index=(IS_OPTN), read=FOrderItemSizeId, write=SetOrderItemSizeId, stored = OrderItemSizeId_Specified };
  __property __int64    OrderItemSizeTaxProfileId = { index=(IS_OPTN), read=FOrderItemSizeTaxProfileId, write=SetOrderItemSizeTaxProfileId, stored = OrderItemSizeTaxProfileId_Specified };
  __property double     Percentage = { index=(IS_OPTN), read=FPercentage, write=SetPercentage, stored = Percentage_Specified };
  __property int          Priority = { index=(IS_OPTN), read=FPriority, write=SetPriority, stored = Priority_Specified };
  __property double           Rate = { index=(IS_OPTN), read=FRate, write=SetRate, stored = Rate_Specified };
  __property TaxProfileType TaxProfileType = { index=(IS_OPTN), read=FTaxProfileType, write=SetTaxProfileType, stored = TaxProfileType_Specified };
  __property double          Value = { index=(IS_OPTN), read=FValue, write=SetValue, stored = Value_Specified };
};




// ************************************************************************ //
// XML       : SiteOrderModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class SiteOrderModel2 : public SiteOrderModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderItemModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemModel2 : public OrderItemModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderItemSizeModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeModel2 : public OrderItemSizeModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderItemSizeDiscountModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeDiscountModel2 : public OrderItemSizeDiscountModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderItemSizeTaxProfileModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.OnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeTaxProfileModel2 : public OrderItemSizeTaxProfileModel {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IWCFServiceLoyaltyMate/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceLoyaltyMate
// service   : WCFServiceLoyaltyMate
// port      : basicHttpBinding_ServiceLoyaltyMate
// URL       : http://localhost:8734/MenumateServices/LoyaltyMate/
// ************************************************************************ //
__interface INTERFACE_UUID("{57F4745D-2B20-3175-961E-BD77B1A09506}") IWCFServiceLoyaltyMate : public IInvokable
{
public:
  virtual LoyaltyMemberResponse* SaveMember(const UnicodeString inSyndicateCode, const MemberInfo* inInfo) = 0; 
  virtual LoyaltyMemberResponse* GetMemberByUniqueId(const UnicodeString inSyndicateCode, const RequestInfo* requestInfo) = 0; 
  virtual LoyaltyMemberResponse* GetMemberByCardCode(const UnicodeString inSyndicateCode, const RequestInfo* requestInfo) = 0; 
  virtual LoyaltyMemberResponse* GetMemberByEmail(const UnicodeString inSyndicateCode, const RequestInfo* requestInfo) = 0; 
  virtual LoyaltyResponse* UpdateMemberCardCode(const UnicodeString inSyndicateCode, const UnicodeString uniqueId, const UnicodeString memberCardCode) = 0; 
  virtual LoyaltyResponse* PostTransaction(const UnicodeString inSyndicateCode, const TransactionInfo* transaction) = 0; 
  virtual LoyaltyCompanyResponse* GetCompanyInformation(const UnicodeString inSyndicateCode) = 0; 
  virtual LoyaltyGiftCardResponse* GetGiftCardBalance(const UnicodeString inSyndicateCode, const RequestInfo* requestInfo) = 0; 
  virtual LoyaltyVoucherResponse* GetPocketVoucherDetail(const UnicodeString inSyndicateCode, const RequestInfo* requestInfo) = 0; 
  virtual VoucherTransactionResponse* ProcessVoucherTransaction(const UnicodeString inSyndicateCode, const VoucherTransactionInfo* transaction) = 0; 
  virtual LoyaltyResponse* ReleaseVouchers(const UnicodeString inSyndicateCode, const ReleasedVoucherInfo* releasedVoucherInfo) = 0; 
  virtual LoyaltyResponse* SyncMenu(const UnicodeString inSyndicateCode, const SiteMenuInfo* siteViewModel) = 0; 
  virtual LoyaltyResponse* SyncTaxSettings(const UnicodeString inSyndicateCode, const SiteTaxSettingsinfo* siteTaxSettings) = 0; 
  virtual void            GetOrdersFromWeb(const UnicodeString inSyndicateCode, const UnicodeString orders) = 0; 
  virtual LoyaltyResponse* PostOnlineOrderInvoiceInfo(const UnicodeString inSyndicateCode, const SiteOrderModel* siteOrderModel) = 0; 
  virtual LoyaltyOnlineOrderingResponse* SyncOnlineOrderingDetails(const UnicodeString inSyndicateCode, const int siteCode) = 0; 
};
typedef DelphiInterface<IWCFServiceLoyaltyMate> _di_IWCFServiceLoyaltyMate;

_di_IWCFServiceLoyaltyMate GetIWCFServiceLoyaltyMate(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__LoyaltyMateWSDL;
#endif



#endif // _H
