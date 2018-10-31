// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8745/MenumateServices/AdyenService/?wsdl
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?wsdl>0
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd0
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd2
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd3
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd5
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd8
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd4
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd6
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd7
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd9
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd10
//  >Import : http://localhost:8745/MenumateServices/AdyenService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (31/10/2018 10:53:43 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   AdyenWSDLH
#define   AdyenWSDLH

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


namespace NS__AdyenWSDL {

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

class SOAP_REMOTABLE_CLASS SaleSoftware;
class SOAP_REMOTABLE_CLASS SaleTerminalData;
class SOAP_REMOTABLE_CLASS SaleProfile;
class SOAP_REMOTABLE_CLASS PaymentRequest;
class SOAP_REMOTABLE_CLASS PaymentData;
class SOAP_REMOTABLE_CLASS PaymentTransaction;
class SOAP_REMOTABLE_CLASS SaleData;
class SOAP_REMOTABLE_CLASS AmountsReq;
class SOAP_REMOTABLE_CLASS SaleTransactionID;
class SOAP_REMOTABLE_CLASS ReversalRequest;
class SOAP_REMOTABLE_CLASS OriginalPOITransaction;
class SOAP_REMOTABLE_CLASS POITransactionID;
class SOAP_REMOTABLE_CLASS Response;
class SOAP_REMOTABLE_CLASS POIData;
class SOAP_REMOTABLE_CLASS PaymentResponse;
class SOAP_REMOTABLE_CLASS PaymentResult;
class SOAP_REMOTABLE_CLASS PaymentReceipt;
class SOAP_REMOTABLE_CLASS OutputContent;
class SOAP_REMOTABLE_CLASS OutputText;
class SOAP_REMOTABLE_CLASS AmountsResp;
class SOAP_REMOTABLE_CLASS PaymentAcquirerData;
class SOAP_REMOTABLE_CLASS PaymentInstrumentData;
class SOAP_REMOTABLE_CLASS AcquirerTransactionID;
class SOAP_REMOTABLE_CLASS CardData;
class SOAP_REMOTABLE_CLASS PaymentToken;
class SOAP_REMOTABLE_CLASS SensitiveCardData;
class SOAP_REMOTABLE_CLASS AllowedProduct;
class SOAP_REMOTABLE_CLASS CustomerOrder;
class SOAP_REMOTABLE_CLASS TrackData;
class SOAP_REMOTABLE_CLASS SaleSoftware2;
class SOAP_REMOTABLE_CLASS SaleTerminalData2;
class SOAP_REMOTABLE_CLASS SaleProfile2;
class SOAP_REMOTABLE_CLASS PaymentRequest2;
class SOAP_REMOTABLE_CLASS PaymentData2;
class SOAP_REMOTABLE_CLASS PaymentTransaction2;
class SOAP_REMOTABLE_CLASS AmountsReq2;
class SOAP_REMOTABLE_CLASS SaleData2;
class SOAP_REMOTABLE_CLASS SaleTransactionID2;
class SOAP_REMOTABLE_CLASS ReversalRequest2;
class SOAP_REMOTABLE_CLASS OriginalPOITransaction2;
class SOAP_REMOTABLE_CLASS POITransactionID2;
class SOAP_REMOTABLE_CLASS Response2;
class SOAP_REMOTABLE_CLASS PaymentResponse2;
class SOAP_REMOTABLE_CLASS POIData2;
class SOAP_REMOTABLE_CLASS PaymentReceipt2;
class SOAP_REMOTABLE_CLASS OutputContent2;
class SOAP_REMOTABLE_CLASS OutputText2;
class SOAP_REMOTABLE_CLASS PaymentResult2;
class SOAP_REMOTABLE_CLASS AmountsResp2;
class SOAP_REMOTABLE_CLASS PaymentAcquirerData2;
class SOAP_REMOTABLE_CLASS AcquirerTransactionID2;
class SOAP_REMOTABLE_CLASS PaymentInstrumentData2;
class SOAP_REMOTABLE_CLASS CardData2;
class SOAP_REMOTABLE_CLASS AllowedProduct2;
class SOAP_REMOTABLE_CLASS CustomerOrder2;
class SOAP_REMOTABLE_CLASS PaymentToken2;
class SOAP_REMOTABLE_CLASS SensitiveCardData2;
class SOAP_REMOTABLE_CLASS TrackData2;
class SOAP_REMOTABLE_CLASS DiagnosisRequest;
class SOAP_REMOTABLE_CLASS DiagnosisResponse;
class SOAP_REMOTABLE_CLASS POIStatus;
class SOAP_REMOTABLE_CLASS EventNotification;
class SOAP_REMOTABLE_CLASS DiagnosisRequest2;
class SOAP_REMOTABLE_CLASS DiagnosisResponse2;
class SOAP_REMOTABLE_CLASS POIStatus2;
class SOAP_REMOTABLE_CLASS EventNotification2;
class SOAP_REMOTABLE_CLASS LoginRequest;
class SOAP_REMOTABLE_CLASS LogoutRequest;
class SOAP_REMOTABLE_CLASS LoginResponse;
class SOAP_REMOTABLE_CLASS POISystemData;
class SOAP_REMOTABLE_CLASS POISoftware;
class SOAP_REMOTABLE_CLASS LogoutResponse;
class SOAP_REMOTABLE_CLASS LoginRequest2;
class SOAP_REMOTABLE_CLASS LogoutRequest2;
class SOAP_REMOTABLE_CLASS LoginResponse2;
class SOAP_REMOTABLE_CLASS POISystemData2;
class SOAP_REMOTABLE_CLASS POISoftware2;
class SOAP_REMOTABLE_CLASS LogoutResponse2;
class SOAP_REMOTABLE_CLASS MessageHeader;
class SOAP_REMOTABLE_CLASS ResourceDetails;
class SOAP_REMOTABLE_CLASS MessageHeader2;
class SOAP_REMOTABLE_CLASS ResourceDetails2;
class SOAP_REMOTABLE_CLASS MessageReference;
class SOAP_REMOTABLE_CLASS TransactionStatusRequest;
class SOAP_REMOTABLE_CLASS TransactionStatusResponse;
class SOAP_REMOTABLE_CLASS RepeatedMessageResponse;
class SOAP_REMOTABLE_CLASS RepeatedResponseMessageBody;
class SOAP_REMOTABLE_CLASS TransactionStatusRequest2;
class SOAP_REMOTABLE_CLASS MessageReference2;
class SOAP_REMOTABLE_CLASS TransactionStatusResponse2;
class SOAP_REMOTABLE_CLASS RepeatedMessageResponse2;
class SOAP_REMOTABLE_CLASS RepeatedResponseMessageBody2;
class SOAP_REMOTABLE_CLASS Envelop;
class SOAP_REMOTABLE_CLASS SaleToPOIRequest;
class SOAP_REMOTABLE_CLASS Envelop2;
class SOAP_REMOTABLE_CLASS SaleToPOIRequest2;
class SOAP_REMOTABLE_CLASS SaleToPOIResponse;
class SOAP_REMOTABLE_CLASS SaleToPOIResponse2;
class SOAP_REMOTABLE_CLASS CaptureModifiedAmount;
class SOAP_REMOTABLE_CLASS ModificationAmount;
class SOAP_REMOTABLE_CLASS AdjustAndCaptureResponse;
class SOAP_REMOTABLE_CLASS AdjustAuthorisation;
class SOAP_REMOTABLE_CLASS AdditionalData;
class SOAP_REMOTABLE_CLASS CaptureModifiedAmount2;
class SOAP_REMOTABLE_CLASS ModificationAmount2;
class SOAP_REMOTABLE_CLASS AdjustAndCaptureResponse2;
class SOAP_REMOTABLE_CLASS AdjustAuthorisation2;
class SOAP_REMOTABLE_CLASS AdditionalData2;

typedef DynamicArray<UnicodeString> ArrayOfstring;  /* "http://schemas.microsoft.com/2003/10/Serialization/Arrays"[GblCplx] */


// ************************************************************************ //
// XML       : SaleSoftware, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleSoftware : public TRemotable {
private:
  UnicodeString   FApplicationName;
  bool            FApplicationName_Specified;
  UnicodeString   FCertificationCode;
  bool            FCertificationCode_Specified;
  UnicodeString   FManufacturerID;
  bool            FManufacturerID_Specified;
  UnicodeString   FSoftwareVersion;
  bool            FSoftwareVersion_Specified;
  void __fastcall SetApplicationName(int Index, UnicodeString _prop_val)
  {  FApplicationName = _prop_val; FApplicationName_Specified = true;  }
  bool __fastcall ApplicationName_Specified(int Index)
  {  return FApplicationName_Specified;  } 
  void __fastcall SetCertificationCode(int Index, UnicodeString _prop_val)
  {  FCertificationCode = _prop_val; FCertificationCode_Specified = true;  }
  bool __fastcall CertificationCode_Specified(int Index)
  {  return FCertificationCode_Specified;  } 
  void __fastcall SetManufacturerID(int Index, UnicodeString _prop_val)
  {  FManufacturerID = _prop_val; FManufacturerID_Specified = true;  }
  bool __fastcall ManufacturerID_Specified(int Index)
  {  return FManufacturerID_Specified;  } 
  void __fastcall SetSoftwareVersion(int Index, UnicodeString _prop_val)
  {  FSoftwareVersion = _prop_val; FSoftwareVersion_Specified = true;  }
  bool __fastcall SoftwareVersion_Specified(int Index)
  {  return FSoftwareVersion_Specified;  } 
__published:
  __property UnicodeString ApplicationName = { index=(IS_OPTN|IS_NLBL), read=FApplicationName, write=SetApplicationName, stored = ApplicationName_Specified };
  __property UnicodeString CertificationCode = { index=(IS_OPTN|IS_NLBL), read=FCertificationCode, write=SetCertificationCode, stored = CertificationCode_Specified };
  __property UnicodeString ManufacturerID = { index=(IS_OPTN|IS_NLBL), read=FManufacturerID, write=SetManufacturerID, stored = ManufacturerID_Specified };
  __property UnicodeString SoftwareVersion = { index=(IS_OPTN|IS_NLBL), read=FSoftwareVersion, write=SetSoftwareVersion, stored = SoftwareVersion_Specified };
};




// ************************************************************************ //
// XML       : SaleTerminalData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleTerminalData : public TRemotable {
private:
  SaleProfile*    FSaleProfile;
  bool            FSaleProfile_Specified;
  UnicodeString   FTerminalEnvironment;
  bool            FTerminalEnvironment_Specified;
  UnicodeString   FTotalsGroupID;
  bool            FTotalsGroupID_Specified;
  void __fastcall SetSaleProfile(int Index, SaleProfile* _prop_val)
  {  FSaleProfile = _prop_val; FSaleProfile_Specified = true;  }
  bool __fastcall SaleProfile_Specified(int Index)
  {  return FSaleProfile_Specified;  } 
  void __fastcall SetTerminalEnvironment(int Index, UnicodeString _prop_val)
  {  FTerminalEnvironment = _prop_val; FTerminalEnvironment_Specified = true;  }
  bool __fastcall TerminalEnvironment_Specified(int Index)
  {  return FTerminalEnvironment_Specified;  } 
  void __fastcall SetTotalsGroupID(int Index, UnicodeString _prop_val)
  {  FTotalsGroupID = _prop_val; FTotalsGroupID_Specified = true;  }
  bool __fastcall TotalsGroupID_Specified(int Index)
  {  return FTotalsGroupID_Specified;  } 

public:
  __fastcall ~SaleTerminalData();
__published:
  __property SaleProfile* SaleProfile = { index=(IS_OPTN|IS_NLBL), read=FSaleProfile, write=SetSaleProfile, stored = SaleProfile_Specified };
  __property UnicodeString TerminalEnvironment = { index=(IS_OPTN|IS_NLBL), read=FTerminalEnvironment, write=SetTerminalEnvironment, stored = TerminalEnvironment_Specified };
  __property UnicodeString TotalsGroupID = { index=(IS_OPTN|IS_NLBL), read=FTotalsGroupID, write=SetTotalsGroupID, stored = TotalsGroupID_Specified };
};




// ************************************************************************ //
// XML       : SaleProfile, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleProfile : public TRemotable {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentRequest : public TRemotable {
private:
  PaymentData*    FPaymentData;
  bool            FPaymentData_Specified;
  PaymentTransaction* FPaymentTransaction;
  bool            FPaymentTransaction_Specified;
  SaleData*       FSaleData;
  bool            FSaleData_Specified;
  void __fastcall SetPaymentData(int Index, PaymentData* _prop_val)
  {  FPaymentData = _prop_val; FPaymentData_Specified = true;  }
  bool __fastcall PaymentData_Specified(int Index)
  {  return FPaymentData_Specified;  } 
  void __fastcall SetPaymentTransaction(int Index, PaymentTransaction* _prop_val)
  {  FPaymentTransaction = _prop_val; FPaymentTransaction_Specified = true;  }
  bool __fastcall PaymentTransaction_Specified(int Index)
  {  return FPaymentTransaction_Specified;  } 
  void __fastcall SetSaleData(int Index, SaleData* _prop_val)
  {  FSaleData = _prop_val; FSaleData_Specified = true;  }
  bool __fastcall SaleData_Specified(int Index)
  {  return FSaleData_Specified;  } 

public:
  __fastcall ~PaymentRequest();
__published:
  __property PaymentData* PaymentData = { index=(IS_OPTN|IS_NLBL), read=FPaymentData, write=SetPaymentData, stored = PaymentData_Specified };
  __property PaymentTransaction* PaymentTransaction = { index=(IS_OPTN|IS_NLBL), read=FPaymentTransaction, write=SetPaymentTransaction, stored = PaymentTransaction_Specified };
  __property SaleData*    SaleData = { index=(IS_OPTN|IS_NLBL), read=FSaleData, write=SetSaleData, stored = SaleData_Specified };
};




// ************************************************************************ //
// XML       : PaymentData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentData : public TRemotable {
private:
  UnicodeString   FPaymentType;
  bool            FPaymentType_Specified;
  void __fastcall SetPaymentType(int Index, UnicodeString _prop_val)
  {  FPaymentType = _prop_val; FPaymentType_Specified = true;  }
  bool __fastcall PaymentType_Specified(int Index)
  {  return FPaymentType_Specified;  } 
__published:
  __property UnicodeString PaymentType = { index=(IS_OPTN|IS_NLBL), read=FPaymentType, write=SetPaymentType, stored = PaymentType_Specified };
};




// ************************************************************************ //
// XML       : PaymentTransaction, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentTransaction : public TRemotable {
private:
  AmountsReq*     FAmountsReq;
  bool            FAmountsReq_Specified;
  void __fastcall SetAmountsReq(int Index, AmountsReq* _prop_val)
  {  FAmountsReq = _prop_val; FAmountsReq_Specified = true;  }
  bool __fastcall AmountsReq_Specified(int Index)
  {  return FAmountsReq_Specified;  } 

public:
  __fastcall ~PaymentTransaction();
__published:
  __property AmountsReq* AmountsReq = { index=(IS_OPTN|IS_NLBL), read=FAmountsReq, write=SetAmountsReq, stored = AmountsReq_Specified };
};




// ************************************************************************ //
// XML       : SaleData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleData : public TRemotable {
private:
  UnicodeString   FSaleReferenceID;
  bool            FSaleReferenceID_Specified;
  UnicodeString   FSaleToAcquirerData;
  bool            FSaleToAcquirerData_Specified;
  SaleTransactionID* FSaleTransactionID;
  bool            FSaleTransactionID_Specified;
  UnicodeString   FTokenRequestedType;
  bool            FTokenRequestedType_Specified;
  void __fastcall SetSaleReferenceID(int Index, UnicodeString _prop_val)
  {  FSaleReferenceID = _prop_val; FSaleReferenceID_Specified = true;  }
  bool __fastcall SaleReferenceID_Specified(int Index)
  {  return FSaleReferenceID_Specified;  } 
  void __fastcall SetSaleToAcquirerData(int Index, UnicodeString _prop_val)
  {  FSaleToAcquirerData = _prop_val; FSaleToAcquirerData_Specified = true;  }
  bool __fastcall SaleToAcquirerData_Specified(int Index)
  {  return FSaleToAcquirerData_Specified;  } 
  void __fastcall SetSaleTransactionID(int Index, SaleTransactionID* _prop_val)
  {  FSaleTransactionID = _prop_val; FSaleTransactionID_Specified = true;  }
  bool __fastcall SaleTransactionID_Specified(int Index)
  {  return FSaleTransactionID_Specified;  } 
  void __fastcall SetTokenRequestedType(int Index, UnicodeString _prop_val)
  {  FTokenRequestedType = _prop_val; FTokenRequestedType_Specified = true;  }
  bool __fastcall TokenRequestedType_Specified(int Index)
  {  return FTokenRequestedType_Specified;  } 

public:
  __fastcall ~SaleData();
__published:
  __property UnicodeString SaleReferenceID = { index=(IS_OPTN|IS_NLBL), read=FSaleReferenceID, write=SetSaleReferenceID, stored = SaleReferenceID_Specified };
  __property UnicodeString SaleToAcquirerData = { index=(IS_OPTN|IS_NLBL), read=FSaleToAcquirerData, write=SetSaleToAcquirerData, stored = SaleToAcquirerData_Specified };
  __property SaleTransactionID* SaleTransactionID = { index=(IS_OPTN|IS_NLBL), read=FSaleTransactionID, write=SetSaleTransactionID, stored = SaleTransactionID_Specified };
  __property UnicodeString TokenRequestedType = { index=(IS_OPTN|IS_NLBL), read=FTokenRequestedType, write=SetTokenRequestedType, stored = TokenRequestedType_Specified };
};




// ************************************************************************ //
// XML       : AmountsReq, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class AmountsReq : public TRemotable {
private:
  UnicodeString   FCurrency;
  bool            FCurrency_Specified;
  double          FRequestedAmount;
  bool            FRequestedAmount_Specified;
  void __fastcall SetCurrency(int Index, UnicodeString _prop_val)
  {  FCurrency = _prop_val; FCurrency_Specified = true;  }
  bool __fastcall Currency_Specified(int Index)
  {  return FCurrency_Specified;  } 
  void __fastcall SetRequestedAmount(int Index, double _prop_val)
  {  FRequestedAmount = _prop_val; FRequestedAmount_Specified = true;  }
  bool __fastcall RequestedAmount_Specified(int Index)
  {  return FRequestedAmount_Specified;  } 
__published:
  __property UnicodeString   Currency = { index=(IS_OPTN|IS_NLBL), read=FCurrency, write=SetCurrency, stored = Currency_Specified };
  __property double     RequestedAmount = { index=(IS_OPTN), read=FRequestedAmount, write=SetRequestedAmount, stored = RequestedAmount_Specified };
};




// ************************************************************************ //
// XML       : SaleTransactionID, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleTransactionID : public TRemotable {
private:
  UnicodeString   FTimeStamp;
  bool            FTimeStamp_Specified;
  UnicodeString   FTransactionID;
  bool            FTransactionID_Specified;
  void __fastcall SetTimeStamp(int Index, UnicodeString _prop_val)
  {  FTimeStamp = _prop_val; FTimeStamp_Specified = true;  }
  bool __fastcall TimeStamp_Specified(int Index)
  {  return FTimeStamp_Specified;  } 
  void __fastcall SetTransactionID(int Index, UnicodeString _prop_val)
  {  FTransactionID = _prop_val; FTransactionID_Specified = true;  }
  bool __fastcall TransactionID_Specified(int Index)
  {  return FTransactionID_Specified;  } 
__published:
  __property UnicodeString  TimeStamp = { index=(IS_OPTN|IS_NLBL), read=FTimeStamp, write=SetTimeStamp, stored = TimeStamp_Specified };
  __property UnicodeString TransactionID = { index=(IS_OPTN|IS_NLBL), read=FTransactionID, write=SetTransactionID, stored = TransactionID_Specified };
};




// ************************************************************************ //
// XML       : ReversalRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class ReversalRequest : public TRemotable {
private:
  OriginalPOITransaction* FOriginalPOITransaction;
  bool            FOriginalPOITransaction_Specified;
  UnicodeString   FReversalReason;
  bool            FReversalReason_Specified;
  void __fastcall SetOriginalPOITransaction(int Index, OriginalPOITransaction* _prop_val)
  {  FOriginalPOITransaction = _prop_val; FOriginalPOITransaction_Specified = true;  }
  bool __fastcall OriginalPOITransaction_Specified(int Index)
  {  return FOriginalPOITransaction_Specified;  } 
  void __fastcall SetReversalReason(int Index, UnicodeString _prop_val)
  {  FReversalReason = _prop_val; FReversalReason_Specified = true;  }
  bool __fastcall ReversalReason_Specified(int Index)
  {  return FReversalReason_Specified;  } 

public:
  __fastcall ~ReversalRequest();
__published:
  __property OriginalPOITransaction* OriginalPOITransaction = { index=(IS_OPTN|IS_NLBL), read=FOriginalPOITransaction, write=SetOriginalPOITransaction, stored = OriginalPOITransaction_Specified };
  __property UnicodeString ReversalReason = { index=(IS_OPTN|IS_NLBL), read=FReversalReason, write=SetReversalReason, stored = ReversalReason_Specified };
};




// ************************************************************************ //
// XML       : OriginalPOITransaction, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class OriginalPOITransaction : public TRemotable {
private:
  POITransactionID* FPOITransactionID;
  bool            FPOITransactionID_Specified;
  void __fastcall SetPOITransactionID(int Index, POITransactionID* _prop_val)
  {  FPOITransactionID = _prop_val; FPOITransactionID_Specified = true;  }
  bool __fastcall POITransactionID_Specified(int Index)
  {  return FPOITransactionID_Specified;  } 

public:
  __fastcall ~OriginalPOITransaction();
__published:
  __property POITransactionID* POITransactionID = { index=(IS_OPTN|IS_NLBL), read=FPOITransactionID, write=SetPOITransactionID, stored = POITransactionID_Specified };
};




// ************************************************************************ //
// XML       : POITransactionID, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class POITransactionID : public TRemotable {
private:
  UnicodeString   FTimeStamp;
  bool            FTimeStamp_Specified;
  UnicodeString   FTransactionID;
  bool            FTransactionID_Specified;
  void __fastcall SetTimeStamp(int Index, UnicodeString _prop_val)
  {  FTimeStamp = _prop_val; FTimeStamp_Specified = true;  }
  bool __fastcall TimeStamp_Specified(int Index)
  {  return FTimeStamp_Specified;  } 
  void __fastcall SetTransactionID(int Index, UnicodeString _prop_val)
  {  FTransactionID = _prop_val; FTransactionID_Specified = true;  }
  bool __fastcall TransactionID_Specified(int Index)
  {  return FTransactionID_Specified;  } 
__published:
  __property UnicodeString  TimeStamp = { index=(IS_OPTN|IS_NLBL), read=FTimeStamp, write=SetTimeStamp, stored = TimeStamp_Specified };
  __property UnicodeString TransactionID = { index=(IS_OPTN|IS_NLBL), read=FTransactionID, write=SetTransactionID, stored = TransactionID_Specified };
};




// ************************************************************************ //
// XML       : Response, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class Response : public TRemotable {
private:
  UnicodeString   FAdditionalResponse;
  bool            FAdditionalResponse_Specified;
  UnicodeString   FErrorCondition;
  bool            FErrorCondition_Specified;
  UnicodeString   FResult;
  bool            FResult_Specified;
  void __fastcall SetAdditionalResponse(int Index, UnicodeString _prop_val)
  {  FAdditionalResponse = _prop_val; FAdditionalResponse_Specified = true;  }
  bool __fastcall AdditionalResponse_Specified(int Index)
  {  return FAdditionalResponse_Specified;  } 
  void __fastcall SetErrorCondition(int Index, UnicodeString _prop_val)
  {  FErrorCondition = _prop_val; FErrorCondition_Specified = true;  }
  bool __fastcall ErrorCondition_Specified(int Index)
  {  return FErrorCondition_Specified;  } 
  void __fastcall SetResult(int Index, UnicodeString _prop_val)
  {  FResult = _prop_val; FResult_Specified = true;  }
  bool __fastcall Result_Specified(int Index)
  {  return FResult_Specified;  } 
__published:
  __property UnicodeString AdditionalResponse = { index=(IS_OPTN|IS_NLBL), read=FAdditionalResponse, write=SetAdditionalResponse, stored = AdditionalResponse_Specified };
  __property UnicodeString ErrorCondition = { index=(IS_OPTN|IS_NLBL), read=FErrorCondition, write=SetErrorCondition, stored = ErrorCondition_Specified };
  __property UnicodeString     Result = { index=(IS_OPTN|IS_NLBL), read=FResult, write=SetResult, stored = Result_Specified };
};




// ************************************************************************ //
// XML       : POIData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class POIData : public TRemotable {
private:
  POITransactionID* FPOITransactionID;
  bool            FPOITransactionID_Specified;
  void __fastcall SetPOITransactionID(int Index, POITransactionID* _prop_val)
  {  FPOITransactionID = _prop_val; FPOITransactionID_Specified = true;  }
  bool __fastcall POITransactionID_Specified(int Index)
  {  return FPOITransactionID_Specified;  } 

public:
  __fastcall ~POIData();
__published:
  __property POITransactionID* POITransactionID = { index=(IS_OPTN|IS_NLBL), read=FPOITransactionID, write=SetPOITransactionID, stored = POITransactionID_Specified };
};


typedef DynamicArray<PaymentReceipt*> ArrayOfPaymentReceipt; /* "http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : PaymentResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentResponse : public TRemotable {
private:
  POIData*        FPOIData;
  bool            FPOIData_Specified;
  ArrayOfPaymentReceipt FPaymentReceipt;
  bool            FPaymentReceipt_Specified;
  ArrayOfstring   FPaymentReceiptUsable1;
  bool            FPaymentReceiptUsable1_Specified;
  ArrayOfstring   FPaymentReceiptUsable2;
  bool            FPaymentReceiptUsable2_Specified;
  PaymentResult*  FPaymentResult;
  bool            FPaymentResult_Specified;
  Response*       FResponse;
  bool            FResponse_Specified;
  SaleData*       FSaleData;
  bool            FSaleData_Specified;
  void __fastcall SetPOIData(int Index, POIData* _prop_val)
  {  FPOIData = _prop_val; FPOIData_Specified = true;  }
  bool __fastcall POIData_Specified(int Index)
  {  return FPOIData_Specified;  } 
  void __fastcall SetPaymentReceipt(int Index, ArrayOfPaymentReceipt _prop_val)
  {  FPaymentReceipt = _prop_val; FPaymentReceipt_Specified = true;  }
  bool __fastcall PaymentReceipt_Specified(int Index)
  {  return FPaymentReceipt_Specified;  } 
  void __fastcall SetPaymentReceiptUsable1(int Index, ArrayOfstring _prop_val)
  {  FPaymentReceiptUsable1 = _prop_val; FPaymentReceiptUsable1_Specified = true;  }
  bool __fastcall PaymentReceiptUsable1_Specified(int Index)
  {  return FPaymentReceiptUsable1_Specified;  } 
  void __fastcall SetPaymentReceiptUsable2(int Index, ArrayOfstring _prop_val)
  {  FPaymentReceiptUsable2 = _prop_val; FPaymentReceiptUsable2_Specified = true;  }
  bool __fastcall PaymentReceiptUsable2_Specified(int Index)
  {  return FPaymentReceiptUsable2_Specified;  } 
  void __fastcall SetPaymentResult(int Index, PaymentResult* _prop_val)
  {  FPaymentResult = _prop_val; FPaymentResult_Specified = true;  }
  bool __fastcall PaymentResult_Specified(int Index)
  {  return FPaymentResult_Specified;  } 
  void __fastcall SetResponse(int Index, Response* _prop_val)
  {  FResponse = _prop_val; FResponse_Specified = true;  }
  bool __fastcall Response_Specified(int Index)
  {  return FResponse_Specified;  } 
  void __fastcall SetSaleData(int Index, SaleData* _prop_val)
  {  FSaleData = _prop_val; FSaleData_Specified = true;  }
  bool __fastcall SaleData_Specified(int Index)
  {  return FSaleData_Specified;  } 

public:
  __fastcall ~PaymentResponse();
__published:
  __property POIData*      POIData = { index=(IS_OPTN|IS_NLBL), read=FPOIData, write=SetPOIData, stored = POIData_Specified };
  __property ArrayOfPaymentReceipt PaymentReceipt = { index=(IS_OPTN|IS_NLBL), read=FPaymentReceipt, write=SetPaymentReceipt, stored = PaymentReceipt_Specified };
  __property ArrayOfstring PaymentReceiptUsable1 = { index=(IS_OPTN|IS_NLBL), read=FPaymentReceiptUsable1, write=SetPaymentReceiptUsable1, stored = PaymentReceiptUsable1_Specified };
  __property ArrayOfstring PaymentReceiptUsable2 = { index=(IS_OPTN|IS_NLBL), read=FPaymentReceiptUsable2, write=SetPaymentReceiptUsable2, stored = PaymentReceiptUsable2_Specified };
  __property PaymentResult* PaymentResult = { index=(IS_OPTN|IS_NLBL), read=FPaymentResult, write=SetPaymentResult, stored = PaymentResult_Specified };
  __property Response*    Response = { index=(IS_OPTN|IS_NLBL), read=FResponse, write=SetResponse, stored = Response_Specified };
  __property SaleData*    SaleData = { index=(IS_OPTN|IS_NLBL), read=FSaleData, write=SetSaleData, stored = SaleData_Specified };
};




// ************************************************************************ //
// XML       : PaymentResult, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentResult : public TRemotable {
private:
  AmountsResp*    FAmountsResp;
  bool            FAmountsResp_Specified;
  PaymentAcquirerData* FPaymentAcquirerData;
  bool            FPaymentAcquirerData_Specified;
  PaymentInstrumentData* FPaymentInstrumentData;
  bool            FPaymentInstrumentData_Specified;
  UnicodeString   FPaymentType;
  bool            FPaymentType_Specified;
  void __fastcall SetAmountsResp(int Index, AmountsResp* _prop_val)
  {  FAmountsResp = _prop_val; FAmountsResp_Specified = true;  }
  bool __fastcall AmountsResp_Specified(int Index)
  {  return FAmountsResp_Specified;  } 
  void __fastcall SetPaymentAcquirerData(int Index, PaymentAcquirerData* _prop_val)
  {  FPaymentAcquirerData = _prop_val; FPaymentAcquirerData_Specified = true;  }
  bool __fastcall PaymentAcquirerData_Specified(int Index)
  {  return FPaymentAcquirerData_Specified;  } 
  void __fastcall SetPaymentInstrumentData(int Index, PaymentInstrumentData* _prop_val)
  {  FPaymentInstrumentData = _prop_val; FPaymentInstrumentData_Specified = true;  }
  bool __fastcall PaymentInstrumentData_Specified(int Index)
  {  return FPaymentInstrumentData_Specified;  } 
  void __fastcall SetPaymentType(int Index, UnicodeString _prop_val)
  {  FPaymentType = _prop_val; FPaymentType_Specified = true;  }
  bool __fastcall PaymentType_Specified(int Index)
  {  return FPaymentType_Specified;  } 

public:
  __fastcall ~PaymentResult();
__published:
  __property AmountsResp* AmountsResp = { index=(IS_OPTN|IS_NLBL), read=FAmountsResp, write=SetAmountsResp, stored = AmountsResp_Specified };
  __property PaymentAcquirerData* PaymentAcquirerData = { index=(IS_OPTN|IS_NLBL), read=FPaymentAcquirerData, write=SetPaymentAcquirerData, stored = PaymentAcquirerData_Specified };
  __property PaymentInstrumentData* PaymentInstrumentData = { index=(IS_OPTN|IS_NLBL), read=FPaymentInstrumentData, write=SetPaymentInstrumentData, stored = PaymentInstrumentData_Specified };
  __property UnicodeString PaymentType = { index=(IS_OPTN|IS_NLBL), read=FPaymentType, write=SetPaymentType, stored = PaymentType_Specified };
};




// ************************************************************************ //
// XML       : PaymentReceipt, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentReceipt : public TRemotable {
private:
  UnicodeString   FDocumentQualifier;
  bool            FDocumentQualifier_Specified;
  UnicodeString   FIntegratedPrintFlag;
  bool            FIntegratedPrintFlag_Specified;
  OutputContent*  FOutputContent;
  bool            FOutputContent_Specified;
  UnicodeString   FRequiredSignatureFlag;
  bool            FRequiredSignatureFlag_Specified;
  void __fastcall SetDocumentQualifier(int Index, UnicodeString _prop_val)
  {  FDocumentQualifier = _prop_val; FDocumentQualifier_Specified = true;  }
  bool __fastcall DocumentQualifier_Specified(int Index)
  {  return FDocumentQualifier_Specified;  } 
  void __fastcall SetIntegratedPrintFlag(int Index, UnicodeString _prop_val)
  {  FIntegratedPrintFlag = _prop_val; FIntegratedPrintFlag_Specified = true;  }
  bool __fastcall IntegratedPrintFlag_Specified(int Index)
  {  return FIntegratedPrintFlag_Specified;  } 
  void __fastcall SetOutputContent(int Index, OutputContent* _prop_val)
  {  FOutputContent = _prop_val; FOutputContent_Specified = true;  }
  bool __fastcall OutputContent_Specified(int Index)
  {  return FOutputContent_Specified;  } 
  void __fastcall SetRequiredSignatureFlag(int Index, UnicodeString _prop_val)
  {  FRequiredSignatureFlag = _prop_val; FRequiredSignatureFlag_Specified = true;  }
  bool __fastcall RequiredSignatureFlag_Specified(int Index)
  {  return FRequiredSignatureFlag_Specified;  } 

public:
  __fastcall ~PaymentReceipt();
__published:
  __property UnicodeString DocumentQualifier = { index=(IS_OPTN|IS_NLBL), read=FDocumentQualifier, write=SetDocumentQualifier, stored = DocumentQualifier_Specified };
  __property UnicodeString IntegratedPrintFlag = { index=(IS_OPTN|IS_NLBL), read=FIntegratedPrintFlag, write=SetIntegratedPrintFlag, stored = IntegratedPrintFlag_Specified };
  __property OutputContent* OutputContent = { index=(IS_OPTN|IS_NLBL), read=FOutputContent, write=SetOutputContent, stored = OutputContent_Specified };
  __property UnicodeString RequiredSignatureFlag = { index=(IS_OPTN|IS_NLBL), read=FRequiredSignatureFlag, write=SetRequiredSignatureFlag, stored = RequiredSignatureFlag_Specified };
};


typedef DynamicArray<OutputText*> ArrayOfOutputText; /* "http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : OutputContent, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class OutputContent : public TRemotable {
private:
  UnicodeString   FOutputFormat;
  bool            FOutputFormat_Specified;
  ArrayOfOutputText FOutputText;
  bool            FOutputText_Specified;
  void __fastcall SetOutputFormat(int Index, UnicodeString _prop_val)
  {  FOutputFormat = _prop_val; FOutputFormat_Specified = true;  }
  bool __fastcall OutputFormat_Specified(int Index)
  {  return FOutputFormat_Specified;  } 
  void __fastcall SetOutputText(int Index, ArrayOfOutputText _prop_val)
  {  FOutputText = _prop_val; FOutputText_Specified = true;  }
  bool __fastcall OutputText_Specified(int Index)
  {  return FOutputText_Specified;  } 

public:
  __fastcall ~OutputContent();
__published:
  __property UnicodeString OutputFormat = { index=(IS_OPTN|IS_NLBL), read=FOutputFormat, write=SetOutputFormat, stored = OutputFormat_Specified };
  __property ArrayOfOutputText OutputText = { index=(IS_OPTN|IS_NLBL), read=FOutputText, write=SetOutputText, stored = OutputText_Specified };
};




// ************************************************************************ //
// XML       : OutputText, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class OutputText : public TRemotable {
private:
  UnicodeString   FAlignment;
  bool            FAlignment_Specified;
  UnicodeString   FCharacterHeight;
  bool            FCharacterHeight_Specified;
  int             FCharacterSet;
  bool            FCharacterSet_Specified;
  UnicodeString   FCharacterStyle;
  bool            FCharacterStyle_Specified;
  UnicodeString   FCharacterWidth;
  bool            FCharacterWidth_Specified;
  UnicodeString   FColor;
  bool            FColor_Specified;
  UnicodeString   FEndOfLineFlag;
  bool            FEndOfLineFlag_Specified;
  UnicodeString   FFont;
  bool            FFont_Specified;
  int             FStartColumn;
  bool            FStartColumn_Specified;
  int             FStartRow;
  bool            FStartRow_Specified;
  UnicodeString   FText;
  bool            FText_Specified;
  void __fastcall SetAlignment(int Index, UnicodeString _prop_val)
  {  FAlignment = _prop_val; FAlignment_Specified = true;  }
  bool __fastcall Alignment_Specified(int Index)
  {  return FAlignment_Specified;  } 
  void __fastcall SetCharacterHeight(int Index, UnicodeString _prop_val)
  {  FCharacterHeight = _prop_val; FCharacterHeight_Specified = true;  }
  bool __fastcall CharacterHeight_Specified(int Index)
  {  return FCharacterHeight_Specified;  } 
  void __fastcall SetCharacterSet(int Index, int _prop_val)
  {  FCharacterSet = _prop_val; FCharacterSet_Specified = true;  }
  bool __fastcall CharacterSet_Specified(int Index)
  {  return FCharacterSet_Specified;  } 
  void __fastcall SetCharacterStyle(int Index, UnicodeString _prop_val)
  {  FCharacterStyle = _prop_val; FCharacterStyle_Specified = true;  }
  bool __fastcall CharacterStyle_Specified(int Index)
  {  return FCharacterStyle_Specified;  } 
  void __fastcall SetCharacterWidth(int Index, UnicodeString _prop_val)
  {  FCharacterWidth = _prop_val; FCharacterWidth_Specified = true;  }
  bool __fastcall CharacterWidth_Specified(int Index)
  {  return FCharacterWidth_Specified;  } 
  void __fastcall SetColor(int Index, UnicodeString _prop_val)
  {  FColor = _prop_val; FColor_Specified = true;  }
  bool __fastcall Color_Specified(int Index)
  {  return FColor_Specified;  } 
  void __fastcall SetEndOfLineFlag(int Index, UnicodeString _prop_val)
  {  FEndOfLineFlag = _prop_val; FEndOfLineFlag_Specified = true;  }
  bool __fastcall EndOfLineFlag_Specified(int Index)
  {  return FEndOfLineFlag_Specified;  } 
  void __fastcall SetFont(int Index, UnicodeString _prop_val)
  {  FFont = _prop_val; FFont_Specified = true;  }
  bool __fastcall Font_Specified(int Index)
  {  return FFont_Specified;  } 
  void __fastcall SetStartColumn(int Index, int _prop_val)
  {  FStartColumn = _prop_val; FStartColumn_Specified = true;  }
  bool __fastcall StartColumn_Specified(int Index)
  {  return FStartColumn_Specified;  } 
  void __fastcall SetStartRow(int Index, int _prop_val)
  {  FStartRow = _prop_val; FStartRow_Specified = true;  }
  bool __fastcall StartRow_Specified(int Index)
  {  return FStartRow_Specified;  } 
  void __fastcall SetText(int Index, UnicodeString _prop_val)
  {  FText = _prop_val; FText_Specified = true;  }
  bool __fastcall Text_Specified(int Index)
  {  return FText_Specified;  } 
__published:
  __property UnicodeString  Alignment = { index=(IS_OPTN|IS_NLBL), read=FAlignment, write=SetAlignment, stored = Alignment_Specified };
  __property UnicodeString CharacterHeight = { index=(IS_OPTN|IS_NLBL), read=FCharacterHeight, write=SetCharacterHeight, stored = CharacterHeight_Specified };
  __property int        CharacterSet = { index=(IS_OPTN), read=FCharacterSet, write=SetCharacterSet, stored = CharacterSet_Specified };
  __property UnicodeString CharacterStyle = { index=(IS_OPTN|IS_NLBL), read=FCharacterStyle, write=SetCharacterStyle, stored = CharacterStyle_Specified };
  __property UnicodeString CharacterWidth = { index=(IS_OPTN|IS_NLBL), read=FCharacterWidth, write=SetCharacterWidth, stored = CharacterWidth_Specified };
  __property UnicodeString      Color = { index=(IS_OPTN|IS_NLBL), read=FColor, write=SetColor, stored = Color_Specified };
  __property UnicodeString EndOfLineFlag = { index=(IS_OPTN|IS_NLBL), read=FEndOfLineFlag, write=SetEndOfLineFlag, stored = EndOfLineFlag_Specified };
  __property UnicodeString       Font = { index=(IS_OPTN|IS_NLBL), read=FFont, write=SetFont, stored = Font_Specified };
  __property int        StartColumn = { index=(IS_OPTN), read=FStartColumn, write=SetStartColumn, stored = StartColumn_Specified };
  __property int          StartRow = { index=(IS_OPTN), read=FStartRow, write=SetStartRow, stored = StartRow_Specified };
  __property UnicodeString       Text = { index=(IS_OPTN|IS_NLBL), read=FText, write=SetText, stored = Text_Specified };
};




// ************************************************************************ //
// XML       : AmountsResp, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class AmountsResp : public TRemotable {
private:
  double          FAuthorizedAmount;
  bool            FAuthorizedAmount_Specified;
  double          FCashBackAmount;
  bool            FCashBackAmount_Specified;
  UnicodeString   FCurrency;
  bool            FCurrency_Specified;
  double          FTipAmount;
  bool            FTipAmount_Specified;
  double          FTotalFeesAmount;
  bool            FTotalFeesAmount_Specified;
  double          FTotalRebatesAmount;
  bool            FTotalRebatesAmount_Specified;
  void __fastcall SetAuthorizedAmount(int Index, double _prop_val)
  {  FAuthorizedAmount = _prop_val; FAuthorizedAmount_Specified = true;  }
  bool __fastcall AuthorizedAmount_Specified(int Index)
  {  return FAuthorizedAmount_Specified;  } 
  void __fastcall SetCashBackAmount(int Index, double _prop_val)
  {  FCashBackAmount = _prop_val; FCashBackAmount_Specified = true;  }
  bool __fastcall CashBackAmount_Specified(int Index)
  {  return FCashBackAmount_Specified;  } 
  void __fastcall SetCurrency(int Index, UnicodeString _prop_val)
  {  FCurrency = _prop_val; FCurrency_Specified = true;  }
  bool __fastcall Currency_Specified(int Index)
  {  return FCurrency_Specified;  } 
  void __fastcall SetTipAmount(int Index, double _prop_val)
  {  FTipAmount = _prop_val; FTipAmount_Specified = true;  }
  bool __fastcall TipAmount_Specified(int Index)
  {  return FTipAmount_Specified;  } 
  void __fastcall SetTotalFeesAmount(int Index, double _prop_val)
  {  FTotalFeesAmount = _prop_val; FTotalFeesAmount_Specified = true;  }
  bool __fastcall TotalFeesAmount_Specified(int Index)
  {  return FTotalFeesAmount_Specified;  } 
  void __fastcall SetTotalRebatesAmount(int Index, double _prop_val)
  {  FTotalRebatesAmount = _prop_val; FTotalRebatesAmount_Specified = true;  }
  bool __fastcall TotalRebatesAmount_Specified(int Index)
  {  return FTotalRebatesAmount_Specified;  } 
__published:
  __property double     AuthorizedAmount = { index=(IS_OPTN), read=FAuthorizedAmount, write=SetAuthorizedAmount, stored = AuthorizedAmount_Specified };
  __property double     CashBackAmount = { index=(IS_OPTN), read=FCashBackAmount, write=SetCashBackAmount, stored = CashBackAmount_Specified };
  __property UnicodeString   Currency = { index=(IS_OPTN|IS_NLBL), read=FCurrency, write=SetCurrency, stored = Currency_Specified };
  __property double      TipAmount = { index=(IS_OPTN), read=FTipAmount, write=SetTipAmount, stored = TipAmount_Specified };
  __property double     TotalFeesAmount = { index=(IS_OPTN), read=FTotalFeesAmount, write=SetTotalFeesAmount, stored = TotalFeesAmount_Specified };
  __property double     TotalRebatesAmount = { index=(IS_OPTN), read=FTotalRebatesAmount, write=SetTotalRebatesAmount, stored = TotalRebatesAmount_Specified };
};




// ************************************************************************ //
// XML       : PaymentAcquirerData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentAcquirerData : public TRemotable {
private:
  UnicodeString   FAcquirerID;
  bool            FAcquirerID_Specified;
  UnicodeString   FAcquirerPOIID;
  bool            FAcquirerPOIID_Specified;
  AcquirerTransactionID* FAcquirerTransactionID;
  bool            FAcquirerTransactionID_Specified;
  UnicodeString   FApprovalCode;
  bool            FApprovalCode_Specified;
  UnicodeString   FHostReconciliationID;
  bool            FHostReconciliationID_Specified;
  UnicodeString   FMerchantID;
  bool            FMerchantID_Specified;
  void __fastcall SetAcquirerID(int Index, UnicodeString _prop_val)
  {  FAcquirerID = _prop_val; FAcquirerID_Specified = true;  }
  bool __fastcall AcquirerID_Specified(int Index)
  {  return FAcquirerID_Specified;  } 
  void __fastcall SetAcquirerPOIID(int Index, UnicodeString _prop_val)
  {  FAcquirerPOIID = _prop_val; FAcquirerPOIID_Specified = true;  }
  bool __fastcall AcquirerPOIID_Specified(int Index)
  {  return FAcquirerPOIID_Specified;  } 
  void __fastcall SetAcquirerTransactionID(int Index, AcquirerTransactionID* _prop_val)
  {  FAcquirerTransactionID = _prop_val; FAcquirerTransactionID_Specified = true;  }
  bool __fastcall AcquirerTransactionID_Specified(int Index)
  {  return FAcquirerTransactionID_Specified;  } 
  void __fastcall SetApprovalCode(int Index, UnicodeString _prop_val)
  {  FApprovalCode = _prop_val; FApprovalCode_Specified = true;  }
  bool __fastcall ApprovalCode_Specified(int Index)
  {  return FApprovalCode_Specified;  } 
  void __fastcall SetHostReconciliationID(int Index, UnicodeString _prop_val)
  {  FHostReconciliationID = _prop_val; FHostReconciliationID_Specified = true;  }
  bool __fastcall HostReconciliationID_Specified(int Index)
  {  return FHostReconciliationID_Specified;  } 
  void __fastcall SetMerchantID(int Index, UnicodeString _prop_val)
  {  FMerchantID = _prop_val; FMerchantID_Specified = true;  }
  bool __fastcall MerchantID_Specified(int Index)
  {  return FMerchantID_Specified;  } 

public:
  __fastcall ~PaymentAcquirerData();
__published:
  __property UnicodeString AcquirerID = { index=(IS_OPTN|IS_NLBL), read=FAcquirerID, write=SetAcquirerID, stored = AcquirerID_Specified };
  __property UnicodeString AcquirerPOIID = { index=(IS_OPTN|IS_NLBL), read=FAcquirerPOIID, write=SetAcquirerPOIID, stored = AcquirerPOIID_Specified };
  __property AcquirerTransactionID* AcquirerTransactionID = { index=(IS_OPTN|IS_NLBL), read=FAcquirerTransactionID, write=SetAcquirerTransactionID, stored = AcquirerTransactionID_Specified };
  __property UnicodeString ApprovalCode = { index=(IS_OPTN|IS_NLBL), read=FApprovalCode, write=SetApprovalCode, stored = ApprovalCode_Specified };
  __property UnicodeString HostReconciliationID = { index=(IS_OPTN|IS_NLBL), read=FHostReconciliationID, write=SetHostReconciliationID, stored = HostReconciliationID_Specified };
  __property UnicodeString MerchantID = { index=(IS_OPTN|IS_NLBL), read=FMerchantID, write=SetMerchantID, stored = MerchantID_Specified };
};




// ************************************************************************ //
// XML       : PaymentInstrumentData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentInstrumentData : public TRemotable {
private:
  CardData*       FCardData;
  bool            FCardData_Specified;
  UnicodeString   FPaymentInstrumentType;
  bool            FPaymentInstrumentType_Specified;
  UnicodeString   FProtectedCardData;
  bool            FProtectedCardData_Specified;
  void __fastcall SetCardData(int Index, CardData* _prop_val)
  {  FCardData = _prop_val; FCardData_Specified = true;  }
  bool __fastcall CardData_Specified(int Index)
  {  return FCardData_Specified;  } 
  void __fastcall SetPaymentInstrumentType(int Index, UnicodeString _prop_val)
  {  FPaymentInstrumentType = _prop_val; FPaymentInstrumentType_Specified = true;  }
  bool __fastcall PaymentInstrumentType_Specified(int Index)
  {  return FPaymentInstrumentType_Specified;  } 
  void __fastcall SetProtectedCardData(int Index, UnicodeString _prop_val)
  {  FProtectedCardData = _prop_val; FProtectedCardData_Specified = true;  }
  bool __fastcall ProtectedCardData_Specified(int Index)
  {  return FProtectedCardData_Specified;  } 

public:
  __fastcall ~PaymentInstrumentData();
__published:
  __property CardData*    CardData = { index=(IS_OPTN|IS_NLBL), read=FCardData, write=SetCardData, stored = CardData_Specified };
  __property UnicodeString PaymentInstrumentType = { index=(IS_OPTN|IS_NLBL), read=FPaymentInstrumentType, write=SetPaymentInstrumentType, stored = PaymentInstrumentType_Specified };
  __property UnicodeString ProtectedCardData = { index=(IS_OPTN|IS_NLBL), read=FProtectedCardData, write=SetProtectedCardData, stored = ProtectedCardData_Specified };
};




// ************************************************************************ //
// XML       : AcquirerTransactionID, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class AcquirerTransactionID : public TRemotable {
private:
  UnicodeString   FTimeStamp;
  bool            FTimeStamp_Specified;
  UnicodeString   FTransactionID;
  bool            FTransactionID_Specified;
  void __fastcall SetTimeStamp(int Index, UnicodeString _prop_val)
  {  FTimeStamp = _prop_val; FTimeStamp_Specified = true;  }
  bool __fastcall TimeStamp_Specified(int Index)
  {  return FTimeStamp_Specified;  } 
  void __fastcall SetTransactionID(int Index, UnicodeString _prop_val)
  {  FTransactionID = _prop_val; FTransactionID_Specified = true;  }
  bool __fastcall TransactionID_Specified(int Index)
  {  return FTransactionID_Specified;  } 
__published:
  __property UnicodeString  TimeStamp = { index=(IS_OPTN|IS_NLBL), read=FTimeStamp, write=SetTimeStamp, stored = TimeStamp_Specified };
  __property UnicodeString TransactionID = { index=(IS_OPTN|IS_NLBL), read=FTransactionID, write=SetTransactionID, stored = TransactionID_Specified };
};


typedef DynamicArray<AllowedProduct*> ArrayOfAllowedProduct; /* "http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain"[GblCplx] */
typedef DynamicArray<CustomerOrder*> ArrayOfCustomerOrder; /* "http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : CardData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class CardData : public TRemotable {
private:
  ArrayOfAllowedProduct FAllowedProduct;
  bool            FAllowedProduct_Specified;
  ArrayOfstring   FAllowedProductCode;
  bool            FAllowedProductCode_Specified;
  UnicodeString   FCardCountryCode;
  bool            FCardCountryCode_Specified;
  ArrayOfCustomerOrder FCustomerOrder;
  bool            FCustomerOrder_Specified;
  ArrayOfstring   FEntryMode;
  bool            FEntryMode_Specified;
  UnicodeString   FMaskedPan;
  bool            FMaskedPan_Specified;
  UnicodeString   FPaymentAccountRef;
  bool            FPaymentAccountRef_Specified;
  UnicodeString   FPaymentBrand;
  bool            FPaymentBrand_Specified;
  PaymentToken*   FPaymentToken;
  bool            FPaymentToken_Specified;
  UnicodeString   FProtectedCardData;
  bool            FProtectedCardData_Specified;
  SensitiveCardData* FSensitiveCardData;
  bool            FSensitiveCardData_Specified;
  void __fastcall SetAllowedProduct(int Index, ArrayOfAllowedProduct _prop_val)
  {  FAllowedProduct = _prop_val; FAllowedProduct_Specified = true;  }
  bool __fastcall AllowedProduct_Specified(int Index)
  {  return FAllowedProduct_Specified;  } 
  void __fastcall SetAllowedProductCode(int Index, ArrayOfstring _prop_val)
  {  FAllowedProductCode = _prop_val; FAllowedProductCode_Specified = true;  }
  bool __fastcall AllowedProductCode_Specified(int Index)
  {  return FAllowedProductCode_Specified;  } 
  void __fastcall SetCardCountryCode(int Index, UnicodeString _prop_val)
  {  FCardCountryCode = _prop_val; FCardCountryCode_Specified = true;  }
  bool __fastcall CardCountryCode_Specified(int Index)
  {  return FCardCountryCode_Specified;  } 
  void __fastcall SetCustomerOrder(int Index, ArrayOfCustomerOrder _prop_val)
  {  FCustomerOrder = _prop_val; FCustomerOrder_Specified = true;  }
  bool __fastcall CustomerOrder_Specified(int Index)
  {  return FCustomerOrder_Specified;  } 
  void __fastcall SetEntryMode(int Index, ArrayOfstring _prop_val)
  {  FEntryMode = _prop_val; FEntryMode_Specified = true;  }
  bool __fastcall EntryMode_Specified(int Index)
  {  return FEntryMode_Specified;  } 
  void __fastcall SetMaskedPan(int Index, UnicodeString _prop_val)
  {  FMaskedPan = _prop_val; FMaskedPan_Specified = true;  }
  bool __fastcall MaskedPan_Specified(int Index)
  {  return FMaskedPan_Specified;  } 
  void __fastcall SetPaymentAccountRef(int Index, UnicodeString _prop_val)
  {  FPaymentAccountRef = _prop_val; FPaymentAccountRef_Specified = true;  }
  bool __fastcall PaymentAccountRef_Specified(int Index)
  {  return FPaymentAccountRef_Specified;  } 
  void __fastcall SetPaymentBrand(int Index, UnicodeString _prop_val)
  {  FPaymentBrand = _prop_val; FPaymentBrand_Specified = true;  }
  bool __fastcall PaymentBrand_Specified(int Index)
  {  return FPaymentBrand_Specified;  } 
  void __fastcall SetPaymentToken(int Index, PaymentToken* _prop_val)
  {  FPaymentToken = _prop_val; FPaymentToken_Specified = true;  }
  bool __fastcall PaymentToken_Specified(int Index)
  {  return FPaymentToken_Specified;  } 
  void __fastcall SetProtectedCardData(int Index, UnicodeString _prop_val)
  {  FProtectedCardData = _prop_val; FProtectedCardData_Specified = true;  }
  bool __fastcall ProtectedCardData_Specified(int Index)
  {  return FProtectedCardData_Specified;  } 
  void __fastcall SetSensitiveCardData(int Index, SensitiveCardData* _prop_val)
  {  FSensitiveCardData = _prop_val; FSensitiveCardData_Specified = true;  }
  bool __fastcall SensitiveCardData_Specified(int Index)
  {  return FSensitiveCardData_Specified;  } 

public:
  __fastcall ~CardData();
__published:
  __property ArrayOfAllowedProduct AllowedProduct = { index=(IS_OPTN|IS_NLBL), read=FAllowedProduct, write=SetAllowedProduct, stored = AllowedProduct_Specified };
  __property ArrayOfstring AllowedProductCode = { index=(IS_OPTN|IS_NLBL), read=FAllowedProductCode, write=SetAllowedProductCode, stored = AllowedProductCode_Specified };
  __property UnicodeString CardCountryCode = { index=(IS_OPTN|IS_NLBL), read=FCardCountryCode, write=SetCardCountryCode, stored = CardCountryCode_Specified };
  __property ArrayOfCustomerOrder CustomerOrder = { index=(IS_OPTN|IS_NLBL), read=FCustomerOrder, write=SetCustomerOrder, stored = CustomerOrder_Specified };
  __property ArrayOfstring  EntryMode = { index=(IS_OPTN|IS_NLBL), read=FEntryMode, write=SetEntryMode, stored = EntryMode_Specified };
  __property UnicodeString  MaskedPan = { index=(IS_OPTN|IS_NLBL), read=FMaskedPan, write=SetMaskedPan, stored = MaskedPan_Specified };
  __property UnicodeString PaymentAccountRef = { index=(IS_OPTN|IS_NLBL), read=FPaymentAccountRef, write=SetPaymentAccountRef, stored = PaymentAccountRef_Specified };
  __property UnicodeString PaymentBrand = { index=(IS_OPTN|IS_NLBL), read=FPaymentBrand, write=SetPaymentBrand, stored = PaymentBrand_Specified };
  __property PaymentToken* PaymentToken = { index=(IS_OPTN|IS_NLBL), read=FPaymentToken, write=SetPaymentToken, stored = PaymentToken_Specified };
  __property UnicodeString ProtectedCardData = { index=(IS_OPTN|IS_NLBL), read=FProtectedCardData, write=SetProtectedCardData, stored = ProtectedCardData_Specified };
  __property SensitiveCardData* SensitiveCardData = { index=(IS_OPTN|IS_NLBL), read=FSensitiveCardData, write=SetSensitiveCardData, stored = SensitiveCardData_Specified };
};




// ************************************************************************ //
// XML       : PaymentToken, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentToken : public TRemotable {
private:
  UnicodeString   FExpiryDateTime;
  bool            FExpiryDateTime_Specified;
  UnicodeString   FTokenRequestedType;
  bool            FTokenRequestedType_Specified;
  UnicodeString   FTokenValue;
  bool            FTokenValue_Specified;
  void __fastcall SetExpiryDateTime(int Index, UnicodeString _prop_val)
  {  FExpiryDateTime = _prop_val; FExpiryDateTime_Specified = true;  }
  bool __fastcall ExpiryDateTime_Specified(int Index)
  {  return FExpiryDateTime_Specified;  } 
  void __fastcall SetTokenRequestedType(int Index, UnicodeString _prop_val)
  {  FTokenRequestedType = _prop_val; FTokenRequestedType_Specified = true;  }
  bool __fastcall TokenRequestedType_Specified(int Index)
  {  return FTokenRequestedType_Specified;  } 
  void __fastcall SetTokenValue(int Index, UnicodeString _prop_val)
  {  FTokenValue = _prop_val; FTokenValue_Specified = true;  }
  bool __fastcall TokenValue_Specified(int Index)
  {  return FTokenValue_Specified;  } 
__published:
  __property UnicodeString ExpiryDateTime = { index=(IS_OPTN|IS_NLBL), read=FExpiryDateTime, write=SetExpiryDateTime, stored = ExpiryDateTime_Specified };
  __property UnicodeString TokenRequestedType = { index=(IS_OPTN|IS_NLBL), read=FTokenRequestedType, write=SetTokenRequestedType, stored = TokenRequestedType_Specified };
  __property UnicodeString TokenValue = { index=(IS_OPTN|IS_NLBL), read=FTokenValue, write=SetTokenValue, stored = TokenValue_Specified };
};




// ************************************************************************ //
// XML       : SensitiveCardData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SensitiveCardData : public TRemotable {
private:
  UnicodeString   FCardSeqNumb;
  bool            FCardSeqNumb_Specified;
  UnicodeString   FExpiryDate;
  bool            FExpiryDate_Specified;
  UnicodeString   FPAN;
  bool            FPAN_Specified;
  TrackData*      FTrackData;
  bool            FTrackData_Specified;
  void __fastcall SetCardSeqNumb(int Index, UnicodeString _prop_val)
  {  FCardSeqNumb = _prop_val; FCardSeqNumb_Specified = true;  }
  bool __fastcall CardSeqNumb_Specified(int Index)
  {  return FCardSeqNumb_Specified;  } 
  void __fastcall SetExpiryDate(int Index, UnicodeString _prop_val)
  {  FExpiryDate = _prop_val; FExpiryDate_Specified = true;  }
  bool __fastcall ExpiryDate_Specified(int Index)
  {  return FExpiryDate_Specified;  } 
  void __fastcall SetPAN(int Index, UnicodeString _prop_val)
  {  FPAN = _prop_val; FPAN_Specified = true;  }
  bool __fastcall PAN_Specified(int Index)
  {  return FPAN_Specified;  } 
  void __fastcall SetTrackData(int Index, TrackData* _prop_val)
  {  FTrackData = _prop_val; FTrackData_Specified = true;  }
  bool __fastcall TrackData_Specified(int Index)
  {  return FTrackData_Specified;  } 

public:
  __fastcall ~SensitiveCardData();
__published:
  __property UnicodeString CardSeqNumb = { index=(IS_OPTN|IS_NLBL), read=FCardSeqNumb, write=SetCardSeqNumb, stored = CardSeqNumb_Specified };
  __property UnicodeString ExpiryDate = { index=(IS_OPTN|IS_NLBL), read=FExpiryDate, write=SetExpiryDate, stored = ExpiryDate_Specified };
  __property UnicodeString        PAN = { index=(IS_OPTN|IS_NLBL), read=FPAN, write=SetPAN, stored = PAN_Specified };
  __property TrackData*  TrackData = { index=(IS_OPTN|IS_NLBL), read=FTrackData, write=SetTrackData, stored = TrackData_Specified };
};




// ************************************************************************ //
// XML       : AllowedProduct, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class AllowedProduct : public TRemotable {
private:
  UnicodeString   FAdditionalProductInfo;
  bool            FAdditionalProductInfo_Specified;
  UnicodeString   FEanUpc;
  bool            FEanUpc_Specified;
  UnicodeString   FProductCode;
  bool            FProductCode_Specified;
  UnicodeString   FProductLabel;
  bool            FProductLabel_Specified;
  void __fastcall SetAdditionalProductInfo(int Index, UnicodeString _prop_val)
  {  FAdditionalProductInfo = _prop_val; FAdditionalProductInfo_Specified = true;  }
  bool __fastcall AdditionalProductInfo_Specified(int Index)
  {  return FAdditionalProductInfo_Specified;  } 
  void __fastcall SetEanUpc(int Index, UnicodeString _prop_val)
  {  FEanUpc = _prop_val; FEanUpc_Specified = true;  }
  bool __fastcall EanUpc_Specified(int Index)
  {  return FEanUpc_Specified;  } 
  void __fastcall SetProductCode(int Index, UnicodeString _prop_val)
  {  FProductCode = _prop_val; FProductCode_Specified = true;  }
  bool __fastcall ProductCode_Specified(int Index)
  {  return FProductCode_Specified;  } 
  void __fastcall SetProductLabel(int Index, UnicodeString _prop_val)
  {  FProductLabel = _prop_val; FProductLabel_Specified = true;  }
  bool __fastcall ProductLabel_Specified(int Index)
  {  return FProductLabel_Specified;  } 
__published:
  __property UnicodeString AdditionalProductInfo = { index=(IS_OPTN|IS_NLBL), read=FAdditionalProductInfo, write=SetAdditionalProductInfo, stored = AdditionalProductInfo_Specified };
  __property UnicodeString     EanUpc = { index=(IS_OPTN|IS_NLBL), read=FEanUpc, write=SetEanUpc, stored = EanUpc_Specified };
  __property UnicodeString ProductCode = { index=(IS_OPTN|IS_NLBL), read=FProductCode, write=SetProductCode, stored = ProductCode_Specified };
  __property UnicodeString ProductLabel = { index=(IS_OPTN|IS_NLBL), read=FProductLabel, write=SetProductLabel, stored = ProductLabel_Specified };
};




// ************************************************************************ //
// XML       : CustomerOrder, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class CustomerOrder : public TRemotable {
private:
  UnicodeString   FAccessedBy;
  bool            FAccessedBy_Specified;
  UnicodeString   FAdditionalInformation;
  bool            FAdditionalInformation_Specified;
  UnicodeString   FCurrency;
  bool            FCurrency_Specified;
  UnicodeString   FCurrentAmount;
  bool            FCurrentAmount_Specified;
  UnicodeString   FCustomerOrderID;
  bool            FCustomerOrderID_Specified;
  UnicodeString   FEndDate;
  bool            FEndDate_Specified;
  UnicodeString   FForecastedAmount;
  bool            FForecastedAmount_Specified;
  UnicodeString   FOpenOrderState;
  bool            FOpenOrderState_Specified;
  UnicodeString   FSaleReferenceID;
  bool            FSaleReferenceID_Specified;
  UnicodeString   FStartDate;
  bool            FStartDate_Specified;
  void __fastcall SetAccessedBy(int Index, UnicodeString _prop_val)
  {  FAccessedBy = _prop_val; FAccessedBy_Specified = true;  }
  bool __fastcall AccessedBy_Specified(int Index)
  {  return FAccessedBy_Specified;  } 
  void __fastcall SetAdditionalInformation(int Index, UnicodeString _prop_val)
  {  FAdditionalInformation = _prop_val; FAdditionalInformation_Specified = true;  }
  bool __fastcall AdditionalInformation_Specified(int Index)
  {  return FAdditionalInformation_Specified;  } 
  void __fastcall SetCurrency(int Index, UnicodeString _prop_val)
  {  FCurrency = _prop_val; FCurrency_Specified = true;  }
  bool __fastcall Currency_Specified(int Index)
  {  return FCurrency_Specified;  } 
  void __fastcall SetCurrentAmount(int Index, UnicodeString _prop_val)
  {  FCurrentAmount = _prop_val; FCurrentAmount_Specified = true;  }
  bool __fastcall CurrentAmount_Specified(int Index)
  {  return FCurrentAmount_Specified;  } 
  void __fastcall SetCustomerOrderID(int Index, UnicodeString _prop_val)
  {  FCustomerOrderID = _prop_val; FCustomerOrderID_Specified = true;  }
  bool __fastcall CustomerOrderID_Specified(int Index)
  {  return FCustomerOrderID_Specified;  } 
  void __fastcall SetEndDate(int Index, UnicodeString _prop_val)
  {  FEndDate = _prop_val; FEndDate_Specified = true;  }
  bool __fastcall EndDate_Specified(int Index)
  {  return FEndDate_Specified;  } 
  void __fastcall SetForecastedAmount(int Index, UnicodeString _prop_val)
  {  FForecastedAmount = _prop_val; FForecastedAmount_Specified = true;  }
  bool __fastcall ForecastedAmount_Specified(int Index)
  {  return FForecastedAmount_Specified;  } 
  void __fastcall SetOpenOrderState(int Index, UnicodeString _prop_val)
  {  FOpenOrderState = _prop_val; FOpenOrderState_Specified = true;  }
  bool __fastcall OpenOrderState_Specified(int Index)
  {  return FOpenOrderState_Specified;  } 
  void __fastcall SetSaleReferenceID(int Index, UnicodeString _prop_val)
  {  FSaleReferenceID = _prop_val; FSaleReferenceID_Specified = true;  }
  bool __fastcall SaleReferenceID_Specified(int Index)
  {  return FSaleReferenceID_Specified;  } 
  void __fastcall SetStartDate(int Index, UnicodeString _prop_val)
  {  FStartDate = _prop_val; FStartDate_Specified = true;  }
  bool __fastcall StartDate_Specified(int Index)
  {  return FStartDate_Specified;  } 
__published:
  __property UnicodeString AccessedBy = { index=(IS_OPTN|IS_NLBL), read=FAccessedBy, write=SetAccessedBy, stored = AccessedBy_Specified };
  __property UnicodeString AdditionalInformation = { index=(IS_OPTN|IS_NLBL), read=FAdditionalInformation, write=SetAdditionalInformation, stored = AdditionalInformation_Specified };
  __property UnicodeString   Currency = { index=(IS_OPTN|IS_NLBL), read=FCurrency, write=SetCurrency, stored = Currency_Specified };
  __property UnicodeString CurrentAmount = { index=(IS_OPTN|IS_NLBL), read=FCurrentAmount, write=SetCurrentAmount, stored = CurrentAmount_Specified };
  __property UnicodeString CustomerOrderID = { index=(IS_OPTN|IS_NLBL), read=FCustomerOrderID, write=SetCustomerOrderID, stored = CustomerOrderID_Specified };
  __property UnicodeString    EndDate = { index=(IS_OPTN|IS_NLBL), read=FEndDate, write=SetEndDate, stored = EndDate_Specified };
  __property UnicodeString ForecastedAmount = { index=(IS_OPTN|IS_NLBL), read=FForecastedAmount, write=SetForecastedAmount, stored = ForecastedAmount_Specified };
  __property UnicodeString OpenOrderState = { index=(IS_OPTN|IS_NLBL), read=FOpenOrderState, write=SetOpenOrderState, stored = OpenOrderState_Specified };
  __property UnicodeString SaleReferenceID = { index=(IS_OPTN|IS_NLBL), read=FSaleReferenceID, write=SetSaleReferenceID, stored = SaleReferenceID_Specified };
  __property UnicodeString  StartDate = { index=(IS_OPTN|IS_NLBL), read=FStartDate, write=SetStartDate, stored = StartDate_Specified };
};




// ************************************************************************ //
// XML       : TrackData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class TrackData : public TRemotable {
private:
  UnicodeString   FTrackFormat;
  bool            FTrackFormat_Specified;
  int             FTrackNumb;
  bool            FTrackNumb_Specified;
  UnicodeString   FTrackValue;
  bool            FTrackValue_Specified;
  void __fastcall SetTrackFormat(int Index, UnicodeString _prop_val)
  {  FTrackFormat = _prop_val; FTrackFormat_Specified = true;  }
  bool __fastcall TrackFormat_Specified(int Index)
  {  return FTrackFormat_Specified;  } 
  void __fastcall SetTrackNumb(int Index, int _prop_val)
  {  FTrackNumb = _prop_val; FTrackNumb_Specified = true;  }
  bool __fastcall TrackNumb_Specified(int Index)
  {  return FTrackNumb_Specified;  } 
  void __fastcall SetTrackValue(int Index, UnicodeString _prop_val)
  {  FTrackValue = _prop_val; FTrackValue_Specified = true;  }
  bool __fastcall TrackValue_Specified(int Index)
  {  return FTrackValue_Specified;  } 
__published:
  __property UnicodeString TrackFormat = { index=(IS_OPTN|IS_NLBL), read=FTrackFormat, write=SetTrackFormat, stored = TrackFormat_Specified };
  __property int         TrackNumb = { index=(IS_OPTN), read=FTrackNumb, write=SetTrackNumb, stored = TrackNumb_Specified };
  __property UnicodeString TrackValue = { index=(IS_OPTN|IS_NLBL), read=FTrackValue, write=SetTrackValue, stored = TrackValue_Specified };
};




// ************************************************************************ //
// XML       : SaleSoftware, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleSoftware2 : public SaleSoftware {
private:
__published:
};




// ************************************************************************ //
// XML       : SaleTerminalData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleTerminalData2 : public SaleTerminalData {
private:
__published:
};




// ************************************************************************ //
// XML       : SaleProfile, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleProfile2 : public SaleProfile {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentRequest2 : public PaymentRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentData2 : public PaymentData {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentTransaction, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentTransaction2 : public PaymentTransaction {
private:
__published:
};




// ************************************************************************ //
// XML       : AmountsReq, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class AmountsReq2 : public AmountsReq {
private:
__published:
};




// ************************************************************************ //
// XML       : SaleData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleData2 : public SaleData {
private:
__published:
};




// ************************************************************************ //
// XML       : SaleTransactionID, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SaleTransactionID2 : public SaleTransactionID {
private:
__published:
};




// ************************************************************************ //
// XML       : ReversalRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class ReversalRequest2 : public ReversalRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : OriginalPOITransaction, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class OriginalPOITransaction2 : public OriginalPOITransaction {
private:
__published:
};




// ************************************************************************ //
// XML       : POITransactionID, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class POITransactionID2 : public POITransactionID {
private:
__published:
};




// ************************************************************************ //
// XML       : Response, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class Response2 : public Response {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentResponse2 : public PaymentResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : POIData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class POIData2 : public POIData {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentReceipt, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentReceipt2 : public PaymentReceipt {
private:
__published:
};




// ************************************************************************ //
// XML       : OutputContent, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class OutputContent2 : public OutputContent {
private:
__published:
};




// ************************************************************************ //
// XML       : OutputText, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class OutputText2 : public OutputText {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentResult, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentResult2 : public PaymentResult {
private:
__published:
};




// ************************************************************************ //
// XML       : AmountsResp, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class AmountsResp2 : public AmountsResp {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentAcquirerData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentAcquirerData2 : public PaymentAcquirerData {
private:
__published:
};




// ************************************************************************ //
// XML       : AcquirerTransactionID, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class AcquirerTransactionID2 : public AcquirerTransactionID {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentInstrumentData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentInstrumentData2 : public PaymentInstrumentData {
private:
__published:
};




// ************************************************************************ //
// XML       : CardData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class CardData2 : public CardData {
private:
__published:
};




// ************************************************************************ //
// XML       : AllowedProduct, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class AllowedProduct2 : public AllowedProduct {
private:
__published:
};




// ************************************************************************ //
// XML       : CustomerOrder, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class CustomerOrder2 : public CustomerOrder {
private:
__published:
};




// ************************************************************************ //
// XML       : PaymentToken, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class PaymentToken2 : public PaymentToken {
private:
__published:
};




// ************************************************************************ //
// XML       : SensitiveCardData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class SensitiveCardData2 : public SensitiveCardData {
private:
__published:
};




// ************************************************************************ //
// XML       : TrackData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain
// ************************************************************************ //
class TrackData2 : public TrackData {
private:
__published:
};




// ************************************************************************ //
// XML       : DiagnosisRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis
// ************************************************************************ //
class DiagnosisRequest : public TRemotable {
private:
__published:
};




// ************************************************************************ //
// XML       : DiagnosisResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis
// ************************************************************************ //
class DiagnosisResponse : public TRemotable {
private:
  POIStatus*      FPOIStatus;
  bool            FPOIStatus_Specified;
  Response*       FResponse;
  bool            FResponse_Specified;
  void __fastcall SetPOIStatus(int Index, POIStatus* _prop_val)
  {  FPOIStatus = _prop_val; FPOIStatus_Specified = true;  }
  bool __fastcall POIStatus_Specified(int Index)
  {  return FPOIStatus_Specified;  } 
  void __fastcall SetResponse(int Index, Response* _prop_val)
  {  FResponse = _prop_val; FResponse_Specified = true;  }
  bool __fastcall Response_Specified(int Index)
  {  return FResponse_Specified;  } 

public:
  __fastcall ~DiagnosisResponse();
__published:
  __property POIStatus*  POIStatus = { index=(IS_OPTN|IS_NLBL), read=FPOIStatus, write=SetPOIStatus, stored = POIStatus_Specified };
  __property Response*    Response = { index=(IS_OPTN|IS_NLBL), read=FResponse, write=SetResponse, stored = Response_Specified };
};




// ************************************************************************ //
// XML       : POIStatus, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis
// ************************************************************************ //
class POIStatus : public TRemotable {
private:
  UnicodeString   FCommunicationOKFlag;
  bool            FCommunicationOKFlag_Specified;
  UnicodeString   FGlobalStatus;
  bool            FGlobalStatus_Specified;
  void __fastcall SetCommunicationOKFlag(int Index, UnicodeString _prop_val)
  {  FCommunicationOKFlag = _prop_val; FCommunicationOKFlag_Specified = true;  }
  bool __fastcall CommunicationOKFlag_Specified(int Index)
  {  return FCommunicationOKFlag_Specified;  } 
  void __fastcall SetGlobalStatus(int Index, UnicodeString _prop_val)
  {  FGlobalStatus = _prop_val; FGlobalStatus_Specified = true;  }
  bool __fastcall GlobalStatus_Specified(int Index)
  {  return FGlobalStatus_Specified;  } 
__published:
  __property UnicodeString CommunicationOKFlag = { index=(IS_OPTN|IS_NLBL), read=FCommunicationOKFlag, write=SetCommunicationOKFlag, stored = CommunicationOKFlag_Specified };
  __property UnicodeString GlobalStatus = { index=(IS_OPTN|IS_NLBL), read=FGlobalStatus, write=SetGlobalStatus, stored = GlobalStatus_Specified };
};




// ************************************************************************ //
// XML       : EventNotification, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis
// ************************************************************************ //
class EventNotification : public TRemotable {
private:
  UnicodeString   FEventDetails;
  bool            FEventDetails_Specified;
  UnicodeString   FEventToNotify;
  bool            FEventToNotify_Specified;
  UnicodeString   FRejectedMessage;
  bool            FRejectedMessage_Specified;
  UnicodeString   FTimeStamp;
  bool            FTimeStamp_Specified;
  void __fastcall SetEventDetails(int Index, UnicodeString _prop_val)
  {  FEventDetails = _prop_val; FEventDetails_Specified = true;  }
  bool __fastcall EventDetails_Specified(int Index)
  {  return FEventDetails_Specified;  } 
  void __fastcall SetEventToNotify(int Index, UnicodeString _prop_val)
  {  FEventToNotify = _prop_val; FEventToNotify_Specified = true;  }
  bool __fastcall EventToNotify_Specified(int Index)
  {  return FEventToNotify_Specified;  } 
  void __fastcall SetRejectedMessage(int Index, UnicodeString _prop_val)
  {  FRejectedMessage = _prop_val; FRejectedMessage_Specified = true;  }
  bool __fastcall RejectedMessage_Specified(int Index)
  {  return FRejectedMessage_Specified;  } 
  void __fastcall SetTimeStamp(int Index, UnicodeString _prop_val)
  {  FTimeStamp = _prop_val; FTimeStamp_Specified = true;  }
  bool __fastcall TimeStamp_Specified(int Index)
  {  return FTimeStamp_Specified;  } 
__published:
  __property UnicodeString EventDetails = { index=(IS_OPTN|IS_NLBL), read=FEventDetails, write=SetEventDetails, stored = EventDetails_Specified };
  __property UnicodeString EventToNotify = { index=(IS_OPTN|IS_NLBL), read=FEventToNotify, write=SetEventToNotify, stored = EventToNotify_Specified };
  __property UnicodeString RejectedMessage = { index=(IS_OPTN|IS_NLBL), read=FRejectedMessage, write=SetRejectedMessage, stored = RejectedMessage_Specified };
  __property UnicodeString  TimeStamp = { index=(IS_OPTN|IS_NLBL), read=FTimeStamp, write=SetTimeStamp, stored = TimeStamp_Specified };
};




// ************************************************************************ //
// XML       : DiagnosisRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis
// ************************************************************************ //
class DiagnosisRequest2 : public DiagnosisRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : DiagnosisResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis
// ************************************************************************ //
class DiagnosisResponse2 : public DiagnosisResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : POIStatus, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis
// ************************************************************************ //
class POIStatus2 : public POIStatus {
private:
__published:
};




// ************************************************************************ //
// XML       : EventNotification, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Diagnosis
// ************************************************************************ //
class EventNotification2 : public EventNotification {
private:
__published:
};




// ************************************************************************ //
// XML       : LoginRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class LoginRequest : public TRemotable {
private:
  UnicodeString   FDateTime;
  bool            FDateTime_Specified;
  UnicodeString   FOperatorID;
  bool            FOperatorID_Specified;
  UnicodeString   FOperatorLanguage;
  bool            FOperatorLanguage_Specified;
  SaleSoftware*   FSaleSoftware;
  bool            FSaleSoftware_Specified;
  SaleTerminalData* FSaleTerminalData;
  bool            FSaleTerminalData_Specified;
  void __fastcall SetDateTime(int Index, UnicodeString _prop_val)
  {  FDateTime = _prop_val; FDateTime_Specified = true;  }
  bool __fastcall DateTime_Specified(int Index)
  {  return FDateTime_Specified;  } 
  void __fastcall SetOperatorID(int Index, UnicodeString _prop_val)
  {  FOperatorID = _prop_val; FOperatorID_Specified = true;  }
  bool __fastcall OperatorID_Specified(int Index)
  {  return FOperatorID_Specified;  } 
  void __fastcall SetOperatorLanguage(int Index, UnicodeString _prop_val)
  {  FOperatorLanguage = _prop_val; FOperatorLanguage_Specified = true;  }
  bool __fastcall OperatorLanguage_Specified(int Index)
  {  return FOperatorLanguage_Specified;  } 
  void __fastcall SetSaleSoftware(int Index, SaleSoftware* _prop_val)
  {  FSaleSoftware = _prop_val; FSaleSoftware_Specified = true;  }
  bool __fastcall SaleSoftware_Specified(int Index)
  {  return FSaleSoftware_Specified;  } 
  void __fastcall SetSaleTerminalData(int Index, SaleTerminalData* _prop_val)
  {  FSaleTerminalData = _prop_val; FSaleTerminalData_Specified = true;  }
  bool __fastcall SaleTerminalData_Specified(int Index)
  {  return FSaleTerminalData_Specified;  } 

public:
  __fastcall ~LoginRequest();
__published:
  __property UnicodeString   DateTime = { index=(IS_OPTN|IS_NLBL), read=FDateTime, write=SetDateTime, stored = DateTime_Specified };
  __property UnicodeString OperatorID = { index=(IS_OPTN|IS_NLBL), read=FOperatorID, write=SetOperatorID, stored = OperatorID_Specified };
  __property UnicodeString OperatorLanguage = { index=(IS_OPTN|IS_NLBL), read=FOperatorLanguage, write=SetOperatorLanguage, stored = OperatorLanguage_Specified };
  __property SaleSoftware* SaleSoftware = { index=(IS_OPTN|IS_NLBL), read=FSaleSoftware, write=SetSaleSoftware, stored = SaleSoftware_Specified };
  __property SaleTerminalData* SaleTerminalData = { index=(IS_OPTN|IS_NLBL), read=FSaleTerminalData, write=SetSaleTerminalData, stored = SaleTerminalData_Specified };
};




// ************************************************************************ //
// XML       : LogoutRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class LogoutRequest : public TRemotable {
private:
__published:
};




// ************************************************************************ //
// XML       : LoginResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class LoginResponse : public TRemotable {
private:
  POISystemData*  FPOISystemData;
  bool            FPOISystemData_Specified;
  Response*       FResponse;
  bool            FResponse_Specified;
  void __fastcall SetPOISystemData(int Index, POISystemData* _prop_val)
  {  FPOISystemData = _prop_val; FPOISystemData_Specified = true;  }
  bool __fastcall POISystemData_Specified(int Index)
  {  return FPOISystemData_Specified;  } 
  void __fastcall SetResponse(int Index, Response* _prop_val)
  {  FResponse = _prop_val; FResponse_Specified = true;  }
  bool __fastcall Response_Specified(int Index)
  {  return FResponse_Specified;  } 

public:
  __fastcall ~LoginResponse();
__published:
  __property POISystemData* POISystemData = { index=(IS_OPTN|IS_NLBL), read=FPOISystemData, write=SetPOISystemData, stored = POISystemData_Specified };
  __property Response*    Response = { index=(IS_OPTN|IS_NLBL), read=FResponse, write=SetResponse, stored = Response_Specified };
};




// ************************************************************************ //
// XML       : POISystemData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class POISystemData : public TRemotable {
private:
  UnicodeString   FDateTime;
  bool            FDateTime_Specified;
  POIStatus*      FMyProperty;
  bool            FMyProperty_Specified;
  POISoftware*    FPOISoftware;
  bool            FPOISoftware_Specified;
  void __fastcall SetDateTime(int Index, UnicodeString _prop_val)
  {  FDateTime = _prop_val; FDateTime_Specified = true;  }
  bool __fastcall DateTime_Specified(int Index)
  {  return FDateTime_Specified;  } 
  void __fastcall SetMyProperty(int Index, POIStatus* _prop_val)
  {  FMyProperty = _prop_val; FMyProperty_Specified = true;  }
  bool __fastcall MyProperty_Specified(int Index)
  {  return FMyProperty_Specified;  } 
  void __fastcall SetPOISoftware(int Index, POISoftware* _prop_val)
  {  FPOISoftware = _prop_val; FPOISoftware_Specified = true;  }
  bool __fastcall POISoftware_Specified(int Index)
  {  return FPOISoftware_Specified;  } 

public:
  __fastcall ~POISystemData();
__published:
  __property UnicodeString   DateTime = { index=(IS_OPTN|IS_NLBL), read=FDateTime, write=SetDateTime, stored = DateTime_Specified };
  __property POIStatus* MyProperty = { index=(IS_OPTN|IS_NLBL), read=FMyProperty, write=SetMyProperty, stored = MyProperty_Specified };
  __property POISoftware* POISoftware = { index=(IS_OPTN|IS_NLBL), read=FPOISoftware, write=SetPOISoftware, stored = POISoftware_Specified };
};




// ************************************************************************ //
// XML       : POISoftware, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class POISoftware : public TRemotable {
private:
  UnicodeString   FApplicationName;
  bool            FApplicationName_Specified;
  UnicodeString   FCertificationCode;
  bool            FCertificationCode_Specified;
  UnicodeString   FManufacturerID;
  bool            FManufacturerID_Specified;
  UnicodeString   FSoftwareVersion;
  bool            FSoftwareVersion_Specified;
  void __fastcall SetApplicationName(int Index, UnicodeString _prop_val)
  {  FApplicationName = _prop_val; FApplicationName_Specified = true;  }
  bool __fastcall ApplicationName_Specified(int Index)
  {  return FApplicationName_Specified;  } 
  void __fastcall SetCertificationCode(int Index, UnicodeString _prop_val)
  {  FCertificationCode = _prop_val; FCertificationCode_Specified = true;  }
  bool __fastcall CertificationCode_Specified(int Index)
  {  return FCertificationCode_Specified;  } 
  void __fastcall SetManufacturerID(int Index, UnicodeString _prop_val)
  {  FManufacturerID = _prop_val; FManufacturerID_Specified = true;  }
  bool __fastcall ManufacturerID_Specified(int Index)
  {  return FManufacturerID_Specified;  } 
  void __fastcall SetSoftwareVersion(int Index, UnicodeString _prop_val)
  {  FSoftwareVersion = _prop_val; FSoftwareVersion_Specified = true;  }
  bool __fastcall SoftwareVersion_Specified(int Index)
  {  return FSoftwareVersion_Specified;  } 
__published:
  __property UnicodeString ApplicationName = { index=(IS_OPTN|IS_NLBL), read=FApplicationName, write=SetApplicationName, stored = ApplicationName_Specified };
  __property UnicodeString CertificationCode = { index=(IS_OPTN|IS_NLBL), read=FCertificationCode, write=SetCertificationCode, stored = CertificationCode_Specified };
  __property UnicodeString ManufacturerID = { index=(IS_OPTN|IS_NLBL), read=FManufacturerID, write=SetManufacturerID, stored = ManufacturerID_Specified };
  __property UnicodeString SoftwareVersion = { index=(IS_OPTN|IS_NLBL), read=FSoftwareVersion, write=SetSoftwareVersion, stored = SoftwareVersion_Specified };
};




// ************************************************************************ //
// XML       : LogoutResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class LogoutResponse : public TRemotable {
private:
  Response*       FResponse;
  bool            FResponse_Specified;
  void __fastcall SetResponse(int Index, Response* _prop_val)
  {  FResponse = _prop_val; FResponse_Specified = true;  }
  bool __fastcall Response_Specified(int Index)
  {  return FResponse_Specified;  } 

public:
  __fastcall ~LogoutResponse();
__published:
  __property Response*    Response = { index=(IS_OPTN|IS_NLBL), read=FResponse, write=SetResponse, stored = Response_Specified };
};




// ************************************************************************ //
// XML       : LoginRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class LoginRequest2 : public LoginRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : LogoutRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class LogoutRequest2 : public LogoutRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : LoginResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class LoginResponse2 : public LoginResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : POISystemData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class POISystemData2 : public POISystemData {
private:
__published:
};




// ************************************************************************ //
// XML       : POISoftware, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class POISoftware2 : public POISoftware {
private:
__published:
};




// ************************************************************************ //
// XML       : LogoutResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.LoginLogOut
// ************************************************************************ //
class LogoutResponse2 : public LogoutResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : MessageHeader, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Common
// ************************************************************************ //
class MessageHeader : public TRemotable {
private:
  UnicodeString   FMessageCategory;
  bool            FMessageCategory_Specified;
  UnicodeString   FMessageClass;
  bool            FMessageClass_Specified;
  UnicodeString   FMessageType;
  bool            FMessageType_Specified;
  UnicodeString   FPOIID;
  bool            FPOIID_Specified;
  UnicodeString   FProtocolVersion;
  bool            FProtocolVersion_Specified;
  UnicodeString   FSaleID;
  bool            FSaleID_Specified;
  UnicodeString   FServiceID;
  bool            FServiceID_Specified;
  void __fastcall SetMessageCategory(int Index, UnicodeString _prop_val)
  {  FMessageCategory = _prop_val; FMessageCategory_Specified = true;  }
  bool __fastcall MessageCategory_Specified(int Index)
  {  return FMessageCategory_Specified;  } 
  void __fastcall SetMessageClass(int Index, UnicodeString _prop_val)
  {  FMessageClass = _prop_val; FMessageClass_Specified = true;  }
  bool __fastcall MessageClass_Specified(int Index)
  {  return FMessageClass_Specified;  } 
  void __fastcall SetMessageType(int Index, UnicodeString _prop_val)
  {  FMessageType = _prop_val; FMessageType_Specified = true;  }
  bool __fastcall MessageType_Specified(int Index)
  {  return FMessageType_Specified;  } 
  void __fastcall SetPOIID(int Index, UnicodeString _prop_val)
  {  FPOIID = _prop_val; FPOIID_Specified = true;  }
  bool __fastcall POIID_Specified(int Index)
  {  return FPOIID_Specified;  } 
  void __fastcall SetProtocolVersion(int Index, UnicodeString _prop_val)
  {  FProtocolVersion = _prop_val; FProtocolVersion_Specified = true;  }
  bool __fastcall ProtocolVersion_Specified(int Index)
  {  return FProtocolVersion_Specified;  } 
  void __fastcall SetSaleID(int Index, UnicodeString _prop_val)
  {  FSaleID = _prop_val; FSaleID_Specified = true;  }
  bool __fastcall SaleID_Specified(int Index)
  {  return FSaleID_Specified;  } 
  void __fastcall SetServiceID(int Index, UnicodeString _prop_val)
  {  FServiceID = _prop_val; FServiceID_Specified = true;  }
  bool __fastcall ServiceID_Specified(int Index)
  {  return FServiceID_Specified;  } 
__published:
  __property UnicodeString MessageCategory = { index=(IS_OPTN|IS_NLBL), read=FMessageCategory, write=SetMessageCategory, stored = MessageCategory_Specified };
  __property UnicodeString MessageClass = { index=(IS_OPTN|IS_NLBL), read=FMessageClass, write=SetMessageClass, stored = MessageClass_Specified };
  __property UnicodeString MessageType = { index=(IS_OPTN|IS_NLBL), read=FMessageType, write=SetMessageType, stored = MessageType_Specified };
  __property UnicodeString      POIID = { index=(IS_OPTN|IS_NLBL), read=FPOIID, write=SetPOIID, stored = POIID_Specified };
  __property UnicodeString ProtocolVersion = { index=(IS_OPTN|IS_NLBL), read=FProtocolVersion, write=SetProtocolVersion, stored = ProtocolVersion_Specified };
  __property UnicodeString     SaleID = { index=(IS_OPTN|IS_NLBL), read=FSaleID, write=SetSaleID, stored = SaleID_Specified };
  __property UnicodeString  ServiceID = { index=(IS_OPTN|IS_NLBL), read=FServiceID, write=SetServiceID, stored = ServiceID_Specified };
};




// ************************************************************************ //
// XML       : ResourceDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Common
// ************************************************************************ //
class ResourceDetails : public TRemotable {
private:
  UnicodeString   FAPIKey;
  bool            FAPIKey_Specified;
  UnicodeString   FURL;
  bool            FURL_Specified;
  void __fastcall SetAPIKey(int Index, UnicodeString _prop_val)
  {  FAPIKey = _prop_val; FAPIKey_Specified = true;  }
  bool __fastcall APIKey_Specified(int Index)
  {  return FAPIKey_Specified;  } 
  void __fastcall SetURL(int Index, UnicodeString _prop_val)
  {  FURL = _prop_val; FURL_Specified = true;  }
  bool __fastcall URL_Specified(int Index)
  {  return FURL_Specified;  } 
__published:
  __property UnicodeString     APIKey = { index=(IS_OPTN|IS_NLBL), read=FAPIKey, write=SetAPIKey, stored = APIKey_Specified };
  __property UnicodeString        URL = { index=(IS_OPTN|IS_NLBL), read=FURL, write=SetURL, stored = URL_Specified };
};




// ************************************************************************ //
// XML       : MessageHeader, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Common
// ************************************************************************ //
class MessageHeader2 : public MessageHeader {
private:
__published:
};




// ************************************************************************ //
// XML       : ResourceDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.Common
// ************************************************************************ //
class ResourceDetails2 : public ResourceDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : MessageReference, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class MessageReference : public TRemotable {
private:
  UnicodeString   FMessageCategory;
  bool            FMessageCategory_Specified;
  UnicodeString   FSaleID;
  bool            FSaleID_Specified;
  UnicodeString   FServiceID;
  bool            FServiceID_Specified;
  void __fastcall SetMessageCategory(int Index, UnicodeString _prop_val)
  {  FMessageCategory = _prop_val; FMessageCategory_Specified = true;  }
  bool __fastcall MessageCategory_Specified(int Index)
  {  return FMessageCategory_Specified;  } 
  void __fastcall SetSaleID(int Index, UnicodeString _prop_val)
  {  FSaleID = _prop_val; FSaleID_Specified = true;  }
  bool __fastcall SaleID_Specified(int Index)
  {  return FSaleID_Specified;  } 
  void __fastcall SetServiceID(int Index, UnicodeString _prop_val)
  {  FServiceID = _prop_val; FServiceID_Specified = true;  }
  bool __fastcall ServiceID_Specified(int Index)
  {  return FServiceID_Specified;  } 
__published:
  __property UnicodeString MessageCategory = { index=(IS_OPTN|IS_NLBL), read=FMessageCategory, write=SetMessageCategory, stored = MessageCategory_Specified };
  __property UnicodeString     SaleID = { index=(IS_OPTN|IS_NLBL), read=FSaleID, write=SetSaleID, stored = SaleID_Specified };
  __property UnicodeString  ServiceID = { index=(IS_OPTN|IS_NLBL), read=FServiceID, write=SetServiceID, stored = ServiceID_Specified };
};




// ************************************************************************ //
// XML       : TransactionStatusRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class TransactionStatusRequest : public TRemotable {
private:
  ArrayOfstring   FDocumentQualifier;
  bool            FDocumentQualifier_Specified;
  MessageReference* FMessageReference;
  bool            FMessageReference_Specified;
  bool            FReceiptReprintFlag;
  bool            FReceiptReprintFlag_Specified;
  void __fastcall SetDocumentQualifier(int Index, ArrayOfstring _prop_val)
  {  FDocumentQualifier = _prop_val; FDocumentQualifier_Specified = true;  }
  bool __fastcall DocumentQualifier_Specified(int Index)
  {  return FDocumentQualifier_Specified;  } 
  void __fastcall SetMessageReference(int Index, MessageReference* _prop_val)
  {  FMessageReference = _prop_val; FMessageReference_Specified = true;  }
  bool __fastcall MessageReference_Specified(int Index)
  {  return FMessageReference_Specified;  } 
  void __fastcall SetReceiptReprintFlag(int Index, bool _prop_val)
  {  FReceiptReprintFlag = _prop_val; FReceiptReprintFlag_Specified = true;  }
  bool __fastcall ReceiptReprintFlag_Specified(int Index)
  {  return FReceiptReprintFlag_Specified;  } 

public:
  __fastcall ~TransactionStatusRequest();
__published:
  __property ArrayOfstring DocumentQualifier = { index=(IS_OPTN|IS_NLBL), read=FDocumentQualifier, write=SetDocumentQualifier, stored = DocumentQualifier_Specified };
  __property MessageReference* MessageReference = { index=(IS_OPTN|IS_NLBL), read=FMessageReference, write=SetMessageReference, stored = MessageReference_Specified };
  __property bool       ReceiptReprintFlag = { index=(IS_OPTN), read=FReceiptReprintFlag, write=SetReceiptReprintFlag, stored = ReceiptReprintFlag_Specified };
};




// ************************************************************************ //
// XML       : TransactionStatusResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class TransactionStatusResponse : public TRemotable {
private:
  MessageReference* FMessageReference;
  bool            FMessageReference_Specified;
  RepeatedMessageResponse* FRepeatedMessageResponse;
  bool            FRepeatedMessageResponse_Specified;
  Response*       FResponse;
  bool            FResponse_Specified;
  void __fastcall SetMessageReference(int Index, MessageReference* _prop_val)
  {  FMessageReference = _prop_val; FMessageReference_Specified = true;  }
  bool __fastcall MessageReference_Specified(int Index)
  {  return FMessageReference_Specified;  } 
  void __fastcall SetRepeatedMessageResponse(int Index, RepeatedMessageResponse* _prop_val)
  {  FRepeatedMessageResponse = _prop_val; FRepeatedMessageResponse_Specified = true;  }
  bool __fastcall RepeatedMessageResponse_Specified(int Index)
  {  return FRepeatedMessageResponse_Specified;  } 
  void __fastcall SetResponse(int Index, Response* _prop_val)
  {  FResponse = _prop_val; FResponse_Specified = true;  }
  bool __fastcall Response_Specified(int Index)
  {  return FResponse_Specified;  } 

public:
  __fastcall ~TransactionStatusResponse();
__published:
  __property MessageReference* MessageReference = { index=(IS_OPTN|IS_NLBL), read=FMessageReference, write=SetMessageReference, stored = MessageReference_Specified };
  __property RepeatedMessageResponse* RepeatedMessageResponse = { index=(IS_OPTN|IS_NLBL), read=FRepeatedMessageResponse, write=SetRepeatedMessageResponse, stored = RepeatedMessageResponse_Specified };
  __property Response*    Response = { index=(IS_OPTN|IS_NLBL), read=FResponse, write=SetResponse, stored = Response_Specified };
};




// ************************************************************************ //
// XML       : RepeatedMessageResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class RepeatedMessageResponse : public TRemotable {
private:
  MessageHeader*  FMessageHeader;
  bool            FMessageHeader_Specified;
  RepeatedResponseMessageBody* FRepeatedResponseMessageBody;
  bool            FRepeatedResponseMessageBody_Specified;
  void __fastcall SetMessageHeader(int Index, MessageHeader* _prop_val)
  {  FMessageHeader = _prop_val; FMessageHeader_Specified = true;  }
  bool __fastcall MessageHeader_Specified(int Index)
  {  return FMessageHeader_Specified;  } 
  void __fastcall SetRepeatedResponseMessageBody(int Index, RepeatedResponseMessageBody* _prop_val)
  {  FRepeatedResponseMessageBody = _prop_val; FRepeatedResponseMessageBody_Specified = true;  }
  bool __fastcall RepeatedResponseMessageBody_Specified(int Index)
  {  return FRepeatedResponseMessageBody_Specified;  } 

public:
  __fastcall ~RepeatedMessageResponse();
__published:
  __property MessageHeader* MessageHeader = { index=(IS_OPTN|IS_NLBL), read=FMessageHeader, write=SetMessageHeader, stored = MessageHeader_Specified };
  __property RepeatedResponseMessageBody* RepeatedResponseMessageBody = { index=(IS_OPTN|IS_NLBL), read=FRepeatedResponseMessageBody, write=SetRepeatedResponseMessageBody, stored = RepeatedResponseMessageBody_Specified };
};




// ************************************************************************ //
// XML       : RepeatedResponseMessageBody, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class RepeatedResponseMessageBody : public TRemotable {
private:
  PaymentResponse* FPaymentResponse;
  bool            FPaymentResponse_Specified;
  void __fastcall SetPaymentResponse(int Index, PaymentResponse* _prop_val)
  {  FPaymentResponse = _prop_val; FPaymentResponse_Specified = true;  }
  bool __fastcall PaymentResponse_Specified(int Index)
  {  return FPaymentResponse_Specified;  } 

public:
  __fastcall ~RepeatedResponseMessageBody();
__published:
  __property PaymentResponse* PaymentResponse = { index=(IS_OPTN|IS_NLBL), read=FPaymentResponse, write=SetPaymentResponse, stored = PaymentResponse_Specified };
};




// ************************************************************************ //
// XML       : TransactionStatusRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class TransactionStatusRequest2 : public TransactionStatusRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : MessageReference, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class MessageReference2 : public MessageReference {
private:
__published:
};




// ************************************************************************ //
// XML       : TransactionStatusResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class TransactionStatusResponse2 : public TransactionStatusResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : RepeatedMessageResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class RepeatedMessageResponse2 : public RepeatedMessageResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : RepeatedResponseMessageBody, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.TransactionStatus
// ************************************************************************ //
class RepeatedResponseMessageBody2 : public RepeatedResponseMessageBody {
private:
__published:
};




// ************************************************************************ //
// XML       : Envelop, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.RequestEnvelop
// ************************************************************************ //
class Envelop : public TRemotable {
private:
  SaleToPOIRequest* FSaleToPOIRequest;
  bool            FSaleToPOIRequest_Specified;
  void __fastcall SetSaleToPOIRequest(int Index, SaleToPOIRequest* _prop_val)
  {  FSaleToPOIRequest = _prop_val; FSaleToPOIRequest_Specified = true;  }
  bool __fastcall SaleToPOIRequest_Specified(int Index)
  {  return FSaleToPOIRequest_Specified;  } 

public:
  __fastcall ~Envelop();
__published:
  __property SaleToPOIRequest* SaleToPOIRequest = { index=(IS_OPTN|IS_NLBL), read=FSaleToPOIRequest, write=SetSaleToPOIRequest, stored = SaleToPOIRequest_Specified };
};




// ************************************************************************ //
// XML       : SaleToPOIRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.RequestEnvelop
// ************************************************************************ //
class SaleToPOIRequest : public TRemotable {
private:
  DiagnosisRequest* FDiagnosisRequest;
  bool            FDiagnosisRequest_Specified;
  LoginRequest*   FLoginRequest;
  bool            FLoginRequest_Specified;
  LogoutRequest*  FLogoutRequest;
  bool            FLogoutRequest_Specified;
  MessageHeader*  FMessageHeader;
  bool            FMessageHeader_Specified;
  PaymentRequest* FPaymentRequest;
  bool            FPaymentRequest_Specified;
  ReversalRequest* FReversalRequest;
  bool            FReversalRequest_Specified;
  TransactionStatusRequest* FTransactionStatusRequest;
  bool            FTransactionStatusRequest_Specified;
  void __fastcall SetDiagnosisRequest(int Index, DiagnosisRequest* _prop_val)
  {  FDiagnosisRequest = _prop_val; FDiagnosisRequest_Specified = true;  }
  bool __fastcall DiagnosisRequest_Specified(int Index)
  {  return FDiagnosisRequest_Specified;  } 
  void __fastcall SetLoginRequest(int Index, LoginRequest* _prop_val)
  {  FLoginRequest = _prop_val; FLoginRequest_Specified = true;  }
  bool __fastcall LoginRequest_Specified(int Index)
  {  return FLoginRequest_Specified;  } 
  void __fastcall SetLogoutRequest(int Index, LogoutRequest* _prop_val)
  {  FLogoutRequest = _prop_val; FLogoutRequest_Specified = true;  }
  bool __fastcall LogoutRequest_Specified(int Index)
  {  return FLogoutRequest_Specified;  } 
  void __fastcall SetMessageHeader(int Index, MessageHeader* _prop_val)
  {  FMessageHeader = _prop_val; FMessageHeader_Specified = true;  }
  bool __fastcall MessageHeader_Specified(int Index)
  {  return FMessageHeader_Specified;  } 
  void __fastcall SetPaymentRequest(int Index, PaymentRequest* _prop_val)
  {  FPaymentRequest = _prop_val; FPaymentRequest_Specified = true;  }
  bool __fastcall PaymentRequest_Specified(int Index)
  {  return FPaymentRequest_Specified;  } 
  void __fastcall SetReversalRequest(int Index, ReversalRequest* _prop_val)
  {  FReversalRequest = _prop_val; FReversalRequest_Specified = true;  }
  bool __fastcall ReversalRequest_Specified(int Index)
  {  return FReversalRequest_Specified;  } 
  void __fastcall SetTransactionStatusRequest(int Index, TransactionStatusRequest* _prop_val)
  {  FTransactionStatusRequest = _prop_val; FTransactionStatusRequest_Specified = true;  }
  bool __fastcall TransactionStatusRequest_Specified(int Index)
  {  return FTransactionStatusRequest_Specified;  } 

public:
  __fastcall ~SaleToPOIRequest();
__published:
  __property DiagnosisRequest* DiagnosisRequest = { index=(IS_OPTN|IS_NLBL), read=FDiagnosisRequest, write=SetDiagnosisRequest, stored = DiagnosisRequest_Specified };
  __property LoginRequest* LoginRequest = { index=(IS_OPTN|IS_NLBL), read=FLoginRequest, write=SetLoginRequest, stored = LoginRequest_Specified };
  __property LogoutRequest* LogoutRequest = { index=(IS_OPTN|IS_NLBL), read=FLogoutRequest, write=SetLogoutRequest, stored = LogoutRequest_Specified };
  __property MessageHeader* MessageHeader = { index=(IS_OPTN|IS_NLBL), read=FMessageHeader, write=SetMessageHeader, stored = MessageHeader_Specified };
  __property PaymentRequest* PaymentRequest = { index=(IS_OPTN|IS_NLBL), read=FPaymentRequest, write=SetPaymentRequest, stored = PaymentRequest_Specified };
  __property ReversalRequest* ReversalRequest = { index=(IS_OPTN|IS_NLBL), read=FReversalRequest, write=SetReversalRequest, stored = ReversalRequest_Specified };
  __property TransactionStatusRequest* TransactionStatusRequest = { index=(IS_OPTN|IS_NLBL), read=FTransactionStatusRequest, write=SetTransactionStatusRequest, stored = TransactionStatusRequest_Specified };
};




// ************************************************************************ //
// XML       : Envelop, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.RequestEnvelop
// ************************************************************************ //
class Envelop2 : public Envelop {
private:
__published:
};




// ************************************************************************ //
// XML       : SaleToPOIRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.RequestEnvelop
// ************************************************************************ //
class SaleToPOIRequest2 : public SaleToPOIRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : SaleToPOIResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.ResponseEnvelop
// ************************************************************************ //
class SaleToPOIResponse : public TRemotable {
private:
  DiagnosisResponse* FDiagnosisResponse;
  bool            FDiagnosisResponse_Specified;
  EventNotification* FEventNotification;
  bool            FEventNotification_Specified;
  LoginResponse*  FLoginResponse;
  bool            FLoginResponse_Specified;
  LogoutResponse* FLogoutResponse;
  bool            FLogoutResponse_Specified;
  MessageHeader*  FMessageHeader;
  bool            FMessageHeader_Specified;
  PaymentResponse* FPaymentResponse;
  bool            FPaymentResponse_Specified;
  TransactionStatusResponse* FTransactionStatusResponse;
  bool            FTransactionStatusResponse_Specified;
  void __fastcall SetDiagnosisResponse(int Index, DiagnosisResponse* _prop_val)
  {  FDiagnosisResponse = _prop_val; FDiagnosisResponse_Specified = true;  }
  bool __fastcall DiagnosisResponse_Specified(int Index)
  {  return FDiagnosisResponse_Specified;  } 
  void __fastcall SetEventNotification(int Index, EventNotification* _prop_val)
  {  FEventNotification = _prop_val; FEventNotification_Specified = true;  }
  bool __fastcall EventNotification_Specified(int Index)
  {  return FEventNotification_Specified;  } 
  void __fastcall SetLoginResponse(int Index, LoginResponse* _prop_val)
  {  FLoginResponse = _prop_val; FLoginResponse_Specified = true;  }
  bool __fastcall LoginResponse_Specified(int Index)
  {  return FLoginResponse_Specified;  } 
  void __fastcall SetLogoutResponse(int Index, LogoutResponse* _prop_val)
  {  FLogoutResponse = _prop_val; FLogoutResponse_Specified = true;  }
  bool __fastcall LogoutResponse_Specified(int Index)
  {  return FLogoutResponse_Specified;  } 
  void __fastcall SetMessageHeader(int Index, MessageHeader* _prop_val)
  {  FMessageHeader = _prop_val; FMessageHeader_Specified = true;  }
  bool __fastcall MessageHeader_Specified(int Index)
  {  return FMessageHeader_Specified;  } 
  void __fastcall SetPaymentResponse(int Index, PaymentResponse* _prop_val)
  {  FPaymentResponse = _prop_val; FPaymentResponse_Specified = true;  }
  bool __fastcall PaymentResponse_Specified(int Index)
  {  return FPaymentResponse_Specified;  } 
  void __fastcall SetTransactionStatusResponse(int Index, TransactionStatusResponse* _prop_val)
  {  FTransactionStatusResponse = _prop_val; FTransactionStatusResponse_Specified = true;  }
  bool __fastcall TransactionStatusResponse_Specified(int Index)
  {  return FTransactionStatusResponse_Specified;  } 

public:
  __fastcall ~SaleToPOIResponse();
__published:
  __property DiagnosisResponse* DiagnosisResponse = { index=(IS_OPTN|IS_NLBL), read=FDiagnosisResponse, write=SetDiagnosisResponse, stored = DiagnosisResponse_Specified };
  __property EventNotification* EventNotification = { index=(IS_OPTN|IS_NLBL), read=FEventNotification, write=SetEventNotification, stored = EventNotification_Specified };
  __property LoginResponse* LoginResponse = { index=(IS_OPTN|IS_NLBL), read=FLoginResponse, write=SetLoginResponse, stored = LoginResponse_Specified };
  __property LogoutResponse* LogoutResponse = { index=(IS_OPTN|IS_NLBL), read=FLogoutResponse, write=SetLogoutResponse, stored = LogoutResponse_Specified };
  __property MessageHeader* MessageHeader = { index=(IS_OPTN|IS_NLBL), read=FMessageHeader, write=SetMessageHeader, stored = MessageHeader_Specified };
  __property PaymentResponse* PaymentResponse = { index=(IS_OPTN|IS_NLBL), read=FPaymentResponse, write=SetPaymentResponse, stored = PaymentResponse_Specified };
  __property TransactionStatusResponse* TransactionStatusResponse = { index=(IS_OPTN|IS_NLBL), read=FTransactionStatusResponse, write=SetTransactionStatusResponse, stored = TransactionStatusResponse_Specified };
};




// ************************************************************************ //
// XML       : SaleToPOIResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.ResponseEnvelop
// ************************************************************************ //
class SaleToPOIResponse2 : public SaleToPOIResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : CaptureModifiedAmount, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class CaptureModifiedAmount : public TRemotable {
private:
  UnicodeString   FmerchantAccount;
  bool            FmerchantAccount_Specified;
  ModificationAmount* FmodificationAmount;
  bool            FmodificationAmount_Specified;
  UnicodeString   ForiginalReference;
  bool            ForiginalReference_Specified;
  UnicodeString   Freference;
  bool            Freference_Specified;
  void __fastcall SetmerchantAccount(int Index, UnicodeString _prop_val)
  {  FmerchantAccount = _prop_val; FmerchantAccount_Specified = true;  }
  bool __fastcall merchantAccount_Specified(int Index)
  {  return FmerchantAccount_Specified;  } 
  void __fastcall SetmodificationAmount(int Index, ModificationAmount* _prop_val)
  {  FmodificationAmount = _prop_val; FmodificationAmount_Specified = true;  }
  bool __fastcall modificationAmount_Specified(int Index)
  {  return FmodificationAmount_Specified;  } 
  void __fastcall SetoriginalReference(int Index, UnicodeString _prop_val)
  {  ForiginalReference = _prop_val; ForiginalReference_Specified = true;  }
  bool __fastcall originalReference_Specified(int Index)
  {  return ForiginalReference_Specified;  } 
  void __fastcall Setreference(int Index, UnicodeString _prop_val)
  {  Freference = _prop_val; Freference_Specified = true;  }
  bool __fastcall reference_Specified(int Index)
  {  return Freference_Specified;  } 

public:
  __fastcall ~CaptureModifiedAmount();
__published:
  __property UnicodeString merchantAccount = { index=(IS_OPTN|IS_NLBL), read=FmerchantAccount, write=SetmerchantAccount, stored = merchantAccount_Specified };
  __property ModificationAmount* modificationAmount = { index=(IS_OPTN|IS_NLBL), read=FmodificationAmount, write=SetmodificationAmount, stored = modificationAmount_Specified };
  __property UnicodeString originalReference = { index=(IS_OPTN|IS_NLBL), read=ForiginalReference, write=SetoriginalReference, stored = originalReference_Specified };
  __property UnicodeString  reference = { index=(IS_OPTN|IS_NLBL), read=Freference, write=Setreference, stored = reference_Specified };
};




// ************************************************************************ //
// XML       : ModificationAmount, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class ModificationAmount : public TRemotable {
private:
  UnicodeString   Fcurrency;
  bool            Fcurrency_Specified;
  int             Fvalue;
  bool            Fvalue_Specified;
  void __fastcall Setcurrency(int Index, UnicodeString _prop_val)
  {  Fcurrency = _prop_val; Fcurrency_Specified = true;  }
  bool __fastcall currency_Specified(int Index)
  {  return Fcurrency_Specified;  } 
  void __fastcall Setvalue(int Index, int _prop_val)
  {  Fvalue = _prop_val; Fvalue_Specified = true;  }
  bool __fastcall value_Specified(int Index)
  {  return Fvalue_Specified;  } 
__published:
  __property UnicodeString   currency = { index=(IS_OPTN|IS_NLBL), read=Fcurrency, write=Setcurrency, stored = currency_Specified };
  __property int             value = { index=(IS_OPTN), read=Fvalue, write=Setvalue, stored = value_Specified };
};




// ************************************************************************ //
// XML       : AdjustAndCaptureResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class AdjustAndCaptureResponse : public TRemotable {
private:
  UnicodeString   FpspReference;
  bool            FpspReference_Specified;
  UnicodeString   Fresponse;
  bool            Fresponse_Specified;
  void __fastcall SetpspReference(int Index, UnicodeString _prop_val)
  {  FpspReference = _prop_val; FpspReference_Specified = true;  }
  bool __fastcall pspReference_Specified(int Index)
  {  return FpspReference_Specified;  } 
  void __fastcall Setresponse(int Index, UnicodeString _prop_val)
  {  Fresponse = _prop_val; Fresponse_Specified = true;  }
  bool __fastcall response_Specified(int Index)
  {  return Fresponse_Specified;  } 
__published:
  __property UnicodeString pspReference = { index=(IS_OPTN|IS_NLBL), read=FpspReference, write=SetpspReference, stored = pspReference_Specified };
  __property UnicodeString   response = { index=(IS_OPTN|IS_NLBL), read=Fresponse, write=Setresponse, stored = response_Specified };
};




// ************************************************************************ //
// XML       : AdjustAuthorisation, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class AdjustAuthorisation : public TRemotable {
private:
  AdditionalData* FadditionalData;
  bool            FadditionalData_Specified;
  UnicodeString   FmerchantAccount;
  bool            FmerchantAccount_Specified;
  ModificationAmount* FmodificationAmount;
  bool            FmodificationAmount_Specified;
  UnicodeString   ForiginalReference;
  bool            ForiginalReference_Specified;
  UnicodeString   Freference;
  bool            Freference_Specified;
  void __fastcall SetadditionalData(int Index, AdditionalData* _prop_val)
  {  FadditionalData = _prop_val; FadditionalData_Specified = true;  }
  bool __fastcall additionalData_Specified(int Index)
  {  return FadditionalData_Specified;  } 
  void __fastcall SetmerchantAccount(int Index, UnicodeString _prop_val)
  {  FmerchantAccount = _prop_val; FmerchantAccount_Specified = true;  }
  bool __fastcall merchantAccount_Specified(int Index)
  {  return FmerchantAccount_Specified;  } 
  void __fastcall SetmodificationAmount(int Index, ModificationAmount* _prop_val)
  {  FmodificationAmount = _prop_val; FmodificationAmount_Specified = true;  }
  bool __fastcall modificationAmount_Specified(int Index)
  {  return FmodificationAmount_Specified;  } 
  void __fastcall SetoriginalReference(int Index, UnicodeString _prop_val)
  {  ForiginalReference = _prop_val; ForiginalReference_Specified = true;  }
  bool __fastcall originalReference_Specified(int Index)
  {  return ForiginalReference_Specified;  } 
  void __fastcall Setreference(int Index, UnicodeString _prop_val)
  {  Freference = _prop_val; Freference_Specified = true;  }
  bool __fastcall reference_Specified(int Index)
  {  return Freference_Specified;  } 

public:
  __fastcall ~AdjustAuthorisation();
__published:
  __property AdditionalData* additionalData = { index=(IS_OPTN|IS_NLBL), read=FadditionalData, write=SetadditionalData, stored = additionalData_Specified };
  __property UnicodeString merchantAccount = { index=(IS_OPTN|IS_NLBL), read=FmerchantAccount, write=SetmerchantAccount, stored = merchantAccount_Specified };
  __property ModificationAmount* modificationAmount = { index=(IS_OPTN|IS_NLBL), read=FmodificationAmount, write=SetmodificationAmount, stored = modificationAmount_Specified };
  __property UnicodeString originalReference = { index=(IS_OPTN|IS_NLBL), read=ForiginalReference, write=SetoriginalReference, stored = originalReference_Specified };
  __property UnicodeString  reference = { index=(IS_OPTN|IS_NLBL), read=Freference, write=Setreference, stored = reference_Specified };
};




// ************************************************************************ //
// XML       : AdditionalData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class AdditionalData : public TRemotable {
private:
  UnicodeString   FindustryUsage;
  bool            FindustryUsage_Specified;
  void __fastcall SetindustryUsage(int Index, UnicodeString _prop_val)
  {  FindustryUsage = _prop_val; FindustryUsage_Specified = true;  }
  bool __fastcall industryUsage_Specified(int Index)
  {  return FindustryUsage_Specified;  } 
__published:
  __property UnicodeString industryUsage = { index=(IS_OPTN|IS_NLBL), read=FindustryUsage, write=SetindustryUsage, stored = industryUsage_Specified };
};




// ************************************************************************ //
// XML       : CaptureModifiedAmount, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class CaptureModifiedAmount2 : public CaptureModifiedAmount {
private:
__published:
};




// ************************************************************************ //
// XML       : ModificationAmount, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class ModificationAmount2 : public ModificationAmount {
private:
__published:
};




// ************************************************************************ //
// XML       : AdjustAndCaptureResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class AdjustAndCaptureResponse2 : public AdjustAndCaptureResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : AdjustAuthorisation, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class AdjustAuthorisation2 : public AdjustAuthorisation {
private:
__published:
};




// ************************************************************************ //
// XML       : AdditionalData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AdyenIntegration.Domain.AdjustAuthorisation
// ************************************************************************ //
class AdditionalData2 : public AdditionalData {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IAdyenIntegrationWebService/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceAdyen
// service   : AdyenIntegrationWebService
// port      : basicHttpBinding_ServiceAdyen
// URL       : http://localhost:8745/MenumateServices/AdyenService/
// ************************************************************************ //
__interface INTERFACE_UUID("{CB7DA158-8289-7B0C-E0D7-1BD2CB8F2A59}") IAdyenIntegrationWebService : public IInvokable
{
public:
  virtual SaleToPOIResponse* PingTerminal(const Envelop* envelop, const ResourceDetails* details) = 0; 
  virtual SaleToPOIResponse* Purchase(const Envelop* envelop, const ResourceDetails* details) = 0; 
  virtual SaleToPOIResponse* Refund(const Envelop* envelop, const ResourceDetails* details) = 0; 
  virtual SaleToPOIResponse* GetTransactionStatus(const Envelop* envelop, const ResourceDetails* details) = 0; 
  virtual SaleToPOIResponse* LoginToSystem(const Envelop* envelop, const ResourceDetails* details) = 0; 
  virtual SaleToPOIResponse* LogoutSystem(const Envelop* envelop, const ResourceDetails* details) = 0; 
  virtual AdjustAndCaptureResponse* CaptureModifiedAmount(const CaptureModifiedAmount* authRequest, const ResourceDetails* details) = 0; 
  virtual AdjustAndCaptureResponse* AdjustAuthorisation(const AdjustAuthorisation* authRequest, const ResourceDetails* details) = 0; 
};
typedef DelphiInterface<IAdyenIntegrationWebService> _di_IAdyenIntegrationWebService;

_di_IAdyenIntegrationWebService GetIAdyenIntegrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__AdyenWSDL;
#endif



#endif // _H
