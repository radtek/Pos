// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8749/MenumateServices/RegistrationService/?wsdl
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?wsdl>0
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd0
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd2
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd3
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (23/01/2019 9:21:20 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   RegistrationWSDLH
#define   RegistrationWSDLH

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


namespace NS__RegistrationWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:long            - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS MMServiceResponse;
class SOAP_REMOTABLE_CLASS MMServiceResponse2;
class SOAP_REMOTABLE_CLASS TerminalModel;
class SOAP_REMOTABLE_CLASS LicenceSettingModel;
class SOAP_REMOTABLE_CLASS RegistrationResponse;
class SOAP_REMOTABLE_CLASS RegistrationWebResponse;
class SOAP_REMOTABLE_CLASS TerminalModel2;
class SOAP_REMOTABLE_CLASS LicenceSettingModel2;
class SOAP_REMOTABLE_CLASS RegistrationResponse2;
class SOAP_REMOTABLE_CLASS RegistrationWebResponse2;

enum class RegistrationResponseCode   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration"[GblSmpl] */
{
  Successful, 
  AuthenticationFailed, 
  RegistrationUpdateFailed, 
  CompanyInfoValidationFailed
};

class RegistrationResponseCode_TypeInfoHolder : public TObject {
  RegistrationResponseCode __instanceType;
public:
__published:
  __property RegistrationResponseCode __propType = { read=__instanceType };
};



// ************************************************************************ //
// XML       : MMServiceResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO
// ************************************************************************ //
class MMServiceResponse : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FMessage;
  bool            FMessage_Specified;
  bool            FSuccessful;
  bool            FSuccessful_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetMessage(int Index, UnicodeString _prop_val)
  {  FMessage = _prop_val; FMessage_Specified = true;  }
  bool __fastcall Message_Specified(int Index)
  {  return FMessage_Specified;  } 
  void __fastcall SetSuccessful(int Index, bool _prop_val)
  {  FSuccessful = _prop_val; FSuccessful_Specified = true;  }
  bool __fastcall Successful_Specified(int Index)
  {  return FSuccessful_Specified;  } 
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString    Message = { index=(IS_OPTN|IS_NLBL), read=FMessage, write=SetMessage, stored = Message_Specified };
  __property bool       Successful = { index=(IS_OPTN), read=FSuccessful, write=SetSuccessful, stored = Successful_Specified };
};




// ************************************************************************ //
// XML       : MMServiceResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO
// ************************************************************************ //
class MMServiceResponse2 : public MMServiceResponse {
private:
__published:
};


typedef DynamicArray<LicenceSettingModel*> ArrayOfLicenceSettingModel; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration"[GblCplx] */


// ************************************************************************ //
// XML       : TerminalModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class TerminalModel : public TRemotable {
private:
  UnicodeString   FComputerName;
  bool            FComputerName_Specified;
  ArrayOfLicenceSettingModel FLicenceSettingsModel;
  bool            FLicenceSettingsModel_Specified;
  UnicodeString   FMacAdress;
  bool            FMacAdress_Specified;
  UnicodeString   FMenumateVersion;
  bool            FMenumateVersion_Specified;
  UnicodeString   FOperatingSystemName;
  bool            FOperatingSystemName_Specified;
  __int64         FSiteCode;
  bool            FSiteCode_Specified;
  UnicodeString   FStaffName;
  bool            FStaffName_Specified;
  UnicodeString   FSyndicateCode;
  bool            FSyndicateCode_Specified;
  UnicodeString   FTerminalDescription;
  bool            FTerminalDescription_Specified;
  UnicodeString   FTerminalName;
  bool            FTerminalName_Specified;
  void __fastcall SetComputerName(int Index, UnicodeString _prop_val)
  {  FComputerName = _prop_val; FComputerName_Specified = true;  }
  bool __fastcall ComputerName_Specified(int Index)
  {  return FComputerName_Specified;  } 
  void __fastcall SetLicenceSettingsModel(int Index, ArrayOfLicenceSettingModel _prop_val)
  {  FLicenceSettingsModel = _prop_val; FLicenceSettingsModel_Specified = true;  }
  bool __fastcall LicenceSettingsModel_Specified(int Index)
  {  return FLicenceSettingsModel_Specified;  } 
  void __fastcall SetMacAdress(int Index, UnicodeString _prop_val)
  {  FMacAdress = _prop_val; FMacAdress_Specified = true;  }
  bool __fastcall MacAdress_Specified(int Index)
  {  return FMacAdress_Specified;  } 
  void __fastcall SetMenumateVersion(int Index, UnicodeString _prop_val)
  {  FMenumateVersion = _prop_val; FMenumateVersion_Specified = true;  }
  bool __fastcall MenumateVersion_Specified(int Index)
  {  return FMenumateVersion_Specified;  } 
  void __fastcall SetOperatingSystemName(int Index, UnicodeString _prop_val)
  {  FOperatingSystemName = _prop_val; FOperatingSystemName_Specified = true;  }
  bool __fastcall OperatingSystemName_Specified(int Index)
  {  return FOperatingSystemName_Specified;  } 
  void __fastcall SetSiteCode(int Index, __int64 _prop_val)
  {  FSiteCode = _prop_val; FSiteCode_Specified = true;  }
  bool __fastcall SiteCode_Specified(int Index)
  {  return FSiteCode_Specified;  } 
  void __fastcall SetStaffName(int Index, UnicodeString _prop_val)
  {  FStaffName = _prop_val; FStaffName_Specified = true;  }
  bool __fastcall StaffName_Specified(int Index)
  {  return FStaffName_Specified;  } 
  void __fastcall SetSyndicateCode(int Index, UnicodeString _prop_val)
  {  FSyndicateCode = _prop_val; FSyndicateCode_Specified = true;  }
  bool __fastcall SyndicateCode_Specified(int Index)
  {  return FSyndicateCode_Specified;  } 
  void __fastcall SetTerminalDescription(int Index, UnicodeString _prop_val)
  {  FTerminalDescription = _prop_val; FTerminalDescription_Specified = true;  }
  bool __fastcall TerminalDescription_Specified(int Index)
  {  return FTerminalDescription_Specified;  } 
  void __fastcall SetTerminalName(int Index, UnicodeString _prop_val)
  {  FTerminalName = _prop_val; FTerminalName_Specified = true;  }
  bool __fastcall TerminalName_Specified(int Index)
  {  return FTerminalName_Specified;  } 

public:
  __fastcall ~TerminalModel();
__published:
  __property UnicodeString ComputerName = { index=(IS_OPTN|IS_NLBL), read=FComputerName, write=SetComputerName, stored = ComputerName_Specified };
  __property ArrayOfLicenceSettingModel LicenceSettingsModel = { index=(IS_OPTN|IS_NLBL), read=FLicenceSettingsModel, write=SetLicenceSettingsModel, stored = LicenceSettingsModel_Specified };
  __property UnicodeString  MacAdress = { index=(IS_OPTN|IS_NLBL), read=FMacAdress, write=SetMacAdress, stored = MacAdress_Specified };
  __property UnicodeString MenumateVersion = { index=(IS_OPTN|IS_NLBL), read=FMenumateVersion, write=SetMenumateVersion, stored = MenumateVersion_Specified };
  __property UnicodeString OperatingSystemName = { index=(IS_OPTN|IS_NLBL), read=FOperatingSystemName, write=SetOperatingSystemName, stored = OperatingSystemName_Specified };
  __property __int64      SiteCode = { index=(IS_OPTN), read=FSiteCode, write=SetSiteCode, stored = SiteCode_Specified };
  __property UnicodeString  StaffName = { index=(IS_OPTN|IS_NLBL), read=FStaffName, write=SetStaffName, stored = StaffName_Specified };
  __property UnicodeString SyndicateCode = { index=(IS_OPTN|IS_NLBL), read=FSyndicateCode, write=SetSyndicateCode, stored = SyndicateCode_Specified };
  __property UnicodeString TerminalDescription = { index=(IS_OPTN|IS_NLBL), read=FTerminalDescription, write=SetTerminalDescription, stored = TerminalDescription_Specified };
  __property UnicodeString TerminalName = { index=(IS_OPTN|IS_NLBL), read=FTerminalName, write=SetTerminalName, stored = TerminalName_Specified };
};




// ************************************************************************ //
// XML       : LicenceSettingModel, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class LicenceSettingModel : public TRemotable {
private:
  bool            FIsActive;
  bool            FIsActive_Specified;
  UnicodeString   FSettingSubType;
  bool            FSettingSubType_Specified;
  UnicodeString   FSettingType;
  bool            FSettingType_Specified;
  void __fastcall SetIsActive(int Index, bool _prop_val)
  {  FIsActive = _prop_val; FIsActive_Specified = true;  }
  bool __fastcall IsActive_Specified(int Index)
  {  return FIsActive_Specified;  } 
  void __fastcall SetSettingSubType(int Index, UnicodeString _prop_val)
  {  FSettingSubType = _prop_val; FSettingSubType_Specified = true;  }
  bool __fastcall SettingSubType_Specified(int Index)
  {  return FSettingSubType_Specified;  } 
  void __fastcall SetSettingType(int Index, UnicodeString _prop_val)
  {  FSettingType = _prop_val; FSettingType_Specified = true;  }
  bool __fastcall SettingType_Specified(int Index)
  {  return FSettingType_Specified;  } 
__published:
  __property bool         IsActive = { index=(IS_OPTN), read=FIsActive, write=SetIsActive, stored = IsActive_Specified };
  __property UnicodeString SettingSubType = { index=(IS_OPTN|IS_NLBL), read=FSettingSubType, write=SetSettingSubType, stored = SettingSubType_Specified };
  __property UnicodeString SettingType = { index=(IS_OPTN|IS_NLBL), read=FSettingType, write=SetSettingType, stored = SettingType_Specified };
};




// ************************************************************************ //
// XML       : RegistrationResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class RegistrationResponse : public MMServiceResponse {
private:
  RegistrationResponseCode FResponseCode;
  bool            FResponseCode_Specified;
  void __fastcall SetResponseCode(int Index, RegistrationResponseCode _prop_val)
  {  FResponseCode = _prop_val; FResponseCode_Specified = true;  }
  bool __fastcall ResponseCode_Specified(int Index)
  {  return FResponseCode_Specified;  } 
__published:
  __property RegistrationResponseCode ResponseCode = { index=(IS_OPTN), read=FResponseCode, write=SetResponseCode, stored = ResponseCode_Specified };
};




// ************************************************************************ //
// XML       : RegistrationWebResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class RegistrationWebResponse : public TRemotable {
private:
  bool            FIsSuccessful;
  bool            FIsSuccessful_Specified;
  UnicodeString   FResponseText;
  bool            FResponseText_Specified;
  void __fastcall SetIsSuccessful(int Index, bool _prop_val)
  {  FIsSuccessful = _prop_val; FIsSuccessful_Specified = true;  }
  bool __fastcall IsSuccessful_Specified(int Index)
  {  return FIsSuccessful_Specified;  } 
  void __fastcall SetResponseText(int Index, UnicodeString _prop_val)
  {  FResponseText = _prop_val; FResponseText_Specified = true;  }
  bool __fastcall ResponseText_Specified(int Index)
  {  return FResponseText_Specified;  } 
__published:
  __property bool       IsSuccessful = { index=(IS_OPTN), read=FIsSuccessful, write=SetIsSuccessful, stored = IsSuccessful_Specified };
  __property UnicodeString ResponseText = { index=(IS_OPTN|IS_NLBL), read=FResponseText, write=SetResponseText, stored = ResponseText_Specified };
};




// ************************************************************************ //
// XML       : TerminalModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class TerminalModel2 : public TerminalModel {
private:
__published:
};




// ************************************************************************ //
// XML       : LicenceSettingModel, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class LicenceSettingModel2 : public LicenceSettingModel {
private:
__published:
};




// ************************************************************************ //
// XML       : RegistrationResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class RegistrationResponse2 : public RegistrationResponse {
private:
__published:
};




// ************************************************************************ //
// XML       : RegistrationWebResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class RegistrationWebResponse2 : public RegistrationWebResponse {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IRegistrationIntegrationWebService/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceRegistration
// service   : RegistrationIntegrationWebService
// port      : basicHttpBinding_ServiceRegistration
// URL       : http://localhost:8749/MenumateServices/RegistrationService/
// ************************************************************************ //
__interface INTERFACE_UUID("{1FDFDF23-7F95-26E8-7096-7DC998DCEF05}") IRegistrationIntegrationWebService : public IInvokable
{
public:
  virtual RegistrationResponse* UpdateTerminalRegistrationInfo(const UnicodeString inSyndicateCode, const TerminalModel* terminalRegistrationInfo) = 0; 
  virtual RegistrationWebResponse* ValidateCompanyInfo(const UnicodeString inSyndicateCode, const int siteCode) = 0; 
};
typedef DelphiInterface<IRegistrationIntegrationWebService> _di_IRegistrationIntegrationWebService;

_di_IRegistrationIntegrationWebService GetIRegistrationIntegrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__RegistrationWSDL;
#endif



#endif // _H
