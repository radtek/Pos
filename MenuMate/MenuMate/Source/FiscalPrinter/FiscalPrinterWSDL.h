// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8744/MenumateServices/FiscalPrinter/?wsdl
//  >Import : http://localhost:8744/MenumateServices/FiscalPrinter/?wsdl>0
//  >Import : http://localhost:8744/MenumateServices/FiscalPrinter/?xsd=xsd0
//  >Import : http://localhost:8744/MenumateServices/FiscalPrinter/?xsd=xsd2
//  >Import : http://localhost:8744/MenumateServices/FiscalPrinter/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (13/03/2018 7:45:09 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   FiscalPrinterWSDLH
#define   FiscalPrinterWSDLH

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

namespace NS__FIscalPrinterWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS FiscalDataDetails;
class SOAP_REMOTABLE_CLASS DiscountDetails;
class SOAP_REMOTABLE_CLASS FiscalService;
class SOAP_REMOTABLE_CLASS FiscalPayment;
class SOAP_REMOTABLE_CLASS FiscalResponseDetails;
class SOAP_REMOTABLE_CLASS FiscalDataDetails2;
class SOAP_REMOTABLE_CLASS DiscountDetails2;
class SOAP_REMOTABLE_CLASS FiscalService2;
class SOAP_REMOTABLE_CLASS FiscalPayment2;
class SOAP_REMOTABLE_CLASS FiscalResponseDetails2;

typedef DynamicArray<DiscountDetails*> ArrayOfDiscountDetails; /* "http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain"[GblCplx] */
typedef DynamicArray<FiscalService*> ArrayOfFiscalService; /* "http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain"[GblCplx] */
typedef DynamicArray<FiscalPayment*> ArrayOfFiscalPayment; /* "http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : FiscalDataDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class FiscalDataDetails : public TRemotable {
private:
  UnicodeString   FBillno;
  bool            FBillno_Specified;
  UnicodeString   FCashier;
  bool            FCashier_Specified;
  UnicodeString   FCashno;
  bool            FCashno_Specified;
  UnicodeString   FDate;
  bool            FDate_Specified;
  ArrayOfDiscountDetails FDiscountList;
  bool            FDiscountList_Specified;
  UnicodeString   FInvoiceNumber;
  bool            FInvoiceNumber_Specified;
  ArrayOfFiscalService FItemList;
  bool            FItemList_Specified;
  ArrayOfFiscalPayment FPaymentList;
  bool            FPaymentList_Specified;
  UnicodeString   FSource;
  bool            FSource_Specified;
  UnicodeString   FTime;
  bool            FTime_Specified;
  void __fastcall SetBillno(int Index, UnicodeString _prop_val)
  {  FBillno = _prop_val; FBillno_Specified = true;  }
  bool __fastcall Billno_Specified(int Index)
  {  return FBillno_Specified;  } 
  void __fastcall SetCashier(int Index, UnicodeString _prop_val)
  {  FCashier = _prop_val; FCashier_Specified = true;  }
  bool __fastcall Cashier_Specified(int Index)
  {  return FCashier_Specified;  } 
  void __fastcall SetCashno(int Index, UnicodeString _prop_val)
  {  FCashno = _prop_val; FCashno_Specified = true;  }
  bool __fastcall Cashno_Specified(int Index)
  {  return FCashno_Specified;  } 
  void __fastcall SetDate(int Index, UnicodeString _prop_val)
  {  FDate = _prop_val; FDate_Specified = true;  }
  bool __fastcall Date_Specified(int Index)
  {  return FDate_Specified;  } 
  void __fastcall SetDiscountList(int Index, ArrayOfDiscountDetails _prop_val)
  {  FDiscountList = _prop_val; FDiscountList_Specified = true;  }
  bool __fastcall DiscountList_Specified(int Index)
  {  return FDiscountList_Specified;  } 
  void __fastcall SetInvoiceNumber(int Index, UnicodeString _prop_val)
  {  FInvoiceNumber = _prop_val; FInvoiceNumber_Specified = true;  }
  bool __fastcall InvoiceNumber_Specified(int Index)
  {  return FInvoiceNumber_Specified;  } 
  void __fastcall SetItemList(int Index, ArrayOfFiscalService _prop_val)
  {  FItemList = _prop_val; FItemList_Specified = true;  }
  bool __fastcall ItemList_Specified(int Index)
  {  return FItemList_Specified;  } 
  void __fastcall SetPaymentList(int Index, ArrayOfFiscalPayment _prop_val)
  {  FPaymentList = _prop_val; FPaymentList_Specified = true;  }
  bool __fastcall PaymentList_Specified(int Index)
  {  return FPaymentList_Specified;  } 
  void __fastcall SetSource(int Index, UnicodeString _prop_val)
  {  FSource = _prop_val; FSource_Specified = true;  }
  bool __fastcall Source_Specified(int Index)
  {  return FSource_Specified;  } 
  void __fastcall SetTime(int Index, UnicodeString _prop_val)
  {  FTime = _prop_val; FTime_Specified = true;  }
  bool __fastcall Time_Specified(int Index)
  {  return FTime_Specified;  } 

public:
  __fastcall ~FiscalDataDetails();
__published:
  __property UnicodeString     Billno = { index=(IS_OPTN|IS_NLBL), read=FBillno, write=SetBillno, stored = Billno_Specified };
  __property UnicodeString    Cashier = { index=(IS_OPTN|IS_NLBL), read=FCashier, write=SetCashier, stored = Cashier_Specified };
  __property UnicodeString     Cashno = { index=(IS_OPTN|IS_NLBL), read=FCashno, write=SetCashno, stored = Cashno_Specified };
  __property UnicodeString       Date = { index=(IS_OPTN|IS_NLBL), read=FDate, write=SetDate, stored = Date_Specified };
  __property ArrayOfDiscountDetails DiscountList = { index=(IS_OPTN|IS_NLBL), read=FDiscountList, write=SetDiscountList, stored = DiscountList_Specified };
  __property UnicodeString InvoiceNumber = { index=(IS_OPTN|IS_NLBL), read=FInvoiceNumber, write=SetInvoiceNumber, stored = InvoiceNumber_Specified };
  __property ArrayOfFiscalService   ItemList = { index=(IS_OPTN|IS_NLBL), read=FItemList, write=SetItemList, stored = ItemList_Specified };
  __property ArrayOfFiscalPayment PaymentList = { index=(IS_OPTN|IS_NLBL), read=FPaymentList, write=SetPaymentList, stored = PaymentList_Specified };
  __property UnicodeString     Source = { index=(IS_OPTN|IS_NLBL), read=FSource, write=SetSource, stored = Source_Specified };
  __property UnicodeString       Time = { index=(IS_OPTN|IS_NLBL), read=FTime, write=SetTime, stored = Time_Specified };
};




// ************************************************************************ //
// XML       : DiscountDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class DiscountDetails : public TRemotable {
private:
  UnicodeString   FAmount;
  bool            FAmount_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FDiscountGroup;
  bool            FDiscountGroup_Specified;
  UnicodeString   FDiscountMode;
  bool            FDiscountMode_Specified;
  UnicodeString   FType;
  bool            FType_Specified;
  void __fastcall SetAmount(int Index, UnicodeString _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetDiscountGroup(int Index, UnicodeString _prop_val)
  {  FDiscountGroup = _prop_val; FDiscountGroup_Specified = true;  }
  bool __fastcall DiscountGroup_Specified(int Index)
  {  return FDiscountGroup_Specified;  } 
  void __fastcall SetDiscountMode(int Index, UnicodeString _prop_val)
  {  FDiscountMode = _prop_val; FDiscountMode_Specified = true;  }
  bool __fastcall DiscountMode_Specified(int Index)
  {  return FDiscountMode_Specified;  } 
  void __fastcall SetType(int Index, UnicodeString _prop_val)
  {  FType = _prop_val; FType_Specified = true;  }
  bool __fastcall Type_Specified(int Index)
  {  return FType_Specified;  } 
__published:
  __property UnicodeString     Amount = { index=(IS_OPTN|IS_NLBL), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString DiscountGroup = { index=(IS_OPTN|IS_NLBL), read=FDiscountGroup, write=SetDiscountGroup, stored = DiscountGroup_Specified };
  __property UnicodeString DiscountMode = { index=(IS_OPTN|IS_NLBL), read=FDiscountMode, write=SetDiscountMode, stored = DiscountMode_Specified };
  __property UnicodeString       Type = { index=(IS_OPTN|IS_NLBL), read=FType, write=SetType, stored = Type_Specified };
};




// ************************************************************************ //
// XML       : FiscalService, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class FiscalService : public TRemotable {
private:
  UnicodeString   FChitNumber;
  bool            FChitNumber_Specified;
  UnicodeString   FGuestName;
  bool            FGuestName_Specified;
  UnicodeString   FItemCategory;
  bool            FItemCategory_Specified;
  UnicodeString   FItemDescription;
  bool            FItemDescription_Specified;
  UnicodeString   FMemberName;
  bool            FMemberName_Specified;
  UnicodeString   FPartyName;
  bool            FPartyName_Specified;
  UnicodeString   FPricePerUnit;
  bool            FPricePerUnit_Specified;
  UnicodeString   FPriceTotal;
  bool            FPriceTotal_Specified;
  UnicodeString   FSizeName;
  bool            FSizeName_Specified;
  UnicodeString   FTableNo;
  bool            FTableNo_Specified;
  UnicodeString   FVATPercentage;
  bool            FVATPercentage_Specified;
  UnicodeString   Fquantity;
  bool            Fquantity_Specified;
  void __fastcall SetChitNumber(int Index, UnicodeString _prop_val)
  {  FChitNumber = _prop_val; FChitNumber_Specified = true;  }
  bool __fastcall ChitNumber_Specified(int Index)
  {  return FChitNumber_Specified;  } 
  void __fastcall SetGuestName(int Index, UnicodeString _prop_val)
  {  FGuestName = _prop_val; FGuestName_Specified = true;  }
  bool __fastcall GuestName_Specified(int Index)
  {  return FGuestName_Specified;  } 
  void __fastcall SetItemCategory(int Index, UnicodeString _prop_val)
  {  FItemCategory = _prop_val; FItemCategory_Specified = true;  }
  bool __fastcall ItemCategory_Specified(int Index)
  {  return FItemCategory_Specified;  } 
  void __fastcall SetItemDescription(int Index, UnicodeString _prop_val)
  {  FItemDescription = _prop_val; FItemDescription_Specified = true;  }
  bool __fastcall ItemDescription_Specified(int Index)
  {  return FItemDescription_Specified;  } 
  void __fastcall SetMemberName(int Index, UnicodeString _prop_val)
  {  FMemberName = _prop_val; FMemberName_Specified = true;  }
  bool __fastcall MemberName_Specified(int Index)
  {  return FMemberName_Specified;  } 
  void __fastcall SetPartyName(int Index, UnicodeString _prop_val)
  {  FPartyName = _prop_val; FPartyName_Specified = true;  }
  bool __fastcall PartyName_Specified(int Index)
  {  return FPartyName_Specified;  } 
  void __fastcall SetPricePerUnit(int Index, UnicodeString _prop_val)
  {  FPricePerUnit = _prop_val; FPricePerUnit_Specified = true;  }
  bool __fastcall PricePerUnit_Specified(int Index)
  {  return FPricePerUnit_Specified;  } 
  void __fastcall SetPriceTotal(int Index, UnicodeString _prop_val)
  {  FPriceTotal = _prop_val; FPriceTotal_Specified = true;  }
  bool __fastcall PriceTotal_Specified(int Index)
  {  return FPriceTotal_Specified;  } 
  void __fastcall SetSizeName(int Index, UnicodeString _prop_val)
  {  FSizeName = _prop_val; FSizeName_Specified = true;  }
  bool __fastcall SizeName_Specified(int Index)
  {  return FSizeName_Specified;  } 
  void __fastcall SetTableNo(int Index, UnicodeString _prop_val)
  {  FTableNo = _prop_val; FTableNo_Specified = true;  }
  bool __fastcall TableNo_Specified(int Index)
  {  return FTableNo_Specified;  } 
  void __fastcall SetVATPercentage(int Index, UnicodeString _prop_val)
  {  FVATPercentage = _prop_val; FVATPercentage_Specified = true;  }
  bool __fastcall VATPercentage_Specified(int Index)
  {  return FVATPercentage_Specified;  } 
  void __fastcall Setquantity(int Index, UnicodeString _prop_val)
  {  Fquantity = _prop_val; Fquantity_Specified = true;  }
  bool __fastcall quantity_Specified(int Index)
  {  return Fquantity_Specified;  } 
__published:
  __property UnicodeString ChitNumber = { index=(IS_OPTN|IS_NLBL), read=FChitNumber, write=SetChitNumber, stored = ChitNumber_Specified };
  __property UnicodeString  GuestName = { index=(IS_OPTN|IS_NLBL), read=FGuestName, write=SetGuestName, stored = GuestName_Specified };
  __property UnicodeString ItemCategory = { index=(IS_OPTN|IS_NLBL), read=FItemCategory, write=SetItemCategory, stored = ItemCategory_Specified };
  __property UnicodeString ItemDescription = { index=(IS_OPTN|IS_NLBL), read=FItemDescription, write=SetItemDescription, stored = ItemDescription_Specified };
  __property UnicodeString MemberName = { index=(IS_OPTN|IS_NLBL), read=FMemberName, write=SetMemberName, stored = MemberName_Specified };
  __property UnicodeString  PartyName = { index=(IS_OPTN|IS_NLBL), read=FPartyName, write=SetPartyName, stored = PartyName_Specified };
  __property UnicodeString PricePerUnit = { index=(IS_OPTN|IS_NLBL), read=FPricePerUnit, write=SetPricePerUnit, stored = PricePerUnit_Specified };
  __property UnicodeString PriceTotal = { index=(IS_OPTN|IS_NLBL), read=FPriceTotal, write=SetPriceTotal, stored = PriceTotal_Specified };
  __property UnicodeString   SizeName = { index=(IS_OPTN|IS_NLBL), read=FSizeName, write=SetSizeName, stored = SizeName_Specified };
  __property UnicodeString    TableNo = { index=(IS_OPTN|IS_NLBL), read=FTableNo, write=SetTableNo, stored = TableNo_Specified };
  __property UnicodeString VATPercentage = { index=(IS_OPTN|IS_NLBL), read=FVATPercentage, write=SetVATPercentage, stored = VATPercentage_Specified };
  __property UnicodeString   quantity = { index=(IS_OPTN|IS_NLBL), read=Fquantity, write=Setquantity, stored = quantity_Specified };
};




// ************************************************************************ //
// XML       : FiscalPayment, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class FiscalPayment : public TRemotable {
private:
  UnicodeString   FAmount;
  bool            FAmount_Specified;
  UnicodeString   FBillno;
  bool            FBillno_Specified;
  UnicodeString   FCashier;
  bool            FCashier_Specified;
  UnicodeString   FCashno;
  bool            FCashno_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FSource;
  bool            FSource_Specified;
  UnicodeString   FType;
  bool            FType_Specified;
  void __fastcall SetAmount(int Index, UnicodeString _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  } 
  void __fastcall SetBillno(int Index, UnicodeString _prop_val)
  {  FBillno = _prop_val; FBillno_Specified = true;  }
  bool __fastcall Billno_Specified(int Index)
  {  return FBillno_Specified;  } 
  void __fastcall SetCashier(int Index, UnicodeString _prop_val)
  {  FCashier = _prop_val; FCashier_Specified = true;  }
  bool __fastcall Cashier_Specified(int Index)
  {  return FCashier_Specified;  } 
  void __fastcall SetCashno(int Index, UnicodeString _prop_val)
  {  FCashno = _prop_val; FCashno_Specified = true;  }
  bool __fastcall Cashno_Specified(int Index)
  {  return FCashno_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetSource(int Index, UnicodeString _prop_val)
  {  FSource = _prop_val; FSource_Specified = true;  }
  bool __fastcall Source_Specified(int Index)
  {  return FSource_Specified;  } 
  void __fastcall SetType(int Index, UnicodeString _prop_val)
  {  FType = _prop_val; FType_Specified = true;  }
  bool __fastcall Type_Specified(int Index)
  {  return FType_Specified;  } 
__published:
  __property UnicodeString     Amount = { index=(IS_OPTN|IS_NLBL), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString     Billno = { index=(IS_OPTN|IS_NLBL), read=FBillno, write=SetBillno, stored = Billno_Specified };
  __property UnicodeString    Cashier = { index=(IS_OPTN|IS_NLBL), read=FCashier, write=SetCashier, stored = Cashier_Specified };
  __property UnicodeString     Cashno = { index=(IS_OPTN|IS_NLBL), read=FCashno, write=SetCashno, stored = Cashno_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString     Source = { index=(IS_OPTN|IS_NLBL), read=FSource, write=SetSource, stored = Source_Specified };
  __property UnicodeString       Type = { index=(IS_OPTN|IS_NLBL), read=FType, write=SetType, stored = Type_Specified };
};




// ************************************************************************ //
// XML       : FiscalResponseDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class FiscalResponseDetails : public TRemotable {
private:
  bool            FIsSuccessful;
  bool            FIsSuccessful_Specified;
  UnicodeString   FResponse;
  bool            FResponse_Specified;
  void __fastcall SetIsSuccessful(int Index, bool _prop_val)
  {  FIsSuccessful = _prop_val; FIsSuccessful_Specified = true;  }
  bool __fastcall IsSuccessful_Specified(int Index)
  {  return FIsSuccessful_Specified;  } 
  void __fastcall SetResponse(int Index, UnicodeString _prop_val)
  {  FResponse = _prop_val; FResponse_Specified = true;  }
  bool __fastcall Response_Specified(int Index)
  {  return FResponse_Specified;  } 
__published:
  __property bool       IsSuccessful = { index=(IS_OPTN), read=FIsSuccessful, write=SetIsSuccessful, stored = IsSuccessful_Specified };
  __property UnicodeString   Response = { index=(IS_OPTN|IS_NLBL), read=FResponse, write=SetResponse, stored = Response_Specified };
};




// ************************************************************************ //
// XML       : FiscalDataDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class FiscalDataDetails2 : public FiscalDataDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : DiscountDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class DiscountDetails2 : public DiscountDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : FiscalService, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class FiscalService2 : public FiscalService {
private:
__published:
};




// ************************************************************************ //
// XML       : FiscalPayment, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class FiscalPayment2 : public FiscalPayment {
private:
__published:
};




// ************************************************************************ //
// XML       : FiscalResponseDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain
// ************************************************************************ //
class FiscalResponseDetails2 : public FiscalResponseDetails {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IWCFServiceFiscalPrinter/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceFiscalPrinter
// service   : WCFServiceFiscalPrinter
// port      : basicHttpBinding_ServiceFiscalPrinter
// URL       : http://localhost:8744/MenumateServices/FiscalPrinter/
// ************************************************************************ //
__interface INTERFACE_UUID("{BCA88294-6E3E-74DE-222B-1B2239432EB1}") IWCFServiceFiscalPrinter : public IInvokable
{
public:
  virtual FiscalResponseDetails* PrintFiscalReceipt(const FiscalDataDetails* fiscalDataDetails) = 0; 
  virtual FiscalResponseDetails* PrintZSettlement() = 0; 
};
typedef DelphiInterface<IWCFServiceFiscalPrinter> _di_IWCFServiceFiscalPrinter;

_di_IWCFServiceFiscalPrinter GetIWCFServiceFiscalPrinter(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__FIscalPrinterWSDL;
#endif



#endif // _H
