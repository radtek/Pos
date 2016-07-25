// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8734/MenumateServices/LoyaltyMate/?wsdl
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?wsdl>0
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd0
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd2
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd3
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (4/02/2015 9:33:26 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   LoyaltyMateWSDLH
#define   LoyaltyMateWSDLH

#include <System.hpp>
#include <InvokeRegistry.hpp>
#include <XSBuiltIns.hpp>
#include <winsock2.h>
#include <SOAPHTTPClient.hpp>
#include "GlobalSettings.h"

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


namespace NS_LoyaltyMateWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:dateTime        - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:long            - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS MMServiceResponse;
class SOAP_REMOTABLE_CLASS MMServiceResponse2;
class SOAP_REMOTABLE_CLASS MemberInfo;
class SOAP_REMOTABLE_CLASS LoyaltyMemberResponse;
class SOAP_REMOTABLE_CLASS LoyaltyResponse;
class SOAP_REMOTABLE_CLASS LoyaltyMemberListResponse;
class SOAP_REMOTABLE_CLASS TransactionInfo;
class SOAP_REMOTABLE_CLASS TierLevelInfo;
class SOAP_REMOTABLE_CLASS LoyaltyTierResponse;
class SOAP_REMOTABLE_CLASS LoyaltyTierListResponse;
class SOAP_REMOTABLE_CLASS PointsInfo;
class SOAP_REMOTABLE_CLASS LoyaltyPointsInfoResponse;
class SOAP_REMOTABLE_CLASS MemberInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyMemberResponse2;
class SOAP_REMOTABLE_CLASS LoyaltyResponse2;
class SOAP_REMOTABLE_CLASS LoyaltyMemberListResponse2;
class SOAP_REMOTABLE_CLASS TransactionInfo2;
class SOAP_REMOTABLE_CLASS TierLevelInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyTierResponse2;
class SOAP_REMOTABLE_CLASS LoyaltyTierListResponse2;
class SOAP_REMOTABLE_CLASS PointsInfo2;
class SOAP_REMOTABLE_CLASS LoyaltyPointsInfoResponse2;

enum class LoyaltyResponseCode   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate"[GblSmpl] */
{
  Successful,
  AuthenticationFailed,
  CreateMemberFailed,
  UpdateMemberFailed,
  DeleteMemberFailed,
  GetMemberFailed,
  PostTransactionFailed,
  CreateTierFailed,
  UpdateTierFailed,
  DeleteTierFailed,
  MemberNotExist
};

class LoyaltyResponseCode_TypeInfoHolder : public TObject {
  LoyaltyResponseCode __instanceType;
public:
__published:
  __property LoyaltyResponseCode __propType = { read=__instanceType };
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
  bool            FSuccesful;
  bool            FSuccesful_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  }
  void __fastcall SetMessage(int Index, UnicodeString _prop_val)
  {  FMessage = _prop_val; FMessage_Specified = true;  }
  bool __fastcall Message_Specified(int Index)
  {  return FMessage_Specified;  }
  void __fastcall SetSuccesful(int Index, bool _prop_val)
  {  FSuccesful = _prop_val; FSuccesful_Specified = true;  }
  bool __fastcall Succesful_Specified(int Index)
  {  return FSuccesful_Specified;  }
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString    Message = { index=(IS_OPTN|IS_NLBL), read=FMessage, write=SetMessage, stored = Message_Specified };
  __property bool        Succesful = { index=(IS_OPTN), read=FSuccesful, write=SetSuccesful, stored = Succesful_Specified };
};




// ************************************************************************ //
// XML       : MMServiceResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO
// ************************************************************************ //
class MMServiceResponse2 : public MMServiceResponse {
private:
__published:
};




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
  __property __int64    HomeSiteId = { index=(IS_OPTN), read=FHomeSiteId, write=SetHomeSiteId, stored = HomeSiteId_Specified };
  __property bool       IsFirstVisitRewarded = { index=(IS_OPTN), read=FIsFirstVisitRewarded, write=SetIsFirstVisitRewarded, stored = IsFirstVisitRewarded_Specified };
  __property TXSDateTime* LastModified = { index=(IS_OPTN), read=FLastModified, write=SetLastModified, stored = LastModified_Specified };
  __property UnicodeString   LastName = { index=(IS_OPTN|IS_NLBL), read=FLastName, write=SetLastName, stored = LastName_Specified };
  __property TXSDateTime* LastVisitDate = { index=(IS_OPTN|IS_NLBL), read=FLastVisitDate, write=SetLastVisitDate, stored = LastVisitDate_Specified };
  __property double     LoadedPoints = { index=(IS_OPTN), read=FLoadedPoints, write=SetLoadedPoints, stored = LoadedPoints_Specified };
  __property UnicodeString MemberCardCode = { index=(IS_OPTN|IS_NLBL), read=FMemberCardCode, write=SetMemberCardCode, stored = MemberCardCode_Specified };
  __property int        MemberType = { index=(IS_OPTN), read=FMemberType, write=SetMemberType, stored = MemberType_Specified };
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


typedef DynamicArray<MemberInfo*> ArrayOfMemberInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate"[GblCplx] */


// ************************************************************************ //
// XML       : LoyaltyMemberListResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyMemberListResponse : public MMServiceResponse {
private:
  ArrayOfMemberInfo FMemberList;
  bool            FMemberList_Specified;
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetMemberList(int Index, ArrayOfMemberInfo _prop_val)
  {  FMemberList = _prop_val; FMemberList_Specified = true;  }
  bool __fastcall MemberList_Specified(int Index)
  {  return FMemberList_Specified;  }
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  }

public:
  __fastcall ~LoyaltyMemberListResponse();
__published:
  __property ArrayOfMemberInfo MemberList = { index=(IS_OPTN|IS_NLBL), read=FMemberList, write=SetMemberList, stored = MemberList_Specified };
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
};




// ************************************************************************ //
// XML       : TransactionInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class TransactionInfo : public TRemotable {
private:
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
  __property double     PointsDelta = { index=(IS_OPTN), read=FPointsDelta, write=SetPointsDelta, stored = PointsDelta_Specified };
  __property int        PointsType = { index=(IS_OPTN), read=FPointsType, write=SetPointsType, stored = PointsType_Specified };
  __property __int64      SiteCode = { index=(IS_OPTN), read=FSiteCode, write=SetSiteCode, stored = SiteCode_Specified };
  __property TXSDateTime* TransactionDate = { index=(IS_OPTN), read=FTransactionDate, write=SetTransactionDate, stored = TransactionDate_Specified };
  __property UnicodeString   UniqueId = { index=(IS_OPTN|IS_NLBL), read=FUniqueId, write=SetUniqueId, stored = UniqueId_Specified };
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
// XML       : LoyaltyTierResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyTierResponse : public MMServiceResponse {
private:
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  TierLevelInfo*  FTierInfo;
  bool            FTierInfo_Specified;
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  }
  void __fastcall SetTierInfo(int Index, TierLevelInfo* _prop_val)
  {  FTierInfo = _prop_val; FTierInfo_Specified = true;  }
  bool __fastcall TierInfo_Specified(int Index)
  {  return FTierInfo_Specified;  }

public:
  __fastcall ~LoyaltyTierResponse();
__published:
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
  __property TierLevelInfo*   TierInfo = { index=(IS_OPTN|IS_NLBL), read=FTierInfo, write=SetTierInfo, stored = TierInfo_Specified };
};


typedef DynamicArray<TierLevelInfo*> ArrayOfTierLevelInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate"[GblCplx] */


// ************************************************************************ //
// XML       : LoyaltyTierListResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyTierListResponse : public MMServiceResponse {
private:
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  ArrayOfTierLevelInfo FTierLevelList;
  bool            FTierLevelList_Specified;
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  }
  void __fastcall SetTierLevelList(int Index, ArrayOfTierLevelInfo _prop_val)
  {  FTierLevelList = _prop_val; FTierLevelList_Specified = true;  }
  bool __fastcall TierLevelList_Specified(int Index)
  {  return FTierLevelList_Specified;  }

public:
  __fastcall ~LoyaltyTierListResponse();
__published:
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
  __property ArrayOfTierLevelInfo TierLevelList = { index=(IS_OPTN|IS_NLBL), read=FTierLevelList, write=SetTierLevelList, stored = TierLevelList_Specified };
};




// ************************************************************************ //
// XML       : PointsInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class PointsInfo : public TRemotable {
private:
  double          FBalance;
  bool            FBalance_Specified;
  TXSDateTime*    FEndDate;
  bool            FEndDate_Specified;
  int             FPointsType;
  bool            FPointsType_Specified;
  TXSDateTime*    FStartDate;
  bool            FStartDate_Specified;
  UnicodeString   FUniqueId;
  bool            FUniqueId_Specified;
  void __fastcall SetBalance(int Index, double _prop_val)
  {  FBalance = _prop_val; FBalance_Specified = true;  }
  bool __fastcall Balance_Specified(int Index)
  {  return FBalance_Specified;  }
  void __fastcall SetEndDate(int Index, TXSDateTime* _prop_val)
  {  FEndDate = _prop_val; FEndDate_Specified = true;  }
  bool __fastcall EndDate_Specified(int Index)
  {  return FEndDate_Specified;  }
  void __fastcall SetPointsType(int Index, int _prop_val)
  {  FPointsType = _prop_val; FPointsType_Specified = true;  }
  bool __fastcall PointsType_Specified(int Index)
  {  return FPointsType_Specified;  }
  void __fastcall SetStartDate(int Index, TXSDateTime* _prop_val)
  {  FStartDate = _prop_val; FStartDate_Specified = true;  }
  bool __fastcall StartDate_Specified(int Index)
  {  return FStartDate_Specified;  }
  void __fastcall SetUniqueId(int Index, UnicodeString _prop_val)
  {  FUniqueId = _prop_val; FUniqueId_Specified = true;  }
  bool __fastcall UniqueId_Specified(int Index)
  {  return FUniqueId_Specified;  }

public:
  __fastcall ~PointsInfo();
__published:
  __property double        Balance = { index=(IS_OPTN), read=FBalance, write=SetBalance, stored = Balance_Specified };
  __property TXSDateTime*    EndDate = { index=(IS_OPTN), read=FEndDate, write=SetEndDate, stored = EndDate_Specified };
  __property int        PointsType = { index=(IS_OPTN), read=FPointsType, write=SetPointsType, stored = PointsType_Specified };
  __property TXSDateTime*  StartDate = { index=(IS_OPTN), read=FStartDate, write=SetStartDate, stored = StartDate_Specified };
  __property UnicodeString   UniqueId = { index=(IS_OPTN|IS_NLBL), read=FUniqueId, write=SetUniqueId, stored = UniqueId_Specified };
};




// ************************************************************************ //
// XML       : LoyaltyPointsInfoResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyPointsInfoResponse : public MMServiceResponse {
private:
  PointsInfo*     FPointsInfo;
  bool            FPointsInfo_Specified;
  LoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetPointsInfo(int Index, PointsInfo* _prop_val)
  {  FPointsInfo = _prop_val; FPointsInfo_Specified = true;  }
  bool __fastcall PointsInfo_Specified(int Index)
  {  return FPointsInfo_Specified;  }
  void __fastcall SetResponseCode(int Index, LoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  }

public:
  __fastcall ~LoyaltyPointsInfoResponse();
__published:
  __property PointsInfo* PointsInfo = { index=(IS_OPTN|IS_NLBL), read=FPointsInfo, write=SetPointsInfo, stored = PointsInfo_Specified };
  __property LoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
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
// XML       : LoyaltyMemberResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyMemberResponse2 : public LoyaltyMemberResponse {
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
// XML       : LoyaltyMemberListResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyMemberListResponse2 : public LoyaltyMemberListResponse {
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
// XML       : TierLevelInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class TierLevelInfo2 : public TierLevelInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyTierResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyTierResponse2 : public LoyaltyTierResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyTierListResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyTierListResponse2 : public LoyaltyTierListResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : PointsInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class PointsInfo2 : public PointsInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyPointsInfoResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate
// ************************************************************************ //
class LoyaltyPointsInfoResponse2 : public LoyaltyPointsInfoResponse {
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
  virtual LoyaltyResponse* DeleteMember(const UnicodeString inSyndicateCode, const UnicodeString inUniqueId) = 0;
  virtual LoyaltyMemberResponse* GetMemberByUniqueId(const UnicodeString inSyndicateCode, const UnicodeString inUniqueId) = 0;
  virtual LoyaltyMemberResponse* GetMemberByCardCode(const UnicodeString inSyndicateCode, const UnicodeString inMemberCode) = 0;
  virtual LoyaltyMemberResponse* GetMemberByEmail(const UnicodeString inSyndicateCode, const UnicodeString inMemberEmail) = 0;
  virtual LoyaltyMemberListResponse* GetMemberList(const UnicodeString inSyndicateCode) = 0;
  virtual LoyaltyResponse* PostTransaction(const UnicodeString inSyndicateCode, const TransactionInfo* transaction) = 0;
  virtual LoyaltyTierResponse* SaveTierLevel(const UnicodeString inSyndicateCode, const TierLevelInfo* inInfo) = 0;
  virtual LoyaltyResponse* DeleteTierLevel(const UnicodeString inSyndicateCode, const int tierLevelId) = 0;
  virtual LoyaltyTierResponse* GetTierLevel(const UnicodeString inSyndicateCode, const int tierId) = 0;
  virtual LoyaltyTierListResponse* GetAllTierLevel(const UnicodeString inSyndicateCode) = 0;
  virtual LoyaltyPointsInfoResponse* GetPointsInRange(const UnicodeString inSyndicateCode, const PointsInfo* inInfo) = 0;
  virtual LoyaltyResponse* UpdateMemberCardCode(const UnicodeString inSyndicateCode, const UnicodeString uniqueId, const UnicodeString memberCardCode) = 0;
};
typedef DelphiInterface<IWCFServiceLoyaltyMate> _di_IWCFServiceLoyaltyMate;

_di_IWCFServiceLoyaltyMate GetIWCFServiceLoyaltyMate(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__


#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_LoyaltyMateWSDL;
#endif



#endif // _H

