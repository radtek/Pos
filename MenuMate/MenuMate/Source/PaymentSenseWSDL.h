// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?wsdl
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?wsdl>0
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd0
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd2
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd3
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd4
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (6/06/2018 1:33:00 a.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   PaymentSenseWSDLH
#define   PaymentSenseWSDLH

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


namespace NS__PaymentSenseWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS AuthorizationDetails;
class SOAP_REMOTABLE_CLASS PACTerminalWrapper;
class SOAP_REMOTABLE_CLASS PACTerminal;
class SOAP_REMOTABLE_CLASS TransactionRequest;
class SOAP_REMOTABLE_CLASS PostRequestResponse;
class SOAP_REMOTABLE_CLASS Reports;
class SOAP_REMOTABLE_CLASS TransactionDataResponse;
class SOAP_REMOTABLE_CLASS ReceiptLines;
class SOAP_REMOTABLE_CLASS ReceiptData;
class SOAP_REMOTABLE_CLASS AuthorizationDetails2;
class SOAP_REMOTABLE_CLASS PACTerminalWrapper2;
class SOAP_REMOTABLE_CLASS PACTerminal2;
class SOAP_REMOTABLE_CLASS TransactionRequest2;
class SOAP_REMOTABLE_CLASS PostRequestResponse2;
class SOAP_REMOTABLE_CLASS Reports2;
class SOAP_REMOTABLE_CLASS TransactionDataResponse2;
class SOAP_REMOTABLE_CLASS ReceiptLines2;
class SOAP_REMOTABLE_CLASS ReceiptData2;
class SOAP_REMOTABLE_CLASS SignatureRequest;
class SOAP_REMOTABLE_CLASS SignatureRequest2;

typedef DynamicArray<UnicodeString> ArrayOfstring;  /* "http://schemas.microsoft.com/2003/10/Serialization/Arrays"[GblCplx] */


// ************************************************************************ //
// XML       : AuthorizationDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class AuthorizationDetails : public TRemotable {
private:
  UnicodeString   FPassword;
  bool            FPassword_Specified;
  UnicodeString   FTerminalId;
  bool            FTerminalId_Specified;
  UnicodeString   FURL;
  bool            FURL_Specified;
  UnicodeString   FUserName;
  bool            FUserName_Specified;
  void __fastcall SetPassword(int Index, UnicodeString _prop_val)
  {  FPassword = _prop_val; FPassword_Specified = true;  }
  bool __fastcall Password_Specified(int Index)
  {  return FPassword_Specified;  } 
  void __fastcall SetTerminalId(int Index, UnicodeString _prop_val)
  {  FTerminalId = _prop_val; FTerminalId_Specified = true;  }
  bool __fastcall TerminalId_Specified(int Index)
  {  return FTerminalId_Specified;  } 
  void __fastcall SetURL(int Index, UnicodeString _prop_val)
  {  FURL = _prop_val; FURL_Specified = true;  }
  bool __fastcall URL_Specified(int Index)
  {  return FURL_Specified;  } 
  void __fastcall SetUserName(int Index, UnicodeString _prop_val)
  {  FUserName = _prop_val; FUserName_Specified = true;  }
  bool __fastcall UserName_Specified(int Index)
  {  return FUserName_Specified;  } 
__published:
  __property UnicodeString   Password = { index=(IS_OPTN|IS_NLBL), read=FPassword, write=SetPassword, stored = Password_Specified };
  __property UnicodeString TerminalId = { index=(IS_OPTN|IS_NLBL), read=FTerminalId, write=SetTerminalId, stored = TerminalId_Specified };
  __property UnicodeString        URL = { index=(IS_OPTN|IS_NLBL), read=FURL, write=SetURL, stored = URL_Specified };
  __property UnicodeString   UserName = { index=(IS_OPTN|IS_NLBL), read=FUserName, write=SetUserName, stored = UserName_Specified };
};


typedef DynamicArray<PACTerminal*> ArrayOfPACTerminal; /* "http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : PACTerminalWrapper, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class PACTerminalWrapper : public TRemotable {
private:
  ArrayOfPACTerminal FTerminals;
  bool            FTerminals_Specified;
  void __fastcall SetTerminals(int Index, ArrayOfPACTerminal _prop_val)
  {  FTerminals = _prop_val; FTerminals_Specified = true;  }
  bool __fastcall Terminals_Specified(int Index)
  {  return FTerminals_Specified;  } 

public:
  __fastcall ~PACTerminalWrapper();
__published:
  __property ArrayOfPACTerminal  Terminals = { index=(IS_OPTN|IS_NLBL), read=FTerminals, write=SetTerminals, stored = Terminals_Specified };
};




// ************************************************************************ //
// XML       : PACTerminal, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class PACTerminal : public TRemotable {
private:
  ArrayOfstring   FCapabilities;
  bool            FCapabilities_Specified;
  UnicodeString   FCurrency;
  bool            FCurrency_Specified;
  UnicodeString   FLocation;
  bool            FLocation_Specified;
  UnicodeString   FStatus;
  bool            FStatus_Specified;
  UnicodeString   FTPI;
  bool            FTPI_Specified;
  void __fastcall SetCapabilities(int Index, ArrayOfstring _prop_val)
  {  FCapabilities = _prop_val; FCapabilities_Specified = true;  }
  bool __fastcall Capabilities_Specified(int Index)
  {  return FCapabilities_Specified;  } 
  void __fastcall SetCurrency(int Index, UnicodeString _prop_val)
  {  FCurrency = _prop_val; FCurrency_Specified = true;  }
  bool __fastcall Currency_Specified(int Index)
  {  return FCurrency_Specified;  } 
  void __fastcall SetLocation(int Index, UnicodeString _prop_val)
  {  FLocation = _prop_val; FLocation_Specified = true;  }
  bool __fastcall Location_Specified(int Index)
  {  return FLocation_Specified;  } 
  void __fastcall SetStatus(int Index, UnicodeString _prop_val)
  {  FStatus = _prop_val; FStatus_Specified = true;  }
  bool __fastcall Status_Specified(int Index)
  {  return FStatus_Specified;  } 
  void __fastcall SetTPI(int Index, UnicodeString _prop_val)
  {  FTPI = _prop_val; FTPI_Specified = true;  }
  bool __fastcall TPI_Specified(int Index)
  {  return FTPI_Specified;  } 
__published:
  __property ArrayOfstring Capabilities = { index=(IS_OPTN|IS_NLBL), read=FCapabilities, write=SetCapabilities, stored = Capabilities_Specified };
  __property UnicodeString   Currency = { index=(IS_OPTN|IS_NLBL), read=FCurrency, write=SetCurrency, stored = Currency_Specified };
  __property UnicodeString   Location = { index=(IS_OPTN|IS_NLBL), read=FLocation, write=SetLocation, stored = Location_Specified };
  __property UnicodeString     Status = { index=(IS_OPTN|IS_NLBL), read=FStatus, write=SetStatus, stored = Status_Specified };
  __property UnicodeString        TPI = { index=(IS_OPTN|IS_NLBL), read=FTPI, write=SetTPI, stored = TPI_Specified };
};




// ************************************************************************ //
// XML       : TransactionRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class TransactionRequest : public TRemotable {
private:
  double          Famount;
  bool            Famount_Specified;
  UnicodeString   Fcurrency;
  bool            Fcurrency_Specified;
  UnicodeString   FtransactionType;
  bool            FtransactionType_Specified;
  void __fastcall Setamount(int Index, double _prop_val)
  {  Famount = _prop_val; Famount_Specified = true;  }
  bool __fastcall amount_Specified(int Index)
  {  return Famount_Specified;  } 
  void __fastcall Setcurrency(int Index, UnicodeString _prop_val)
  {  Fcurrency = _prop_val; Fcurrency_Specified = true;  }
  bool __fastcall currency_Specified(int Index)
  {  return Fcurrency_Specified;  } 
  void __fastcall SettransactionType(int Index, UnicodeString _prop_val)
  {  FtransactionType = _prop_val; FtransactionType_Specified = true;  }
  bool __fastcall transactionType_Specified(int Index)
  {  return FtransactionType_Specified;  } 
__published:
  __property double         amount = { index=(IS_OPTN), read=Famount, write=Setamount, stored = amount_Specified };
  __property UnicodeString   currency = { index=(IS_OPTN|IS_NLBL), read=Fcurrency, write=Setcurrency, stored = currency_Specified };
  __property UnicodeString transactionType = { index=(IS_OPTN|IS_NLBL), read=FtransactionType, write=SettransactionType, stored = transactionType_Specified };
};




// ************************************************************************ //
// XML       : PostRequestResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class PostRequestResponse : public TRemotable {
private:
  UnicodeString   FLocation;
  bool            FLocation_Specified;
  UnicodeString   FRequestId;
  bool            FRequestId_Specified;
  void __fastcall SetLocation(int Index, UnicodeString _prop_val)
  {  FLocation = _prop_val; FLocation_Specified = true;  }
  bool __fastcall Location_Specified(int Index)
  {  return FLocation_Specified;  } 
  void __fastcall SetRequestId(int Index, UnicodeString _prop_val)
  {  FRequestId = _prop_val; FRequestId_Specified = true;  }
  bool __fastcall RequestId_Specified(int Index)
  {  return FRequestId_Specified;  } 
__published:
  __property UnicodeString   Location = { index=(IS_OPTN|IS_NLBL), read=FLocation, write=SetLocation, stored = Location_Specified };
  __property UnicodeString  RequestId = { index=(IS_OPTN|IS_NLBL), read=FRequestId, write=SetRequestId, stored = RequestId_Specified };
};




// ************************************************************************ //
// XML       : Reports, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class Reports : public TRemotable {
private:
  UnicodeString   FreportType;
  bool            FreportType_Specified;
  void __fastcall SetreportType(int Index, UnicodeString _prop_val)
  {  FreportType = _prop_val; FreportType_Specified = true;  }
  bool __fastcall reportType_Specified(int Index)
  {  return FreportType_Specified;  } 
__published:
  __property UnicodeString reportType = { index=(IS_OPTN|IS_NLBL), read=FreportType, write=SetreportType, stored = reportType_Specified };
};




// ************************************************************************ //
// XML       : TransactionDataResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class TransactionDataResponse : public TRemotable {
private:
  UnicodeString   FAmountBase;
  bool            FAmountBase_Specified;
  UnicodeString   FAmountCashBack;
  bool            FAmountCashBack_Specified;
  UnicodeString   FAmountGratuity;
  bool            FAmountGratuity_Specified;
  UnicodeString   FAmountTotal;
  bool            FAmountTotal_Specified;
  UnicodeString   FApplicationId;
  bool            FApplicationId_Specified;
  UnicodeString   FApplicationLabel;
  bool            FApplicationLabel_Specified;
  UnicodeString   FAuthCode;
  bool            FAuthCode_Specified;
  UnicodeString   FCardSchemeName;
  bool            FCardSchemeName_Specified;
  UnicodeString   FCardholderVerificationMethod;
  bool            FCardholderVerificationMethod_Specified;
  UnicodeString   FCurrency;
  bool            FCurrency_Specified;
  UnicodeString   FDateOfExpiry;
  bool            FDateOfExpiry_Specified;
  UnicodeString   FDateOfStart;
  bool            FDateOfStart_Specified;
  UnicodeString   FLocation;
  bool            FLocation_Specified;
  ArrayOfstring   FNotifications;
  bool            FNotifications_Specified;
  UnicodeString   FPaymentMethod;
  bool            FPaymentMethod_Specified;
  UnicodeString   FPrimaryAccountNumber;
  bool            FPrimaryAccountNumber_Specified;
  UnicodeString   FPrimaryAccountNumberSequence;
  bool            FPrimaryAccountNumberSequence_Specified;
  ReceiptLines*   FReceiptLines;
  bool            FReceiptLines_Specified;
  UnicodeString   FRequestId;
  bool            FRequestId_Specified;
  UnicodeString   FTransactionId;
  bool            FTransactionId_Specified;
  UnicodeString   FTransactionNumber;
  bool            FTransactionNumber_Specified;
  UnicodeString   FTransactionResult;
  bool            FTransactionResult_Specified;
  UnicodeString   FTransactionTime;
  bool            FTransactionTime_Specified;
  UnicodeString   FTransactionType;
  bool            FTransactionType_Specified;
  void __fastcall SetAmountBase(int Index, UnicodeString _prop_val)
  {  FAmountBase = _prop_val; FAmountBase_Specified = true;  }
  bool __fastcall AmountBase_Specified(int Index)
  {  return FAmountBase_Specified;  } 
  void __fastcall SetAmountCashBack(int Index, UnicodeString _prop_val)
  {  FAmountCashBack = _prop_val; FAmountCashBack_Specified = true;  }
  bool __fastcall AmountCashBack_Specified(int Index)
  {  return FAmountCashBack_Specified;  } 
  void __fastcall SetAmountGratuity(int Index, UnicodeString _prop_val)
  {  FAmountGratuity = _prop_val; FAmountGratuity_Specified = true;  }
  bool __fastcall AmountGratuity_Specified(int Index)
  {  return FAmountGratuity_Specified;  } 
  void __fastcall SetAmountTotal(int Index, UnicodeString _prop_val)
  {  FAmountTotal = _prop_val; FAmountTotal_Specified = true;  }
  bool __fastcall AmountTotal_Specified(int Index)
  {  return FAmountTotal_Specified;  } 
  void __fastcall SetApplicationId(int Index, UnicodeString _prop_val)
  {  FApplicationId = _prop_val; FApplicationId_Specified = true;  }
  bool __fastcall ApplicationId_Specified(int Index)
  {  return FApplicationId_Specified;  } 
  void __fastcall SetApplicationLabel(int Index, UnicodeString _prop_val)
  {  FApplicationLabel = _prop_val; FApplicationLabel_Specified = true;  }
  bool __fastcall ApplicationLabel_Specified(int Index)
  {  return FApplicationLabel_Specified;  } 
  void __fastcall SetAuthCode(int Index, UnicodeString _prop_val)
  {  FAuthCode = _prop_val; FAuthCode_Specified = true;  }
  bool __fastcall AuthCode_Specified(int Index)
  {  return FAuthCode_Specified;  } 
  void __fastcall SetCardSchemeName(int Index, UnicodeString _prop_val)
  {  FCardSchemeName = _prop_val; FCardSchemeName_Specified = true;  }
  bool __fastcall CardSchemeName_Specified(int Index)
  {  return FCardSchemeName_Specified;  } 
  void __fastcall SetCardholderVerificationMethod(int Index, UnicodeString _prop_val)
  {  FCardholderVerificationMethod = _prop_val; FCardholderVerificationMethod_Specified = true;  }
  bool __fastcall CardholderVerificationMethod_Specified(int Index)
  {  return FCardholderVerificationMethod_Specified;  } 
  void __fastcall SetCurrency(int Index, UnicodeString _prop_val)
  {  FCurrency = _prop_val; FCurrency_Specified = true;  }
  bool __fastcall Currency_Specified(int Index)
  {  return FCurrency_Specified;  } 
  void __fastcall SetDateOfExpiry(int Index, UnicodeString _prop_val)
  {  FDateOfExpiry = _prop_val; FDateOfExpiry_Specified = true;  }
  bool __fastcall DateOfExpiry_Specified(int Index)
  {  return FDateOfExpiry_Specified;  } 
  void __fastcall SetDateOfStart(int Index, UnicodeString _prop_val)
  {  FDateOfStart = _prop_val; FDateOfStart_Specified = true;  }
  bool __fastcall DateOfStart_Specified(int Index)
  {  return FDateOfStart_Specified;  } 
  void __fastcall SetLocation(int Index, UnicodeString _prop_val)
  {  FLocation = _prop_val; FLocation_Specified = true;  }
  bool __fastcall Location_Specified(int Index)
  {  return FLocation_Specified;  } 
  void __fastcall SetNotifications(int Index, ArrayOfstring _prop_val)
  {  FNotifications = _prop_val; FNotifications_Specified = true;  }
  bool __fastcall Notifications_Specified(int Index)
  {  return FNotifications_Specified;  } 
  void __fastcall SetPaymentMethod(int Index, UnicodeString _prop_val)
  {  FPaymentMethod = _prop_val; FPaymentMethod_Specified = true;  }
  bool __fastcall PaymentMethod_Specified(int Index)
  {  return FPaymentMethod_Specified;  } 
  void __fastcall SetPrimaryAccountNumber(int Index, UnicodeString _prop_val)
  {  FPrimaryAccountNumber = _prop_val; FPrimaryAccountNumber_Specified = true;  }
  bool __fastcall PrimaryAccountNumber_Specified(int Index)
  {  return FPrimaryAccountNumber_Specified;  } 
  void __fastcall SetPrimaryAccountNumberSequence(int Index, UnicodeString _prop_val)
  {  FPrimaryAccountNumberSequence = _prop_val; FPrimaryAccountNumberSequence_Specified = true;  }
  bool __fastcall PrimaryAccountNumberSequence_Specified(int Index)
  {  return FPrimaryAccountNumberSequence_Specified;  } 
  void __fastcall SetReceiptLines(int Index, ReceiptLines* _prop_val)
  {  FReceiptLines = _prop_val; FReceiptLines_Specified = true;  }
  bool __fastcall ReceiptLines_Specified(int Index)
  {  return FReceiptLines_Specified;  } 
  void __fastcall SetRequestId(int Index, UnicodeString _prop_val)
  {  FRequestId = _prop_val; FRequestId_Specified = true;  }
  bool __fastcall RequestId_Specified(int Index)
  {  return FRequestId_Specified;  } 
  void __fastcall SetTransactionId(int Index, UnicodeString _prop_val)
  {  FTransactionId = _prop_val; FTransactionId_Specified = true;  }
  bool __fastcall TransactionId_Specified(int Index)
  {  return FTransactionId_Specified;  } 
  void __fastcall SetTransactionNumber(int Index, UnicodeString _prop_val)
  {  FTransactionNumber = _prop_val; FTransactionNumber_Specified = true;  }
  bool __fastcall TransactionNumber_Specified(int Index)
  {  return FTransactionNumber_Specified;  } 
  void __fastcall SetTransactionResult(int Index, UnicodeString _prop_val)
  {  FTransactionResult = _prop_val; FTransactionResult_Specified = true;  }
  bool __fastcall TransactionResult_Specified(int Index)
  {  return FTransactionResult_Specified;  } 
  void __fastcall SetTransactionTime(int Index, UnicodeString _prop_val)
  {  FTransactionTime = _prop_val; FTransactionTime_Specified = true;  }
  bool __fastcall TransactionTime_Specified(int Index)
  {  return FTransactionTime_Specified;  } 
  void __fastcall SetTransactionType(int Index, UnicodeString _prop_val)
  {  FTransactionType = _prop_val; FTransactionType_Specified = true;  }
  bool __fastcall TransactionType_Specified(int Index)
  {  return FTransactionType_Specified;  } 

public:
  __fastcall ~TransactionDataResponse();
__published:
  __property UnicodeString AmountBase = { index=(IS_OPTN|IS_NLBL), read=FAmountBase, write=SetAmountBase, stored = AmountBase_Specified };
  __property UnicodeString AmountCashBack = { index=(IS_OPTN|IS_NLBL), read=FAmountCashBack, write=SetAmountCashBack, stored = AmountCashBack_Specified };
  __property UnicodeString AmountGratuity = { index=(IS_OPTN|IS_NLBL), read=FAmountGratuity, write=SetAmountGratuity, stored = AmountGratuity_Specified };
  __property UnicodeString AmountTotal = { index=(IS_OPTN|IS_NLBL), read=FAmountTotal, write=SetAmountTotal, stored = AmountTotal_Specified };
  __property UnicodeString ApplicationId = { index=(IS_OPTN|IS_NLBL), read=FApplicationId, write=SetApplicationId, stored = ApplicationId_Specified };
  __property UnicodeString ApplicationLabel = { index=(IS_OPTN|IS_NLBL), read=FApplicationLabel, write=SetApplicationLabel, stored = ApplicationLabel_Specified };
  __property UnicodeString   AuthCode = { index=(IS_OPTN|IS_NLBL), read=FAuthCode, write=SetAuthCode, stored = AuthCode_Specified };
  __property UnicodeString CardSchemeName = { index=(IS_OPTN|IS_NLBL), read=FCardSchemeName, write=SetCardSchemeName, stored = CardSchemeName_Specified };
  __property UnicodeString CardholderVerificationMethod = { index=(IS_OPTN|IS_NLBL), read=FCardholderVerificationMethod, write=SetCardholderVerificationMethod, stored = CardholderVerificationMethod_Specified };
  __property UnicodeString   Currency = { index=(IS_OPTN|IS_NLBL), read=FCurrency, write=SetCurrency, stored = Currency_Specified };
  __property UnicodeString DateOfExpiry = { index=(IS_OPTN|IS_NLBL), read=FDateOfExpiry, write=SetDateOfExpiry, stored = DateOfExpiry_Specified };
  __property UnicodeString DateOfStart = { index=(IS_OPTN|IS_NLBL), read=FDateOfStart, write=SetDateOfStart, stored = DateOfStart_Specified };
  __property UnicodeString   Location = { index=(IS_OPTN|IS_NLBL), read=FLocation, write=SetLocation, stored = Location_Specified };
  __property ArrayOfstring Notifications = { index=(IS_OPTN|IS_NLBL), read=FNotifications, write=SetNotifications, stored = Notifications_Specified };
  __property UnicodeString PaymentMethod = { index=(IS_OPTN|IS_NLBL), read=FPaymentMethod, write=SetPaymentMethod, stored = PaymentMethod_Specified };
  __property UnicodeString PrimaryAccountNumber = { index=(IS_OPTN|IS_NLBL), read=FPrimaryAccountNumber, write=SetPrimaryAccountNumber, stored = PrimaryAccountNumber_Specified };
  __property UnicodeString PrimaryAccountNumberSequence = { index=(IS_OPTN|IS_NLBL), read=FPrimaryAccountNumberSequence, write=SetPrimaryAccountNumberSequence, stored = PrimaryAccountNumberSequence_Specified };
  __property ReceiptLines* ReceiptLines = { index=(IS_OPTN|IS_NLBL), read=FReceiptLines, write=SetReceiptLines, stored = ReceiptLines_Specified };
  __property UnicodeString  RequestId = { index=(IS_OPTN|IS_NLBL), read=FRequestId, write=SetRequestId, stored = RequestId_Specified };
  __property UnicodeString TransactionId = { index=(IS_OPTN|IS_NLBL), read=FTransactionId, write=SetTransactionId, stored = TransactionId_Specified };
  __property UnicodeString TransactionNumber = { index=(IS_OPTN|IS_NLBL), read=FTransactionNumber, write=SetTransactionNumber, stored = TransactionNumber_Specified };
  __property UnicodeString TransactionResult = { index=(IS_OPTN|IS_NLBL), read=FTransactionResult, write=SetTransactionResult, stored = TransactionResult_Specified };
  __property UnicodeString TransactionTime = { index=(IS_OPTN|IS_NLBL), read=FTransactionTime, write=SetTransactionTime, stored = TransactionTime_Specified };
  __property UnicodeString TransactionType = { index=(IS_OPTN|IS_NLBL), read=FTransactionType, write=SetTransactionType, stored = TransactionType_Specified };
};


typedef DynamicArray<ReceiptData*> ArrayOfReceiptData; /* "http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : ReceiptLines, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class ReceiptLines : public TRemotable {
private:
  ArrayOfReceiptData FCustomer;
  bool            FCustomer_Specified;
  ArrayOfstring   FCustomerReceipt;
  bool            FCustomerReceipt_Specified;
  ArrayOfReceiptData FMerchant;
  bool            FMerchant_Specified;
  ArrayOfstring   FMerchantReceipt;
  bool            FMerchantReceipt_Specified;
  void __fastcall SetCustomer(int Index, ArrayOfReceiptData _prop_val)
  {  FCustomer = _prop_val; FCustomer_Specified = true;  }
  bool __fastcall Customer_Specified(int Index)
  {  return FCustomer_Specified;  } 
  void __fastcall SetCustomerReceipt(int Index, ArrayOfstring _prop_val)
  {  FCustomerReceipt = _prop_val; FCustomerReceipt_Specified = true;  }
  bool __fastcall CustomerReceipt_Specified(int Index)
  {  return FCustomerReceipt_Specified;  } 
  void __fastcall SetMerchant(int Index, ArrayOfReceiptData _prop_val)
  {  FMerchant = _prop_val; FMerchant_Specified = true;  }
  bool __fastcall Merchant_Specified(int Index)
  {  return FMerchant_Specified;  } 
  void __fastcall SetMerchantReceipt(int Index, ArrayOfstring _prop_val)
  {  FMerchantReceipt = _prop_val; FMerchantReceipt_Specified = true;  }
  bool __fastcall MerchantReceipt_Specified(int Index)
  {  return FMerchantReceipt_Specified;  } 

public:
  __fastcall ~ReceiptLines();
__published:
  __property ArrayOfReceiptData   Customer = { index=(IS_OPTN|IS_NLBL), read=FCustomer, write=SetCustomer, stored = Customer_Specified };
  __property ArrayOfstring CustomerReceipt = { index=(IS_OPTN|IS_NLBL), read=FCustomerReceipt, write=SetCustomerReceipt, stored = CustomerReceipt_Specified };
  __property ArrayOfReceiptData   Merchant = { index=(IS_OPTN|IS_NLBL), read=FMerchant, write=SetMerchant, stored = Merchant_Specified };
  __property ArrayOfstring MerchantReceipt = { index=(IS_OPTN|IS_NLBL), read=FMerchantReceipt, write=SetMerchantReceipt, stored = MerchantReceipt_Specified };
};




// ************************************************************************ //
// XML       : ReceiptData, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class ReceiptData : public TRemotable {
private:
  ArrayOfstring   FFormat;
  bool            FFormat_Specified;
  UnicodeString   FType;
  bool            FType_Specified;
  UnicodeString   FValue;
  bool            FValue_Specified;
  void __fastcall SetFormat(int Index, ArrayOfstring _prop_val)
  {  FFormat = _prop_val; FFormat_Specified = true;  }
  bool __fastcall Format_Specified(int Index)
  {  return FFormat_Specified;  } 
  void __fastcall SetType(int Index, UnicodeString _prop_val)
  {  FType = _prop_val; FType_Specified = true;  }
  bool __fastcall Type_Specified(int Index)
  {  return FType_Specified;  } 
  void __fastcall SetValue(int Index, UnicodeString _prop_val)
  {  FValue = _prop_val; FValue_Specified = true;  }
  bool __fastcall Value_Specified(int Index)
  {  return FValue_Specified;  } 
__published:
  __property ArrayOfstring     Format = { index=(IS_OPTN|IS_NLBL), read=FFormat, write=SetFormat, stored = Format_Specified };
  __property UnicodeString       Type = { index=(IS_OPTN|IS_NLBL), read=FType, write=SetType, stored = Type_Specified };
  __property UnicodeString      Value = { index=(IS_OPTN|IS_NLBL), read=FValue, write=SetValue, stored = Value_Specified };
};




// ************************************************************************ //
// XML       : AuthorizationDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class AuthorizationDetails2 : public AuthorizationDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : PACTerminalWrapper, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class PACTerminalWrapper2 : public PACTerminalWrapper {
private:
__published:
};




// ************************************************************************ //
// XML       : PACTerminal, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class PACTerminal2 : public PACTerminal {
private:
__published:
};




// ************************************************************************ //
// XML       : TransactionRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class TransactionRequest2 : public TransactionRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : PostRequestResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class PostRequestResponse2 : public PostRequestResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : Reports, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class Reports2 : public Reports {
private:
__published:
};




// ************************************************************************ //
// XML       : TransactionDataResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class TransactionDataResponse2 : public TransactionDataResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : ReceiptLines, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class ReceiptLines2 : public ReceiptLines {
private:
__published:
};




// ************************************************************************ //
// XML       : ReceiptData, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain
// ************************************************************************ //
class ReceiptData2 : public ReceiptData {
private:
__published:
};




// ************************************************************************ //
// XML       : SignatureRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain.SignatureRequest
// ************************************************************************ //
class SignatureRequest : public TRemotable {
private:
  UnicodeString   Faccepted;
  bool            Faccepted_Specified;
  void __fastcall Setaccepted(int Index, UnicodeString _prop_val)
  {  Faccepted = _prop_val; Faccepted_Specified = true;  }
  bool __fastcall accepted_Specified(int Index)
  {  return Faccepted_Specified;  } 
__published:
  __property UnicodeString   accepted = { index=(IS_OPTN|IS_NLBL), read=Faccepted, write=Setaccepted, stored = accepted_Specified };
};




// ************************************************************************ //
// XML       : SignatureRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain.SignatureRequest
// ************************************************************************ //
class SignatureRequest2 : public SignatureRequest {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IWCFServicePaymentSense/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServicePaymentSense
// service   : WCFServicePaymentSense
// port      : basicHttpBinding_ServicePaymentSense
// URL       : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/
// ************************************************************************ //
__interface INTERFACE_UUID("{04119E0C-FE8A-C15E-CA06-2E077AB0B3F4}") IWCFServicePaymentSense : public IInvokable
{
public:
  virtual PACTerminalWrapper* GetAllCardTerminals(const AuthorizationDetails* autorizationDetails) = 0; 
  virtual PACTerminal*    PingTerminal(const AuthorizationDetails* autorizationDetails) = 0; 
  virtual PostRequestResponse* DoTransaction(const AuthorizationDetails* autorizationDetails, const TransactionRequest* request) = 0; 
  virtual bool            PrintReports(const AuthorizationDetails* autorizationDetails, const Reports* reportData) = 0; 
  virtual TransactionDataResponse* GetResponseForRequestedId(const AuthorizationDetails* autorizationDetails) = 0; 
  virtual TransactionDataResponse* SignatureVerificationForRequestedId(const AuthorizationDetails* autorizationDetails, const SignatureRequest* signRequest) = 0; 
};
typedef DelphiInterface<IWCFServicePaymentSense> _di_IWCFServicePaymentSense;

_di_IWCFServicePaymentSense GetIWCFServicePaymentSense(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__PaymentSenseWSDL

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__PaymentSenseWSDL;
#endif



#endif // _H
