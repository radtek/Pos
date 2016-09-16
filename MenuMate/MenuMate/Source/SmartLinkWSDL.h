// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8738/MenumateServices/SmartLink/?wsdl
//  >Import : http://localhost:8738/MenumateServices/SmartLink/?wsdl>0
//  >Import : http://localhost:8738/MenumateServices/SmartLink/?xsd=xsd0
//  >Import : http://localhost:8738/MenumateServices/SmartLink/?xsd=xsd2
//  >Import : http://localhost:8738/MenumateServices/SmartLink/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (9/08/2016 6:07:10 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   SmartLinkWSDLH
#define   SmartLinkWSDLH

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


namespace NS__SmartLinkWSDL {

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

class SOAP_REMOTABLE_CLASS DTO_SmartLinkTransactionResponse;
class SOAP_REMOTABLE_CLASS DTO_SmartLinkTransactionResponse2;



// ************************************************************************ //
// XML       : DTO_SmartLinkTransactionResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.SmartLink
// ************************************************************************ //
class DTO_SmartLinkTransactionResponse : public TRemotable {
private:
  UnicodeString   FAcquirerRef;
  bool            FAcquirerRef_Specified;
  UnicodeString   FErrorText;
  bool            FErrorText_Specified;
  UnicodeString   FResult;
  bool            FResult_Specified;
  UnicodeString   FResultText;
  bool            FResultText_Specified;
  bool            FSuccessful;
  bool            FSuccessful_Specified;
  UnicodeString   FTransactionResult;
  bool            FTransactionResult_Specified;
  void __fastcall SetAcquirerRef(int Index, UnicodeString _prop_val)
  {  FAcquirerRef = _prop_val; FAcquirerRef_Specified = true;  }
  bool __fastcall AcquirerRef_Specified(int Index)
  {  return FAcquirerRef_Specified;  } 
  void __fastcall SetErrorText(int Index, UnicodeString _prop_val)
  {  FErrorText = _prop_val; FErrorText_Specified = true;  }
  bool __fastcall ErrorText_Specified(int Index)
  {  return FErrorText_Specified;  } 
  void __fastcall SetResult(int Index, UnicodeString _prop_val)
  {  FResult = _prop_val; FResult_Specified = true;  }
  bool __fastcall Result_Specified(int Index)
  {  return FResult_Specified;  } 
  void __fastcall SetResultText(int Index, UnicodeString _prop_val)
  {  FResultText = _prop_val; FResultText_Specified = true;  }
  bool __fastcall ResultText_Specified(int Index)
  {  return FResultText_Specified;  } 
  void __fastcall SetSuccessful(int Index, bool _prop_val)
  {  FSuccessful = _prop_val; FSuccessful_Specified = true;  }
  bool __fastcall Successful_Specified(int Index)
  {  return FSuccessful_Specified;  } 
  void __fastcall SetTransactionResult(int Index, UnicodeString _prop_val)
  {  FTransactionResult = _prop_val; FTransactionResult_Specified = true;  }
  bool __fastcall TransactionResult_Specified(int Index)
  {  return FTransactionResult_Specified;  } 
__published:
  __property UnicodeString AcquirerRef = { index=(IS_OPTN|IS_NLBL), read=FAcquirerRef, write=SetAcquirerRef, stored = AcquirerRef_Specified };
  __property UnicodeString  ErrorText = { index=(IS_OPTN|IS_NLBL), read=FErrorText, write=SetErrorText, stored = ErrorText_Specified };
  __property UnicodeString     Result = { index=(IS_OPTN|IS_NLBL), read=FResult, write=SetResult, stored = Result_Specified };
  __property UnicodeString ResultText = { index=(IS_OPTN|IS_NLBL), read=FResultText, write=SetResultText, stored = ResultText_Specified };
  __property bool       Successful = { index=(IS_OPTN), read=FSuccessful, write=SetSuccessful, stored = Successful_Specified };
  __property UnicodeString TransactionResult = { index=(IS_OPTN|IS_NLBL), read=FTransactionResult, write=SetTransactionResult, stored = TransactionResult_Specified };
};




// ************************************************************************ //
// XML       : DTO_SmartLinkTransactionResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.SmartLink
// ************************************************************************ //
class DTO_SmartLinkTransactionResponse2 : public DTO_SmartLinkTransactionResponse {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IWCFServiceSmartLink/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceSmartLink
// service   : WCFServiceSmartLink
// port      : basicHttpBinding_ServiceSmartLink
// URL       : http://localhost:8738/MenumateServices/SmartLink/
// ************************************************************************ //
__interface INTERFACE_UUID("{1C2618C7-D95C-89EE-23DC-28D2EFD8FD9D}") IWCFServiceSmartLink : public IInvokable
{
public:
  virtual DTO_SmartLinkTransactionResponse* PingTerminal(const UnicodeString ipAddress) = 0; 
  virtual DTO_SmartLinkTransactionResponse* Login(const UnicodeString ipAddress, const UnicodeString versionString) = 0; 
  virtual DTO_SmartLinkTransactionResponse* SettlementInquiry(const UnicodeString ipAddress) = 0; 
  virtual DTO_SmartLinkTransactionResponse* SettlementCutover(const UnicodeString ipAddress) = 0; 
  virtual DTO_SmartLinkTransactionResponse* Purchase(const UnicodeString ipAddress, const double amount) = 0; 
  virtual DTO_SmartLinkTransactionResponse* PurchasePlusCash(const UnicodeString ipAddress, const double amount, const double cash) = 0; 
  virtual DTO_SmartLinkTransactionResponse* CashOnly(const UnicodeString ipAddress, const double cash) = 0; 
  virtual DTO_SmartLinkTransactionResponse* Refund(const UnicodeString ipAddress, const double refundAmount) = 0; 
  virtual DTO_SmartLinkTransactionResponse* ReprintLastReceipt(const UnicodeString ipAddress) = 0; 
  virtual DTO_SmartLinkTransactionResponse* PrintReceipt(const UnicodeString ipAddress) = 0; 
};
typedef DelphiInterface<IWCFServiceSmartLink> _di_IWCFServiceSmartLink;

_di_IWCFServiceSmartLink GetIWCFServiceSmartLink(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__SmartLinkWSDL;
#endif



#endif // _H
