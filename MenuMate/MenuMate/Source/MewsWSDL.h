// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8747/MenumateServices/MewsService/?wsdl
//  >Import : http://localhost:8747/MenumateServices/MewsService/?wsdl>0
//  >Import : http://localhost:8747/MenumateServices/MewsService/?xsd=xsd0
//  >Import : http://localhost:8747/MenumateServices/MewsService/?xsd=xsd2
//  >Import : http://localhost:8747/MenumateServices/MewsService/?xsd=xsd3
//  >Import : http://localhost:8747/MenumateServices/MewsService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (17/09/2018 5:33:30 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   MewsWSDLH
#define   MewsWSDLH

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


namespace NS__MewsWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:dateTime        - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:anyType         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS BasicInquiry;
class SOAP_REMOTABLE_CLASS Extent;
class SOAP_REMOTABLE_CLASS Service;
class SOAP_REMOTABLE_CLASS Promotions;
class SOAP_REMOTABLE_CLASS AccountingCategory;
class SOAP_REMOTABLE_CLASS Outlet;
class SOAP_REMOTABLE_CLASS CustomerSearch;
class SOAP_REMOTABLE_CLASS Customer;
class SOAP_REMOTABLE_CLASS SpaceDetails;
class SOAP_REMOTABLE_CLASS SpaceCategory;
class SOAP_REMOTABLE_CLASS Space;
class SOAP_REMOTABLE_CLASS BasicInquiry2;
class SOAP_REMOTABLE_CLASS Extent2;
class SOAP_REMOTABLE_CLASS Service2;
class SOAP_REMOTABLE_CLASS Promotions2;
class SOAP_REMOTABLE_CLASS AccountingCategory2;
class SOAP_REMOTABLE_CLASS Outlet2;
class SOAP_REMOTABLE_CLASS CustomerSearch2;
class SOAP_REMOTABLE_CLASS Customer2;
class SOAP_REMOTABLE_CLASS SpaceDetails2;
class SOAP_REMOTABLE_CLASS SpaceCategory2;
class SOAP_REMOTABLE_CLASS Space2;

typedef DynamicArray<UnicodeString> ArrayOfstring;  /* "http://schemas.microsoft.com/2003/10/Serialization/Arrays"[GblCplx] */


// ************************************************************************ //
// XML       : BasicInquiry, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class BasicInquiry : public TRemotable {
private:
  UnicodeString   FAccessToken;
  bool            FAccessToken_Specified;
  UnicodeString   FClientToken;
  bool            FClientToken_Specified;
  Extent*         FExtent;
  bool            FExtent_Specified;
  void __fastcall SetAccessToken(int Index, UnicodeString _prop_val)
  {  FAccessToken = _prop_val; FAccessToken_Specified = true;  }
  bool __fastcall AccessToken_Specified(int Index)
  {  return FAccessToken_Specified;  } 
  void __fastcall SetClientToken(int Index, UnicodeString _prop_val)
  {  FClientToken = _prop_val; FClientToken_Specified = true;  }
  bool __fastcall ClientToken_Specified(int Index)
  {  return FClientToken_Specified;  } 
  void __fastcall SetExtent(int Index, Extent* _prop_val)
  {  FExtent = _prop_val; FExtent_Specified = true;  }
  bool __fastcall Extent_Specified(int Index)
  {  return FExtent_Specified;  } 

public:
  __fastcall ~BasicInquiry();
__published:
  __property UnicodeString AccessToken = { index=(IS_OPTN|IS_NLBL), read=FAccessToken, write=SetAccessToken, stored = AccessToken_Specified };
  __property UnicodeString ClientToken = { index=(IS_OPTN|IS_NLBL), read=FClientToken, write=SetClientToken, stored = ClientToken_Specified };
  __property Extent*        Extent = { index=(IS_OPTN|IS_NLBL), read=FExtent, write=SetExtent, stored = Extent_Specified };
};




// ************************************************************************ //
// XML       : Extent, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Extent : public TRemotable {
private:
  bool            FInactive;
  bool            FInactive_Specified;
  bool            FSpaceCategories;
  bool            FSpaceCategories_Specified;
  bool            FSpaceFeatures;
  bool            FSpaceFeatures_Specified;
  bool            FSpaces;
  bool            FSpaces_Specified;
  void __fastcall SetInactive(int Index, bool _prop_val)
  {  FInactive = _prop_val; FInactive_Specified = true;  }
  bool __fastcall Inactive_Specified(int Index)
  {  return FInactive_Specified;  } 
  void __fastcall SetSpaceCategories(int Index, bool _prop_val)
  {  FSpaceCategories = _prop_val; FSpaceCategories_Specified = true;  }
  bool __fastcall SpaceCategories_Specified(int Index)
  {  return FSpaceCategories_Specified;  } 
  void __fastcall SetSpaceFeatures(int Index, bool _prop_val)
  {  FSpaceFeatures = _prop_val; FSpaceFeatures_Specified = true;  }
  bool __fastcall SpaceFeatures_Specified(int Index)
  {  return FSpaceFeatures_Specified;  } 
  void __fastcall SetSpaces(int Index, bool _prop_val)
  {  FSpaces = _prop_val; FSpaces_Specified = true;  }
  bool __fastcall Spaces_Specified(int Index)
  {  return FSpaces_Specified;  } 
__published:
  __property bool         Inactive = { index=(IS_OPTN), read=FInactive, write=SetInactive, stored = Inactive_Specified };
  __property bool       SpaceCategories = { index=(IS_OPTN), read=FSpaceCategories, write=SetSpaceCategories, stored = SpaceCategories_Specified };
  __property bool       SpaceFeatures = { index=(IS_OPTN), read=FSpaceFeatures, write=SetSpaceFeatures, stored = SpaceFeatures_Specified };
  __property bool           Spaces = { index=(IS_OPTN), read=FSpaces, write=SetSpaces, stored = Spaces_Specified };
};


typedef DynamicArray<Service*>    ArrayOfService; /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : Service, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Service : public TRemotable {
private:
  UnicodeString   FEndTime;
  bool            FEndTime_Specified;
  UnicodeString   FId;
  bool            FId_Specified;
  bool            FIsActive;
  bool            FIsActive_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  Promotions*     FPromotions;
  bool            FPromotions_Specified;
  UnicodeString   FStartTime;
  bool            FStartTime_Specified;
  void __fastcall SetEndTime(int Index, UnicodeString _prop_val)
  {  FEndTime = _prop_val; FEndTime_Specified = true;  }
  bool __fastcall EndTime_Specified(int Index)
  {  return FEndTime_Specified;  } 
  void __fastcall SetId(int Index, UnicodeString _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  void __fastcall SetIsActive(int Index, bool _prop_val)
  {  FIsActive = _prop_val; FIsActive_Specified = true;  }
  bool __fastcall IsActive_Specified(int Index)
  {  return FIsActive_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetPromotions(int Index, Promotions* _prop_val)
  {  FPromotions = _prop_val; FPromotions_Specified = true;  }
  bool __fastcall Promotions_Specified(int Index)
  {  return FPromotions_Specified;  } 
  void __fastcall SetStartTime(int Index, UnicodeString _prop_val)
  {  FStartTime = _prop_val; FStartTime_Specified = true;  }
  bool __fastcall StartTime_Specified(int Index)
  {  return FStartTime_Specified;  } 

public:
  __fastcall ~Service();
__published:
  __property UnicodeString    EndTime = { index=(IS_OPTN|IS_NLBL), read=FEndTime, write=SetEndTime, stored = EndTime_Specified };
  __property UnicodeString         Id = { index=(IS_OPTN|IS_NLBL), read=FId, write=SetId, stored = Id_Specified };
  __property bool         IsActive = { index=(IS_OPTN), read=FIsActive, write=SetIsActive, stored = IsActive_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property Promotions* Promotions = { index=(IS_OPTN|IS_NLBL), read=FPromotions, write=SetPromotions, stored = Promotions_Specified };
  __property UnicodeString  StartTime = { index=(IS_OPTN|IS_NLBL), read=FStartTime, write=SetStartTime, stored = StartTime_Specified };
};




// ************************************************************************ //
// XML       : Promotions, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Promotions : public TRemotable {
private:
  bool            FAfterCheckIn;
  bool            FAfterCheckIn_Specified;
  bool            FAfterCheckOut;
  bool            FAfterCheckOut_Specified;
  bool            FBeforeCheckIn;
  bool            FBeforeCheckIn_Specified;
  bool            FBeforeCheckOut;
  bool            FBeforeCheckOut_Specified;
  bool            FDuringStay;
  bool            FDuringStay_Specified;
  void __fastcall SetAfterCheckIn(int Index, bool _prop_val)
  {  FAfterCheckIn = _prop_val; FAfterCheckIn_Specified = true;  }
  bool __fastcall AfterCheckIn_Specified(int Index)
  {  return FAfterCheckIn_Specified;  } 
  void __fastcall SetAfterCheckOut(int Index, bool _prop_val)
  {  FAfterCheckOut = _prop_val; FAfterCheckOut_Specified = true;  }
  bool __fastcall AfterCheckOut_Specified(int Index)
  {  return FAfterCheckOut_Specified;  } 
  void __fastcall SetBeforeCheckIn(int Index, bool _prop_val)
  {  FBeforeCheckIn = _prop_val; FBeforeCheckIn_Specified = true;  }
  bool __fastcall BeforeCheckIn_Specified(int Index)
  {  return FBeforeCheckIn_Specified;  } 
  void __fastcall SetBeforeCheckOut(int Index, bool _prop_val)
  {  FBeforeCheckOut = _prop_val; FBeforeCheckOut_Specified = true;  }
  bool __fastcall BeforeCheckOut_Specified(int Index)
  {  return FBeforeCheckOut_Specified;  } 
  void __fastcall SetDuringStay(int Index, bool _prop_val)
  {  FDuringStay = _prop_val; FDuringStay_Specified = true;  }
  bool __fastcall DuringStay_Specified(int Index)
  {  return FDuringStay_Specified;  } 
__published:
  __property bool       AfterCheckIn = { index=(IS_OPTN), read=FAfterCheckIn, write=SetAfterCheckIn, stored = AfterCheckIn_Specified };
  __property bool       AfterCheckOut = { index=(IS_OPTN), read=FAfterCheckOut, write=SetAfterCheckOut, stored = AfterCheckOut_Specified };
  __property bool       BeforeCheckIn = { index=(IS_OPTN), read=FBeforeCheckIn, write=SetBeforeCheckIn, stored = BeforeCheckIn_Specified };
  __property bool       BeforeCheckOut = { index=(IS_OPTN), read=FBeforeCheckOut, write=SetBeforeCheckOut, stored = BeforeCheckOut_Specified };
  __property bool       DuringStay = { index=(IS_OPTN), read=FDuringStay, write=SetDuringStay, stored = DuringStay_Specified };
};


typedef DynamicArray<AccountingCategory*> ArrayOfAccountingCategory; /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : AccountingCategory, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class AccountingCategory : public TRemotable {
private:
  UnicodeString   FClassification;
  bool            FClassification_Specified;
  UnicodeString   FCode;
  bool            FCode_Specified;
  UnicodeString   FCostCenterCode;
  bool            FCostCenterCode_Specified;
  UnicodeString   FExternalCode;
  bool            FExternalCode_Specified;
  UnicodeString   FId;
  bool            FId_Specified;
  bool            FIsActive;
  bool            FIsActive_Specified;
  UnicodeString   FLedgerAccountCode;
  bool            FLedgerAccountCode_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  UnicodeString   FPostingAccountCode;
  bool            FPostingAccountCode_Specified;
  void __fastcall SetClassification(int Index, UnicodeString _prop_val)
  {  FClassification = _prop_val; FClassification_Specified = true;  }
  bool __fastcall Classification_Specified(int Index)
  {  return FClassification_Specified;  } 
  void __fastcall SetCode(int Index, UnicodeString _prop_val)
  {  FCode = _prop_val; FCode_Specified = true;  }
  bool __fastcall Code_Specified(int Index)
  {  return FCode_Specified;  } 
  void __fastcall SetCostCenterCode(int Index, UnicodeString _prop_val)
  {  FCostCenterCode = _prop_val; FCostCenterCode_Specified = true;  }
  bool __fastcall CostCenterCode_Specified(int Index)
  {  return FCostCenterCode_Specified;  } 
  void __fastcall SetExternalCode(int Index, UnicodeString _prop_val)
  {  FExternalCode = _prop_val; FExternalCode_Specified = true;  }
  bool __fastcall ExternalCode_Specified(int Index)
  {  return FExternalCode_Specified;  } 
  void __fastcall SetId(int Index, UnicodeString _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  void __fastcall SetIsActive(int Index, bool _prop_val)
  {  FIsActive = _prop_val; FIsActive_Specified = true;  }
  bool __fastcall IsActive_Specified(int Index)
  {  return FIsActive_Specified;  } 
  void __fastcall SetLedgerAccountCode(int Index, UnicodeString _prop_val)
  {  FLedgerAccountCode = _prop_val; FLedgerAccountCode_Specified = true;  }
  bool __fastcall LedgerAccountCode_Specified(int Index)
  {  return FLedgerAccountCode_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetPostingAccountCode(int Index, UnicodeString _prop_val)
  {  FPostingAccountCode = _prop_val; FPostingAccountCode_Specified = true;  }
  bool __fastcall PostingAccountCode_Specified(int Index)
  {  return FPostingAccountCode_Specified;  } 
__published:
  __property UnicodeString Classification = { index=(IS_OPTN|IS_NLBL), read=FClassification, write=SetClassification, stored = Classification_Specified };
  __property UnicodeString       Code = { index=(IS_OPTN|IS_NLBL), read=FCode, write=SetCode, stored = Code_Specified };
  __property UnicodeString CostCenterCode = { index=(IS_OPTN|IS_NLBL), read=FCostCenterCode, write=SetCostCenterCode, stored = CostCenterCode_Specified };
  __property UnicodeString ExternalCode = { index=(IS_OPTN|IS_NLBL), read=FExternalCode, write=SetExternalCode, stored = ExternalCode_Specified };
  __property UnicodeString         Id = { index=(IS_OPTN|IS_NLBL), read=FId, write=SetId, stored = Id_Specified };
  __property bool         IsActive = { index=(IS_OPTN), read=FIsActive, write=SetIsActive, stored = IsActive_Specified };
  __property UnicodeString LedgerAccountCode = { index=(IS_OPTN|IS_NLBL), read=FLedgerAccountCode, write=SetLedgerAccountCode, stored = LedgerAccountCode_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property UnicodeString PostingAccountCode = { index=(IS_OPTN|IS_NLBL), read=FPostingAccountCode, write=SetPostingAccountCode, stored = PostingAccountCode_Specified };
};


typedef DynamicArray<Outlet*>     ArrayOfOutlet;  /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : Outlet, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Outlet : public TRemotable {
private:
  UnicodeString   FId;
  bool            FId_Specified;
  bool            FIsActive;
  bool            FIsActive_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  void __fastcall SetId(int Index, UnicodeString _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  void __fastcall SetIsActive(int Index, bool _prop_val)
  {  FIsActive = _prop_val; FIsActive_Specified = true;  }
  bool __fastcall IsActive_Specified(int Index)
  {  return FIsActive_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
__published:
  __property UnicodeString         Id = { index=(IS_OPTN|IS_NLBL), read=FId, write=SetId, stored = Id_Specified };
  __property bool         IsActive = { index=(IS_OPTN), read=FIsActive, write=SetIsActive, stored = IsActive_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
};




// ************************************************************************ //
// XML       : CustomerSearch, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class CustomerSearch : public TRemotable {
private:
  UnicodeString   FAccessToken;
  bool            FAccessToken_Specified;
  UnicodeString   FClientToken;
  bool            FClientToken_Specified;
  UnicodeString   FFirstName;
  bool            FFirstName_Specified;
  UnicodeString   FLastName;
  bool            FLastName_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  UnicodeString   FSpaceId;
  bool            FSpaceId_Specified;
  void __fastcall SetAccessToken(int Index, UnicodeString _prop_val)
  {  FAccessToken = _prop_val; FAccessToken_Specified = true;  }
  bool __fastcall AccessToken_Specified(int Index)
  {  return FAccessToken_Specified;  } 
  void __fastcall SetClientToken(int Index, UnicodeString _prop_val)
  {  FClientToken = _prop_val; FClientToken_Specified = true;  }
  bool __fastcall ClientToken_Specified(int Index)
  {  return FClientToken_Specified;  } 
  void __fastcall SetFirstName(int Index, UnicodeString _prop_val)
  {  FFirstName = _prop_val; FFirstName_Specified = true;  }
  bool __fastcall FirstName_Specified(int Index)
  {  return FFirstName_Specified;  } 
  void __fastcall SetLastName(int Index, UnicodeString _prop_val)
  {  FLastName = _prop_val; FLastName_Specified = true;  }
  bool __fastcall LastName_Specified(int Index)
  {  return FLastName_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetSpaceId(int Index, UnicodeString _prop_val)
  {  FSpaceId = _prop_val; FSpaceId_Specified = true;  }
  bool __fastcall SpaceId_Specified(int Index)
  {  return FSpaceId_Specified;  } 
__published:
  __property UnicodeString AccessToken = { index=(IS_OPTN|IS_NLBL), read=FAccessToken, write=SetAccessToken, stored = AccessToken_Specified };
  __property UnicodeString ClientToken = { index=(IS_OPTN|IS_NLBL), read=FClientToken, write=SetClientToken, stored = ClientToken_Specified };
  __property UnicodeString  FirstName = { index=(IS_OPTN|IS_NLBL), read=FFirstName, write=SetFirstName, stored = FirstName_Specified };
  __property UnicodeString   LastName = { index=(IS_OPTN|IS_NLBL), read=FLastName, write=SetLastName, stored = LastName_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property UnicodeString    SpaceId = { index=(IS_OPTN|IS_NLBL), read=FSpaceId, write=SetSpaceId, stored = SpaceId_Specified };
};


typedef DynamicArray<Customer*>   ArrayOfCustomer; /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : Customer, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Customer : public TRemotable {
private:
  ArrayOfstring   FAddress;
  bool            FAddress_Specified;
  TXSDateTime*    FBirthDate;
  bool            FBirthDate_Specified;
  UnicodeString   FBirthPlace;
  bool            FBirthPlace_Specified;
  Variant         FCategoryId;
  bool            FCategoryId_Specified;
  ArrayOfstring   FClassifications;
  bool            FClassifications_Specified;
  TXSDateTime*    FCreatedUtc;
  bool            FCreatedUtc_Specified;
  UnicodeString   FEmail;
  bool            FEmail_Specified;
  UnicodeString   FFirstName;
  bool            FFirstName_Specified;
  UnicodeString   FGender;
  bool            FGender_Specified;
  UnicodeString   FId;
  bool            FId_Specified;
  UnicodeString   FLanguageCode;
  bool            FLanguageCode_Specified;
  UnicodeString   FLastName;
  bool            FLastName_Specified;
  UnicodeString   FLoyaltyCode;
  bool            FLoyaltyCode_Specified;
  UnicodeString   FNationalityCode;
  bool            FNationalityCode_Specified;
  UnicodeString   FNotes;
  bool            FNotes_Specified;
  UnicodeString   FNumber;
  bool            FNumber_Specified;
  UnicodeString   FPhone;
  bool            FPhone_Specified;
  Variant         FSecondLastName;
  bool            FSecondLastName_Specified;
  UnicodeString   FTaxIdentificationNumber;
  bool            FTaxIdentificationNumber_Specified;
  UnicodeString   FTitle;
  bool            FTitle_Specified;
  TXSDateTime*    FUpdatedUtc;
  bool            FUpdatedUtc_Specified;
  void __fastcall SetAddress(int Index, ArrayOfstring _prop_val)
  {  FAddress = _prop_val; FAddress_Specified = true;  }
  bool __fastcall Address_Specified(int Index)
  {  return FAddress_Specified;  } 
  void __fastcall SetBirthDate(int Index, TXSDateTime* _prop_val)
  {  FBirthDate = _prop_val; FBirthDate_Specified = true;  }
  bool __fastcall BirthDate_Specified(int Index)
  {  return FBirthDate_Specified;  } 
  void __fastcall SetBirthPlace(int Index, UnicodeString _prop_val)
  {  FBirthPlace = _prop_val; FBirthPlace_Specified = true;  }
  bool __fastcall BirthPlace_Specified(int Index)
  {  return FBirthPlace_Specified;  } 
  void __fastcall SetCategoryId(int Index, Variant _prop_val)
  {  FCategoryId = _prop_val; FCategoryId_Specified = true;  }
  bool __fastcall CategoryId_Specified(int Index)
  {  return FCategoryId_Specified;  } 
  void __fastcall SetClassifications(int Index, ArrayOfstring _prop_val)
  {  FClassifications = _prop_val; FClassifications_Specified = true;  }
  bool __fastcall Classifications_Specified(int Index)
  {  return FClassifications_Specified;  } 
  void __fastcall SetCreatedUtc(int Index, TXSDateTime* _prop_val)
  {  FCreatedUtc = _prop_val; FCreatedUtc_Specified = true;  }
  bool __fastcall CreatedUtc_Specified(int Index)
  {  return FCreatedUtc_Specified;  } 
  void __fastcall SetEmail(int Index, UnicodeString _prop_val)
  {  FEmail = _prop_val; FEmail_Specified = true;  }
  bool __fastcall Email_Specified(int Index)
  {  return FEmail_Specified;  } 
  void __fastcall SetFirstName(int Index, UnicodeString _prop_val)
  {  FFirstName = _prop_val; FFirstName_Specified = true;  }
  bool __fastcall FirstName_Specified(int Index)
  {  return FFirstName_Specified;  } 
  void __fastcall SetGender(int Index, UnicodeString _prop_val)
  {  FGender = _prop_val; FGender_Specified = true;  }
  bool __fastcall Gender_Specified(int Index)
  {  return FGender_Specified;  } 
  void __fastcall SetId(int Index, UnicodeString _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  void __fastcall SetLanguageCode(int Index, UnicodeString _prop_val)
  {  FLanguageCode = _prop_val; FLanguageCode_Specified = true;  }
  bool __fastcall LanguageCode_Specified(int Index)
  {  return FLanguageCode_Specified;  } 
  void __fastcall SetLastName(int Index, UnicodeString _prop_val)
  {  FLastName = _prop_val; FLastName_Specified = true;  }
  bool __fastcall LastName_Specified(int Index)
  {  return FLastName_Specified;  } 
  void __fastcall SetLoyaltyCode(int Index, UnicodeString _prop_val)
  {  FLoyaltyCode = _prop_val; FLoyaltyCode_Specified = true;  }
  bool __fastcall LoyaltyCode_Specified(int Index)
  {  return FLoyaltyCode_Specified;  } 
  void __fastcall SetNationalityCode(int Index, UnicodeString _prop_val)
  {  FNationalityCode = _prop_val; FNationalityCode_Specified = true;  }
  bool __fastcall NationalityCode_Specified(int Index)
  {  return FNationalityCode_Specified;  } 
  void __fastcall SetNotes(int Index, UnicodeString _prop_val)
  {  FNotes = _prop_val; FNotes_Specified = true;  }
  bool __fastcall Notes_Specified(int Index)
  {  return FNotes_Specified;  } 
  void __fastcall SetNumber(int Index, UnicodeString _prop_val)
  {  FNumber = _prop_val; FNumber_Specified = true;  }
  bool __fastcall Number_Specified(int Index)
  {  return FNumber_Specified;  } 
  void __fastcall SetPhone(int Index, UnicodeString _prop_val)
  {  FPhone = _prop_val; FPhone_Specified = true;  }
  bool __fastcall Phone_Specified(int Index)
  {  return FPhone_Specified;  } 
  void __fastcall SetSecondLastName(int Index, Variant _prop_val)
  {  FSecondLastName = _prop_val; FSecondLastName_Specified = true;  }
  bool __fastcall SecondLastName_Specified(int Index)
  {  return FSecondLastName_Specified;  } 
  void __fastcall SetTaxIdentificationNumber(int Index, UnicodeString _prop_val)
  {  FTaxIdentificationNumber = _prop_val; FTaxIdentificationNumber_Specified = true;  }
  bool __fastcall TaxIdentificationNumber_Specified(int Index)
  {  return FTaxIdentificationNumber_Specified;  } 
  void __fastcall SetTitle(int Index, UnicodeString _prop_val)
  {  FTitle = _prop_val; FTitle_Specified = true;  }
  bool __fastcall Title_Specified(int Index)
  {  return FTitle_Specified;  } 
  void __fastcall SetUpdatedUtc(int Index, TXSDateTime* _prop_val)
  {  FUpdatedUtc = _prop_val; FUpdatedUtc_Specified = true;  }
  bool __fastcall UpdatedUtc_Specified(int Index)
  {  return FUpdatedUtc_Specified;  } 

public:
  __fastcall ~Customer();
__published:
  __property ArrayOfstring    Address = { index=(IS_OPTN|IS_NLBL), read=FAddress, write=SetAddress, stored = Address_Specified };
  __property TXSDateTime*  BirthDate = { index=(IS_OPTN), read=FBirthDate, write=SetBirthDate, stored = BirthDate_Specified };
  __property UnicodeString BirthPlace = { index=(IS_OPTN|IS_NLBL), read=FBirthPlace, write=SetBirthPlace, stored = BirthPlace_Specified };
  __property Variant    CategoryId = { index=(IS_OPTN|IS_NLBL), read=FCategoryId, write=SetCategoryId, stored = CategoryId_Specified };
  __property ArrayOfstring Classifications = { index=(IS_OPTN|IS_NLBL), read=FClassifications, write=SetClassifications, stored = Classifications_Specified };
  __property TXSDateTime* CreatedUtc = { index=(IS_OPTN), read=FCreatedUtc, write=SetCreatedUtc, stored = CreatedUtc_Specified };
  __property UnicodeString      Email = { index=(IS_OPTN|IS_NLBL), read=FEmail, write=SetEmail, stored = Email_Specified };
  __property UnicodeString  FirstName = { index=(IS_OPTN|IS_NLBL), read=FFirstName, write=SetFirstName, stored = FirstName_Specified };
  __property UnicodeString     Gender = { index=(IS_OPTN|IS_NLBL), read=FGender, write=SetGender, stored = Gender_Specified };
  __property UnicodeString         Id = { index=(IS_OPTN|IS_NLBL), read=FId, write=SetId, stored = Id_Specified };
  __property UnicodeString LanguageCode = { index=(IS_OPTN|IS_NLBL), read=FLanguageCode, write=SetLanguageCode, stored = LanguageCode_Specified };
  __property UnicodeString   LastName = { index=(IS_OPTN|IS_NLBL), read=FLastName, write=SetLastName, stored = LastName_Specified };
  __property UnicodeString LoyaltyCode = { index=(IS_OPTN|IS_NLBL), read=FLoyaltyCode, write=SetLoyaltyCode, stored = LoyaltyCode_Specified };
  __property UnicodeString NationalityCode = { index=(IS_OPTN|IS_NLBL), read=FNationalityCode, write=SetNationalityCode, stored = NationalityCode_Specified };
  __property UnicodeString      Notes = { index=(IS_OPTN|IS_NLBL), read=FNotes, write=SetNotes, stored = Notes_Specified };
  __property UnicodeString     Number = { index=(IS_OPTN|IS_NLBL), read=FNumber, write=SetNumber, stored = Number_Specified };
  __property UnicodeString      Phone = { index=(IS_OPTN|IS_NLBL), read=FPhone, write=SetPhone, stored = Phone_Specified };
  __property Variant    SecondLastName = { index=(IS_OPTN|IS_NLBL), read=FSecondLastName, write=SetSecondLastName, stored = SecondLastName_Specified };
  __property UnicodeString TaxIdentificationNumber = { index=(IS_OPTN|IS_NLBL), read=FTaxIdentificationNumber, write=SetTaxIdentificationNumber, stored = TaxIdentificationNumber_Specified };
  __property UnicodeString      Title = { index=(IS_OPTN|IS_NLBL), read=FTitle, write=SetTitle, stored = Title_Specified };
  __property TXSDateTime* UpdatedUtc = { index=(IS_OPTN), read=FUpdatedUtc, write=SetUpdatedUtc, stored = UpdatedUtc_Specified };
};


typedef DynamicArray<SpaceCategory*> ArrayOfSpaceCategory; /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */
typedef DynamicArray<Space*>      ArrayOfSpace;   /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : SpaceDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class SpaceDetails : public TRemotable {
private:
  ArrayOfSpaceCategory FSpaceCategories;
  bool            FSpaceCategories_Specified;
  ArrayOfSpace    FSpaces;
  bool            FSpaces_Specified;
  void __fastcall SetSpaceCategories(int Index, ArrayOfSpaceCategory _prop_val)
  {  FSpaceCategories = _prop_val; FSpaceCategories_Specified = true;  }
  bool __fastcall SpaceCategories_Specified(int Index)
  {  return FSpaceCategories_Specified;  } 
  void __fastcall SetSpaces(int Index, ArrayOfSpace _prop_val)
  {  FSpaces = _prop_val; FSpaces_Specified = true;  }
  bool __fastcall Spaces_Specified(int Index)
  {  return FSpaces_Specified;  } 

public:
  __fastcall ~SpaceDetails();
__published:
  __property ArrayOfSpaceCategory SpaceCategories = { index=(IS_OPTN|IS_NLBL), read=FSpaceCategories, write=SetSpaceCategories, stored = SpaceCategories_Specified };
  __property ArrayOfSpace     Spaces = { index=(IS_OPTN|IS_NLBL), read=FSpaces, write=SetSpaces, stored = Spaces_Specified };
};




// ************************************************************************ //
// XML       : SpaceCategory, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class SpaceCategory : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  int             FExtraUnitCount;
  bool            FExtraUnitCount_Specified;
  UnicodeString   FId;
  bool            FId_Specified;
  bool            FIsActive;
  bool            FIsActive_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  int             FOrdering;
  bool            FOrdering_Specified;
  UnicodeString   FShortName;
  bool            FShortName_Specified;
  int             FUnitCount;
  bool            FUnitCount_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetExtraUnitCount(int Index, int _prop_val)
  {  FExtraUnitCount = _prop_val; FExtraUnitCount_Specified = true;  }
  bool __fastcall ExtraUnitCount_Specified(int Index)
  {  return FExtraUnitCount_Specified;  } 
  void __fastcall SetId(int Index, UnicodeString _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  void __fastcall SetIsActive(int Index, bool _prop_val)
  {  FIsActive = _prop_val; FIsActive_Specified = true;  }
  bool __fastcall IsActive_Specified(int Index)
  {  return FIsActive_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOrdering(int Index, int _prop_val)
  {  FOrdering = _prop_val; FOrdering_Specified = true;  }
  bool __fastcall Ordering_Specified(int Index)
  {  return FOrdering_Specified;  } 
  void __fastcall SetShortName(int Index, UnicodeString _prop_val)
  {  FShortName = _prop_val; FShortName_Specified = true;  }
  bool __fastcall ShortName_Specified(int Index)
  {  return FShortName_Specified;  } 
  void __fastcall SetUnitCount(int Index, int _prop_val)
  {  FUnitCount = _prop_val; FUnitCount_Specified = true;  }
  bool __fastcall UnitCount_Specified(int Index)
  {  return FUnitCount_Specified;  } 
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property int        ExtraUnitCount = { index=(IS_OPTN), read=FExtraUnitCount, write=SetExtraUnitCount, stored = ExtraUnitCount_Specified };
  __property UnicodeString         Id = { index=(IS_OPTN|IS_NLBL), read=FId, write=SetId, stored = Id_Specified };
  __property bool         IsActive = { index=(IS_OPTN), read=FIsActive, write=SetIsActive, stored = IsActive_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property int          Ordering = { index=(IS_OPTN), read=FOrdering, write=SetOrdering, stored = Ordering_Specified };
  __property UnicodeString  ShortName = { index=(IS_OPTN|IS_NLBL), read=FShortName, write=SetShortName, stored = ShortName_Specified };
  __property int         UnitCount = { index=(IS_OPTN), read=FUnitCount, write=SetUnitCount, stored = UnitCount_Specified };
};




// ************************************************************************ //
// XML       : Space, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Space : public TRemotable {
private:
  UnicodeString   FBuildingNumber;
  bool            FBuildingNumber_Specified;
  UnicodeString   FCategoryId;
  bool            FCategoryId_Specified;
  UnicodeString   FFloorNumber;
  bool            FFloorNumber_Specified;
  UnicodeString   FId;
  bool            FId_Specified;
  bool            FIsActive;
  bool            FIsActive_Specified;
  UnicodeString   FNumber;
  bool            FNumber_Specified;
  UnicodeString   FParentSpaceId;
  bool            FParentSpaceId_Specified;
  UnicodeString   FState;
  bool            FState_Specified;
  UnicodeString   FType;
  bool            FType_Specified;
  void __fastcall SetBuildingNumber(int Index, UnicodeString _prop_val)
  {  FBuildingNumber = _prop_val; FBuildingNumber_Specified = true;  }
  bool __fastcall BuildingNumber_Specified(int Index)
  {  return FBuildingNumber_Specified;  } 
  void __fastcall SetCategoryId(int Index, UnicodeString _prop_val)
  {  FCategoryId = _prop_val; FCategoryId_Specified = true;  }
  bool __fastcall CategoryId_Specified(int Index)
  {  return FCategoryId_Specified;  } 
  void __fastcall SetFloorNumber(int Index, UnicodeString _prop_val)
  {  FFloorNumber = _prop_val; FFloorNumber_Specified = true;  }
  bool __fastcall FloorNumber_Specified(int Index)
  {  return FFloorNumber_Specified;  } 
  void __fastcall SetId(int Index, UnicodeString _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  void __fastcall SetIsActive(int Index, bool _prop_val)
  {  FIsActive = _prop_val; FIsActive_Specified = true;  }
  bool __fastcall IsActive_Specified(int Index)
  {  return FIsActive_Specified;  } 
  void __fastcall SetNumber(int Index, UnicodeString _prop_val)
  {  FNumber = _prop_val; FNumber_Specified = true;  }
  bool __fastcall Number_Specified(int Index)
  {  return FNumber_Specified;  } 
  void __fastcall SetParentSpaceId(int Index, UnicodeString _prop_val)
  {  FParentSpaceId = _prop_val; FParentSpaceId_Specified = true;  }
  bool __fastcall ParentSpaceId_Specified(int Index)
  {  return FParentSpaceId_Specified;  } 
  void __fastcall SetState(int Index, UnicodeString _prop_val)
  {  FState = _prop_val; FState_Specified = true;  }
  bool __fastcall State_Specified(int Index)
  {  return FState_Specified;  } 
  void __fastcall SetType(int Index, UnicodeString _prop_val)
  {  FType = _prop_val; FType_Specified = true;  }
  bool __fastcall Type_Specified(int Index)
  {  return FType_Specified;  } 
__published:
  __property UnicodeString BuildingNumber = { index=(IS_OPTN|IS_NLBL), read=FBuildingNumber, write=SetBuildingNumber, stored = BuildingNumber_Specified };
  __property UnicodeString CategoryId = { index=(IS_OPTN|IS_NLBL), read=FCategoryId, write=SetCategoryId, stored = CategoryId_Specified };
  __property UnicodeString FloorNumber = { index=(IS_OPTN|IS_NLBL), read=FFloorNumber, write=SetFloorNumber, stored = FloorNumber_Specified };
  __property UnicodeString         Id = { index=(IS_OPTN|IS_NLBL), read=FId, write=SetId, stored = Id_Specified };
  __property bool         IsActive = { index=(IS_OPTN), read=FIsActive, write=SetIsActive, stored = IsActive_Specified };
  __property UnicodeString     Number = { index=(IS_OPTN|IS_NLBL), read=FNumber, write=SetNumber, stored = Number_Specified };
  __property UnicodeString ParentSpaceId = { index=(IS_OPTN|IS_NLBL), read=FParentSpaceId, write=SetParentSpaceId, stored = ParentSpaceId_Specified };
  __property UnicodeString      State = { index=(IS_OPTN|IS_NLBL), read=FState, write=SetState, stored = State_Specified };
  __property UnicodeString       Type = { index=(IS_OPTN|IS_NLBL), read=FType, write=SetType, stored = Type_Specified };
};




// ************************************************************************ //
// XML       : BasicInquiry, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class BasicInquiry2 : public BasicInquiry {
private:
__published:
};




// ************************************************************************ //
// XML       : Extent, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Extent2 : public Extent {
private:
__published:
};




// ************************************************************************ //
// XML       : Service, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Service2 : public Service {
private:
__published:
};




// ************************************************************************ //
// XML       : Promotions, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Promotions2 : public Promotions {
private:
__published:
};




// ************************************************************************ //
// XML       : AccountingCategory, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class AccountingCategory2 : public AccountingCategory {
private:
__published:
};




// ************************************************************************ //
// XML       : Outlet, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Outlet2 : public Outlet {
private:
__published:
};




// ************************************************************************ //
// XML       : CustomerSearch, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class CustomerSearch2 : public CustomerSearch {
private:
__published:
};




// ************************************************************************ //
// XML       : Customer, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Customer2 : public Customer {
private:
__published:
};




// ************************************************************************ //
// XML       : SpaceDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class SpaceDetails2 : public SpaceDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : SpaceCategory, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class SpaceCategory2 : public SpaceCategory {
private:
__published:
};




// ************************************************************************ //
// XML       : Space, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Space2 : public Space {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IMewsIntegrationWebService/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceMews
// service   : MewsIntegrationWebService
// port      : basicHttpBinding_ServiceMews
// URL       : http://localhost:8747/MenumateServices/MewsService/
// ************************************************************************ //
__interface INTERFACE_UUID("{D5F1E5D9-ED23-DBA0-5241-1DF75DF958C5}") IMewsIntegrationWebService : public IInvokable
{
public:
  virtual ArrayOfService  GetMewsServices(const UnicodeString platformAddress, const BasicInquiry* basicInquiry) = 0; 
  virtual ArrayOfAccountingCategory GetMewsAccountingCategories(const UnicodeString platformAddress, const BasicInquiry* basicInquiry) = 0; 
  virtual ArrayOfOutlet   GetMewsOutlets(const UnicodeString platformAddress, const BasicInquiry* basicInquiry) = 0; 
  virtual ArrayOfCustomer SearchCustomers(const UnicodeString platformAddress, const CustomerSearch* customerSearch) = 0; 
  virtual SpaceDetails*   GetSpaceIds(const UnicodeString platformAddress, const BasicInquiry* customerSearch) = 0; 
};
typedef DelphiInterface<IMewsIntegrationWebService> _di_IMewsIntegrationWebService;

_di_IMewsIntegrationWebService GetIMewsIntegrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__MewsWSDL;
#endif



#endif // _H
