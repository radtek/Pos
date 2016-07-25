// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8739/MenumateServices/ThorLink/?wsdl
//  >Import : http://localhost:8739/MenumateServices/ThorLink/?wsdl>0
//  >Import : http://localhost:8739/MenumateServices/ThorLink/?xsd=xsd0
//  >Import : http://localhost:8739/MenumateServices/ThorLink/?xsd=xsd2
//  >Import : http://localhost:8739/MenumateServices/ThorLink/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (28/06/2016 7:56:27 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   ThorlinkWSDLH
#define   ThorlinkWSDLH

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


namespace NS__ThorlinkWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS DTO_ThorAuth;
class SOAP_REMOTABLE_CLASS DTO_ThorlinkInquiry;
class SOAP_REMOTABLE_CLASS DTO_TMemberInfo;
class SOAP_REMOTABLE_CLASS DTO_VoucherDetails;
class SOAP_REMOTABLE_CLASS DTO_TenderList;
class SOAP_REMOTABLE_CLASS DTO_ItemDetails;
class SOAP_REMOTABLE_CLASS DTO_TenderDetails;
class SOAP_REMOTABLE_CLASS DTO_TPurchaseInfo;
class SOAP_REMOTABLE_CLASS DTO_RefundDetails;
class SOAP_REMOTABLE_CLASS DTO_ThorAuth2;
class SOAP_REMOTABLE_CLASS DTO_ThorlinkInquiry2;
class SOAP_REMOTABLE_CLASS DTO_TMemberInfo2;
class SOAP_REMOTABLE_CLASS DTO_VoucherDetails2;
class SOAP_REMOTABLE_CLASS DTO_TenderList2;
class SOAP_REMOTABLE_CLASS DTO_ItemDetails2;
class SOAP_REMOTABLE_CLASS DTO_TenderDetails2;
class SOAP_REMOTABLE_CLASS DTO_TPurchaseInfo2;
class SOAP_REMOTABLE_CLASS DTO_RefundDetails2;

enum class TenderTypeThor   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink"[GblSmpl] */
{
  eThorCash, 
  eThorDebitCard, 
  eThorPoints, 
  eThorVoucher, 
  eThorCheque
};

class TenderTypeThor_TypeInfoHolder : public TObject {
  TenderTypeThor __instanceType;
public:
__published:
  __property TenderTypeThor __propType = { read=__instanceType };
};



// ************************************************************************ //
// XML       : DTO_ThorAuth, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_ThorAuth : public TRemotable {
private:
  UnicodeString   FappKey;
  bool            FappKey_Specified;
  UnicodeString   FdeviceCode;
  bool            FdeviceCode_Specified;
  UnicodeString   FmerchantCode;
  bool            FmerchantCode_Specified;
  UnicodeString   FsiteNo;
  bool            FsiteNo_Specified;
  void __fastcall SetappKey(int Index, UnicodeString _prop_val)
  {  FappKey = _prop_val; FappKey_Specified = true;  }
  bool __fastcall appKey_Specified(int Index)
  {  return FappKey_Specified;  } 
  void __fastcall SetdeviceCode(int Index, UnicodeString _prop_val)
  {  FdeviceCode = _prop_val; FdeviceCode_Specified = true;  }
  bool __fastcall deviceCode_Specified(int Index)
  {  return FdeviceCode_Specified;  } 
  void __fastcall SetmerchantCode(int Index, UnicodeString _prop_val)
  {  FmerchantCode = _prop_val; FmerchantCode_Specified = true;  }
  bool __fastcall merchantCode_Specified(int Index)
  {  return FmerchantCode_Specified;  } 
  void __fastcall SetsiteNo(int Index, UnicodeString _prop_val)
  {  FsiteNo = _prop_val; FsiteNo_Specified = true;  }
  bool __fastcall siteNo_Specified(int Index)
  {  return FsiteNo_Specified;  } 
__published:
  __property UnicodeString     appKey = { index=(IS_OPTN|IS_NLBL), read=FappKey, write=SetappKey, stored = appKey_Specified };
  __property UnicodeString deviceCode = { index=(IS_OPTN|IS_NLBL), read=FdeviceCode, write=SetdeviceCode, stored = deviceCode_Specified };
  __property UnicodeString merchantCode = { index=(IS_OPTN|IS_NLBL), read=FmerchantCode, write=SetmerchantCode, stored = merchantCode_Specified };
  __property UnicodeString     siteNo = { index=(IS_OPTN|IS_NLBL), read=FsiteNo, write=SetsiteNo, stored = siteNo_Specified };
};




// ************************************************************************ //
// XML       : DTO_ThorlinkInquiry, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_ThorlinkInquiry : public TRemotable {
private:
  UnicodeString   FcardNo;
  bool            FcardNo_Specified;
  void __fastcall SetcardNo(int Index, UnicodeString _prop_val)
  {  FcardNo = _prop_val; FcardNo_Specified = true;  }
  bool __fastcall cardNo_Specified(int Index)
  {  return FcardNo_Specified;  } 
__published:
  __property UnicodeString     cardNo = { index=(IS_OPTN|IS_NLBL), read=FcardNo, write=SetcardNo, stored = cardNo_Specified };
};


typedef DynamicArray<DTO_VoucherDetails*> ArrayOfDTO_VoucherDetails; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink"[GblCplx] */


// ************************************************************************ //
// XML       : DTO_TMemberInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_TMemberInfo : public TRemotable {
private:
  UnicodeString   Faddress;
  bool            Faddress_Specified;
  UnicodeString   FbirthDate;
  bool            FbirthDate_Specified;
  UnicodeString   Fcity;
  bool            Fcity_Specified;
  UnicodeString   Fcountry;
  bool            Fcountry_Specified;
  UnicodeString   FcountryId;
  bool            FcountryId_Specified;
  double          FcreditBalance;
  bool            FcreditBalance_Specified;
  double          FcreditBalanceMax;
  bool            FcreditBalanceMax_Specified;
  double          FcreditBalanceMin;
  bool            FcreditBalanceMin_Specified;
  UnicodeString   FemailAddress;
  bool            FemailAddress_Specified;
  UnicodeString   FenrollmentDate;
  bool            FenrollmentDate_Specified;
  UnicodeString   FenrollmentTraderId;
  bool            FenrollmentTraderId_Specified;
  UnicodeString   FenrollmentTraderName;
  bool            FenrollmentTraderName_Specified;
  UnicodeString   FfirstName;
  bool            FfirstName_Specified;
  UnicodeString   FgenderId;
  bool            FgenderId_Specified;
  UnicodeString   FgenderName;
  bool            FgenderName_Specified;
  UnicodeString   FinquiryResponse;
  bool            FinquiryResponse_Specified;
  UnicodeString   FlastName;
  bool            FlastName_Specified;
  double          FloyaltyBalance;
  bool            FloyaltyBalance_Specified;
  double          FloyaltyBalanceMax;
  bool            FloyaltyBalanceMax_Specified;
  double          FloyaltyBalanceMin;
  bool            FloyaltyBalanceMin_Specified;
  UnicodeString   FmailingPreferenceId;
  bool            FmailingPreferenceId_Specified;
  UnicodeString   FmailingPreferenceName;
  bool            FmailingPreferenceName_Specified;
  UnicodeString   FmaxCreditTransaction;
  bool            FmaxCreditTransaction_Specified;
  UnicodeString   FmaxPurchaseTransaction;
  bool            FmaxPurchaseTransaction_Specified;
  UnicodeString   FmobileNumber;
  bool            FmobileNumber_Specified;
  UnicodeString   FpostalCode;
  bool            FpostalCode_Specified;
  UnicodeString   FresponseCode;
  bool            FresponseCode_Specified;
  UnicodeString   FresponseDescription;
  bool            FresponseDescription_Specified;
  UnicodeString   Fstate;
  bool            Fstate_Specified;
  UnicodeString   Fsuburb;
  bool            Fsuburb_Specified;
  UnicodeString   FtraderId;
  bool            FtraderId_Specified;
  UnicodeString   FtraderRankId;
  bool            FtraderRankId_Specified;
  UnicodeString   FtraderRankName;
  bool            FtraderRankName_Specified;
  UnicodeString   FtraderStatusId;
  bool            FtraderStatusId_Specified;
  UnicodeString   FtraderStatusName;
  bool            FtraderStatusName_Specified;
  UnicodeString   FtraderTypeId;
  bool            FtraderTypeId_Specified;
  UnicodeString   FtraderTypeName;
  bool            FtraderTypeName_Specified;
  double          FvoucherBalance;
  bool            FvoucherBalance_Specified;
  double          FvoucherBalanceMax;
  bool            FvoucherBalanceMax_Specified;
  double          FvoucherBalanceMin;
  bool            FvoucherBalanceMin_Specified;
  ArrayOfDTO_VoucherDetails FvoucherDetailsOfThor;
  bool            FvoucherDetailsOfThor_Specified;
  void __fastcall Setaddress(int Index, UnicodeString _prop_val)
  {  Faddress = _prop_val; Faddress_Specified = true;  }
  bool __fastcall address_Specified(int Index)
  {  return Faddress_Specified;  } 
  void __fastcall SetbirthDate(int Index, UnicodeString _prop_val)
  {  FbirthDate = _prop_val; FbirthDate_Specified = true;  }
  bool __fastcall birthDate_Specified(int Index)
  {  return FbirthDate_Specified;  } 
  void __fastcall Setcity(int Index, UnicodeString _prop_val)
  {  Fcity = _prop_val; Fcity_Specified = true;  }
  bool __fastcall city_Specified(int Index)
  {  return Fcity_Specified;  } 
  void __fastcall Setcountry(int Index, UnicodeString _prop_val)
  {  Fcountry = _prop_val; Fcountry_Specified = true;  }
  bool __fastcall country_Specified(int Index)
  {  return Fcountry_Specified;  } 
  void __fastcall SetcountryId(int Index, UnicodeString _prop_val)
  {  FcountryId = _prop_val; FcountryId_Specified = true;  }
  bool __fastcall countryId_Specified(int Index)
  {  return FcountryId_Specified;  } 
  void __fastcall SetcreditBalance(int Index, double _prop_val)
  {  FcreditBalance = _prop_val; FcreditBalance_Specified = true;  }
  bool __fastcall creditBalance_Specified(int Index)
  {  return FcreditBalance_Specified;  } 
  void __fastcall SetcreditBalanceMax(int Index, double _prop_val)
  {  FcreditBalanceMax = _prop_val; FcreditBalanceMax_Specified = true;  }
  bool __fastcall creditBalanceMax_Specified(int Index)
  {  return FcreditBalanceMax_Specified;  } 
  void __fastcall SetcreditBalanceMin(int Index, double _prop_val)
  {  FcreditBalanceMin = _prop_val; FcreditBalanceMin_Specified = true;  }
  bool __fastcall creditBalanceMin_Specified(int Index)
  {  return FcreditBalanceMin_Specified;  } 
  void __fastcall SetemailAddress(int Index, UnicodeString _prop_val)
  {  FemailAddress = _prop_val; FemailAddress_Specified = true;  }
  bool __fastcall emailAddress_Specified(int Index)
  {  return FemailAddress_Specified;  } 
  void __fastcall SetenrollmentDate(int Index, UnicodeString _prop_val)
  {  FenrollmentDate = _prop_val; FenrollmentDate_Specified = true;  }
  bool __fastcall enrollmentDate_Specified(int Index)
  {  return FenrollmentDate_Specified;  } 
  void __fastcall SetenrollmentTraderId(int Index, UnicodeString _prop_val)
  {  FenrollmentTraderId = _prop_val; FenrollmentTraderId_Specified = true;  }
  bool __fastcall enrollmentTraderId_Specified(int Index)
  {  return FenrollmentTraderId_Specified;  } 
  void __fastcall SetenrollmentTraderName(int Index, UnicodeString _prop_val)
  {  FenrollmentTraderName = _prop_val; FenrollmentTraderName_Specified = true;  }
  bool __fastcall enrollmentTraderName_Specified(int Index)
  {  return FenrollmentTraderName_Specified;  } 
  void __fastcall SetfirstName(int Index, UnicodeString _prop_val)
  {  FfirstName = _prop_val; FfirstName_Specified = true;  }
  bool __fastcall firstName_Specified(int Index)
  {  return FfirstName_Specified;  } 
  void __fastcall SetgenderId(int Index, UnicodeString _prop_val)
  {  FgenderId = _prop_val; FgenderId_Specified = true;  }
  bool __fastcall genderId_Specified(int Index)
  {  return FgenderId_Specified;  } 
  void __fastcall SetgenderName(int Index, UnicodeString _prop_val)
  {  FgenderName = _prop_val; FgenderName_Specified = true;  }
  bool __fastcall genderName_Specified(int Index)
  {  return FgenderName_Specified;  } 
  void __fastcall SetinquiryResponse(int Index, UnicodeString _prop_val)
  {  FinquiryResponse = _prop_val; FinquiryResponse_Specified = true;  }
  bool __fastcall inquiryResponse_Specified(int Index)
  {  return FinquiryResponse_Specified;  } 
  void __fastcall SetlastName(int Index, UnicodeString _prop_val)
  {  FlastName = _prop_val; FlastName_Specified = true;  }
  bool __fastcall lastName_Specified(int Index)
  {  return FlastName_Specified;  } 
  void __fastcall SetloyaltyBalance(int Index, double _prop_val)
  {  FloyaltyBalance = _prop_val; FloyaltyBalance_Specified = true;  }
  bool __fastcall loyaltyBalance_Specified(int Index)
  {  return FloyaltyBalance_Specified;  } 
  void __fastcall SetloyaltyBalanceMax(int Index, double _prop_val)
  {  FloyaltyBalanceMax = _prop_val; FloyaltyBalanceMax_Specified = true;  }
  bool __fastcall loyaltyBalanceMax_Specified(int Index)
  {  return FloyaltyBalanceMax_Specified;  } 
  void __fastcall SetloyaltyBalanceMin(int Index, double _prop_val)
  {  FloyaltyBalanceMin = _prop_val; FloyaltyBalanceMin_Specified = true;  }
  bool __fastcall loyaltyBalanceMin_Specified(int Index)
  {  return FloyaltyBalanceMin_Specified;  } 
  void __fastcall SetmailingPreferenceId(int Index, UnicodeString _prop_val)
  {  FmailingPreferenceId = _prop_val; FmailingPreferenceId_Specified = true;  }
  bool __fastcall mailingPreferenceId_Specified(int Index)
  {  return FmailingPreferenceId_Specified;  } 
  void __fastcall SetmailingPreferenceName(int Index, UnicodeString _prop_val)
  {  FmailingPreferenceName = _prop_val; FmailingPreferenceName_Specified = true;  }
  bool __fastcall mailingPreferenceName_Specified(int Index)
  {  return FmailingPreferenceName_Specified;  } 
  void __fastcall SetmaxCreditTransaction(int Index, UnicodeString _prop_val)
  {  FmaxCreditTransaction = _prop_val; FmaxCreditTransaction_Specified = true;  }
  bool __fastcall maxCreditTransaction_Specified(int Index)
  {  return FmaxCreditTransaction_Specified;  } 
  void __fastcall SetmaxPurchaseTransaction(int Index, UnicodeString _prop_val)
  {  FmaxPurchaseTransaction = _prop_val; FmaxPurchaseTransaction_Specified = true;  }
  bool __fastcall maxPurchaseTransaction_Specified(int Index)
  {  return FmaxPurchaseTransaction_Specified;  } 
  void __fastcall SetmobileNumber(int Index, UnicodeString _prop_val)
  {  FmobileNumber = _prop_val; FmobileNumber_Specified = true;  }
  bool __fastcall mobileNumber_Specified(int Index)
  {  return FmobileNumber_Specified;  } 
  void __fastcall SetpostalCode(int Index, UnicodeString _prop_val)
  {  FpostalCode = _prop_val; FpostalCode_Specified = true;  }
  bool __fastcall postalCode_Specified(int Index)
  {  return FpostalCode_Specified;  } 
  void __fastcall SetresponseCode(int Index, UnicodeString _prop_val)
  {  FresponseCode = _prop_val; FresponseCode_Specified = true;  }
  bool __fastcall responseCode_Specified(int Index)
  {  return FresponseCode_Specified;  } 
  void __fastcall SetresponseDescription(int Index, UnicodeString _prop_val)
  {  FresponseDescription = _prop_val; FresponseDescription_Specified = true;  }
  bool __fastcall responseDescription_Specified(int Index)
  {  return FresponseDescription_Specified;  } 
  void __fastcall Setstate(int Index, UnicodeString _prop_val)
  {  Fstate = _prop_val; Fstate_Specified = true;  }
  bool __fastcall state_Specified(int Index)
  {  return Fstate_Specified;  } 
  void __fastcall Setsuburb(int Index, UnicodeString _prop_val)
  {  Fsuburb = _prop_val; Fsuburb_Specified = true;  }
  bool __fastcall suburb_Specified(int Index)
  {  return Fsuburb_Specified;  } 
  void __fastcall SettraderId(int Index, UnicodeString _prop_val)
  {  FtraderId = _prop_val; FtraderId_Specified = true;  }
  bool __fastcall traderId_Specified(int Index)
  {  return FtraderId_Specified;  } 
  void __fastcall SettraderRankId(int Index, UnicodeString _prop_val)
  {  FtraderRankId = _prop_val; FtraderRankId_Specified = true;  }
  bool __fastcall traderRankId_Specified(int Index)
  {  return FtraderRankId_Specified;  } 
  void __fastcall SettraderRankName(int Index, UnicodeString _prop_val)
  {  FtraderRankName = _prop_val; FtraderRankName_Specified = true;  }
  bool __fastcall traderRankName_Specified(int Index)
  {  return FtraderRankName_Specified;  } 
  void __fastcall SettraderStatusId(int Index, UnicodeString _prop_val)
  {  FtraderStatusId = _prop_val; FtraderStatusId_Specified = true;  }
  bool __fastcall traderStatusId_Specified(int Index)
  {  return FtraderStatusId_Specified;  } 
  void __fastcall SettraderStatusName(int Index, UnicodeString _prop_val)
  {  FtraderStatusName = _prop_val; FtraderStatusName_Specified = true;  }
  bool __fastcall traderStatusName_Specified(int Index)
  {  return FtraderStatusName_Specified;  } 
  void __fastcall SettraderTypeId(int Index, UnicodeString _prop_val)
  {  FtraderTypeId = _prop_val; FtraderTypeId_Specified = true;  }
  bool __fastcall traderTypeId_Specified(int Index)
  {  return FtraderTypeId_Specified;  } 
  void __fastcall SettraderTypeName(int Index, UnicodeString _prop_val)
  {  FtraderTypeName = _prop_val; FtraderTypeName_Specified = true;  }
  bool __fastcall traderTypeName_Specified(int Index)
  {  return FtraderTypeName_Specified;  } 
  void __fastcall SetvoucherBalance(int Index, double _prop_val)
  {  FvoucherBalance = _prop_val; FvoucherBalance_Specified = true;  }
  bool __fastcall voucherBalance_Specified(int Index)
  {  return FvoucherBalance_Specified;  } 
  void __fastcall SetvoucherBalanceMax(int Index, double _prop_val)
  {  FvoucherBalanceMax = _prop_val; FvoucherBalanceMax_Specified = true;  }
  bool __fastcall voucherBalanceMax_Specified(int Index)
  {  return FvoucherBalanceMax_Specified;  } 
  void __fastcall SetvoucherBalanceMin(int Index, double _prop_val)
  {  FvoucherBalanceMin = _prop_val; FvoucherBalanceMin_Specified = true;  }
  bool __fastcall voucherBalanceMin_Specified(int Index)
  {  return FvoucherBalanceMin_Specified;  } 
  void __fastcall SetvoucherDetailsOfThor(int Index, ArrayOfDTO_VoucherDetails _prop_val)
  {  FvoucherDetailsOfThor = _prop_val; FvoucherDetailsOfThor_Specified = true;  }
  bool __fastcall voucherDetailsOfThor_Specified(int Index)
  {  return FvoucherDetailsOfThor_Specified;  } 

public:
  __fastcall ~DTO_TMemberInfo();
__published:
  __property UnicodeString    address = { index=(IS_OPTN|IS_NLBL), read=Faddress, write=Setaddress, stored = address_Specified };
  __property UnicodeString  birthDate = { index=(IS_OPTN|IS_NLBL), read=FbirthDate, write=SetbirthDate, stored = birthDate_Specified };
  __property UnicodeString       city = { index=(IS_OPTN|IS_NLBL), read=Fcity, write=Setcity, stored = city_Specified };
  __property UnicodeString    country = { index=(IS_OPTN|IS_NLBL), read=Fcountry, write=Setcountry, stored = country_Specified };
  __property UnicodeString  countryId = { index=(IS_OPTN|IS_NLBL), read=FcountryId, write=SetcountryId, stored = countryId_Specified };
  __property double     creditBalance = { index=(IS_OPTN), read=FcreditBalance, write=SetcreditBalance, stored = creditBalance_Specified };
  __property double     creditBalanceMax = { index=(IS_OPTN), read=FcreditBalanceMax, write=SetcreditBalanceMax, stored = creditBalanceMax_Specified };
  __property double     creditBalanceMin = { index=(IS_OPTN), read=FcreditBalanceMin, write=SetcreditBalanceMin, stored = creditBalanceMin_Specified };
  __property UnicodeString emailAddress = { index=(IS_OPTN|IS_NLBL), read=FemailAddress, write=SetemailAddress, stored = emailAddress_Specified };
  __property UnicodeString enrollmentDate = { index=(IS_OPTN|IS_NLBL), read=FenrollmentDate, write=SetenrollmentDate, stored = enrollmentDate_Specified };
  __property UnicodeString enrollmentTraderId = { index=(IS_OPTN|IS_NLBL), read=FenrollmentTraderId, write=SetenrollmentTraderId, stored = enrollmentTraderId_Specified };
  __property UnicodeString enrollmentTraderName = { index=(IS_OPTN|IS_NLBL), read=FenrollmentTraderName, write=SetenrollmentTraderName, stored = enrollmentTraderName_Specified };
  __property UnicodeString  firstName = { index=(IS_OPTN|IS_NLBL), read=FfirstName, write=SetfirstName, stored = firstName_Specified };
  __property UnicodeString   genderId = { index=(IS_OPTN|IS_NLBL), read=FgenderId, write=SetgenderId, stored = genderId_Specified };
  __property UnicodeString genderName = { index=(IS_OPTN|IS_NLBL), read=FgenderName, write=SetgenderName, stored = genderName_Specified };
  __property UnicodeString inquiryResponse = { index=(IS_OPTN|IS_NLBL), read=FinquiryResponse, write=SetinquiryResponse, stored = inquiryResponse_Specified };
  __property UnicodeString   lastName = { index=(IS_OPTN|IS_NLBL), read=FlastName, write=SetlastName, stored = lastName_Specified };
  __property double     loyaltyBalance = { index=(IS_OPTN), read=FloyaltyBalance, write=SetloyaltyBalance, stored = loyaltyBalance_Specified };
  __property double     loyaltyBalanceMax = { index=(IS_OPTN), read=FloyaltyBalanceMax, write=SetloyaltyBalanceMax, stored = loyaltyBalanceMax_Specified };
  __property double     loyaltyBalanceMin = { index=(IS_OPTN), read=FloyaltyBalanceMin, write=SetloyaltyBalanceMin, stored = loyaltyBalanceMin_Specified };
  __property UnicodeString mailingPreferenceId = { index=(IS_OPTN|IS_NLBL), read=FmailingPreferenceId, write=SetmailingPreferenceId, stored = mailingPreferenceId_Specified };
  __property UnicodeString mailingPreferenceName = { index=(IS_OPTN|IS_NLBL), read=FmailingPreferenceName, write=SetmailingPreferenceName, stored = mailingPreferenceName_Specified };
  __property UnicodeString maxCreditTransaction = { index=(IS_OPTN|IS_NLBL), read=FmaxCreditTransaction, write=SetmaxCreditTransaction, stored = maxCreditTransaction_Specified };
  __property UnicodeString maxPurchaseTransaction = { index=(IS_OPTN|IS_NLBL), read=FmaxPurchaseTransaction, write=SetmaxPurchaseTransaction, stored = maxPurchaseTransaction_Specified };
  __property UnicodeString mobileNumber = { index=(IS_OPTN|IS_NLBL), read=FmobileNumber, write=SetmobileNumber, stored = mobileNumber_Specified };
  __property UnicodeString postalCode = { index=(IS_OPTN|IS_NLBL), read=FpostalCode, write=SetpostalCode, stored = postalCode_Specified };
  __property UnicodeString responseCode = { index=(IS_OPTN|IS_NLBL), read=FresponseCode, write=SetresponseCode, stored = responseCode_Specified };
  __property UnicodeString responseDescription = { index=(IS_OPTN|IS_NLBL), read=FresponseDescription, write=SetresponseDescription, stored = responseDescription_Specified };
  __property UnicodeString      state = { index=(IS_OPTN|IS_NLBL), read=Fstate, write=Setstate, stored = state_Specified };
  __property UnicodeString     suburb = { index=(IS_OPTN|IS_NLBL), read=Fsuburb, write=Setsuburb, stored = suburb_Specified };
  __property UnicodeString   traderId = { index=(IS_OPTN|IS_NLBL), read=FtraderId, write=SettraderId, stored = traderId_Specified };
  __property UnicodeString traderRankId = { index=(IS_OPTN|IS_NLBL), read=FtraderRankId, write=SettraderRankId, stored = traderRankId_Specified };
  __property UnicodeString traderRankName = { index=(IS_OPTN|IS_NLBL), read=FtraderRankName, write=SettraderRankName, stored = traderRankName_Specified };
  __property UnicodeString traderStatusId = { index=(IS_OPTN|IS_NLBL), read=FtraderStatusId, write=SettraderStatusId, stored = traderStatusId_Specified };
  __property UnicodeString traderStatusName = { index=(IS_OPTN|IS_NLBL), read=FtraderStatusName, write=SettraderStatusName, stored = traderStatusName_Specified };
  __property UnicodeString traderTypeId = { index=(IS_OPTN|IS_NLBL), read=FtraderTypeId, write=SettraderTypeId, stored = traderTypeId_Specified };
  __property UnicodeString traderTypeName = { index=(IS_OPTN|IS_NLBL), read=FtraderTypeName, write=SettraderTypeName, stored = traderTypeName_Specified };
  __property double     voucherBalance = { index=(IS_OPTN), read=FvoucherBalance, write=SetvoucherBalance, stored = voucherBalance_Specified };
  __property double     voucherBalanceMax = { index=(IS_OPTN), read=FvoucherBalanceMax, write=SetvoucherBalanceMax, stored = voucherBalanceMax_Specified };
  __property double     voucherBalanceMin = { index=(IS_OPTN), read=FvoucherBalanceMin, write=SetvoucherBalanceMin, stored = voucherBalanceMin_Specified };
  __property ArrayOfDTO_VoucherDetails voucherDetailsOfThor = { index=(IS_OPTN|IS_NLBL), read=FvoucherDetailsOfThor, write=SetvoucherDetailsOfThor, stored = voucherDetailsOfThor_Specified };
};




// ************************************************************************ //
// XML       : DTO_VoucherDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_VoucherDetails : public TRemotable {
private:
  int             Factive;
  bool            Factive_Specified;
  int             FavailableInStore;
  bool            FavailableInStore_Specified;
  UnicodeString   Fcode;
  bool            Fcode_Specified;
  UnicodeString   FendDate;
  bool            FendDate_Specified;
  UnicodeString   FexpiryDate;
  bool            FexpiryDate_Specified;
  UnicodeString   Fid;
  bool            Fid_Specified;
  UnicodeString   FimageUrl;
  bool            FimageUrl_Specified;
  UnicodeString   FmerchantName;
  bool            FmerchantName_Specified;
  UnicodeString   Fname;
  bool            Fname_Specified;
  UnicodeString   FpluCode;
  bool            FpluCode_Specified;
  UnicodeString   FsecurityCode;
  bool            FsecurityCode_Specified;
  UnicodeString   FstartDate;
  bool            FstartDate_Specified;
  int             Ftype;
  bool            Ftype_Specified;
  double          Fvalue;
  bool            Fvalue_Specified;
  void __fastcall Setactive(int Index, int _prop_val)
  {  Factive = _prop_val; Factive_Specified = true;  }
  bool __fastcall active_Specified(int Index)
  {  return Factive_Specified;  } 
  void __fastcall SetavailableInStore(int Index, int _prop_val)
  {  FavailableInStore = _prop_val; FavailableInStore_Specified = true;  }
  bool __fastcall availableInStore_Specified(int Index)
  {  return FavailableInStore_Specified;  } 
  void __fastcall Setcode(int Index, UnicodeString _prop_val)
  {  Fcode = _prop_val; Fcode_Specified = true;  }
  bool __fastcall code_Specified(int Index)
  {  return Fcode_Specified;  } 
  void __fastcall SetendDate(int Index, UnicodeString _prop_val)
  {  FendDate = _prop_val; FendDate_Specified = true;  }
  bool __fastcall endDate_Specified(int Index)
  {  return FendDate_Specified;  } 
  void __fastcall SetexpiryDate(int Index, UnicodeString _prop_val)
  {  FexpiryDate = _prop_val; FexpiryDate_Specified = true;  }
  bool __fastcall expiryDate_Specified(int Index)
  {  return FexpiryDate_Specified;  } 
  void __fastcall Setid(int Index, UnicodeString _prop_val)
  {  Fid = _prop_val; Fid_Specified = true;  }
  bool __fastcall id_Specified(int Index)
  {  return Fid_Specified;  } 
  void __fastcall SetimageUrl(int Index, UnicodeString _prop_val)
  {  FimageUrl = _prop_val; FimageUrl_Specified = true;  }
  bool __fastcall imageUrl_Specified(int Index)
  {  return FimageUrl_Specified;  } 
  void __fastcall SetmerchantName(int Index, UnicodeString _prop_val)
  {  FmerchantName = _prop_val; FmerchantName_Specified = true;  }
  bool __fastcall merchantName_Specified(int Index)
  {  return FmerchantName_Specified;  } 
  void __fastcall Setname(int Index, UnicodeString _prop_val)
  {  Fname = _prop_val; Fname_Specified = true;  }
  bool __fastcall name_Specified(int Index)
  {  return Fname_Specified;  } 
  void __fastcall SetpluCode(int Index, UnicodeString _prop_val)
  {  FpluCode = _prop_val; FpluCode_Specified = true;  }
  bool __fastcall pluCode_Specified(int Index)
  {  return FpluCode_Specified;  } 
  void __fastcall SetsecurityCode(int Index, UnicodeString _prop_val)
  {  FsecurityCode = _prop_val; FsecurityCode_Specified = true;  }
  bool __fastcall securityCode_Specified(int Index)
  {  return FsecurityCode_Specified;  } 
  void __fastcall SetstartDate(int Index, UnicodeString _prop_val)
  {  FstartDate = _prop_val; FstartDate_Specified = true;  }
  bool __fastcall startDate_Specified(int Index)
  {  return FstartDate_Specified;  } 
  void __fastcall Settype(int Index, int _prop_val)
  {  Ftype = _prop_val; Ftype_Specified = true;  }
  bool __fastcall type_Specified(int Index)
  {  return Ftype_Specified;  } 
  void __fastcall Setvalue(int Index, double _prop_val)
  {  Fvalue = _prop_val; Fvalue_Specified = true;  }
  bool __fastcall value_Specified(int Index)
  {  return Fvalue_Specified;  } 
__published:
  __property int            active = { index=(IS_OPTN), read=Factive, write=Setactive, stored = active_Specified };
  __property int        availableInStore = { index=(IS_OPTN), read=FavailableInStore, write=SetavailableInStore, stored = availableInStore_Specified };
  __property UnicodeString       code = { index=(IS_OPTN|IS_NLBL), read=Fcode, write=Setcode, stored = code_Specified };
  __property UnicodeString    endDate = { index=(IS_OPTN|IS_NLBL), read=FendDate, write=SetendDate, stored = endDate_Specified };
  __property UnicodeString expiryDate = { index=(IS_OPTN|IS_NLBL), read=FexpiryDate, write=SetexpiryDate, stored = expiryDate_Specified };
  __property UnicodeString         id = { index=(IS_OPTN|IS_NLBL), read=Fid, write=Setid, stored = id_Specified };
  __property UnicodeString   imageUrl = { index=(IS_OPTN|IS_NLBL), read=FimageUrl, write=SetimageUrl, stored = imageUrl_Specified };
  __property UnicodeString merchantName = { index=(IS_OPTN|IS_NLBL), read=FmerchantName, write=SetmerchantName, stored = merchantName_Specified };
  __property UnicodeString       name = { index=(IS_OPTN|IS_NLBL), read=Fname, write=Setname, stored = name_Specified };
  __property UnicodeString    pluCode = { index=(IS_OPTN|IS_NLBL), read=FpluCode, write=SetpluCode, stored = pluCode_Specified };
  __property UnicodeString securityCode = { index=(IS_OPTN|IS_NLBL), read=FsecurityCode, write=SetsecurityCode, stored = securityCode_Specified };
  __property UnicodeString  startDate = { index=(IS_OPTN|IS_NLBL), read=FstartDate, write=SetstartDate, stored = startDate_Specified };
  __property int              type = { index=(IS_OPTN), read=Ftype, write=Settype, stored = type_Specified };
  __property double          value = { index=(IS_OPTN), read=Fvalue, write=Setvalue, stored = value_Specified };
};


typedef DynamicArray<DTO_ItemDetails*> ArrayOfDTO_ItemDetails; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink"[GblCplx] */
typedef DynamicArray<DTO_TenderDetails*> ArrayOfDTO_TenderDetails; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink"[GblCplx] */


// ************************************************************************ //
// XML       : DTO_TenderList, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_TenderList : public TRemotable {
private:
  ArrayOfDTO_ItemDetails FitemDetailsList;
  bool            FitemDetailsList_Specified;
  ArrayOfDTO_TenderDetails FtenderItemDetails;
  bool            FtenderItemDetails_Specified;
  void __fastcall SetitemDetailsList(int Index, ArrayOfDTO_ItemDetails _prop_val)
  {  FitemDetailsList = _prop_val; FitemDetailsList_Specified = true;  }
  bool __fastcall itemDetailsList_Specified(int Index)
  {  return FitemDetailsList_Specified;  } 
  void __fastcall SettenderItemDetails(int Index, ArrayOfDTO_TenderDetails _prop_val)
  {  FtenderItemDetails = _prop_val; FtenderItemDetails_Specified = true;  }
  bool __fastcall tenderItemDetails_Specified(int Index)
  {  return FtenderItemDetails_Specified;  } 

public:
  __fastcall ~DTO_TenderList();
__published:
  __property ArrayOfDTO_ItemDetails itemDetailsList = { index=(IS_OPTN|IS_NLBL), read=FitemDetailsList, write=SetitemDetailsList, stored = itemDetailsList_Specified };
  __property ArrayOfDTO_TenderDetails tenderItemDetails = { index=(IS_OPTN|IS_NLBL), read=FtenderItemDetails, write=SettenderItemDetails, stored = tenderItemDetails_Specified };
};




// ************************************************************************ //
// XML       : DTO_ItemDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_ItemDetails : public TRemotable {
private:
  double          Fqty;
  bool            Fqty_Specified;
  UnicodeString   FthirdPartyCode;
  bool            FthirdPartyCode_Specified;
  double          FunitPrice;
  bool            FunitPrice_Specified;
  void __fastcall Setqty(int Index, double _prop_val)
  {  Fqty = _prop_val; Fqty_Specified = true;  }
  bool __fastcall qty_Specified(int Index)
  {  return Fqty_Specified;  } 
  void __fastcall SetthirdPartyCode(int Index, UnicodeString _prop_val)
  {  FthirdPartyCode = _prop_val; FthirdPartyCode_Specified = true;  }
  bool __fastcall thirdPartyCode_Specified(int Index)
  {  return FthirdPartyCode_Specified;  } 
  void __fastcall SetunitPrice(int Index, double _prop_val)
  {  FunitPrice = _prop_val; FunitPrice_Specified = true;  }
  bool __fastcall unitPrice_Specified(int Index)
  {  return FunitPrice_Specified;  } 
__published:
  __property double            qty = { index=(IS_OPTN), read=Fqty, write=Setqty, stored = qty_Specified };
  __property UnicodeString thirdPartyCode = { index=(IS_OPTN|IS_NLBL), read=FthirdPartyCode, write=SetthirdPartyCode, stored = thirdPartyCode_Specified };
  __property double      unitPrice = { index=(IS_OPTN), read=FunitPrice, write=SetunitPrice, stored = unitPrice_Specified };
};




// ************************************************************************ //
// XML       : DTO_TenderDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_TenderDetails : public TRemotable {
private:
  UnicodeString   FcardNo;
  bool            FcardNo_Specified;
  ArrayOfDTO_ItemDetails FitemDetails;
  bool            FitemDetails_Specified;
  bool            FsendTransactionValue;
  bool            FsendTransactionValue_Specified;
  UnicodeString   FtenderIdentifier;
  bool            FtenderIdentifier_Specified;
  double          FtenderValue;
  bool            FtenderValue_Specified;
  TenderTypeThor  FthorTenderType;
  bool            FthorTenderType_Specified;
  void __fastcall SetcardNo(int Index, UnicodeString _prop_val)
  {  FcardNo = _prop_val; FcardNo_Specified = true;  }
  bool __fastcall cardNo_Specified(int Index)
  {  return FcardNo_Specified;  } 
  void __fastcall SetitemDetails(int Index, ArrayOfDTO_ItemDetails _prop_val)
  {  FitemDetails = _prop_val; FitemDetails_Specified = true;  }
  bool __fastcall itemDetails_Specified(int Index)
  {  return FitemDetails_Specified;  } 
  void __fastcall SetsendTransactionValue(int Index, bool _prop_val)
  {  FsendTransactionValue = _prop_val; FsendTransactionValue_Specified = true;  }
  bool __fastcall sendTransactionValue_Specified(int Index)
  {  return FsendTransactionValue_Specified;  } 
  void __fastcall SettenderIdentifier(int Index, UnicodeString _prop_val)
  {  FtenderIdentifier = _prop_val; FtenderIdentifier_Specified = true;  }
  bool __fastcall tenderIdentifier_Specified(int Index)
  {  return FtenderIdentifier_Specified;  } 
  void __fastcall SettenderValue(int Index, double _prop_val)
  {  FtenderValue = _prop_val; FtenderValue_Specified = true;  }
  bool __fastcall tenderValue_Specified(int Index)
  {  return FtenderValue_Specified;  } 
  void __fastcall SetthorTenderType(int Index, TenderTypeThor _prop_val)
  {  FthorTenderType = _prop_val; FthorTenderType_Specified = true;  }
  bool __fastcall thorTenderType_Specified(int Index)
  {  return FthorTenderType_Specified;  } 

public:
  __fastcall ~DTO_TenderDetails();
__published:
  __property UnicodeString     cardNo = { index=(IS_OPTN|IS_NLBL), read=FcardNo, write=SetcardNo, stored = cardNo_Specified };
  __property ArrayOfDTO_ItemDetails itemDetails = { index=(IS_OPTN|IS_NLBL), read=FitemDetails, write=SetitemDetails, stored = itemDetails_Specified };
  __property bool       sendTransactionValue = { index=(IS_OPTN), read=FsendTransactionValue, write=SetsendTransactionValue, stored = sendTransactionValue_Specified };
  __property UnicodeString tenderIdentifier = { index=(IS_OPTN|IS_NLBL), read=FtenderIdentifier, write=SettenderIdentifier, stored = tenderIdentifier_Specified };
  __property double     tenderValue = { index=(IS_OPTN), read=FtenderValue, write=SettenderValue, stored = tenderValue_Specified };
  __property TenderTypeThor thorTenderType = { index=(IS_OPTN), read=FthorTenderType, write=SetthorTenderType, stored = thorTenderType_Specified };
};




// ************************************************************************ //
// XML       : DTO_TPurchaseInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_TPurchaseInfo : public TRemotable {
private:
  UnicodeString   FCardNumber;
  bool            FCardNumber_Specified;
  double          FCreditValue;
  bool            FCreditValue_Specified;
  double          FLoyaltyValue;
  bool            FLoyaltyValue_Specified;
  int             FResponseCode;
  bool            FResponseCode_Specified;
  UnicodeString   FResponseMessage;
  bool            FResponseMessage_Specified;
  UnicodeString   FTransactionNumber;
  bool            FTransactionNumber_Specified;
  double          FTransactionValue;
  bool            FTransactionValue_Specified;
  void __fastcall SetCardNumber(int Index, UnicodeString _prop_val)
  {  FCardNumber = _prop_val; FCardNumber_Specified = true;  }
  bool __fastcall CardNumber_Specified(int Index)
  {  return FCardNumber_Specified;  } 
  void __fastcall SetCreditValue(int Index, double _prop_val)
  {  FCreditValue = _prop_val; FCreditValue_Specified = true;  }
  bool __fastcall CreditValue_Specified(int Index)
  {  return FCreditValue_Specified;  } 
  void __fastcall SetLoyaltyValue(int Index, double _prop_val)
  {  FLoyaltyValue = _prop_val; FLoyaltyValue_Specified = true;  }
  bool __fastcall LoyaltyValue_Specified(int Index)
  {  return FLoyaltyValue_Specified;  } 
  void __fastcall SetResponseCode(int Index, int _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 
  void __fastcall SetResponseMessage(int Index, UnicodeString _prop_val)
  {  FResponseMessage = _prop_val; FResponseMessage_Specified = true;  }
  bool __fastcall ResponseMessage_Specified(int Index)
  {  return FResponseMessage_Specified;  } 
  void __fastcall SetTransactionNumber(int Index, UnicodeString _prop_val)
  {  FTransactionNumber = _prop_val; FTransactionNumber_Specified = true;  }
  bool __fastcall TransactionNumber_Specified(int Index)
  {  return FTransactionNumber_Specified;  } 
  void __fastcall SetTransactionValue(int Index, double _prop_val)
  {  FTransactionValue = _prop_val; FTransactionValue_Specified = true;  }
  bool __fastcall TransactionValue_Specified(int Index)
  {  return FTransactionValue_Specified;  } 
__published:
  __property UnicodeString CardNumber = { index=(IS_OPTN|IS_NLBL), read=FCardNumber, write=SetCardNumber, stored = CardNumber_Specified };
  __property double     CreditValue = { index=(IS_OPTN), read=FCreditValue, write=SetCreditValue, stored = CreditValue_Specified };
  __property double     LoyaltyValue = { index=(IS_OPTN), read=FLoyaltyValue, write=SetLoyaltyValue, stored = LoyaltyValue_Specified };
  __property int        ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
  __property UnicodeString ResponseMessage = { index=(IS_OPTN|IS_NLBL), read=FResponseMessage, write=SetResponseMessage, stored = ResponseMessage_Specified };
  __property UnicodeString TransactionNumber = { index=(IS_OPTN|IS_NLBL), read=FTransactionNumber, write=SetTransactionNumber, stored = TransactionNumber_Specified };
  __property double     TransactionValue = { index=(IS_OPTN), read=FTransactionValue, write=SetTransactionValue, stored = TransactionValue_Specified };
};




// ************************************************************************ //
// XML       : DTO_RefundDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_RefundDetails : public TRemotable {
private:
  UnicodeString   FCardNo;
  bool            FCardNo_Specified;
  double          FCreditValue;
  bool            FCreditValue_Specified;
  double          FLoyaltyValue;
  bool            FLoyaltyValue_Specified;
  double          FTransactionValue;
  bool            FTransactionValue_Specified;
  void __fastcall SetCardNo(int Index, UnicodeString _prop_val)
  {  FCardNo = _prop_val; FCardNo_Specified = true;  }
  bool __fastcall CardNo_Specified(int Index)
  {  return FCardNo_Specified;  } 
  void __fastcall SetCreditValue(int Index, double _prop_val)
  {  FCreditValue = _prop_val; FCreditValue_Specified = true;  }
  bool __fastcall CreditValue_Specified(int Index)
  {  return FCreditValue_Specified;  } 
  void __fastcall SetLoyaltyValue(int Index, double _prop_val)
  {  FLoyaltyValue = _prop_val; FLoyaltyValue_Specified = true;  }
  bool __fastcall LoyaltyValue_Specified(int Index)
  {  return FLoyaltyValue_Specified;  } 
  void __fastcall SetTransactionValue(int Index, double _prop_val)
  {  FTransactionValue = _prop_val; FTransactionValue_Specified = true;  }
  bool __fastcall TransactionValue_Specified(int Index)
  {  return FTransactionValue_Specified;  } 
__published:
  __property UnicodeString     CardNo = { index=(IS_OPTN|IS_NLBL), read=FCardNo, write=SetCardNo, stored = CardNo_Specified };
  __property double     CreditValue = { index=(IS_OPTN), read=FCreditValue, write=SetCreditValue, stored = CreditValue_Specified };
  __property double     LoyaltyValue = { index=(IS_OPTN), read=FLoyaltyValue, write=SetLoyaltyValue, stored = LoyaltyValue_Specified };
  __property double     TransactionValue = { index=(IS_OPTN), read=FTransactionValue, write=SetTransactionValue, stored = TransactionValue_Specified };
};




// ************************************************************************ //
// XML       : DTO_ThorAuth, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_ThorAuth2 : public DTO_ThorAuth {
private:
__published:
};




// ************************************************************************ //
// XML       : DTO_ThorlinkInquiry, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_ThorlinkInquiry2 : public DTO_ThorlinkInquiry {
private:
__published:
};




// ************************************************************************ //
// XML       : DTO_TMemberInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_TMemberInfo2 : public DTO_TMemberInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : DTO_VoucherDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_VoucherDetails2 : public DTO_VoucherDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : DTO_TenderList, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_TenderList2 : public DTO_TenderList {
private:
__published:
};




// ************************************************************************ //
// XML       : DTO_ItemDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_ItemDetails2 : public DTO_ItemDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : DTO_TenderDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_TenderDetails2 : public DTO_TenderDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : DTO_TPurchaseInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_TPurchaseInfo2 : public DTO_TPurchaseInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : DTO_RefundDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink
// ************************************************************************ //
class DTO_RefundDetails2 : public DTO_RefundDetails {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IWCFServiceThorlink/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceThorLink
// service   : WCFServiceThorlink
// port      : basicHttpBinding_ServiceThorLink
// URL       : http://localhost:8739/MenumateServices/ThorLink/
// ************************************************************************ //
__interface INTERFACE_UUID("{C0EBCEF0-570B-3B9A-84B1-D3BA78CC12E1}") IWCFServiceThorlink : public IInvokable
{
public:
  virtual void            Initialize(const DTO_ThorAuth* dto_thorAuth) = 0; 
  virtual DTO_TMemberInfo* GetMemberInformation(const DTO_ThorlinkInquiry* inquiry) = 0; 
  virtual DTO_TPurchaseInfo* GetTenderInformation(const DTO_TenderList* tenderDetailsList) = 0; 
  virtual DTO_TPurchaseInfo* GetRefundInformation(const DTO_RefundDetails* refundDetails) = 0; 
};
typedef DelphiInterface<IWCFServiceThorlink> _di_IWCFServiceThorlink;

_di_IWCFServiceThorlink GetIWCFServiceThorlink(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__ThorlinkWSDL;
#endif



#endif // _H
