// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8888/MenumateServices/ClippService/?wsdl
//  >Import : http://localhost:8888/MenumateServices/ClippService/?wsdl>0
//  >Import : http://localhost:8888/MenumateServices/ClippService/?xsd=xsd0
//  >Import : http://localhost:8888/MenumateServices/ClippService/?xsd=xsd2
//  >Import : http://localhost:8888/MenumateServices/ClippService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (8/09/2015 5:24:22 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   ClippIntegrationWSDLH
#define   ClippIntegrationWSDLH

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


namespace NS_ClippIntegrationWSDL {

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
// !:decimal         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:long            - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS ClippMessage;
class SOAP_REMOTABLE_CLASS ClippMessageDetail;
class SOAP_REMOTABLE_CLASS Discount;
class SOAP_REMOTABLE_CLASS ClippTabDetailRequest;
class SOAP_REMOTABLE_CLASS ClippTabDetail;
class SOAP_REMOTABLE_CLASS Item;
class SOAP_REMOTABLE_CLASS Payment;
class SOAP_REMOTABLE_CLASS Sundry;
class SOAP_REMOTABLE_CLASS ClippErrorDetail;
class SOAP_REMOTABLE_CLASS PaymentDetailRequest;
class SOAP_REMOTABLE_CLASS PaymentDetailResponse;
class SOAP_REMOTABLE_CLASS ClippMessage2;
class SOAP_REMOTABLE_CLASS ClippMessageDetail2;
class SOAP_REMOTABLE_CLASS Discount2;
class SOAP_REMOTABLE_CLASS ClippTabDetailRequest2;
class SOAP_REMOTABLE_CLASS ClippTabDetail2;
class SOAP_REMOTABLE_CLASS Item2;
class SOAP_REMOTABLE_CLASS Payment2;
class SOAP_REMOTABLE_CLASS Sundry2;
class SOAP_REMOTABLE_CLASS ClippErrorDetail2;
class SOAP_REMOTABLE_CLASS PaymentDetailRequest2;
class SOAP_REMOTABLE_CLASS PaymentDetailResponse2;

enum class ClippMessageType   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp"[GblSmpl] */
{
  TabOpen,
  TabStatus,
  TabUpdate,
  TabPay,
  TabClose
};

class ClippMessageType_TypeInfoHolder : public TObject {
  ClippMessageType __instanceType;
public:
__published:
  __property ClippMessageType __propType = { read=__instanceType };
};

typedef DynamicArray<ClippMessage*> ArrayOfClippMessage; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp"[GblCplx] */


// ************************************************************************ //
// XML       : ClippMessage, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippMessage : public TRemotable {
private:
  TXSDateTime*    FCreatedDate;
  bool            FCreatedDate_Specified;
  ClippMessageDetail* FData;
  bool            FData_Specified;
  __int64         FId;
  bool            FId_Specified;
  UnicodeString   FMessageId;
  bool            FMessageId_Specified;
  UnicodeString   FMessageType;
  bool            FMessageType_Specified;
  ClippMessageType FMessageTypeEnum;
  bool            FMessageTypeEnum_Specified;
  void __fastcall SetCreatedDate(int Index, TXSDateTime* _prop_val)
  {  FCreatedDate = _prop_val; FCreatedDate_Specified = true;  }
  bool __fastcall CreatedDate_Specified(int Index)
  {  return FCreatedDate_Specified;  }
  void __fastcall SetData(int Index, ClippMessageDetail* _prop_val)
  {  FData = _prop_val; FData_Specified = true;  }
  bool __fastcall Data_Specified(int Index)
  {  return FData_Specified;  }
  void __fastcall SetId(int Index, __int64 _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  }
  void __fastcall SetMessageId(int Index, UnicodeString _prop_val)
  {  FMessageId = _prop_val; FMessageId_Specified = true;  }
  bool __fastcall MessageId_Specified(int Index)
  {  return FMessageId_Specified;  }
  void __fastcall SetMessageType(int Index, UnicodeString _prop_val)
  {  FMessageType = _prop_val; FMessageType_Specified = true;  }
  bool __fastcall MessageType_Specified(int Index)
  {  return FMessageType_Specified;  }
  void __fastcall SetMessageTypeEnum(int Index, ClippMessageType _prop_val)
  {  FMessageTypeEnum = _prop_val; FMessageTypeEnum_Specified = true;  }
  bool __fastcall MessageTypeEnum_Specified(int Index)
  {  return FMessageTypeEnum_Specified;  }

public:
  __fastcall ~ClippMessage();
__published:
  __property TXSDateTime* CreatedDate = { index=(IS_OPTN), read=FCreatedDate, write=SetCreatedDate, stored = CreatedDate_Specified };
  __property ClippMessageDetail*       Data = { index=(IS_OPTN|IS_NLBL), read=FData, write=SetData, stored = Data_Specified };
  __property __int64            Id = { index=(IS_OPTN), read=FId, write=SetId, stored = Id_Specified };
  __property UnicodeString  MessageId = { index=(IS_OPTN|IS_NLBL), read=FMessageId, write=SetMessageId, stored = MessageId_Specified };
  __property UnicodeString MessageType = { index=(IS_OPTN|IS_NLBL), read=FMessageType, write=SetMessageType, stored = MessageType_Specified };
  __property ClippMessageType MessageTypeEnum = { index=(IS_OPTN), read=FMessageTypeEnum, write=SetMessageTypeEnum, stored = MessageTypeEnum_Specified };
};


typedef DynamicArray<Discount*>   ArrayOfDiscount; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp"[GblCplx] */


// ************************************************************************ //
// XML       : ClippMessageDetail, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippMessageDetail : public TRemotable {
private:
  UnicodeString   FClippPaymentRef;
  bool            FClippPaymentRef_Specified;
  UnicodeString   FClippTabRef;
  bool            FClippTabRef_Specified;
  UnicodeString   FClippUserRef;
  bool            FClippUserRef_Specified;
  TXSDecimal*     FDiscountAmount;
  bool            FDiscountAmount_Specified;
  ArrayOfDiscount FDiscounts;
  bool            FDiscounts_Specified;
  UnicodeString   FFirstName;
  bool            FFirstName_Specified;
  UnicodeString   FLastName;
  bool            FLastName_Specified;
  UnicodeString   FPhotoUrl;
  bool            FPhotoUrl_Specified;
  UnicodeString   FPosTabRef;
  bool            FPosTabRef_Specified;
  TXSDecimal*     FProcessingFeeAmount;
  bool            FProcessingFeeAmount_Specified;
  TXSDecimal*     FTabLimit;
  bool            FTabLimit_Specified;
  TXSDecimal*     FTipAmount;
  bool            FTipAmount_Specified;
  TXSDecimal*     FTotalPaymentAmount;
  bool            FTotalPaymentAmount_Specified;
  void __fastcall SetClippPaymentRef(int Index, UnicodeString _prop_val)
  {  FClippPaymentRef = _prop_val; FClippPaymentRef_Specified = true;  }
  bool __fastcall ClippPaymentRef_Specified(int Index)
  {  return FClippPaymentRef_Specified;  }
  void __fastcall SetClippTabRef(int Index, UnicodeString _prop_val)
  {  FClippTabRef = _prop_val; FClippTabRef_Specified = true;  }
  bool __fastcall ClippTabRef_Specified(int Index)
  {  return FClippTabRef_Specified;  }
  void __fastcall SetClippUserRef(int Index, UnicodeString _prop_val)
  {  FClippUserRef = _prop_val; FClippUserRef_Specified = true;  }
  bool __fastcall ClippUserRef_Specified(int Index)
  {  return FClippUserRef_Specified;  }
  void __fastcall SetDiscountAmount(int Index, TXSDecimal* _prop_val)
  {  FDiscountAmount = _prop_val; FDiscountAmount_Specified = true;  }
  bool __fastcall DiscountAmount_Specified(int Index)
  {  return FDiscountAmount_Specified;  }
  void __fastcall SetDiscounts(int Index, ArrayOfDiscount _prop_val)
  {  FDiscounts = _prop_val; FDiscounts_Specified = true;  }
  bool __fastcall Discounts_Specified(int Index)
  {  return FDiscounts_Specified;  }
  void __fastcall SetFirstName(int Index, UnicodeString _prop_val)
  {  FFirstName = _prop_val; FFirstName_Specified = true;  }
  bool __fastcall FirstName_Specified(int Index)
  {  return FFirstName_Specified;  }
  void __fastcall SetLastName(int Index, UnicodeString _prop_val)
  {  FLastName = _prop_val; FLastName_Specified = true;  }
  bool __fastcall LastName_Specified(int Index)
  {  return FLastName_Specified;  }
  void __fastcall SetPhotoUrl(int Index, UnicodeString _prop_val)
  {  FPhotoUrl = _prop_val; FPhotoUrl_Specified = true;  }
  bool __fastcall PhotoUrl_Specified(int Index)
  {  return FPhotoUrl_Specified;  }
  void __fastcall SetPosTabRef(int Index, UnicodeString _prop_val)
  {  FPosTabRef = _prop_val; FPosTabRef_Specified = true;  }
  bool __fastcall PosTabRef_Specified(int Index)
  {  return FPosTabRef_Specified;  }
  void __fastcall SetProcessingFeeAmount(int Index, TXSDecimal* _prop_val)
  {  FProcessingFeeAmount = _prop_val; FProcessingFeeAmount_Specified = true;  }
  bool __fastcall ProcessingFeeAmount_Specified(int Index)
  {  return FProcessingFeeAmount_Specified;  }
  void __fastcall SetTabLimit(int Index, TXSDecimal* _prop_val)
  {  FTabLimit = _prop_val; FTabLimit_Specified = true;  }
  bool __fastcall TabLimit_Specified(int Index)
  {  return FTabLimit_Specified;  }
  void __fastcall SetTipAmount(int Index, TXSDecimal* _prop_val)
  {  FTipAmount = _prop_val; FTipAmount_Specified = true;  }
  bool __fastcall TipAmount_Specified(int Index)
  {  return FTipAmount_Specified;  }
  void __fastcall SetTotalPaymentAmount(int Index, TXSDecimal* _prop_val)
  {  FTotalPaymentAmount = _prop_val; FTotalPaymentAmount_Specified = true;  }
  bool __fastcall TotalPaymentAmount_Specified(int Index)
  {  return FTotalPaymentAmount_Specified;  }

public:
  __fastcall ~ClippMessageDetail();
__published:
  __property UnicodeString ClippPaymentRef = { index=(IS_OPTN|IS_NLBL), read=FClippPaymentRef, write=SetClippPaymentRef, stored = ClippPaymentRef_Specified };
  __property UnicodeString ClippTabRef = { index=(IS_OPTN|IS_NLBL), read=FClippTabRef, write=SetClippTabRef, stored = ClippTabRef_Specified };
  __property UnicodeString ClippUserRef = { index=(IS_OPTN|IS_NLBL), read=FClippUserRef, write=SetClippUserRef, stored = ClippUserRef_Specified };
  __property TXSDecimal* DiscountAmount = { index=(IS_OPTN), read=FDiscountAmount, write=SetDiscountAmount, stored = DiscountAmount_Specified };
  __property ArrayOfDiscount  Discounts = { index=(IS_OPTN|IS_NLBL), read=FDiscounts, write=SetDiscounts, stored = Discounts_Specified };
  __property UnicodeString  FirstName = { index=(IS_OPTN|IS_NLBL), read=FFirstName, write=SetFirstName, stored = FirstName_Specified };
  __property UnicodeString   LastName = { index=(IS_OPTN|IS_NLBL), read=FLastName, write=SetLastName, stored = LastName_Specified };
  __property UnicodeString   PhotoUrl = { index=(IS_OPTN|IS_NLBL), read=FPhotoUrl, write=SetPhotoUrl, stored = PhotoUrl_Specified };
  __property UnicodeString  PosTabRef = { index=(IS_OPTN|IS_NLBL), read=FPosTabRef, write=SetPosTabRef, stored = PosTabRef_Specified };
  __property TXSDecimal* ProcessingFeeAmount = { index=(IS_OPTN), read=FProcessingFeeAmount, write=SetProcessingFeeAmount, stored = ProcessingFeeAmount_Specified };
  __property TXSDecimal*   TabLimit = { index=(IS_OPTN), read=FTabLimit, write=SetTabLimit, stored = TabLimit_Specified };
  __property TXSDecimal*  TipAmount = { index=(IS_OPTN), read=FTipAmount, write=SetTipAmount, stored = TipAmount_Specified };
  __property TXSDecimal* TotalPaymentAmount = { index=(IS_OPTN), read=FTotalPaymentAmount, write=SetTotalPaymentAmount, stored = TotalPaymentAmount_Specified };
};




// ************************************************************************ //
// XML       : Discount, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class Discount : public TRemotable {
private:
  TXSDecimal*     FAmount;
  bool            FAmount_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FDiscountType;
  bool            FDiscountType_Specified;
  UnicodeString   FReference;
  bool            FReference_Specified;
  void __fastcall SetAmount(int Index, TXSDecimal* _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  }
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  }
  void __fastcall SetDiscountType(int Index, UnicodeString _prop_val)
  {  FDiscountType = _prop_val; FDiscountType_Specified = true;  }
  bool __fastcall DiscountType_Specified(int Index)
  {  return FDiscountType_Specified;  }
  void __fastcall SetReference(int Index, UnicodeString _prop_val)
  {  FReference = _prop_val; FReference_Specified = true;  }
  bool __fastcall Reference_Specified(int Index)
  {  return FReference_Specified;  }

public:
  __fastcall ~Discount();
__published:
  __property TXSDecimal*     Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString DiscountType = { index=(IS_OPTN|IS_NLBL), read=FDiscountType, write=SetDiscountType, stored = DiscountType_Specified };
  __property UnicodeString  Reference = { index=(IS_OPTN|IS_NLBL), read=FReference, write=SetReference, stored = Reference_Specified };
};




// ************************************************************************ //
// XML       : ClippTabDetailRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippTabDetailRequest : public TRemotable {
private:
  TXSDecimal*     FAmount;
  bool            FAmount_Specified;
  UnicodeString   FClippTabRef;
  bool            FClippTabRef_Specified;
  UnicodeString   FMessageId;
  bool            FMessageId_Specified;
  ClippTabDetail* FTabData;
  bool            FTabData_Specified;
  void __fastcall SetAmount(int Index, TXSDecimal* _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  }
  void __fastcall SetClippTabRef(int Index, UnicodeString _prop_val)
  {  FClippTabRef = _prop_val; FClippTabRef_Specified = true;  }
  bool __fastcall ClippTabRef_Specified(int Index)
  {  return FClippTabRef_Specified;  }
  void __fastcall SetMessageId(int Index, UnicodeString _prop_val)
  {  FMessageId = _prop_val; FMessageId_Specified = true;  }
  bool __fastcall MessageId_Specified(int Index)
  {  return FMessageId_Specified;  }
  void __fastcall SetTabData(int Index, ClippTabDetail* _prop_val)
  {  FTabData = _prop_val; FTabData_Specified = true;  }
  bool __fastcall TabData_Specified(int Index)
  {  return FTabData_Specified;  }

public:
  __fastcall ~ClippTabDetailRequest();
__published:
  __property TXSDecimal*     Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString ClippTabRef = { index=(IS_OPTN|IS_NLBL), read=FClippTabRef, write=SetClippTabRef, stored = ClippTabRef_Specified };
  __property UnicodeString  MessageId = { index=(IS_OPTN|IS_NLBL), read=FMessageId, write=SetMessageId, stored = MessageId_Specified };
  __property ClippTabDetail*    TabData = { index=(IS_OPTN|IS_NLBL), read=FTabData, write=SetTabData, stored = TabData_Specified };
};


typedef DynamicArray<Item*>       ArrayOfItem;    /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp"[GblCplx] */
typedef DynamicArray<Payment*>    ArrayOfPayment; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp"[GblCplx] */
typedef DynamicArray<Sundry*>     ArrayOfSundry;  /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp"[GblCplx] */


// ************************************************************************ //
// XML       : ClippTabDetail, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippTabDetail : public TRemotable {
private:
  TXSDecimal*     FIncludedTaxAmount;
  bool            FIncludedTaxAmount_Specified;
  ArrayOfItem     FItems;
  bool            FItems_Specified;
  TXSDecimal*     FOutstandingAmount;
  bool            FOutstandingAmount_Specified;
  ArrayOfPayment  FPayments;
  bool            FPayments_Specified;
  TXSDateTime*    FReceiptDate;
  bool            FReceiptDate_Specified;
  ArrayOfSundry   FSundries;
  bool            FSundries_Specified;
  TXSDecimal*     FTabLimit;
  bool            FTabLimit_Specified;
  TXSDecimal*     FTotalAmount;
  bool            FTotalAmount_Specified;
  UnicodeString   FVenueReceiptId;
  bool            FVenueReceiptId_Specified;
  UnicodeString   FVenueTabRef;
  bool            FVenueTabRef_Specified;
  void __fastcall SetIncludedTaxAmount(int Index, TXSDecimal* _prop_val)
  {  FIncludedTaxAmount = _prop_val; FIncludedTaxAmount_Specified = true;  }
  bool __fastcall IncludedTaxAmount_Specified(int Index)
  {  return FIncludedTaxAmount_Specified;  }
  void __fastcall SetItems(int Index, ArrayOfItem _prop_val)
  {  FItems = _prop_val; FItems_Specified = true;  }
  bool __fastcall Items_Specified(int Index)
  {  return FItems_Specified;  }
  void __fastcall SetOutstandingAmount(int Index, TXSDecimal* _prop_val)
  {  FOutstandingAmount = _prop_val; FOutstandingAmount_Specified = true;  }
  bool __fastcall OutstandingAmount_Specified(int Index)
  {  return FOutstandingAmount_Specified;  }
  void __fastcall SetPayments(int Index, ArrayOfPayment _prop_val)
  {  FPayments = _prop_val; FPayments_Specified = true;  }
  bool __fastcall Payments_Specified(int Index)
  {  return FPayments_Specified;  }
  void __fastcall SetReceiptDate(int Index, TXSDateTime* _prop_val)
  {  FReceiptDate = _prop_val; FReceiptDate_Specified = true;  }
  bool __fastcall ReceiptDate_Specified(int Index)
  {  return FReceiptDate_Specified;  }
  void __fastcall SetSundries(int Index, ArrayOfSundry _prop_val)
  {  FSundries = _prop_val; FSundries_Specified = true;  }
  bool __fastcall Sundries_Specified(int Index)
  {  return FSundries_Specified;  }
  void __fastcall SetTabLimit(int Index, TXSDecimal* _prop_val)
  {  FTabLimit = _prop_val; FTabLimit_Specified = true;  }
  bool __fastcall TabLimit_Specified(int Index)
  {  return FTabLimit_Specified;  }
  void __fastcall SetTotalAmount(int Index, TXSDecimal* _prop_val)
  {  FTotalAmount = _prop_val; FTotalAmount_Specified = true;  }
  bool __fastcall TotalAmount_Specified(int Index)
  {  return FTotalAmount_Specified;  }
  void __fastcall SetVenueReceiptId(int Index, UnicodeString _prop_val)
  {  FVenueReceiptId = _prop_val; FVenueReceiptId_Specified = true;  }
  bool __fastcall VenueReceiptId_Specified(int Index)
  {  return FVenueReceiptId_Specified;  }
  void __fastcall SetVenueTabRef(int Index, UnicodeString _prop_val)
  {  FVenueTabRef = _prop_val; FVenueTabRef_Specified = true;  }
  bool __fastcall VenueTabRef_Specified(int Index)
  {  return FVenueTabRef_Specified;  }

public:
  __fastcall ~ClippTabDetail();
__published:
  __property TXSDecimal* IncludedTaxAmount = { index=(IS_OPTN), read=FIncludedTaxAmount, write=SetIncludedTaxAmount, stored = IncludedTaxAmount_Specified };
  __property ArrayOfItem      Items = { index=(IS_OPTN|IS_NLBL), read=FItems, write=SetItems, stored = Items_Specified };
  __property TXSDecimal* OutstandingAmount = { index=(IS_OPTN), read=FOutstandingAmount, write=SetOutstandingAmount, stored = OutstandingAmount_Specified };
  __property ArrayOfPayment   Payments = { index=(IS_OPTN|IS_NLBL), read=FPayments, write=SetPayments, stored = Payments_Specified };
  __property TXSDateTime* ReceiptDate = { index=(IS_OPTN), read=FReceiptDate, write=SetReceiptDate, stored = ReceiptDate_Specified };
  __property ArrayOfSundry   Sundries = { index=(IS_OPTN|IS_NLBL), read=FSundries, write=SetSundries, stored = Sundries_Specified };
  __property TXSDecimal*   TabLimit = { index=(IS_OPTN), read=FTabLimit, write=SetTabLimit, stored = TabLimit_Specified };
  __property TXSDecimal* TotalAmount = { index=(IS_OPTN), read=FTotalAmount, write=SetTotalAmount, stored = TotalAmount_Specified };
  __property UnicodeString VenueReceiptId = { index=(IS_OPTN|IS_NLBL), read=FVenueReceiptId, write=SetVenueReceiptId, stored = VenueReceiptId_Specified };
  __property UnicodeString VenueTabRef = { index=(IS_OPTN|IS_NLBL), read=FVenueTabRef, write=SetVenueTabRef, stored = VenueTabRef_Specified };
};




// ************************************************************************ //
// XML       : Item, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class Item : public TRemotable {
private:
  UnicodeString   FCategory;
  bool            FCategory_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  TXSDecimal*     FDiscountAmount;
  bool            FDiscountAmount_Specified;
  UnicodeString   FDiscountDescription;
  bool            FDiscountDescription_Specified;
  UnicodeString   FGroup;
  bool            FGroup_Specified;
  bool            FIsTaxInclusive;
  bool            FIsTaxInclusive_Specified;
  TXSDecimal*     FPrice;
  bool            FPrice_Specified;
  TXSDecimal*     FQuantity;
  bool            FQuantity_Specified;
  ArrayOfItem     FSubItems;
  bool            FSubItems_Specified;
  TXSDecimal*     FTotalAmount;
  bool            FTotalAmount_Specified;
  UnicodeString   FVenueItemRef;
  bool            FVenueItemRef_Specified;
  void __fastcall SetCategory(int Index, UnicodeString _prop_val)
  {  FCategory = _prop_val; FCategory_Specified = true;  }
  bool __fastcall Category_Specified(int Index)
  {  return FCategory_Specified;  }
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  }
  void __fastcall SetDiscountAmount(int Index, TXSDecimal* _prop_val)
  {  FDiscountAmount = _prop_val; FDiscountAmount_Specified = true;  }
  bool __fastcall DiscountAmount_Specified(int Index)
  {  return FDiscountAmount_Specified;  }
  void __fastcall SetDiscountDescription(int Index, UnicodeString _prop_val)
  {  FDiscountDescription = _prop_val; FDiscountDescription_Specified = true;  }
  bool __fastcall DiscountDescription_Specified(int Index)
  {  return FDiscountDescription_Specified;  }
  void __fastcall SetGroup(int Index, UnicodeString _prop_val)
  {  FGroup = _prop_val; FGroup_Specified = true;  }
  bool __fastcall Group_Specified(int Index)
  {  return FGroup_Specified;  }
  void __fastcall SetIsTaxInclusive(int Index, bool _prop_val)
  {  FIsTaxInclusive = _prop_val; FIsTaxInclusive_Specified = true;  }
  bool __fastcall IsTaxInclusive_Specified(int Index)
  {  return FIsTaxInclusive_Specified;  }
  void __fastcall SetPrice(int Index, TXSDecimal* _prop_val)
  {  FPrice = _prop_val; FPrice_Specified = true;  }
  bool __fastcall Price_Specified(int Index)
  {  return FPrice_Specified;  }
  void __fastcall SetQuantity(int Index, TXSDecimal* _prop_val)
  {  FQuantity = _prop_val; FQuantity_Specified = true;  }
  bool __fastcall Quantity_Specified(int Index)
  {  return FQuantity_Specified;  }
  void __fastcall SetSubItems(int Index, ArrayOfItem _prop_val)
  {  FSubItems = _prop_val; FSubItems_Specified = true;  }
  bool __fastcall SubItems_Specified(int Index)
  {  return FSubItems_Specified;  }
  void __fastcall SetTotalAmount(int Index, TXSDecimal* _prop_val)
  {  FTotalAmount = _prop_val; FTotalAmount_Specified = true;  }
  bool __fastcall TotalAmount_Specified(int Index)
  {  return FTotalAmount_Specified;  }
  void __fastcall SetVenueItemRef(int Index, UnicodeString _prop_val)
  {  FVenueItemRef = _prop_val; FVenueItemRef_Specified = true;  }
  bool __fastcall VenueItemRef_Specified(int Index)
  {  return FVenueItemRef_Specified;  }

public:
  __fastcall ~Item();
__published:
  __property UnicodeString   Category = { index=(IS_OPTN|IS_NLBL), read=FCategory, write=SetCategory, stored = Category_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property TXSDecimal* DiscountAmount = { index=(IS_OPTN), read=FDiscountAmount, write=SetDiscountAmount, stored = DiscountAmount_Specified };
  __property UnicodeString DiscountDescription = { index=(IS_OPTN|IS_NLBL), read=FDiscountDescription, write=SetDiscountDescription, stored = DiscountDescription_Specified };
  __property UnicodeString      Group = { index=(IS_OPTN|IS_NLBL), read=FGroup, write=SetGroup, stored = Group_Specified };
  __property bool       IsTaxInclusive = { index=(IS_OPTN), read=FIsTaxInclusive, write=SetIsTaxInclusive, stored = IsTaxInclusive_Specified };
  __property TXSDecimal*      Price = { index=(IS_OPTN), read=FPrice, write=SetPrice, stored = Price_Specified };
  __property TXSDecimal*   Quantity = { index=(IS_OPTN), read=FQuantity, write=SetQuantity, stored = Quantity_Specified };
  __property ArrayOfItem   SubItems = { index=(IS_OPTN|IS_NLBL), read=FSubItems, write=SetSubItems, stored = SubItems_Specified };
  __property TXSDecimal* TotalAmount = { index=(IS_OPTN), read=FTotalAmount, write=SetTotalAmount, stored = TotalAmount_Specified };
  __property UnicodeString VenueItemRef = { index=(IS_OPTN|IS_NLBL), read=FVenueItemRef, write=SetVenueItemRef, stored = VenueItemRef_Specified };
};




// ************************************************************************ //
// XML       : Payment, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class Payment : public TRemotable {
private:
  TXSDecimal*     FAmount;
  bool            FAmount_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FVenuePaymentRef;
  bool            FVenuePaymentRef_Specified;
  void __fastcall SetAmount(int Index, TXSDecimal* _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  }
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  }
  void __fastcall SetVenuePaymentRef(int Index, UnicodeString _prop_val)
  {  FVenuePaymentRef = _prop_val; FVenuePaymentRef_Specified = true;  }
  bool __fastcall VenuePaymentRef_Specified(int Index)
  {  return FVenuePaymentRef_Specified;  }

public:
  __fastcall ~Payment();
__published:
  __property TXSDecimal*     Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString VenuePaymentRef = { index=(IS_OPTN|IS_NLBL), read=FVenuePaymentRef, write=SetVenuePaymentRef, stored = VenuePaymentRef_Specified };
};




// ************************************************************************ //
// XML       : Sundry, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class Sundry : public TRemotable {
private:
  TXSDecimal*     FAmount;
  bool            FAmount_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FVenueSundryRef;
  bool            FVenueSundryRef_Specified;
  void __fastcall SetAmount(int Index, TXSDecimal* _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  }
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  }
  void __fastcall SetVenueSundryRef(int Index, UnicodeString _prop_val)
  {  FVenueSundryRef = _prop_val; FVenueSundryRef_Specified = true;  }
  bool __fastcall VenueSundryRef_Specified(int Index)
  {  return FVenueSundryRef_Specified;  }

public:
  __fastcall ~Sundry();
__published:
  __property TXSDecimal*     Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString VenueSundryRef = { index=(IS_OPTN|IS_NLBL), read=FVenueSundryRef, write=SetVenueSundryRef, stored = VenueSundryRef_Specified };
};




// ************************************************************************ //
// XML       : ClippErrorDetail, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippErrorDetail : public TRemotable {
private:
  UnicodeString   FClippTabRef;
  bool            FClippTabRef_Specified;
  UnicodeString   FErrorCode;
  bool            FErrorCode_Specified;
  UnicodeString   FErrorDescription;
  bool            FErrorDescription_Specified;
  UnicodeString   FMessageId;
  bool            FMessageId_Specified;
  TXSDecimal*     FTotalAmount;
  bool            FTotalAmount_Specified;
  void __fastcall SetClippTabRef(int Index, UnicodeString _prop_val)
  {  FClippTabRef = _prop_val; FClippTabRef_Specified = true;  }
  bool __fastcall ClippTabRef_Specified(int Index)
  {  return FClippTabRef_Specified;  }
  void __fastcall SetErrorCode(int Index, UnicodeString _prop_val)
  {  FErrorCode = _prop_val; FErrorCode_Specified = true;  }
  bool __fastcall ErrorCode_Specified(int Index)
  {  return FErrorCode_Specified;  }
  void __fastcall SetErrorDescription(int Index, UnicodeString _prop_val)
  {  FErrorDescription = _prop_val; FErrorDescription_Specified = true;  }
  bool __fastcall ErrorDescription_Specified(int Index)
  {  return FErrorDescription_Specified;  }
  void __fastcall SetMessageId(int Index, UnicodeString _prop_val)
  {  FMessageId = _prop_val; FMessageId_Specified = true;  }
  bool __fastcall MessageId_Specified(int Index)
  {  return FMessageId_Specified;  }
  void __fastcall SetTotalAmount(int Index, TXSDecimal* _prop_val)
  {  FTotalAmount = _prop_val; FTotalAmount_Specified = true;  }
  bool __fastcall TotalAmount_Specified(int Index)
  {  return FTotalAmount_Specified;  }

public:
  __fastcall ~ClippErrorDetail();
__published:
  __property UnicodeString ClippTabRef = { index=(IS_OPTN|IS_NLBL), read=FClippTabRef, write=SetClippTabRef, stored = ClippTabRef_Specified };
  __property UnicodeString  ErrorCode = { index=(IS_OPTN|IS_NLBL), read=FErrorCode, write=SetErrorCode, stored = ErrorCode_Specified };
  __property UnicodeString ErrorDescription = { index=(IS_OPTN|IS_NLBL), read=FErrorDescription, write=SetErrorDescription, stored = ErrorDescription_Specified };
  __property UnicodeString  MessageId = { index=(IS_OPTN|IS_NLBL), read=FMessageId, write=SetMessageId, stored = MessageId_Specified };
  __property TXSDecimal* TotalAmount = { index=(IS_OPTN), read=FTotalAmount, write=SetTotalAmount, stored = TotalAmount_Specified };
};




// ************************************************************************ //
// XML       : PaymentDetailRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class PaymentDetailRequest : public TRemotable {
private:
  TXSDecimal*     FAmount;
  bool            FAmount_Specified;
  UnicodeString   FClippMessageId;
  bool            FClippMessageId_Specified;
  UnicodeString   FClippTabRef;
  bool            FClippTabRef_Specified;
  UnicodeString   FErrorData;
  bool            FErrorData_Specified;
  void __fastcall SetAmount(int Index, TXSDecimal* _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  }
  void __fastcall SetClippMessageId(int Index, UnicodeString _prop_val)
  {  FClippMessageId = _prop_val; FClippMessageId_Specified = true;  }
  bool __fastcall ClippMessageId_Specified(int Index)
  {  return FClippMessageId_Specified;  }
  void __fastcall SetClippTabRef(int Index, UnicodeString _prop_val)
  {  FClippTabRef = _prop_val; FClippTabRef_Specified = true;  }
  bool __fastcall ClippTabRef_Specified(int Index)
  {  return FClippTabRef_Specified;  }
  void __fastcall SetErrorData(int Index, UnicodeString _prop_val)
  {  FErrorData = _prop_val; FErrorData_Specified = true;  }
  bool __fastcall ErrorData_Specified(int Index)
  {  return FErrorData_Specified;  }

public:
  __fastcall ~PaymentDetailRequest();
__published:
  __property TXSDecimal*     Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString ClippMessageId = { index=(IS_OPTN|IS_NLBL), read=FClippMessageId, write=SetClippMessageId, stored = ClippMessageId_Specified };
  __property UnicodeString ClippTabRef = { index=(IS_OPTN|IS_NLBL), read=FClippTabRef, write=SetClippTabRef, stored = ClippTabRef_Specified };
  __property UnicodeString  ErrorData = { index=(IS_OPTN|IS_NLBL), read=FErrorData, write=SetErrorData, stored = ErrorData_Specified };
};




// ************************************************************************ //
// XML       : PaymentDetailResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class PaymentDetailResponse : public TRemotable {
private:
  UnicodeString   FClippPaymentRef;
  bool            FClippPaymentRef_Specified;
  UnicodeString   FClippTabRef;
  bool            FClippTabRef_Specified;
  UnicodeString   FPosTabRef;
  bool            FPosTabRef_Specified;
  TXSDecimal*     FProcessingFeeAmount;
  bool            FProcessingFeeAmount_Specified;
  TXSDecimal*     FTotalPaymentAmount;
  bool            FTotalPaymentAmount_Specified;
  void __fastcall SetClippPaymentRef(int Index, UnicodeString _prop_val)
  {  FClippPaymentRef = _prop_val; FClippPaymentRef_Specified = true;  }
  bool __fastcall ClippPaymentRef_Specified(int Index)
  {  return FClippPaymentRef_Specified;  }
  void __fastcall SetClippTabRef(int Index, UnicodeString _prop_val)
  {  FClippTabRef = _prop_val; FClippTabRef_Specified = true;  }
  bool __fastcall ClippTabRef_Specified(int Index)
  {  return FClippTabRef_Specified;  }
  void __fastcall SetPosTabRef(int Index, UnicodeString _prop_val)
  {  FPosTabRef = _prop_val; FPosTabRef_Specified = true;  }
  bool __fastcall PosTabRef_Specified(int Index)
  {  return FPosTabRef_Specified;  }
  void __fastcall SetProcessingFeeAmount(int Index, TXSDecimal* _prop_val)
  {  FProcessingFeeAmount = _prop_val; FProcessingFeeAmount_Specified = true;  }
  bool __fastcall ProcessingFeeAmount_Specified(int Index)
  {  return FProcessingFeeAmount_Specified;  }
  void __fastcall SetTotalPaymentAmount(int Index, TXSDecimal* _prop_val)
  {  FTotalPaymentAmount = _prop_val; FTotalPaymentAmount_Specified = true;  }
  bool __fastcall TotalPaymentAmount_Specified(int Index)
  {  return FTotalPaymentAmount_Specified;  }

public:
  __fastcall ~PaymentDetailResponse();
__published:
  __property UnicodeString ClippPaymentRef = { index=(IS_OPTN|IS_NLBL), read=FClippPaymentRef, write=SetClippPaymentRef, stored = ClippPaymentRef_Specified };
  __property UnicodeString ClippTabRef = { index=(IS_OPTN|IS_NLBL), read=FClippTabRef, write=SetClippTabRef, stored = ClippTabRef_Specified };
  __property UnicodeString  PosTabRef = { index=(IS_OPTN|IS_NLBL), read=FPosTabRef, write=SetPosTabRef, stored = PosTabRef_Specified };
  __property TXSDecimal* ProcessingFeeAmount = { index=(IS_OPTN), read=FProcessingFeeAmount, write=SetProcessingFeeAmount, stored = ProcessingFeeAmount_Specified };
  __property TXSDecimal* TotalPaymentAmount = { index=(IS_OPTN), read=FTotalPaymentAmount, write=SetTotalPaymentAmount, stored = TotalPaymentAmount_Specified };
};




// ************************************************************************ //
// XML       : ClippMessage, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippMessage2 : public ClippMessage {
private:
__published:
};




// ************************************************************************ //
// XML       : ClippMessageDetail, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippMessageDetail2 : public ClippMessageDetail {
private:
__published:
};




// ************************************************************************ //
// XML       : Discount, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class Discount2 : public Discount {
private:
__published:
};




// ************************************************************************ //
// XML       : ClippTabDetailRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippTabDetailRequest2 : public ClippTabDetailRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : ClippTabDetail, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippTabDetail2 : public ClippTabDetail {
private:
__published:
};




// ************************************************************************ //
// XML       : Item, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class Item2 : public Item {
private:
__published:
};




// ************************************************************************ //
// XML       : Payment, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class Payment2 : public Payment {
private:
__published:
};




// ************************************************************************ //
// XML       : Sundry, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class Sundry2 : public Sundry {
private:
__published:
};




// ************************************************************************ //
// XML       : ClippErrorDetail, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class ClippErrorDetail2 : public ClippErrorDetail {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentDetailRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class PaymentDetailRequest2 : public PaymentDetailRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentDetailResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp
// ************************************************************************ //
class PaymentDetailResponse2 : public PaymentDetailResponse {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: |http://tempuri.org/IClippIntergrationWebService/GetOpenMessages|http://tempuri.org/IClippIntergrationWebService/GetPreviousMessages|http://tempuri.org/IClippIntergrationWebService/UpdateTabDetails|UpdateTabDetailsOnError|http://tempuri.org/IClippIntergrationWebService/CloseTab|CloseTabOnError|http://tempuri.org/IClippIntergrationWebService/RequestTabPayment|http://tempuri.org/IClippIntergrationWebService/ForceCloseAllTabs
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceClipp
// service   : ClippIntergrationWebService
// port      : basicHttpBinding_ServiceClipp
// URL       : http://localhost:8736/MenumateServices/ClippService/
// ************************************************************************ //
__interface INTERFACE_UUID("{FBB914B0-3242-2929-5120-0EFCDF95F65A}") IClippIntergrationWebService : public IInvokable
{
public:
  virtual ArrayOfClippMessage GetOpenMessages() = 0;
  virtual ArrayOfClippMessage GetPreviousMessages() = 0;
  virtual void            UpdateTabDetails(const ClippTabDetailRequest* clippTabDetail) = 0;
  virtual void            UpdateTabDetailsOnError(const ClippErrorDetail* clippErrorDetail) = 0;
  virtual ClippMessageDetail* CloseTab(const ClippTabDetailRequest* clippTabDetail) = 0;
  virtual void            CloseTabOnError(const ClippErrorDetail* clippErrorDetail) = 0;
  virtual PaymentDetailResponse* RequestTabPayment(const PaymentDetailRequest* paymentDetailRequest) = 0;
  virtual bool            ForceCloseAllTabs() = 0;
};
typedef DelphiInterface<IClippIntergrationWebService> _di_IClippIntergrationWebService;

_di_IClippIntergrationWebService GetIClippIntergrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS_ClippIntegrationWSDL
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_ClippIntegrationWSDL;
#endif



#endif // _H
