// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8740/MenumateServices/PocketVoucherService/?wsdl
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?wsdl>0
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?xsd=xsd0
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?xsd=xsd2
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?xsd=xsd3
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (18/05/2016 11:36:18 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   PocketVoucherWSDLH
#define   PocketVoucherWSDLH

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


namespace NS__PocketVoucherWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:dateTime        - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS PocketVoucherDetail;
class SOAP_REMOTABLE_CLASS PocketVoucherDetail2;
class SOAP_REMOTABLE_CLASS VoucherRedemptionDetails;
class SOAP_REMOTABLE_CLASS RedemptionResponseDetails;
class SOAP_REMOTABLE_CLASS VoucherRedemptionDetails2;
class SOAP_REMOTABLE_CLASS RedemptionResponseDetails2;



// ************************************************************************ //
// XML       : PocketVoucherDetail, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SalesForceCommunication.Domain
// ************************************************************************ //
class PocketVoucherDetail : public TRemotable {
private:
  TXSDateTime*    FDateUpdated;
  bool            FDateUpdated_Specified;
  UnicodeString   FMerchantId;
  bool            FMerchantId_Specified;
  UnicodeString   FPassword;
  bool            FPassword_Specified;
  bool            FPocketVoucher;
  bool            FPocketVoucher_Specified;
  UnicodeString   FUrl;
  bool            FUrl_Specified;
  UnicodeString   FUserName;
  bool            FUserName_Specified;
  void __fastcall SetDateUpdated(int Index, TXSDateTime* _prop_val)
  {  FDateUpdated = _prop_val; FDateUpdated_Specified = true;  }
  bool __fastcall DateUpdated_Specified(int Index)
  {  return FDateUpdated_Specified;  } 
  void __fastcall SetMerchantId(int Index, UnicodeString _prop_val)
  {  FMerchantId = _prop_val; FMerchantId_Specified = true;  }
  bool __fastcall MerchantId_Specified(int Index)
  {  return FMerchantId_Specified;  } 
  void __fastcall SetPassword(int Index, UnicodeString _prop_val)
  {  FPassword = _prop_val; FPassword_Specified = true;  }
  bool __fastcall Password_Specified(int Index)
  {  return FPassword_Specified;  } 
  void __fastcall SetPocketVoucher(int Index, bool _prop_val)
  {  FPocketVoucher = _prop_val; FPocketVoucher_Specified = true;  }
  bool __fastcall PocketVoucher_Specified(int Index)
  {  return FPocketVoucher_Specified;  } 
  void __fastcall SetUrl(int Index, UnicodeString _prop_val)
  {  FUrl = _prop_val; FUrl_Specified = true;  }
  bool __fastcall Url_Specified(int Index)
  {  return FUrl_Specified;  } 
  void __fastcall SetUserName(int Index, UnicodeString _prop_val)
  {  FUserName = _prop_val; FUserName_Specified = true;  }
  bool __fastcall UserName_Specified(int Index)
  {  return FUserName_Specified;  } 

public:
  __fastcall ~PocketVoucherDetail();
__published:
  __property TXSDateTime* DateUpdated = { index=(IS_OPTN|IS_NLBL), read=FDateUpdated, write=SetDateUpdated, stored = DateUpdated_Specified };
  __property UnicodeString MerchantId = { index=(IS_OPTN|IS_NLBL), read=FMerchantId, write=SetMerchantId, stored = MerchantId_Specified };
  __property UnicodeString   Password = { index=(IS_OPTN|IS_NLBL), read=FPassword, write=SetPassword, stored = Password_Specified };
  __property bool       PocketVoucher = { index=(IS_OPTN|IS_NLBL), read=FPocketVoucher, write=SetPocketVoucher, stored = PocketVoucher_Specified };
  __property UnicodeString        Url = { index=(IS_OPTN|IS_NLBL), read=FUrl, write=SetUrl, stored = Url_Specified };
  __property UnicodeString   UserName = { index=(IS_OPTN|IS_NLBL), read=FUserName, write=SetUserName, stored = UserName_Specified };
};




// ************************************************************************ //
// XML       : PocketVoucherDetail, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SalesForceCommunication.Domain
// ************************************************************************ //
class PocketVoucherDetail2 : public PocketVoucherDetail {
private:
__published:
};




// ************************************************************************ //
// XML       : VoucherRedemptionDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PocketVoucherIntegration.RedemptionDetails
// ************************************************************************ //
class VoucherRedemptionDetails : public TRemotable {
private:
  UnicodeString   FMerchantId;
  bool            FMerchantId_Specified;
  UnicodeString   FMerchantReference;
  bool            FMerchantReference_Specified;
  UnicodeString   FPassword;
  bool            FPassword_Specified;
  UnicodeString   FTerminalId;
  bool            FTerminalId_Specified;
  UnicodeString   FTransactionAmount;
  bool            FTransactionAmount_Specified;
  UnicodeString   FUserName;
  bool            FUserName_Specified;
  UnicodeString   FVoucherAmount;
  bool            FVoucherAmount_Specified;
  UnicodeString   FVoucherCode;
  bool            FVoucherCode_Specified;
  void __fastcall SetMerchantId(int Index, UnicodeString _prop_val)
  {  FMerchantId = _prop_val; FMerchantId_Specified = true;  }
  bool __fastcall MerchantId_Specified(int Index)
  {  return FMerchantId_Specified;  } 
  void __fastcall SetMerchantReference(int Index, UnicodeString _prop_val)
  {  FMerchantReference = _prop_val; FMerchantReference_Specified = true;  }
  bool __fastcall MerchantReference_Specified(int Index)
  {  return FMerchantReference_Specified;  } 
  void __fastcall SetPassword(int Index, UnicodeString _prop_val)
  {  FPassword = _prop_val; FPassword_Specified = true;  }
  bool __fastcall Password_Specified(int Index)
  {  return FPassword_Specified;  } 
  void __fastcall SetTerminalId(int Index, UnicodeString _prop_val)
  {  FTerminalId = _prop_val; FTerminalId_Specified = true;  }
  bool __fastcall TerminalId_Specified(int Index)
  {  return FTerminalId_Specified;  } 
  void __fastcall SetTransactionAmount(int Index, UnicodeString _prop_val)
  {  FTransactionAmount = _prop_val; FTransactionAmount_Specified = true;  }
  bool __fastcall TransactionAmount_Specified(int Index)
  {  return FTransactionAmount_Specified;  } 
  void __fastcall SetUserName(int Index, UnicodeString _prop_val)
  {  FUserName = _prop_val; FUserName_Specified = true;  }
  bool __fastcall UserName_Specified(int Index)
  {  return FUserName_Specified;  } 
  void __fastcall SetVoucherAmount(int Index, UnicodeString _prop_val)
  {  FVoucherAmount = _prop_val; FVoucherAmount_Specified = true;  }
  bool __fastcall VoucherAmount_Specified(int Index)
  {  return FVoucherAmount_Specified;  } 
  void __fastcall SetVoucherCode(int Index, UnicodeString _prop_val)
  {  FVoucherCode = _prop_val; FVoucherCode_Specified = true;  }
  bool __fastcall VoucherCode_Specified(int Index)
  {  return FVoucherCode_Specified;  } 
__published:
  __property UnicodeString MerchantId = { index=(IS_OPTN|IS_NLBL), read=FMerchantId, write=SetMerchantId, stored = MerchantId_Specified };
  __property UnicodeString MerchantReference = { index=(IS_OPTN|IS_NLBL), read=FMerchantReference, write=SetMerchantReference, stored = MerchantReference_Specified };
  __property UnicodeString   Password = { index=(IS_OPTN|IS_NLBL), read=FPassword, write=SetPassword, stored = Password_Specified };
  __property UnicodeString TerminalId = { index=(IS_OPTN|IS_NLBL), read=FTerminalId, write=SetTerminalId, stored = TerminalId_Specified };
  __property UnicodeString TransactionAmount = { index=(IS_OPTN|IS_NLBL), read=FTransactionAmount, write=SetTransactionAmount, stored = TransactionAmount_Specified };
  __property UnicodeString   UserName = { index=(IS_OPTN|IS_NLBL), read=FUserName, write=SetUserName, stored = UserName_Specified };
  __property UnicodeString VoucherAmount = { index=(IS_OPTN|IS_NLBL), read=FVoucherAmount, write=SetVoucherAmount, stored = VoucherAmount_Specified };
  __property UnicodeString VoucherCode = { index=(IS_OPTN|IS_NLBL), read=FVoucherCode, write=SetVoucherCode, stored = VoucherCode_Specified };
};




// ************************************************************************ //
// XML       : RedemptionResponseDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/PocketVoucherIntegration.RedemptionDetails
// ************************************************************************ //
class RedemptionResponseDetails : public TRemotable {
private:
  UnicodeString   FRedemptionStatus;
  bool            FRedemptionStatus_Specified;
  UnicodeString   FResponseCode;
  bool            FResponseCode_Specified;
  UnicodeString   FResponseMessage;
  bool            FResponseMessage_Specified;
  void __fastcall SetRedemptionStatus(int Index, UnicodeString _prop_val)
  {  FRedemptionStatus = _prop_val; FRedemptionStatus_Specified = true;  }
  bool __fastcall RedemptionStatus_Specified(int Index)
  {  return FRedemptionStatus_Specified;  } 
  void __fastcall SetResponseCode(int Index, UnicodeString _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 
  void __fastcall SetResponseMessage(int Index, UnicodeString _prop_val)
  {  FResponseMessage = _prop_val; FResponseMessage_Specified = true;  }
  bool __fastcall ResponseMessage_Specified(int Index)
  {  return FResponseMessage_Specified;  } 
__published:
  __property UnicodeString RedemptionStatus = { index=(IS_OPTN|IS_NLBL), read=FRedemptionStatus, write=SetRedemptionStatus, stored = RedemptionStatus_Specified };
  __property UnicodeString ResponseCode = { index=(IS_OPTN|IS_NLBL), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
  __property UnicodeString ResponseMessage = { index=(IS_OPTN|IS_NLBL), read=FResponseMessage, write=SetResponseMessage, stored = ResponseMessage_Specified };
};




// ************************************************************************ //
// XML       : VoucherRedemptionDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PocketVoucherIntegration.RedemptionDetails
// ************************************************************************ //
class VoucherRedemptionDetails2 : public VoucherRedemptionDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : RedemptionResponseDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/PocketVoucherIntegration.RedemptionDetails
// ************************************************************************ //
class RedemptionResponseDetails2 : public RedemptionResponseDetails {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IPocketVoucherWebService/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServicePocketVoucher
// service   : PocketVoucherWebService
// port      : basicHttpBinding_ServicePocketVoucher
// URL       : http://localhost:8740/MenumateServices/PocketVoucherService/
// ************************************************************************ //
__interface INTERFACE_UUID("{E136A1F9-63D7-38AC-4950-EA03D44141CB}") IPocketVoucherWebService : public IInvokable
{
public:
  virtual PocketVoucherDetail* GetPocketVoucherDetail(const UnicodeString accountName) = 0; 
  virtual bool            EnablePocketVouchers(const UnicodeString accountName) = 0; 
  virtual bool            DisablePocketVouchers(const UnicodeString accountName) = 0; 
  virtual RedemptionResponseDetails* RedeemVoucher(const VoucherRedemptionDetails* redemptionDetails) = 0; 
};
typedef DelphiInterface<IPocketVoucherWebService> _di_IPocketVoucherWebService;

_di_IPocketVoucherWebService GetIPocketVoucherWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__PocketVoucherWSDL;
#endif



#endif // _H
