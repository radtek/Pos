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
// (20/03/2019 8:16:50 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   OnlineOrderingWSDLH
#define   OnlineOrderingWSDLH

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


namespace NS__OnlineOrderingWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:long            - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:base64Binary    - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:dateTime        - "http://www.w3.org/2001/XMLSchema"[Gbl]

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
class SOAP_REMOTABLE_CLASS OOServiceResponse;
class SOAP_REMOTABLE_CLASS OOLoyaltyResponse;
class SOAP_REMOTABLE_CLASS LoyaltyOOResponse;
class SOAP_REMOTABLE_CLASS OOLoyaltyResponse2;
class SOAP_REMOTABLE_CLASS OOServiceResponse2;
class SOAP_REMOTABLE_CLASS LoyaltyOOResponse2;
class SOAP_REMOTABLE_CLASS OrderInvoiceTransactionModel;
class SOAP_REMOTABLE_CLASS SiteOrderModel;
class SOAP_REMOTABLE_CLASS InvoiceTransactionModel;
class SOAP_REMOTABLE_CLASS OrderItemModel;
class SOAP_REMOTABLE_CLASS OrderItemSizeModel;
class SOAP_REMOTABLE_CLASS OrderItemSizeDiscountModel;
class SOAP_REMOTABLE_CLASS OrderItemSizeTaxProfileModel;
class SOAP_REMOTABLE_CLASS SiteOrderModel2;
class SOAP_REMOTABLE_CLASS OrderInvoiceTransactionModel2;
class SOAP_REMOTABLE_CLASS InvoiceTransactionModel2;
class SOAP_REMOTABLE_CLASS OrderItemModel2;
class SOAP_REMOTABLE_CLASS OrderItemSizeModel2;
class SOAP_REMOTABLE_CLASS OrderItemSizeDiscountModel2;
class SOAP_REMOTABLE_CLASS OrderItemSizeTaxProfileModel2;

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

enum class OOLoyaltyResponseCode   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering"[GblSmpl] */
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
  PostOnlineOrderInvoiceInfoFailed, 
  MultipleGUIDExist, 
  GUIDNotFound
};

class OOLoyaltyResponseCode_TypeInfoHolder : public TObject {
  OOLoyaltyResponseCode __instanceType;
public:
__published:
  __property OOLoyaltyResponseCode __propType = { read=__instanceType };
};

typedef DynamicArray<MenuConsumableInfo*> ArrayOfMenuConsumableInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : SiteMenuInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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


typedef DynamicArray<CourseInfo*> ArrayOfCourseInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : MenuConsumableInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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


typedef DynamicArray<SiteItemInfo*> ArrayOfSiteItemInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : CourseInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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


typedef DynamicArray<ItemSizeInfo*> ArrayOfItemSizeInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels"[GblCplx] */
typedef DynamicArray<SideGroupInfo*> ArrayOfSideGroupInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : SiteItemInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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


typedef DynamicArray<ItemSizeTaxProfileInfo*> ArrayOfItemSizeTaxProfileInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : ItemSizeInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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


typedef DynamicArray<ItemSideInfo*> ArrayOfItemSideInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : SideGroupInfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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


typedef DynamicArray<TaxSettingsInfo*> ArrayOfTaxSettingsInfo; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels"[GblCplx] */


// ************************************************************************ //
// XML       : SiteTaxSettingsinfo, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
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
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class SiteMenuInfo2 : public SiteMenuInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : MenuConsumableInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class MenuConsumableInfo2 : public MenuConsumableInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : CourseInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class CourseInfo2 : public CourseInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : SiteItemInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class SiteItemInfo2 : public SiteItemInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : ItemSizeInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSizeInfo2 : public ItemSizeInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : ItemSizeTaxProfileInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSizeTaxProfileInfo2 : public ItemSizeTaxProfileInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : SideGroupInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class SideGroupInfo2 : public SideGroupInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : ItemSideInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class ItemSideInfo2 : public ItemSideInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : SiteTaxSettingsinfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class SiteTaxSettingsinfo2 : public SiteTaxSettingsinfo {
private:
__published:
};




// ************************************************************************ //
// XML       : TaxSettingsInfo, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.MenuModels
// ************************************************************************ //
class TaxSettingsInfo2 : public TaxSettingsInfo {
private:
__published:
};




// ************************************************************************ //
// XML       : OOServiceResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering
// ************************************************************************ //
class OOServiceResponse : public TRemotable {
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
// XML       : OOLoyaltyResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering
// ************************************************************************ //
class OOLoyaltyResponse : public OOServiceResponse {
private:
  OOLoyaltyResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetResponseCode(int Index, OOLoyaltyResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 
__published:
  __property OOLoyaltyResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
};




// ************************************************************************ //
// XML       : LoyaltyOOResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering
// ************************************************************************ //
class LoyaltyOOResponse : public TRemotable {
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
// XML       : OOLoyaltyResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering
// ************************************************************************ //
class OOLoyaltyResponse2 : public OOLoyaltyResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : OOServiceResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering
// ************************************************************************ //
class OOServiceResponse2 : public OOServiceResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : LoyaltyOOResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering
// ************************************************************************ //
class LoyaltyOOResponse2 : public LoyaltyOOResponse {
private:
__published:
};


typedef UnicodeString guid; /* "http://schemas.microsoft.com/2003/10/Serialization/"[GblSmpl] */


// ************************************************************************ //
// XML       : OrderInvoiceTransactionModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class OrderInvoiceTransactionModel : public TRemotable {
private:
  InvoiceTransactionModel* FInvoiceTransaction;
  bool            FInvoiceTransaction_Specified;
  __int64         FInvoiceTransactionId;
  bool            FInvoiceTransactionId_Specified;
  __int64         FOrderId;
  bool            FOrderId_Specified;
  __int64         FOrderInvoiceTransactionId;
  bool            FOrderInvoiceTransactionId_Specified;
  void __fastcall SetInvoiceTransaction(int Index, InvoiceTransactionModel* _prop_val)
  {  FInvoiceTransaction = _prop_val; FInvoiceTransaction_Specified = true;  }
  bool __fastcall InvoiceTransaction_Specified(int Index)
  {  return FInvoiceTransaction_Specified;  } 
  void __fastcall SetInvoiceTransactionId(int Index, __int64 _prop_val)
  {  FInvoiceTransactionId = _prop_val; FInvoiceTransactionId_Specified = true;  }
  bool __fastcall InvoiceTransactionId_Specified(int Index)
  {  return FInvoiceTransactionId_Specified;  } 
  void __fastcall SetOrderId(int Index, __int64 _prop_val)
  {  FOrderId = _prop_val; FOrderId_Specified = true;  }
  bool __fastcall OrderId_Specified(int Index)
  {  return FOrderId_Specified;  } 
  void __fastcall SetOrderInvoiceTransactionId(int Index, __int64 _prop_val)
  {  FOrderInvoiceTransactionId = _prop_val; FOrderInvoiceTransactionId_Specified = true;  }
  bool __fastcall OrderInvoiceTransactionId_Specified(int Index)
  {  return FOrderInvoiceTransactionId_Specified;  } 

public:
  __fastcall ~OrderInvoiceTransactionModel();
__published:
  __property InvoiceTransactionModel* InvoiceTransaction = { index=(IS_OPTN|IS_NLBL), read=FInvoiceTransaction, write=SetInvoiceTransaction, stored = InvoiceTransaction_Specified };
  __property __int64    InvoiceTransactionId = { index=(IS_OPTN), read=FInvoiceTransactionId, write=SetInvoiceTransactionId, stored = InvoiceTransactionId_Specified };
  __property __int64       OrderId = { index=(IS_OPTN), read=FOrderId, write=SetOrderId, stored = OrderId_Specified };
  __property __int64    OrderInvoiceTransactionId = { index=(IS_OPTN), read=FOrderInvoiceTransactionId, write=SetOrderInvoiceTransactionId, stored = OrderInvoiceTransactionId_Specified };
};


typedef DynamicArray<OrderItemModel*> ArrayOfOrderItemModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels"[GblCplx] */


// ************************************************************************ //
// XML       : SiteOrderModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class SiteOrderModel : public TRemotable {
private:
  __int64         FCompanyId;
  bool            FCompanyId_Specified;
  UnicodeString   FContainerName;
  bool            FContainerName_Specified;
  UnicodeString   FContainerNumber;
  bool            FContainerNumber_Specified;
  OrderContainerType FContainerType;
  bool            FContainerType_Specified;
  bool            FIsConfirmed;
  bool            FIsConfirmed_Specified;
  bool            FIsHappyHourApplied;
  bool            FIsHappyHourApplied_Specified;
  UnicodeString   FLocation;
  bool            FLocation_Specified;
  guid            FMemberGuid;
  bool            FMemberGuid_Specified;
  UnicodeString   FOrderGuid;
  bool            FOrderGuid_Specified;
  __int64         FOrderId;
  bool            FOrderId_Specified;
  OrderInvoiceTransactionModel* FOrderInvoiceTransaction;
  bool            FOrderInvoiceTransaction_Specified;
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
  UnicodeString   FUserEmailId;
  bool            FUserEmailId_Specified;
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
  void __fastcall SetContainerNumber(int Index, UnicodeString _prop_val)
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
  void __fastcall SetIsHappyHourApplied(int Index, bool _prop_val)
  {  FIsHappyHourApplied = _prop_val; FIsHappyHourApplied_Specified = true;  }
  bool __fastcall IsHappyHourApplied_Specified(int Index)
  {  return FIsHappyHourApplied_Specified;  } 
  void __fastcall SetLocation(int Index, UnicodeString _prop_val)
  {  FLocation = _prop_val; FLocation_Specified = true;  }
  bool __fastcall Location_Specified(int Index)
  {  return FLocation_Specified;  } 
  void __fastcall SetMemberGuid(int Index, guid _prop_val)
  {  FMemberGuid = _prop_val; FMemberGuid_Specified = true;  }
  bool __fastcall MemberGuid_Specified(int Index)
  {  return FMemberGuid_Specified;  } 
  void __fastcall SetOrderGuid(int Index, UnicodeString _prop_val)
  {  FOrderGuid = _prop_val; FOrderGuid_Specified = true;  }
  bool __fastcall OrderGuid_Specified(int Index)
  {  return FOrderGuid_Specified;  } 
  void __fastcall SetOrderId(int Index, __int64 _prop_val)
  {  FOrderId = _prop_val; FOrderId_Specified = true;  }
  bool __fastcall OrderId_Specified(int Index)
  {  return FOrderId_Specified;  } 
  void __fastcall SetOrderInvoiceTransaction(int Index, OrderInvoiceTransactionModel* _prop_val)
  {  FOrderInvoiceTransaction = _prop_val; FOrderInvoiceTransaction_Specified = true;  }
  bool __fastcall OrderInvoiceTransaction_Specified(int Index)
  {  return FOrderInvoiceTransaction_Specified;  } 
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
  void __fastcall SetUserEmailId(int Index, UnicodeString _prop_val)
  {  FUserEmailId = _prop_val; FUserEmailId_Specified = true;  }
  bool __fastcall UserEmailId_Specified(int Index)
  {  return FUserEmailId_Specified;  } 
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
  __property UnicodeString ContainerNumber = { index=(IS_OPTN|IS_NLBL), read=FContainerNumber, write=SetContainerNumber, stored = ContainerNumber_Specified };
  __property OrderContainerType ContainerType = { index=(IS_OPTN), read=FContainerType, write=SetContainerType, stored = ContainerType_Specified };
  __property bool       IsConfirmed = { index=(IS_OPTN), read=FIsConfirmed, write=SetIsConfirmed, stored = IsConfirmed_Specified };
  __property bool       IsHappyHourApplied = { index=(IS_OPTN), read=FIsHappyHourApplied, write=SetIsHappyHourApplied, stored = IsHappyHourApplied_Specified };
  __property UnicodeString   Location = { index=(IS_OPTN|IS_NLBL), read=FLocation, write=SetLocation, stored = Location_Specified };
  __property guid       MemberGuid = { index=(IS_OPTN|IS_NLBL), read=FMemberGuid, write=SetMemberGuid, stored = MemberGuid_Specified };
  __property UnicodeString  OrderGuid = { index=(IS_OPTN|IS_NLBL), read=FOrderGuid, write=SetOrderGuid, stored = OrderGuid_Specified };
  __property __int64       OrderId = { index=(IS_OPTN), read=FOrderId, write=SetOrderId, stored = OrderId_Specified };
  __property OrderInvoiceTransactionModel* OrderInvoiceTransaction = { index=(IS_OPTN|IS_NLBL), read=FOrderInvoiceTransaction, write=SetOrderInvoiceTransaction, stored = OrderInvoiceTransaction_Specified };
  __property ArrayOfOrderItemModel OrderItems = { index=(IS_OPTN|IS_NLBL), read=FOrderItems, write=SetOrderItems, stored = OrderItems_Specified };
  __property OrderType   OrderType = { index=(IS_OPTN), read=FOrderType, write=SetOrderType, stored = OrderType_Specified };
  __property __int64        SiteId = { index=(IS_OPTN), read=FSiteId, write=SetSiteId, stored = SiteId_Specified };
  __property UnicodeString TerminalName = { index=(IS_OPTN|IS_NLBL), read=FTerminalName, write=SetTerminalName, stored = TerminalName_Specified };
  __property double     TotalAmount = { index=(IS_OPTN), read=FTotalAmount, write=SetTotalAmount, stored = TotalAmount_Specified };
  __property TXSDateTime* TransactionDate = { index=(IS_OPTN), read=FTransactionDate, write=SetTransactionDate, stored = TransactionDate_Specified };
  __property SiteSettingType TransactionType = { index=(IS_OPTN), read=FTransactionType, write=SetTransactionType, stored = TransactionType_Specified };
  __property UnicodeString UserEmailId = { index=(IS_OPTN|IS_NLBL), read=FUserEmailId, write=SetUserEmailId, stored = UserEmailId_Specified };
  __property __int64    UserReferenceId = { index=(IS_OPTN), read=FUserReferenceId, write=SetUserReferenceId, stored = UserReferenceId_Specified };
  __property UserType     UserType = { index=(IS_OPTN), read=FUserType, write=SetUserType, stored = UserType_Specified };
};




// ************************************************************************ //
// XML       : InvoiceTransactionModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class InvoiceTransactionModel : public TRemotable {
private:
  UnicodeString   FInvoiceNumber;
  bool            FInvoiceNumber_Specified;
  __int64         FInvoiceTransactionId;
  bool            FInvoiceTransactionId_Specified;
  TByteDynArray   FReceipt;
  bool            FReceipt_Specified;
  UnicodeString   FReceiptPath;
  bool            FReceiptPath_Specified;
  double          FRounding;
  bool            FRounding_Specified;
  __int64         FSiteId;
  bool            FSiteId_Specified;
  UnicodeString   FTerminalName;
  bool            FTerminalName_Specified;
  double          FTotalSaleAmount;
  bool            FTotalSaleAmount_Specified;
  TXSDateTime*    FTransactionDate;
  bool            FTransactionDate_Specified;
  __int64         FUserReferenceId;
  bool            FUserReferenceId_Specified;
  UserType        FUserType;
  bool            FUserType_Specified;
  void __fastcall SetInvoiceNumber(int Index, UnicodeString _prop_val)
  {  FInvoiceNumber = _prop_val; FInvoiceNumber_Specified = true;  }
  bool __fastcall InvoiceNumber_Specified(int Index)
  {  return FInvoiceNumber_Specified;  } 
  void __fastcall SetInvoiceTransactionId(int Index, __int64 _prop_val)
  {  FInvoiceTransactionId = _prop_val; FInvoiceTransactionId_Specified = true;  }
  bool __fastcall InvoiceTransactionId_Specified(int Index)
  {  return FInvoiceTransactionId_Specified;  } 
  void __fastcall SetReceipt(int Index, TByteDynArray _prop_val)
  {  FReceipt = _prop_val; FReceipt_Specified = true;  }
  bool __fastcall Receipt_Specified(int Index)
  {  return FReceipt_Specified;  } 
  void __fastcall SetReceiptPath(int Index, UnicodeString _prop_val)
  {  FReceiptPath = _prop_val; FReceiptPath_Specified = true;  }
  bool __fastcall ReceiptPath_Specified(int Index)
  {  return FReceiptPath_Specified;  } 
  void __fastcall SetRounding(int Index, double _prop_val)
  {  FRounding = _prop_val; FRounding_Specified = true;  }
  bool __fastcall Rounding_Specified(int Index)
  {  return FRounding_Specified;  } 
  void __fastcall SetSiteId(int Index, __int64 _prop_val)
  {  FSiteId = _prop_val; FSiteId_Specified = true;  }
  bool __fastcall SiteId_Specified(int Index)
  {  return FSiteId_Specified;  } 
  void __fastcall SetTerminalName(int Index, UnicodeString _prop_val)
  {  FTerminalName = _prop_val; FTerminalName_Specified = true;  }
  bool __fastcall TerminalName_Specified(int Index)
  {  return FTerminalName_Specified;  } 
  void __fastcall SetTotalSaleAmount(int Index, double _prop_val)
  {  FTotalSaleAmount = _prop_val; FTotalSaleAmount_Specified = true;  }
  bool __fastcall TotalSaleAmount_Specified(int Index)
  {  return FTotalSaleAmount_Specified;  } 
  void __fastcall SetTransactionDate(int Index, TXSDateTime* _prop_val)
  {  FTransactionDate = _prop_val; FTransactionDate_Specified = true;  }
  bool __fastcall TransactionDate_Specified(int Index)
  {  return FTransactionDate_Specified;  } 
  void __fastcall SetUserReferenceId(int Index, __int64 _prop_val)
  {  FUserReferenceId = _prop_val; FUserReferenceId_Specified = true;  }
  bool __fastcall UserReferenceId_Specified(int Index)
  {  return FUserReferenceId_Specified;  } 
  void __fastcall SetUserType(int Index, UserType _prop_val)
  {  FUserType = _prop_val; FUserType_Specified = true;  }
  bool __fastcall UserType_Specified(int Index)
  {  return FUserType_Specified;  } 

public:
  __fastcall ~InvoiceTransactionModel();
__published:
  __property UnicodeString InvoiceNumber = { index=(IS_OPTN|IS_NLBL), read=FInvoiceNumber, write=SetInvoiceNumber, stored = InvoiceNumber_Specified };
  __property __int64    InvoiceTransactionId = { index=(IS_OPTN), read=FInvoiceTransactionId, write=SetInvoiceTransactionId, stored = InvoiceTransactionId_Specified };
  __property TByteDynArray    Receipt = { index=(IS_OPTN|IS_NLBL), read=FReceipt, write=SetReceipt, stored = Receipt_Specified };
  __property UnicodeString ReceiptPath = { index=(IS_OPTN|IS_NLBL), read=FReceiptPath, write=SetReceiptPath, stored = ReceiptPath_Specified };
  __property double       Rounding = { index=(IS_OPTN), read=FRounding, write=SetRounding, stored = Rounding_Specified };
  __property __int64        SiteId = { index=(IS_OPTN), read=FSiteId, write=SetSiteId, stored = SiteId_Specified };
  __property UnicodeString TerminalName = { index=(IS_OPTN|IS_NLBL), read=FTerminalName, write=SetTerminalName, stored = TerminalName_Specified };
  __property double     TotalSaleAmount = { index=(IS_OPTN), read=FTotalSaleAmount, write=SetTotalSaleAmount, stored = TotalSaleAmount_Specified };
  __property TXSDateTime* TransactionDate = { index=(IS_OPTN), read=FTransactionDate, write=SetTransactionDate, stored = TransactionDate_Specified };
  __property __int64    UserReferenceId = { index=(IS_OPTN), read=FUserReferenceId, write=SetUserReferenceId, stored = UserReferenceId_Specified };
  __property UserType     UserType = { index=(IS_OPTN), read=FUserType, write=SetUserType, stored = UserType_Specified };
};


typedef DynamicArray<OrderItemSizeModel*> ArrayOfOrderItemSizeModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels"[GblCplx] */


// ************************************************************************ //
// XML       : OrderItemModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemModel : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FItemUniqueId;
  bool            FItemUniqueId_Specified;
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
  void __fastcall SetItemUniqueId(int Index, UnicodeString _prop_val)
  {  FItemUniqueId = _prop_val; FItemUniqueId_Specified = true;  }
  bool __fastcall ItemUniqueId_Specified(int Index)
  {  return FItemUniqueId_Specified;  } 
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
  __property UnicodeString ItemUniqueId = { index=(IS_OPTN|IS_NLBL), read=FItemUniqueId, write=SetItemUniqueId, stored = ItemUniqueId_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property __int64       OrderId = { index=(IS_OPTN), read=FOrderId, write=SetOrderId, stored = OrderId_Specified };
  __property __int64    OrderItemId = { index=(IS_OPTN), read=FOrderItemId, write=SetOrderItemId, stored = OrderItemId_Specified };
  __property ArrayOfOrderItemSizeModel OrderItemSizes = { index=(IS_OPTN|IS_NLBL), read=FOrderItemSizes, write=SetOrderItemSizes, stored = OrderItemSizes_Specified };
  __property double          Price = { index=(IS_OPTN), read=FPrice, write=SetPrice, stored = Price_Specified };
  __property __int64    SiteItemId = { index=(IS_OPTN), read=FSiteItemId, write=SetSiteItemId, stored = SiteItemId_Specified };
};


typedef DynamicArray<OrderItemSizeDiscountModel*> ArrayOfOrderItemSizeDiscountModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels"[GblCplx] */
typedef DynamicArray<OrderItemSizeTaxProfileModel*> ArrayOfOrderItemSizeTaxProfileModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels"[GblCplx] */


// ************************************************************************ //
// XML       : OrderItemSizeModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeModel : public TRemotable {
private:
  double          FBasePrice;
  bool            FBasePrice_Specified;
  __int64         FItemSizeId;
  bool            FItemSizeId_Specified;
  UnicodeString   FItemSizeUniqueId;
  bool            FItemSizeUniqueId_Specified;
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
  void __fastcall SetItemSizeUniqueId(int Index, UnicodeString _prop_val)
  {  FItemSizeUniqueId = _prop_val; FItemSizeUniqueId_Specified = true;  }
  bool __fastcall ItemSizeUniqueId_Specified(int Index)
  {  return FItemSizeUniqueId_Specified;  } 
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
  __property UnicodeString ItemSizeUniqueId = { index=(IS_OPTN|IS_NLBL), read=FItemSizeUniqueId, write=SetItemSizeUniqueId, stored = ItemSizeUniqueId_Specified };
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
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
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
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
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
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class SiteOrderModel2 : public SiteOrderModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderInvoiceTransactionModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class OrderInvoiceTransactionModel2 : public OrderInvoiceTransactionModel {
private:
__published:
};




// ************************************************************************ //
// XML       : InvoiceTransactionModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class InvoiceTransactionModel2 : public InvoiceTransactionModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderItemModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemModel2 : public OrderItemModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderItemSizeModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeModel2 : public OrderItemSizeModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderItemSizeDiscountModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeDiscountModel2 : public OrderItemSizeDiscountModel {
private:
__published:
};




// ************************************************************************ //
// XML       : OrderItemSizeTaxProfileModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateOnlineOrdering.OrderModels
// ************************************************************************ //
class OrderItemSizeTaxProfileModel2 : public OrderItemSizeTaxProfileModel {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IWCFServiceOnlineOrdering/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceOnlineOrdering
// service   : WCFServiceOnlineOrdering
// port      : basicHttpBinding_ServiceOnlineOrdering
// URL       : http://localhost:8750/MenumateServices/OnlineOrdering/
// ************************************************************************ //
__interface INTERFACE_UUID("{4D85DFF9-1B47-5459-2BB1-1B1CB084C1DB}") IWCFServiceOnlineOrdering : public IInvokable
{
public:
  virtual OOLoyaltyResponse* SyncMenu(const UnicodeString inSyndicateCode, const SiteMenuInfo* siteViewModel) = 0; 
  virtual OOLoyaltyResponse* SyncTaxSettings(const UnicodeString inSyndicateCode, const SiteTaxSettingsinfo* siteTaxSettings) = 0; 
  virtual void            GetOrdersFromWeb(const UnicodeString inSyndicateCode, const UnicodeString orders) = 0; 
  virtual OOLoyaltyResponse* PostOnlineOrderInvoiceInfo(const UnicodeString inSyndicateCode, const SiteOrderModel* siteOrderModel) = 0; 
  virtual LoyaltyOOResponse* SyncOnlineOrderingDetails(const UnicodeString inSyndicateCode, const int siteCode) = 0; 
  virtual bool            UnsetOrderingDetails(const UnicodeString inSyndicateCode, const int siteCode) = 0; 
};
typedef DelphiInterface<IWCFServiceOnlineOrdering> _di_IWCFServiceOnlineOrdering;

_di_IWCFServiceOnlineOrdering GetIWCFServiceOnlineOrdering(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__OnlineOrderingWSDL;
#endif



#endif // _H
