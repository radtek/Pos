// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8742/MenumateServices/WalletPayments/?wsdl
//  >Import : http://localhost:8742/MenumateServices/WalletPayments/?wsdl>0
//  >Import : http://localhost:8742/MenumateServices/WalletPayments/?xsd=xsd0
//  >Import : http://localhost:8742/MenumateServices/WalletPayments/?xsd=xsd2
//  >Import : http://localhost:8742/MenumateServices/WalletPayments/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (28/03/2017 12:04:28 a.m. - - $Rev: 25127 $)
// ************************************************************************ //


#ifndef   WalletPaymentsWSDLH
#define   WalletPaymentsWSDLH

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


namespace NS_WalletPaymentsWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS WalletAccount;
class SOAP_REMOTABLE_CLASS WalletActionResponse;
class SOAP_REMOTABLE_CLASS WalletTransaction;
class SOAP_REMOTABLE_CLASS WalletAccount2;
class SOAP_REMOTABLE_CLASS WalletActionResponse2;
class SOAP_REMOTABLE_CLASS WalletTransaction2;



// ************************************************************************ //
// XML       : WalletAccount, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments
// ************************************************************************ //
class WalletAccount : public TRemotable {
private:
  UnicodeString   FMerchentId;
  bool            FMerchentId_Specified;
  UnicodeString   FPassword;
  bool            FPassword_Specified;
  UnicodeString   FSignKey;
  bool            FSignKey_Specified;
  UnicodeString   FTerminalId;
  bool            FTerminalId_Specified;
  UnicodeString   FUserName;
  bool            FUserName_Specified;
  int             FWalletType;
  bool            FWalletType_Specified;
  void __fastcall SetMerchentId(int Index, UnicodeString _prop_val)
  {  FMerchentId = _prop_val; FMerchentId_Specified = true;  }
  bool __fastcall MerchentId_Specified(int Index)
  {  return FMerchentId_Specified;  }
  void __fastcall SetPassword(int Index, UnicodeString _prop_val)
  {  FPassword = _prop_val; FPassword_Specified = true;  }
  bool __fastcall Password_Specified(int Index)
  {  return FPassword_Specified;  }
  void __fastcall SetSignKey(int Index, UnicodeString _prop_val)
  {  FSignKey = _prop_val; FSignKey_Specified = true;  }
  bool __fastcall SignKey_Specified(int Index)
  {  return FSignKey_Specified;  }
  void __fastcall SetTerminalId(int Index, UnicodeString _prop_val)
  {  FTerminalId = _prop_val; FTerminalId_Specified = true;  }
  bool __fastcall TerminalId_Specified(int Index)
  {  return FTerminalId_Specified;  }
  void __fastcall SetUserName(int Index, UnicodeString _prop_val)
  {  FUserName = _prop_val; FUserName_Specified = true;  }
  bool __fastcall UserName_Specified(int Index)
  {  return FUserName_Specified;  }
  void __fastcall SetWalletType(int Index, int _prop_val)
  {  FWalletType = _prop_val; FWalletType_Specified = true;  }
  bool __fastcall WalletType_Specified(int Index)
  {  return FWalletType_Specified;  }
__published:
  __property UnicodeString MerchentId = { index=(IS_OPTN|IS_NLBL), read=FMerchentId, write=SetMerchentId, stored = MerchentId_Specified };
  __property UnicodeString   Password = { index=(IS_OPTN|IS_NLBL), read=FPassword, write=SetPassword, stored = Password_Specified };
  __property UnicodeString    SignKey = { index=(IS_OPTN|IS_NLBL), read=FSignKey, write=SetSignKey, stored = SignKey_Specified };
  __property UnicodeString TerminalId = { index=(IS_OPTN|IS_NLBL), read=FTerminalId, write=SetTerminalId, stored = TerminalId_Specified };
  __property UnicodeString   UserName = { index=(IS_OPTN|IS_NLBL), read=FUserName, write=SetUserName, stored = UserName_Specified };
  __property int        WalletType = { index=(IS_OPTN), read=FWalletType, write=SetWalletType, stored = WalletType_Specified };
};




// ************************************************************************ //
// XML       : WalletActionResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments
// ************************************************************************ //
class WalletActionResponse : public TRemotable {
private:
  int             FExpiresIn;
  bool            FExpiresIn_Specified;
  UnicodeString   FOrderId;
  bool            FOrderId_Specified;
  UnicodeString   FResponseMessage;
  bool            FResponseMessage_Specified;
  bool            FResponseSuccessful;
  bool            FResponseSuccessful_Specified;
  UnicodeString   FSecurityToken;
  bool            FSecurityToken_Specified;
  UnicodeString   FSignKey;
  bool            FSignKey_Specified;
  void __fastcall SetExpiresIn(int Index, int _prop_val)
  {  FExpiresIn = _prop_val; FExpiresIn_Specified = true;  }
  bool __fastcall ExpiresIn_Specified(int Index)
  {  return FExpiresIn_Specified;  }
  void __fastcall SetOrderId(int Index, UnicodeString _prop_val)
  {  FOrderId = _prop_val; FOrderId_Specified = true;  }
  bool __fastcall OrderId_Specified(int Index)
  {  return FOrderId_Specified;  }
  void __fastcall SetResponseMessage(int Index, UnicodeString _prop_val)
  {  FResponseMessage = _prop_val; FResponseMessage_Specified = true;  }
  bool __fastcall ResponseMessage_Specified(int Index)
  {  return FResponseMessage_Specified;  }
  void __fastcall SetResponseSuccessful(int Index, bool _prop_val)
  {  FResponseSuccessful = _prop_val; FResponseSuccessful_Specified = true;  }
  bool __fastcall ResponseSuccessful_Specified(int Index)
  {  return FResponseSuccessful_Specified;  }
  void __fastcall SetSecurityToken(int Index, UnicodeString _prop_val)
  {  FSecurityToken = _prop_val; FSecurityToken_Specified = true;  }
  bool __fastcall SecurityToken_Specified(int Index)
  {  return FSecurityToken_Specified;  }
  void __fastcall SetSignKey(int Index, UnicodeString _prop_val)
  {  FSignKey = _prop_val; FSignKey_Specified = true;  }
  bool __fastcall SignKey_Specified(int Index)
  {  return FSignKey_Specified;  }
__published:
  __property int         ExpiresIn = { index=(IS_OPTN), read=FExpiresIn, write=SetExpiresIn, stored = ExpiresIn_Specified };
  __property UnicodeString    OrderId = { index=(IS_OPTN|IS_NLBL), read=FOrderId, write=SetOrderId, stored = OrderId_Specified };
  __property UnicodeString ResponseMessage = { index=(IS_OPTN|IS_NLBL), read=FResponseMessage, write=SetResponseMessage, stored = ResponseMessage_Specified };
  __property bool       ResponseSuccessful = { index=(IS_OPTN), read=FResponseSuccessful, write=SetResponseSuccessful, stored = ResponseSuccessful_Specified };
  __property UnicodeString SecurityToken = { index=(IS_OPTN|IS_NLBL), read=FSecurityToken, write=SetSecurityToken, stored = SecurityToken_Specified };
  __property UnicodeString    SignKey = { index=(IS_OPTN|IS_NLBL), read=FSignKey, write=SetSignKey, stored = SignKey_Specified };
};




// ************************************************************************ //
// XML       : WalletTransaction, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments
// ************************************************************************ //
class WalletTransaction : public TRemotable {
private:
  double          FAmount;
  bool            FAmount_Specified;
  UnicodeString   FReferenceNumber;
  bool            FReferenceNumber_Specified;
  UnicodeString   FScannedCode;
  bool            FScannedCode_Specified;
  void __fastcall SetAmount(int Index, double _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  }
  void __fastcall SetReferenceNumber(int Index, UnicodeString _prop_val)
  {  FReferenceNumber = _prop_val; FReferenceNumber_Specified = true;  }
  bool __fastcall ReferenceNumber_Specified(int Index)
  {  return FReferenceNumber_Specified;  }
  void __fastcall SetScannedCode(int Index, UnicodeString _prop_val)
  {  FScannedCode = _prop_val; FScannedCode_Specified = true;  }
  bool __fastcall ScannedCode_Specified(int Index)
  {  return FScannedCode_Specified;  }
__published:
  __property double         Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString ReferenceNumber = { index=(IS_OPTN|IS_NLBL), read=FReferenceNumber, write=SetReferenceNumber, stored = ReferenceNumber_Specified };
  __property UnicodeString ScannedCode = { index=(IS_OPTN|IS_NLBL), read=FScannedCode, write=SetScannedCode, stored = ScannedCode_Specified };
};




// ************************************************************************ //
// XML       : WalletAccount, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments
// ************************************************************************ //
class WalletAccount2 : public WalletAccount {
private:
__published:
};




// ************************************************************************ //
// XML       : WalletActionResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments
// ************************************************************************ //
class WalletActionResponse2 : public WalletActionResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : WalletTransaction, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments
// ************************************************************************ //
class WalletTransaction2 : public WalletTransaction {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IWCFServiceWalletPayments/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceWalletPayments
// service   : WCFServiceWalletPayments
// port      : basicHttpBinding_ServiceWalletPayments
// URL       : http://localhost:8743/MenumateServices/WalletPayments/
// ************************************************************************ //
__interface INTERFACE_UUID("{D80B471D-30EB-B244-9A18-19E59F601365}") IWCFServiceWalletPayments : public IInvokable
{
public:
  virtual WalletActionResponse* Login(const WalletAccount* inWalletAccount) = 0;
  virtual WalletActionResponse* DoTransaction(const WalletAccount* inWalletAccount, const WalletTransaction* inWalletTransaction) = 0;
};
typedef DelphiInterface<IWCFServiceWalletPayments> _di_IWCFServiceWalletPayments;

_di_IWCFServiceWalletPayments GetIWCFServiceWalletPayments(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_WalletPaymentsWSDL;
#endif



#endif // _H
