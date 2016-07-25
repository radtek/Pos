// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8741/MenumateServices/SalesForceService/?wsdl
//  >Import : http://localhost:8741/MenumateServices/SalesForceService/?wsdl>0
//  >Import : http://localhost:8741/MenumateServices/SalesForceService/?xsd=xsd0
//  >Import : http://localhost:8741/MenumateServices/SalesForceService/?xsd=xsd2
//  >Import : http://localhost:8741/MenumateServices/SalesForceService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (27/04/2016 11:39:32 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   SalesForceWSDLH
#define   SalesForceWSDLH

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


namespace NS__SalesForceWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS SalesForceModuleStatus;
class SOAP_REMOTABLE_CLASS SalesForceModuleStatus2;



// ************************************************************************ //
// XML       : SalesForceModuleStatus, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SalesForceCommunication.Domain
// ************************************************************************ //
class SalesForceModuleStatus : public TRemotable {
private:
  UnicodeString   FAccountName;
  bool            FAccountName_Specified;
  bool            FIsChefMateActivated;
  bool            FIsChefMateActivated_Specified;
  bool            FIsLoyaltyActivated;
  bool            FIsLoyaltyActivated_Specified;
  bool            FIsMobiToGoActivated;
  bool            FIsMobiToGoActivated_Specified;
  bool            FIsPalmMateActivated;
  bool            FIsPalmMateActivated_Specified;
  bool            FIsStockActivated;
  bool            FIsStockActivated_Specified;
  bool            FIsThorlinkActivated;
  bool            FIsThorlinkActivated_Specified;
  bool            FIsWebMateActivated;
  bool            FIsWebMateActivated_Specified;
  bool            FIsXeroActivated;
  bool            FIsXeroActivated_Specified;
  UnicodeString   FSalesForceVersion;
  bool            FSalesForceVersion_Specified;
  void __fastcall SetAccountName(int Index, UnicodeString _prop_val)
  {  FAccountName = _prop_val; FAccountName_Specified = true;  }
  bool __fastcall AccountName_Specified(int Index)
  {  return FAccountName_Specified;  } 
  void __fastcall SetIsChefMateActivated(int Index, bool _prop_val)
  {  FIsChefMateActivated = _prop_val; FIsChefMateActivated_Specified = true;  }
  bool __fastcall IsChefMateActivated_Specified(int Index)
  {  return FIsChefMateActivated_Specified;  } 
  void __fastcall SetIsLoyaltyActivated(int Index, bool _prop_val)
  {  FIsLoyaltyActivated = _prop_val; FIsLoyaltyActivated_Specified = true;  }
  bool __fastcall IsLoyaltyActivated_Specified(int Index)
  {  return FIsLoyaltyActivated_Specified;  } 
  void __fastcall SetIsMobiToGoActivated(int Index, bool _prop_val)
  {  FIsMobiToGoActivated = _prop_val; FIsMobiToGoActivated_Specified = true;  }
  bool __fastcall IsMobiToGoActivated_Specified(int Index)
  {  return FIsMobiToGoActivated_Specified;  } 
  void __fastcall SetIsPalmMateActivated(int Index, bool _prop_val)
  {  FIsPalmMateActivated = _prop_val; FIsPalmMateActivated_Specified = true;  }
  bool __fastcall IsPalmMateActivated_Specified(int Index)
  {  return FIsPalmMateActivated_Specified;  } 
  void __fastcall SetIsStockActivated(int Index, bool _prop_val)
  {  FIsStockActivated = _prop_val; FIsStockActivated_Specified = true;  }
  bool __fastcall IsStockActivated_Specified(int Index)
  {  return FIsStockActivated_Specified;  } 
  void __fastcall SetIsThorlinkActivated(int Index, bool _prop_val)
  {  FIsThorlinkActivated = _prop_val; FIsThorlinkActivated_Specified = true;  }
  bool __fastcall IsThorlinkActivated_Specified(int Index)
  {  return FIsThorlinkActivated_Specified;  } 
  void __fastcall SetIsWebMateActivated(int Index, bool _prop_val)
  {  FIsWebMateActivated = _prop_val; FIsWebMateActivated_Specified = true;  }
  bool __fastcall IsWebMateActivated_Specified(int Index)
  {  return FIsWebMateActivated_Specified;  } 
  void __fastcall SetIsXeroActivated(int Index, bool _prop_val)
  {  FIsXeroActivated = _prop_val; FIsXeroActivated_Specified = true;  }
  bool __fastcall IsXeroActivated_Specified(int Index)
  {  return FIsXeroActivated_Specified;  } 
  void __fastcall SetSalesForceVersion(int Index, UnicodeString _prop_val)
  {  FSalesForceVersion = _prop_val; FSalesForceVersion_Specified = true;  }
  bool __fastcall SalesForceVersion_Specified(int Index)
  {  return FSalesForceVersion_Specified;  } 
__published:
  __property UnicodeString AccountName = { index=(IS_OPTN|IS_NLBL), read=FAccountName, write=SetAccountName, stored = AccountName_Specified };
  __property bool       IsChefMateActivated = { index=(IS_OPTN), read=FIsChefMateActivated, write=SetIsChefMateActivated, stored = IsChefMateActivated_Specified };
  __property bool       IsLoyaltyActivated = { index=(IS_OPTN), read=FIsLoyaltyActivated, write=SetIsLoyaltyActivated, stored = IsLoyaltyActivated_Specified };
  __property bool       IsMobiToGoActivated = { index=(IS_OPTN), read=FIsMobiToGoActivated, write=SetIsMobiToGoActivated, stored = IsMobiToGoActivated_Specified };
  __property bool       IsPalmMateActivated = { index=(IS_OPTN), read=FIsPalmMateActivated, write=SetIsPalmMateActivated, stored = IsPalmMateActivated_Specified };
  __property bool       IsStockActivated = { index=(IS_OPTN), read=FIsStockActivated, write=SetIsStockActivated, stored = IsStockActivated_Specified };
  __property bool       IsThorlinkActivated = { index=(IS_OPTN), read=FIsThorlinkActivated, write=SetIsThorlinkActivated, stored = IsThorlinkActivated_Specified };
  __property bool       IsWebMateActivated = { index=(IS_OPTN), read=FIsWebMateActivated, write=SetIsWebMateActivated, stored = IsWebMateActivated_Specified };
  __property bool       IsXeroActivated = { index=(IS_OPTN), read=FIsXeroActivated, write=SetIsXeroActivated, stored = IsXeroActivated_Specified };
  __property UnicodeString SalesForceVersion = { index=(IS_OPTN|IS_NLBL), read=FSalesForceVersion, write=SetSalesForceVersion, stored = SalesForceVersion_Specified };
};




// ************************************************************************ //
// XML       : SalesForceModuleStatus, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SalesForceCommunication.Domain
// ************************************************************************ //
class SalesForceModuleStatus2 : public SalesForceModuleStatus {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/ISalesForceIntegrationWebService/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceSalesForce
// service   : SalesForceIntegrationWebService
// port      : basicHttpBinding_ServiceSalesForce
// URL       : http://localhost:8741/MenumateServices/SalesForceService/
// ************************************************************************ //
__interface INTERFACE_UUID("{A2D2310E-86B9-0F73-775C-DF34C81E5088}") ISalesForceIntegrationWebService : public IInvokable
{
public:
  virtual bool            UpdateSalesForceVersion(const UnicodeString versionNumber, const UnicodeString accountName) = 0; 
  virtual bool            EnableMobiToGo(const UnicodeString accountName) = 0; 
  virtual bool            DisableMobiToGo(const UnicodeString accountName) = 0; 
  virtual bool            UpdateSalesForceModulesStatus(const SalesForceModuleStatus* salesForceModuleStatus) = 0; 
};
typedef DelphiInterface<ISalesForceIntegrationWebService> _di_ISalesForceIntegrationWebService;

_di_ISalesForceIntegrationWebService GetISalesForceIntegrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__SalesForceWSDL;
#endif



#endif // _H
