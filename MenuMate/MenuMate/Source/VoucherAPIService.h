// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : C:\Code\5.5\Static Files\wsdl schema\VoucherAPIService.wsdl
//  >Import : C:\Code\5.5\Static Files\wsdl schema\VoucherAPIService.wsdl>0
// Encoding : utf-8
// Version  : 1.0
// (14/03/2012 11:51:19 a.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   VoucherAPIServiceH
#define   VoucherAPIServiceH

#include <winsock2.h>
#include <System.hpp>
#include <InvokeRegistry.hpp>
#include <XSBuiltIns.hpp>
#include <SOAPHTTPClient.hpp>

#if !defined(SOAP_REMOTABLE_CLASS)
#define SOAP_REMOTABLE_CLASS __declspec(delphiclass)
#endif
#if !defined(IS_OPTN)
#define IS_OPTN 0x0001
#endif
#if !defined(IS_REF)
#define IS_REF 0x0080
#endif


namespace NS_VoucherAPIService {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:decimal         - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS voucherResponse;



// ************************************************************************ //
// XML       : voucherResponse, global, <complexType>
// Namespace : http://voucher.menumate.com/
// ************************************************************************ //
class voucherResponse : public TRemotable {
private:
  int             FStatus;
  UnicodeString   FMsg;
  bool            FMsg_Specified;
  void __fastcall SetMsg(int Index, UnicodeString _prop_val)
  {  FMsg = _prop_val; FMsg_Specified = true;  }
  bool __fastcall Msg_Specified(int Index)
  {  return FMsg_Specified;  } 
__published:
  __property int            Status = { read=FStatus, write=FStatus };
  __property UnicodeString        Msg = { index=(IS_OPTN), read=FMsg, write=SetMsg, stored = Msg_Specified };
};



// ************************************************************************ //
// Namespace : http://voucher.menumate.com/
// soapAction: http://voucher.menumate.com/VoucherAPI/%operationName%Request
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : BasicHttpBinding_VoucherAPI
// service   : VoucherAPIImplementation
// port      : BasicHttpBinding_VoucherAPI
// URL       : http://pos.host.pocketvouchers.com/menumate.aspx
// ************************************************************************ //
__interface INTERFACE_UUID("{D5D13FCE-3C70-44CC-3272-4B7750A7A137}") VoucherAPI : public IInvokable
{
public:
  virtual UnicodeString   AuthToken(const UnicodeString arg0, const UnicodeString arg1) = 0; 
  virtual voucherResponse* RedeemVoucher(const UnicodeString arg0, const UnicodeString arg1, const UnicodeString arg2, const TXSDecimal* arg3) = 0; 
  virtual voucherResponse* CreditVoucher(const UnicodeString arg0, const UnicodeString arg1, const UnicodeString arg2, const TXSDecimal* arg3) = 0; 
};
typedef DelphiInterface<VoucherAPI> _di_VoucherAPI;

_di_VoucherAPI GetVoucherAPI(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS_VoucherAPIService

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_VoucherAPIService;
#endif



#endif // VoucherAPIServiceH
