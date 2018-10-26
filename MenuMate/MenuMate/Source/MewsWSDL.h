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
// (26/10/2018 6:11:02 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   MewsWSDLH
#define   MewsWSDLH

#include <System.hpp>
#include <InvokeRegistry.hpp>
#include <XSBuiltIns.hpp>
#include "winsock2.h"
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
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS BasicInquiry;
class SOAP_REMOTABLE_CLASS Extent;
class SOAP_REMOTABLE_CLASS Service;
class SOAP_REMOTABLE_CLASS AccountingCategory;
class SOAP_REMOTABLE_CLASS Outlet;
class SOAP_REMOTABLE_CLASS CustomerSearch;
class SOAP_REMOTABLE_CLASS CustomerDetailsMews;
class SOAP_REMOTABLE_CLASS SpaceDetails;
class SOAP_REMOTABLE_CLASS Space;
class SOAP_REMOTABLE_CLASS Order;
class SOAP_REMOTABLE_CLASS Bill;
class SOAP_REMOTABLE_CLASS Category;
class SOAP_REMOTABLE_CLASS Item;
class SOAP_REMOTABLE_CLASS UnitCost;
class SOAP_REMOTABLE_CLASS BasicInquiry2;
class SOAP_REMOTABLE_CLASS Extent2;
class SOAP_REMOTABLE_CLASS Service2;
class SOAP_REMOTABLE_CLASS AccountingCategory2;
class SOAP_REMOTABLE_CLASS Outlet2;
class SOAP_REMOTABLE_CLASS CustomerSearch2;
class SOAP_REMOTABLE_CLASS CustomerDetailsMews2;
class SOAP_REMOTABLE_CLASS SpaceDetails2;
class SOAP_REMOTABLE_CLASS Space2;
class SOAP_REMOTABLE_CLASS Order2;
class SOAP_REMOTABLE_CLASS Bill2;
class SOAP_REMOTABLE_CLASS Item2;
class SOAP_REMOTABLE_CLASS Category2;
class SOAP_REMOTABLE_CLASS UnitCost2;

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
  void __fastcall SetStartTime(int Index, UnicodeString _prop_val)
  {  FStartTime = _prop_val; FStartTime_Specified = true;  }
  bool __fastcall StartTime_Specified(int Index)
  {  return FStartTime_Specified;  } 
__published:
  __property UnicodeString    EndTime = { index=(IS_OPTN|IS_NLBL), read=FEndTime, write=SetEndTime, stored = EndTime_Specified };
  __property UnicodeString         Id = { index=(IS_OPTN|IS_NLBL), read=FId, write=SetId, stored = Id_Specified };
  __property bool         IsActive = { index=(IS_OPTN), read=FIsActive, write=SetIsActive, stored = IsActive_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property UnicodeString  StartTime = { index=(IS_OPTN|IS_NLBL), read=FStartTime, write=SetStartTime, stored = StartTime_Specified };
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


typedef DynamicArray<CustomerDetailsMews*> ArrayOfCustomerDetailsMews; /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : CustomerDetailsMews, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class CustomerDetailsMews : public TRemotable {
private:
  ArrayOfstring   FClassifications;
  bool            FClassifications_Specified;
  UnicodeString   FEmail;
  bool            FEmail_Specified;
  UnicodeString   FFirstName;
  bool            FFirstName_Specified;
  UnicodeString   FGender;
  bool            FGender_Specified;
  UnicodeString   FId;
  bool            FId_Specified;
  UnicodeString   FLastName;
  bool            FLastName_Specified;
  UnicodeString   FNumber;
  bool            FNumber_Specified;
  UnicodeString   FPhone;
  bool            FPhone_Specified;
  UnicodeString   FTitle;
  bool            FTitle_Specified;
  void __fastcall SetClassifications(int Index, ArrayOfstring _prop_val)
  {  FClassifications = _prop_val; FClassifications_Specified = true;  }
  bool __fastcall Classifications_Specified(int Index)
  {  return FClassifications_Specified;  } 
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
  void __fastcall SetLastName(int Index, UnicodeString _prop_val)
  {  FLastName = _prop_val; FLastName_Specified = true;  }
  bool __fastcall LastName_Specified(int Index)
  {  return FLastName_Specified;  } 
  void __fastcall SetNumber(int Index, UnicodeString _prop_val)
  {  FNumber = _prop_val; FNumber_Specified = true;  }
  bool __fastcall Number_Specified(int Index)
  {  return FNumber_Specified;  } 
  void __fastcall SetPhone(int Index, UnicodeString _prop_val)
  {  FPhone = _prop_val; FPhone_Specified = true;  }
  bool __fastcall Phone_Specified(int Index)
  {  return FPhone_Specified;  } 
  void __fastcall SetTitle(int Index, UnicodeString _prop_val)
  {  FTitle = _prop_val; FTitle_Specified = true;  }
  bool __fastcall Title_Specified(int Index)
  {  return FTitle_Specified;  } 
__published:
  __property ArrayOfstring Classifications = { index=(IS_OPTN|IS_NLBL), read=FClassifications, write=SetClassifications, stored = Classifications_Specified };
  __property UnicodeString      Email = { index=(IS_OPTN|IS_NLBL), read=FEmail, write=SetEmail, stored = Email_Specified };
  __property UnicodeString  FirstName = { index=(IS_OPTN|IS_NLBL), read=FFirstName, write=SetFirstName, stored = FirstName_Specified };
  __property UnicodeString     Gender = { index=(IS_OPTN|IS_NLBL), read=FGender, write=SetGender, stored = Gender_Specified };
  __property UnicodeString         Id = { index=(IS_OPTN|IS_NLBL), read=FId, write=SetId, stored = Id_Specified };
  __property UnicodeString   LastName = { index=(IS_OPTN|IS_NLBL), read=FLastName, write=SetLastName, stored = LastName_Specified };
  __property UnicodeString     Number = { index=(IS_OPTN|IS_NLBL), read=FNumber, write=SetNumber, stored = Number_Specified };
  __property UnicodeString      Phone = { index=(IS_OPTN|IS_NLBL), read=FPhone, write=SetPhone, stored = Phone_Specified };
  __property UnicodeString      Title = { index=(IS_OPTN|IS_NLBL), read=FTitle, write=SetTitle, stored = Title_Specified };
};


typedef DynamicArray<Space*>      ArrayOfSpace;   /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : SpaceDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class SpaceDetails : public TRemotable {
private:
  ArrayOfSpace    FSpaces;
  bool            FSpaces_Specified;
  void __fastcall SetSpaces(int Index, ArrayOfSpace _prop_val)
  {  FSpaces = _prop_val; FSpaces_Specified = true;  }
  bool __fastcall Spaces_Specified(int Index)
  {  return FSpaces_Specified;  } 

public:
  __fastcall ~SpaceDetails();
__published:
  __property ArrayOfSpace     Spaces = { index=(IS_OPTN|IS_NLBL), read=FSpaces, write=SetSpaces, stored = Spaces_Specified };
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


typedef DynamicArray<Bill*>       ArrayOfBill;    /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */
typedef DynamicArray<Item*>       ArrayOfItem;    /* "http://schemas.datacontract.org/2004/07/MewsIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : Order, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Order : public TRemotable {
private:
  UnicodeString   FAccessToken;
  bool            FAccessToken_Specified;
  ArrayOfBill     FBills;
  bool            FBills_Specified;
  UnicodeString   FClientToken;
  bool            FClientToken_Specified;
  UnicodeString   FConsumptionUtc;
  bool            FConsumptionUtc_Specified;
  UnicodeString   FCustomerId;
  bool            FCustomerId_Specified;
  ArrayOfItem     FItems;
  bool            FItems_Specified;
  UnicodeString   FNotes;
  bool            FNotes_Specified;
  UnicodeString   FServiceId;
  bool            FServiceId_Specified;
  void __fastcall SetAccessToken(int Index, UnicodeString _prop_val)
  {  FAccessToken = _prop_val; FAccessToken_Specified = true;  }
  bool __fastcall AccessToken_Specified(int Index)
  {  return FAccessToken_Specified;  } 
  void __fastcall SetBills(int Index, ArrayOfBill _prop_val)
  {  FBills = _prop_val; FBills_Specified = true;  }
  bool __fastcall Bills_Specified(int Index)
  {  return FBills_Specified;  } 
  void __fastcall SetClientToken(int Index, UnicodeString _prop_val)
  {  FClientToken = _prop_val; FClientToken_Specified = true;  }
  bool __fastcall ClientToken_Specified(int Index)
  {  return FClientToken_Specified;  } 
  void __fastcall SetConsumptionUtc(int Index, UnicodeString _prop_val)
  {  FConsumptionUtc = _prop_val; FConsumptionUtc_Specified = true;  }
  bool __fastcall ConsumptionUtc_Specified(int Index)
  {  return FConsumptionUtc_Specified;  } 
  void __fastcall SetCustomerId(int Index, UnicodeString _prop_val)
  {  FCustomerId = _prop_val; FCustomerId_Specified = true;  }
  bool __fastcall CustomerId_Specified(int Index)
  {  return FCustomerId_Specified;  } 
  void __fastcall SetItems(int Index, ArrayOfItem _prop_val)
  {  FItems = _prop_val; FItems_Specified = true;  }
  bool __fastcall Items_Specified(int Index)
  {  return FItems_Specified;  } 
  void __fastcall SetNotes(int Index, UnicodeString _prop_val)
  {  FNotes = _prop_val; FNotes_Specified = true;  }
  bool __fastcall Notes_Specified(int Index)
  {  return FNotes_Specified;  } 
  void __fastcall SetServiceId(int Index, UnicodeString _prop_val)
  {  FServiceId = _prop_val; FServiceId_Specified = true;  }
  bool __fastcall ServiceId_Specified(int Index)
  {  return FServiceId_Specified;  } 

public:
  __fastcall ~Order();
__published:
  __property UnicodeString AccessToken = { index=(IS_OPTN|IS_NLBL), read=FAccessToken, write=SetAccessToken, stored = AccessToken_Specified };
  __property ArrayOfBill      Bills = { index=(IS_OPTN|IS_NLBL), read=FBills, write=SetBills, stored = Bills_Specified };
  __property UnicodeString ClientToken = { index=(IS_OPTN|IS_NLBL), read=FClientToken, write=SetClientToken, stored = ClientToken_Specified };
  __property UnicodeString ConsumptionUtc = { index=(IS_OPTN|IS_NLBL), read=FConsumptionUtc, write=SetConsumptionUtc, stored = ConsumptionUtc_Specified };
  __property UnicodeString CustomerId = { index=(IS_OPTN|IS_NLBL), read=FCustomerId, write=SetCustomerId, stored = CustomerId_Specified };
  __property ArrayOfItem      Items = { index=(IS_OPTN|IS_NLBL), read=FItems, write=SetItems, stored = Items_Specified };
  __property UnicodeString      Notes = { index=(IS_OPTN|IS_NLBL), read=FNotes, write=SetNotes, stored = Notes_Specified };
  __property UnicodeString  ServiceId = { index=(IS_OPTN|IS_NLBL), read=FServiceId, write=SetServiceId, stored = ServiceId_Specified };
};




// ************************************************************************ //
// XML       : Bill, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Bill : public TRemotable {
private:
  UnicodeString   FClosedUtc;
  bool            FClosedUtc_Specified;
  ArrayOfItem     FItems;
  bool            FItems_Specified;
  UnicodeString   FNumber;
  bool            FNumber_Specified;
  UnicodeString   FOutletId;
  bool            FOutletId_Specified;
  void __fastcall SetClosedUtc(int Index, UnicodeString _prop_val)
  {  FClosedUtc = _prop_val; FClosedUtc_Specified = true;  }
  bool __fastcall ClosedUtc_Specified(int Index)
  {  return FClosedUtc_Specified;  } 
  void __fastcall SetItems(int Index, ArrayOfItem _prop_val)
  {  FItems = _prop_val; FItems_Specified = true;  }
  bool __fastcall Items_Specified(int Index)
  {  return FItems_Specified;  } 
  void __fastcall SetNumber(int Index, UnicodeString _prop_val)
  {  FNumber = _prop_val; FNumber_Specified = true;  }
  bool __fastcall Number_Specified(int Index)
  {  return FNumber_Specified;  } 
  void __fastcall SetOutletId(int Index, UnicodeString _prop_val)
  {  FOutletId = _prop_val; FOutletId_Specified = true;  }
  bool __fastcall OutletId_Specified(int Index)
  {  return FOutletId_Specified;  } 

public:
  __fastcall ~Bill();
__published:
  __property UnicodeString  ClosedUtc = { index=(IS_OPTN|IS_NLBL), read=FClosedUtc, write=SetClosedUtc, stored = ClosedUtc_Specified };
  __property ArrayOfItem      Items = { index=(IS_OPTN|IS_NLBL), read=FItems, write=SetItems, stored = Items_Specified };
  __property UnicodeString     Number = { index=(IS_OPTN|IS_NLBL), read=FNumber, write=SetNumber, stored = Number_Specified };
  __property UnicodeString   OutletId = { index=(IS_OPTN|IS_NLBL), read=FOutletId, write=SetOutletId, stored = OutletId_Specified };
};




// ************************************************************************ //
// XML       : Category, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Category : public TRemotable {
private:
  UnicodeString   FCode;
  bool            FCode_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  void __fastcall SetCode(int Index, UnicodeString _prop_val)
  {  FCode = _prop_val; FCode_Specified = true;  }
  bool __fastcall Code_Specified(int Index)
  {  return FCode_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
__published:
  __property UnicodeString       Code = { index=(IS_OPTN|IS_NLBL), read=FCode, write=SetCode, stored = Code_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
};




// ************************************************************************ //
// XML       : Item, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Item : public TRemotable {
private:
  Category*       FCategory;
  bool            FCategory_Specified;
  UnicodeString   FConsumedUtc;
  bool            FConsumedUtc_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  UnicodeString   FType;
  bool            FType_Specified;
  UnitCost*       FUnitCost;
  bool            FUnitCost_Specified;
  int             FUnitCount;
  bool            FUnitCount_Specified;
  void __fastcall SetCategory(int Index, Category* _prop_val)
  {  FCategory = _prop_val; FCategory_Specified = true;  }
  bool __fastcall Category_Specified(int Index)
  {  return FCategory_Specified;  } 
  void __fastcall SetConsumedUtc(int Index, UnicodeString _prop_val)
  {  FConsumedUtc = _prop_val; FConsumedUtc_Specified = true;  }
  bool __fastcall ConsumedUtc_Specified(int Index)
  {  return FConsumedUtc_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetType(int Index, UnicodeString _prop_val)
  {  FType = _prop_val; FType_Specified = true;  }
  bool __fastcall Type_Specified(int Index)
  {  return FType_Specified;  } 
  void __fastcall SetUnitCost(int Index, UnitCost* _prop_val)
  {  FUnitCost = _prop_val; FUnitCost_Specified = true;  }
  bool __fastcall UnitCost_Specified(int Index)
  {  return FUnitCost_Specified;  } 
  void __fastcall SetUnitCount(int Index, int _prop_val)
  {  FUnitCount = _prop_val; FUnitCount_Specified = true;  }
  bool __fastcall UnitCount_Specified(int Index)
  {  return FUnitCount_Specified;  } 

public:
  __fastcall ~Item();
__published:
  __property Category*    Category = { index=(IS_OPTN|IS_NLBL), read=FCategory, write=SetCategory, stored = Category_Specified };
  __property UnicodeString ConsumedUtc = { index=(IS_OPTN|IS_NLBL), read=FConsumedUtc, write=SetConsumedUtc, stored = ConsumedUtc_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property UnicodeString       Type = { index=(IS_OPTN|IS_NLBL), read=FType, write=SetType, stored = Type_Specified };
  __property UnitCost*    UnitCost = { index=(IS_OPTN|IS_NLBL), read=FUnitCost, write=SetUnitCost, stored = UnitCost_Specified };
  __property int         UnitCount = { index=(IS_OPTN), read=FUnitCount, write=SetUnitCount, stored = UnitCount_Specified };
};




// ************************************************************************ //
// XML       : UnitCost, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class UnitCost : public TRemotable {
private:
  double          FAmount;
  bool            FAmount_Specified;
  UnicodeString   FCurrency;
  bool            FCurrency_Specified;
  double          FTax;
  bool            FTax_Specified;
  void __fastcall SetAmount(int Index, double _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  } 
  void __fastcall SetCurrency(int Index, UnicodeString _prop_val)
  {  FCurrency = _prop_val; FCurrency_Specified = true;  }
  bool __fastcall Currency_Specified(int Index)
  {  return FCurrency_Specified;  } 
  void __fastcall SetTax(int Index, double _prop_val)
  {  FTax = _prop_val; FTax_Specified = true;  }
  bool __fastcall Tax_Specified(int Index)
  {  return FTax_Specified;  } 
__published:
  __property double         Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString   Currency = { index=(IS_OPTN|IS_NLBL), read=FCurrency, write=SetCurrency, stored = Currency_Specified };
  __property double            Tax = { index=(IS_OPTN), read=FTax, write=SetTax, stored = Tax_Specified };
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
// XML       : CustomerDetailsMews, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class CustomerDetailsMews2 : public CustomerDetailsMews {
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
// XML       : Space, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Space2 : public Space {
private:
__published:
};




// ************************************************************************ //
// XML       : Order, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Order2 : public Order {
private:
__published:
};




// ************************************************************************ //
// XML       : Bill, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Bill2 : public Bill {
private:
__published:
};




// ************************************************************************ //
// XML       : Item, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Item2 : public Item {
private:
__published:
};




// ************************************************************************ //
// XML       : Category, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class Category2 : public Category {
private:
__published:
};




// ************************************************************************ //
// XML       : UnitCost, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MewsIntegration.Domain
// ************************************************************************ //
class UnitCost2 : public UnitCost {
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
  virtual ArrayOfCustomerDetailsMews SearchCustomers(const UnicodeString platformAddress, const CustomerSearch* customerSearch) = 0; 
  virtual SpaceDetails*   GetSpaceIds(const UnicodeString platformAddress, const BasicInquiry* customerSearch) = 0; 
  virtual UnicodeString   PostOrder(const UnicodeString platformAddress, const Order* order) = 0; 
  virtual bool            PostBill(const UnicodeString platformAddress, const Order* order) = 0; 
};
typedef DelphiInterface<IMewsIntegrationWebService> _di_IMewsIntegrationWebService;

_di_IMewsIntegrationWebService GetIMewsIntegrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__MewsWSDL;
#endif



#endif // _H
