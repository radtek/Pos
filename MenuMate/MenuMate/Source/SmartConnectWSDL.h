// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8744/MenumateServices/SmartConnect/?wsdl
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?wsdl>0
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?xsd=xsd0
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?xsd=xsd2
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?xsd=xsd3
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (12/10/2017 9:42:58 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   SmartConnectWSDLH
#define   SmartConnectWSDLH

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
#if !defined(IS_NLBL)
#define IS_NLBL 0x0004
#endif
#if !defined(IS_REF)
#define IS_REF 0x0080
#endif


namespace NS__SmartConnnectWSDL {

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
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:short           - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS DateTimeOffset;
class SOAP_REMOTABLE_CLASS DateTimeOffset2;
class SOAP_REMOTABLE_CLASS POSDetails;
class SOAP_REMOTABLE_CLASS PairingTerminal;
class SOAP_REMOTABLE_CLASS SmartConnectResponse;
class SOAP_REMOTABLE_CLASS SmartConnectDataObject;
class SOAP_REMOTABLE_CLASS TransactionTypes;
class SOAP_REMOTABLE_CLASS PairingTerminal2;
class SOAP_REMOTABLE_CLASS POSDetails2;
class SOAP_REMOTABLE_CLASS SmartConnectResponse2;
class SOAP_REMOTABLE_CLASS SmartConnectDataObject2;
class SOAP_REMOTABLE_CLASS TransactionTypes2;



// ************************************************************************ //
// XML       : DateTimeOffset, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/System
// ************************************************************************ //
class DateTimeOffset : public TRemotable {
private:
  TXSDateTime*    FDateTime;
  short           FOffsetMinutes;

public:
  __fastcall ~DateTimeOffset();
__published:
  __property TXSDateTime*   DateTime = { read=FDateTime, write=FDateTime };
  __property short      OffsetMinutes = { read=FOffsetMinutes, write=FOffsetMinutes };
};




// ************************************************************************ //
// XML       : DateTimeOffset, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/System
// ************************************************************************ //
class DateTimeOffset2 : public DateTimeOffset {
private:
__published:
};




// ************************************************************************ //
// XML       : POSDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class POSDetails : public TRemotable {
private:
  UnicodeString   FPosBusinessName;
  bool            FPosBusinessName_Specified;
  UnicodeString   FPosRegisterId;
  bool            FPosRegisterId_Specified;
  UnicodeString   FPosVendorName;
  bool            FPosVendorName_Specified;
  void __fastcall SetPosBusinessName(int Index, UnicodeString _prop_val)
  {  FPosBusinessName = _prop_val; FPosBusinessName_Specified = true;  }
  bool __fastcall PosBusinessName_Specified(int Index)
  {  return FPosBusinessName_Specified;  } 
  void __fastcall SetPosRegisterId(int Index, UnicodeString _prop_val)
  {  FPosRegisterId = _prop_val; FPosRegisterId_Specified = true;  }
  bool __fastcall PosRegisterId_Specified(int Index)
  {  return FPosRegisterId_Specified;  } 
  void __fastcall SetPosVendorName(int Index, UnicodeString _prop_val)
  {  FPosVendorName = _prop_val; FPosVendorName_Specified = true;  }
  bool __fastcall PosVendorName_Specified(int Index)
  {  return FPosVendorName_Specified;  } 
__published:
  __property UnicodeString PosBusinessName = { index=(IS_OPTN|IS_NLBL), read=FPosBusinessName, write=SetPosBusinessName, stored = PosBusinessName_Specified };
  __property UnicodeString PosRegisterId = { index=(IS_OPTN|IS_NLBL), read=FPosRegisterId, write=SetPosRegisterId, stored = PosRegisterId_Specified };
  __property UnicodeString PosVendorName = { index=(IS_OPTN|IS_NLBL), read=FPosVendorName, write=SetPosVendorName, stored = PosVendorName_Specified };
};




// ************************************************************************ //
// XML       : PairingTerminal, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class PairingTerminal : public POSDetails {
private:
  UnicodeString   FPairingCode;
  bool            FPairingCode_Specified;
  UnicodeString   FPosRegisterName;
  bool            FPosRegisterName_Specified;
  void __fastcall SetPairingCode(int Index, UnicodeString _prop_val)
  {  FPairingCode = _prop_val; FPairingCode_Specified = true;  }
  bool __fastcall PairingCode_Specified(int Index)
  {  return FPairingCode_Specified;  } 
  void __fastcall SetPosRegisterName(int Index, UnicodeString _prop_val)
  {  FPosRegisterName = _prop_val; FPosRegisterName_Specified = true;  }
  bool __fastcall PosRegisterName_Specified(int Index)
  {  return FPosRegisterName_Specified;  } 
__published:
  __property UnicodeString PairingCode = { index=(IS_OPTN|IS_NLBL), read=FPairingCode, write=SetPairingCode, stored = PairingCode_Specified };
  __property UnicodeString PosRegisterName = { index=(IS_OPTN|IS_NLBL), read=FPosRegisterName, write=SetPosRegisterName, stored = PosRegisterName_Specified };
};




// ************************************************************************ //
// XML       : SmartConnectResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class SmartConnectResponse : public TRemotable {
private:
  UnicodeString   FDeviceId;
  bool            FDeviceId_Specified;
  UnicodeString   FMerchantId;
  bool            FMerchantId_Specified;
  UnicodeString   FResponseMessage;
  bool            FResponseMessage_Specified;
  bool            FResponseSuccessful;
  bool            FResponseSuccessful_Specified;
  SmartConnectDataObject* FSmartConnectData;
  bool            FSmartConnectData_Specified;
  UnicodeString   FTransactionId;
  bool            FTransactionId_Specified;
  UnicodeString   FTransactionStatus;
  bool            FTransactionStatus_Specified;
  DateTimeOffset* FTransactionTime;
  bool            FTransactionTime_Specified;
  void __fastcall SetDeviceId(int Index, UnicodeString _prop_val)
  {  FDeviceId = _prop_val; FDeviceId_Specified = true;  }
  bool __fastcall DeviceId_Specified(int Index)
  {  return FDeviceId_Specified;  } 
  void __fastcall SetMerchantId(int Index, UnicodeString _prop_val)
  {  FMerchantId = _prop_val; FMerchantId_Specified = true;  }
  bool __fastcall MerchantId_Specified(int Index)
  {  return FMerchantId_Specified;  } 
  void __fastcall SetResponseMessage(int Index, UnicodeString _prop_val)
  {  FResponseMessage = _prop_val; FResponseMessage_Specified = true;  }
  bool __fastcall ResponseMessage_Specified(int Index)
  {  return FResponseMessage_Specified;  } 
  void __fastcall SetResponseSuccessful(int Index, bool _prop_val)
  {  FResponseSuccessful = _prop_val; FResponseSuccessful_Specified = true;  }
  bool __fastcall ResponseSuccessful_Specified(int Index)
  {  return FResponseSuccessful_Specified;  } 
  void __fastcall SetSmartConnectData(int Index, SmartConnectDataObject* _prop_val)
  {  FSmartConnectData = _prop_val; FSmartConnectData_Specified = true;  }
  bool __fastcall SmartConnectData_Specified(int Index)
  {  return FSmartConnectData_Specified;  } 
  void __fastcall SetTransactionId(int Index, UnicodeString _prop_val)
  {  FTransactionId = _prop_val; FTransactionId_Specified = true;  }
  bool __fastcall TransactionId_Specified(int Index)
  {  return FTransactionId_Specified;  } 
  void __fastcall SetTransactionStatus(int Index, UnicodeString _prop_val)
  {  FTransactionStatus = _prop_val; FTransactionStatus_Specified = true;  }
  bool __fastcall TransactionStatus_Specified(int Index)
  {  return FTransactionStatus_Specified;  } 
  void __fastcall SetTransactionTime(int Index, DateTimeOffset* _prop_val)
  {  FTransactionTime = _prop_val; FTransactionTime_Specified = true;  }
  bool __fastcall TransactionTime_Specified(int Index)
  {  return FTransactionTime_Specified;  } 

public:
  __fastcall ~SmartConnectResponse();
__published:
  __property UnicodeString   DeviceId = { index=(IS_OPTN|IS_NLBL), read=FDeviceId, write=SetDeviceId, stored = DeviceId_Specified };
  __property UnicodeString MerchantId = { index=(IS_OPTN|IS_NLBL), read=FMerchantId, write=SetMerchantId, stored = MerchantId_Specified };
  __property UnicodeString ResponseMessage = { index=(IS_OPTN|IS_NLBL), read=FResponseMessage, write=SetResponseMessage, stored = ResponseMessage_Specified };
  __property bool       ResponseSuccessful = { index=(IS_OPTN), read=FResponseSuccessful, write=SetResponseSuccessful, stored = ResponseSuccessful_Specified };
  __property SmartConnectDataObject* SmartConnectData = { index=(IS_OPTN|IS_NLBL), read=FSmartConnectData, write=SetSmartConnectData, stored = SmartConnectData_Specified };
  __property UnicodeString TransactionId = { index=(IS_OPTN|IS_NLBL), read=FTransactionId, write=SetTransactionId, stored = TransactionId_Specified };
  __property UnicodeString TransactionStatus = { index=(IS_OPTN|IS_NLBL), read=FTransactionStatus, write=SetTransactionStatus, stored = TransactionStatus_Specified };
  __property DateTimeOffset* TransactionTime = { index=(IS_OPTN), read=FTransactionTime, write=SetTransactionTime, stored = TransactionTime_Specified };
};




// ************************************************************************ //
// XML       : SmartConnectDataObject, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class SmartConnectDataObject : public TRemotable {
private:
  UnicodeString   FAccountType;
  bool            FAccountType_Specified;
  UnicodeString   FAcquirerRef;
  bool            FAcquirerRef_Specified;
  UnicodeString   FAmountSurcharge;
  bool            FAmountSurcharge_Specified;
  UnicodeString   FAmountTip;
  bool            FAmountTip_Specified;
  UnicodeString   FAmountTotal;
  bool            FAmountTotal_Specified;
  UnicodeString   FAuthId;
  bool            FAuthId_Specified;
  UnicodeString   FCardPan;
  bool            FCardPan_Specified;
  UnicodeString   FCardType;
  bool            FCardType_Specified;
  UnicodeString   FFunction;
  bool            FFunction_Specified;
  UnicodeString   FMerchant;
  bool            FMerchant_Specified;
  UnicodeString   FReceipt;
  bool            FReceipt_Specified;
  UnicodeString   FRequestId;
  bool            FRequestId_Specified;
  UnicodeString   FResult;
  bool            FResult_Specified;
  UnicodeString   FTerminalRef;
  bool            FTerminalRef_Specified;
  TXSDateTime*    FTimestamp;
  bool            FTimestamp_Specified;
  UnicodeString   FTransactionResult;
  bool            FTransactionResult_Specified;
  void __fastcall SetAccountType(int Index, UnicodeString _prop_val)
  {  FAccountType = _prop_val; FAccountType_Specified = true;  }
  bool __fastcall AccountType_Specified(int Index)
  {  return FAccountType_Specified;  } 
  void __fastcall SetAcquirerRef(int Index, UnicodeString _prop_val)
  {  FAcquirerRef = _prop_val; FAcquirerRef_Specified = true;  }
  bool __fastcall AcquirerRef_Specified(int Index)
  {  return FAcquirerRef_Specified;  } 
  void __fastcall SetAmountSurcharge(int Index, UnicodeString _prop_val)
  {  FAmountSurcharge = _prop_val; FAmountSurcharge_Specified = true;  }
  bool __fastcall AmountSurcharge_Specified(int Index)
  {  return FAmountSurcharge_Specified;  } 
  void __fastcall SetAmountTip(int Index, UnicodeString _prop_val)
  {  FAmountTip = _prop_val; FAmountTip_Specified = true;  }
  bool __fastcall AmountTip_Specified(int Index)
  {  return FAmountTip_Specified;  } 
  void __fastcall SetAmountTotal(int Index, UnicodeString _prop_val)
  {  FAmountTotal = _prop_val; FAmountTotal_Specified = true;  }
  bool __fastcall AmountTotal_Specified(int Index)
  {  return FAmountTotal_Specified;  } 
  void __fastcall SetAuthId(int Index, UnicodeString _prop_val)
  {  FAuthId = _prop_val; FAuthId_Specified = true;  }
  bool __fastcall AuthId_Specified(int Index)
  {  return FAuthId_Specified;  } 
  void __fastcall SetCardPan(int Index, UnicodeString _prop_val)
  {  FCardPan = _prop_val; FCardPan_Specified = true;  }
  bool __fastcall CardPan_Specified(int Index)
  {  return FCardPan_Specified;  } 
  void __fastcall SetCardType(int Index, UnicodeString _prop_val)
  {  FCardType = _prop_val; FCardType_Specified = true;  }
  bool __fastcall CardType_Specified(int Index)
  {  return FCardType_Specified;  } 
  void __fastcall SetFunction(int Index, UnicodeString _prop_val)
  {  FFunction = _prop_val; FFunction_Specified = true;  }
  bool __fastcall Function_Specified(int Index)
  {  return FFunction_Specified;  } 
  void __fastcall SetMerchant(int Index, UnicodeString _prop_val)
  {  FMerchant = _prop_val; FMerchant_Specified = true;  }
  bool __fastcall Merchant_Specified(int Index)
  {  return FMerchant_Specified;  } 
  void __fastcall SetReceipt(int Index, UnicodeString _prop_val)
  {  FReceipt = _prop_val; FReceipt_Specified = true;  }
  bool __fastcall Receipt_Specified(int Index)
  {  return FReceipt_Specified;  } 
  void __fastcall SetRequestId(int Index, UnicodeString _prop_val)
  {  FRequestId = _prop_val; FRequestId_Specified = true;  }
  bool __fastcall RequestId_Specified(int Index)
  {  return FRequestId_Specified;  } 
  void __fastcall SetResult(int Index, UnicodeString _prop_val)
  {  FResult = _prop_val; FResult_Specified = true;  }
  bool __fastcall Result_Specified(int Index)
  {  return FResult_Specified;  } 
  void __fastcall SetTerminalRef(int Index, UnicodeString _prop_val)
  {  FTerminalRef = _prop_val; FTerminalRef_Specified = true;  }
  bool __fastcall TerminalRef_Specified(int Index)
  {  return FTerminalRef_Specified;  } 
  void __fastcall SetTimestamp(int Index, TXSDateTime* _prop_val)
  {  FTimestamp = _prop_val; FTimestamp_Specified = true;  }
  bool __fastcall Timestamp_Specified(int Index)
  {  return FTimestamp_Specified;  } 
  void __fastcall SetTransactionResult(int Index, UnicodeString _prop_val)
  {  FTransactionResult = _prop_val; FTransactionResult_Specified = true;  }
  bool __fastcall TransactionResult_Specified(int Index)
  {  return FTransactionResult_Specified;  } 

public:
  __fastcall ~SmartConnectDataObject();
__published:
  __property UnicodeString AccountType = { index=(IS_OPTN|IS_NLBL), read=FAccountType, write=SetAccountType, stored = AccountType_Specified };
  __property UnicodeString AcquirerRef = { index=(IS_OPTN|IS_NLBL), read=FAcquirerRef, write=SetAcquirerRef, stored = AcquirerRef_Specified };
  __property UnicodeString AmountSurcharge = { index=(IS_OPTN|IS_NLBL), read=FAmountSurcharge, write=SetAmountSurcharge, stored = AmountSurcharge_Specified };
  __property UnicodeString  AmountTip = { index=(IS_OPTN|IS_NLBL), read=FAmountTip, write=SetAmountTip, stored = AmountTip_Specified };
  __property UnicodeString AmountTotal = { index=(IS_OPTN|IS_NLBL), read=FAmountTotal, write=SetAmountTotal, stored = AmountTotal_Specified };
  __property UnicodeString     AuthId = { index=(IS_OPTN|IS_NLBL), read=FAuthId, write=SetAuthId, stored = AuthId_Specified };
  __property UnicodeString    CardPan = { index=(IS_OPTN|IS_NLBL), read=FCardPan, write=SetCardPan, stored = CardPan_Specified };
  __property UnicodeString   CardType = { index=(IS_OPTN|IS_NLBL), read=FCardType, write=SetCardType, stored = CardType_Specified };
  __property UnicodeString   Function = { index=(IS_OPTN|IS_NLBL), read=FFunction, write=SetFunction, stored = Function_Specified };
  __property UnicodeString   Merchant = { index=(IS_OPTN|IS_NLBL), read=FMerchant, write=SetMerchant, stored = Merchant_Specified };
  __property UnicodeString    Receipt = { index=(IS_OPTN|IS_NLBL), read=FReceipt, write=SetReceipt, stored = Receipt_Specified };
  __property UnicodeString  RequestId = { index=(IS_OPTN|IS_NLBL), read=FRequestId, write=SetRequestId, stored = RequestId_Specified };
  __property UnicodeString     Result = { index=(IS_OPTN|IS_NLBL), read=FResult, write=SetResult, stored = Result_Specified };
  __property UnicodeString TerminalRef = { index=(IS_OPTN|IS_NLBL), read=FTerminalRef, write=SetTerminalRef, stored = TerminalRef_Specified };
  __property TXSDateTime*  Timestamp = { index=(IS_OPTN), read=FTimestamp, write=SetTimestamp, stored = Timestamp_Specified };
  __property UnicodeString TransactionResult = { index=(IS_OPTN|IS_NLBL), read=FTransactionResult, write=SetTransactionResult, stored = TransactionResult_Specified };
};




// ************************************************************************ //
// XML       : TransactionTypes, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class TransactionTypes : public POSDetails {
private:
  UnicodeString   FTransactiontype;
  bool            FTransactiontype_Specified;
  void __fastcall SetTransactiontype(int Index, UnicodeString _prop_val)
  {  FTransactiontype = _prop_val; FTransactiontype_Specified = true;  }
  bool __fastcall Transactiontype_Specified(int Index)
  {  return FTransactiontype_Specified;  } 
__published:
  __property UnicodeString Transactiontype = { index=(IS_OPTN|IS_NLBL), read=FTransactiontype, write=SetTransactiontype, stored = Transactiontype_Specified };
};




// ************************************************************************ //
// XML       : PairingTerminal, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class PairingTerminal2 : public PairingTerminal {
private:
__published:
};




// ************************************************************************ //
// XML       : POSDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class POSDetails2 : public POSDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : SmartConnectResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class SmartConnectResponse2 : public SmartConnectResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : SmartConnectDataObject, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class SmartConnectDataObject2 : public SmartConnectDataObject {
private:
__published:
};




// ************************************************************************ //
// XML       : TransactionTypes, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain
// ************************************************************************ //
class TransactionTypes2 : public TransactionTypes {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IWCFSmartConnect/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceSmartConnect
// service   : WCFServiceSmartConnect
// port      : basicHttpBinding_ServiceSmartConnect
// URL       : http://localhost:8744/MenumateServices/SmartConnect/
// ************************************************************************ //
__interface INTERFACE_UUID("{B8C7CF94-2122-7657-2546-33A0DAD4D505}") IWCFSmartConnect : public IInvokable
{
public:
  virtual SmartConnectResponse* Pairing(const PairingTerminal* param) = 0; 
  virtual SmartConnectResponse* Logon(const TransactionTypes* logonType) = 0; 
  virtual SmartConnectResponse* SettlementInquiry(const TransactionTypes* settlementEnquiryType) = 0; 
  virtual SmartConnectResponse* SettlementCutover(const TransactionTypes* settlementCutoverType) = 0; 
  virtual SmartConnectResponse* Purchase(const TransactionTypes* purchaseType, const double amount) = 0; 
  virtual SmartConnectResponse* PurchasePlusCash(const TransactionTypes* purchasePlusCashType, const double totalAmount, const double cashAmount) = 0; 
  virtual SmartConnectResponse* CashOutOnly(const TransactionTypes* cashOutOnlyType, const double cashAmount) = 0; 
  virtual SmartConnectResponse* Refund(const TransactionTypes* refundType, const double refundAmount) = 0; 
  virtual SmartConnectResponse* Authorise(const TransactionTypes* authoriseType, const double amountAuth, const UnicodeString transactionRef) = 0; 
  virtual SmartConnectResponse* Finalise(const TransactionTypes* finaliseType, const double amountAuth, const UnicodeString transactionRef) = 0; 
  virtual SmartConnectResponse* GetTransactionResult(const TransactionTypes* transResultType) = 0; 
  virtual SmartConnectResponse* ReprintLastReceipt(const TransactionTypes* reprintReceiptType) = 0; 
  virtual SmartConnectResponse* GetTerminalStatus(const TransactionTypes* terminalStatusType) = 0; 
  virtual SmartConnectResponse* TerminalReadCard(const TransactionTypes* readCardType) = 0; 
  virtual SmartConnectResponse* PrintReceipt(const TransactionTypes* printReceiptType) = 0; 
};
typedef DelphiInterface<IWCFSmartConnect> _di_IWCFSmartConnect;

_di_IWCFSmartConnect GetIWCFSmartConnect(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__SmartConnnectWSDL;
#endif



#endif // _H
