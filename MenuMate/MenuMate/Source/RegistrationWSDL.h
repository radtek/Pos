// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8749/MenumateServices/RegistrationService/?wsdl
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?wsdl>0
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd0
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd2
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd3
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd4
//  >Import : http://localhost:8749/MenumateServices/RegistrationService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (21/01/2019 10:04:03 p.m. - - $Rev: 25127 $)
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
class SOAP_REMOTABLE_CLASS Terminal;
class SOAP_REMOTABLE_CLASS LicenceSettingMapping;
class SOAP_REMOTABLE_CLASS LicenceSetting;
class SOAP_REMOTABLE_CLASS Site;
class SOAP_REMOTABLE_CLASS RegistrationResponse;
class SOAP_REMOTABLE_CLASS Terminal2;
class SOAP_REMOTABLE_CLASS LicenceSettingMapping2;
class SOAP_REMOTABLE_CLASS LicenceSetting2;
class SOAP_REMOTABLE_CLASS Site2;
class SOAP_REMOTABLE_CLASS RegistrationResponse2;

enum class LicenceType   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration.Enums"[GblSmpl] */
{
  Eftpos, 
  Loyalty, 
  Accounting, 
  TimeTracking, 
  Chefmate, 
  PropertyManagement, 
  Rooms, 
  FloorPlan, 
  PosCashier, 
  PosOrder, 
  PosHandHeld, 
  Fiscal, 
  WebMate, 
  PocketVoucher, 
  BarExchange, 
  RunRateBoard, 
  OnlineOrdering
};

class LicenceType_TypeInfoHolder : public TObject {
  LicenceType __instanceType;
public:
__published:
  __property LicenceType __propType = { read=__instanceType };
};

enum class RegistrationResponseCode   /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration"[GblSmpl] */
{
  Successful, 
  AuthenticationFailed, 
  CreateMemberFailed, 
  UpdateMemberFailed, 
  DeleteMemberFailed, 
  GetMemberFailed, 
  PostTransactionFailed, 
  MemberNotExist, 
  CompanySyncFailed, 
  InvalidGiftVoucher, 
  InvalidPocketVoucher, 
  TransactionFailed, 
  GetGiftCardFailed, 
  GetPocketVoucherFailed, 
  MenuSyncingFailed, 
  TaxSettingSyncingFailed, 
  UpdateOnlineOrderStatusFailed, 
  PostOnlineOrderInvoiceInfoFailed, 
  MultipleGUIDExist, 
  GUIDNotFound
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


typedef DynamicArray<LicenceSettingMapping*> ArrayOfLicenceSettingMapping; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration"[GblCplx] */


// ************************************************************************ //
// XML       : Terminal, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class Terminal : public TRemotable {
private:
  UnicodeString   FComputerName;
  bool            FComputerName_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  ArrayOfLicenceSettingMapping FLicenceSettingMappings;
  bool            FLicenceSettingMappings_Specified;
  UnicodeString   FMacAdress;
  bool            FMacAdress_Specified;
  UnicodeString   FMenumateVersion;
  bool            FMenumateVersion_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  UnicodeString   FOperatingSystemName;
  bool            FOperatingSystemName_Specified;
  Site*           FSite;
  bool            FSite_Specified;
  __int64         FSiteId;
  bool            FSiteId_Specified;
  UnicodeString   FStaffName;
  bool            FStaffName_Specified;
  __int64         FTerminalProfileId;
  bool            FTerminalProfileId_Specified;
  void __fastcall SetComputerName(int Index, UnicodeString _prop_val)
  {  FComputerName = _prop_val; FComputerName_Specified = true;  }
  bool __fastcall ComputerName_Specified(int Index)
  {  return FComputerName_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetLicenceSettingMappings(int Index, ArrayOfLicenceSettingMapping _prop_val)
  {  FLicenceSettingMappings = _prop_val; FLicenceSettingMappings_Specified = true;  }
  bool __fastcall LicenceSettingMappings_Specified(int Index)
  {  return FLicenceSettingMappings_Specified;  } 
  void __fastcall SetMacAdress(int Index, UnicodeString _prop_val)
  {  FMacAdress = _prop_val; FMacAdress_Specified = true;  }
  bool __fastcall MacAdress_Specified(int Index)
  {  return FMacAdress_Specified;  } 
  void __fastcall SetMenumateVersion(int Index, UnicodeString _prop_val)
  {  FMenumateVersion = _prop_val; FMenumateVersion_Specified = true;  }
  bool __fastcall MenumateVersion_Specified(int Index)
  {  return FMenumateVersion_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetOperatingSystemName(int Index, UnicodeString _prop_val)
  {  FOperatingSystemName = _prop_val; FOperatingSystemName_Specified = true;  }
  bool __fastcall OperatingSystemName_Specified(int Index)
  {  return FOperatingSystemName_Specified;  } 
  void __fastcall SetSite(int Index, Site* _prop_val)
  {  FSite = _prop_val; FSite_Specified = true;  }
  bool __fastcall Site_Specified(int Index)
  {  return FSite_Specified;  } 
  void __fastcall SetSiteId(int Index, __int64 _prop_val)
  {  FSiteId = _prop_val; FSiteId_Specified = true;  }
  bool __fastcall SiteId_Specified(int Index)
  {  return FSiteId_Specified;  } 
  void __fastcall SetStaffName(int Index, UnicodeString _prop_val)
  {  FStaffName = _prop_val; FStaffName_Specified = true;  }
  bool __fastcall StaffName_Specified(int Index)
  {  return FStaffName_Specified;  } 
  void __fastcall SetTerminalProfileId(int Index, __int64 _prop_val)
  {  FTerminalProfileId = _prop_val; FTerminalProfileId_Specified = true;  }
  bool __fastcall TerminalProfileId_Specified(int Index)
  {  return FTerminalProfileId_Specified;  } 

public:
  __fastcall ~Terminal();
__published:
  __property UnicodeString ComputerName = { index=(IS_OPTN|IS_NLBL), read=FComputerName, write=SetComputerName, stored = ComputerName_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property ArrayOfLicenceSettingMapping LicenceSettingMappings = { index=(IS_OPTN|IS_NLBL), read=FLicenceSettingMappings, write=SetLicenceSettingMappings, stored = LicenceSettingMappings_Specified };
  __property UnicodeString  MacAdress = { index=(IS_OPTN|IS_NLBL), read=FMacAdress, write=SetMacAdress, stored = MacAdress_Specified };
  __property UnicodeString MenumateVersion = { index=(IS_OPTN|IS_NLBL), read=FMenumateVersion, write=SetMenumateVersion, stored = MenumateVersion_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property UnicodeString OperatingSystemName = { index=(IS_OPTN|IS_NLBL), read=FOperatingSystemName, write=SetOperatingSystemName, stored = OperatingSystemName_Specified };
  __property Site*            Site = { index=(IS_OPTN|IS_NLBL), read=FSite, write=SetSite, stored = Site_Specified };
  __property __int64        SiteId = { index=(IS_OPTN), read=FSiteId, write=SetSiteId, stored = SiteId_Specified };
  __property UnicodeString  StaffName = { index=(IS_OPTN|IS_NLBL), read=FStaffName, write=SetStaffName, stored = StaffName_Specified };
  __property __int64    TerminalProfileId = { index=(IS_OPTN), read=FTerminalProfileId, write=SetTerminalProfileId, stored = TerminalProfileId_Specified };
};




// ************************************************************************ //
// XML       : LicenceSettingMapping, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class LicenceSettingMapping : public TRemotable {
private:
  bool            FIsEnabled;
  bool            FIsEnabled_Specified;
  __int64         FLicenceSettingId;
  bool            FLicenceSettingId_Specified;
  LicenceSetting* FLicenceSettingSetting;
  bool            FLicenceSettingSetting_Specified;
  Terminal*       FTerminal;
  bool            FTerminal_Specified;
  __int64         FTerminalId;
  bool            FTerminalId_Specified;
  UnicodeString   FText;
  bool            FText_Specified;
  UnicodeString   FValue;
  bool            FValue_Specified;
  void __fastcall SetIsEnabled(int Index, bool _prop_val)
  {  FIsEnabled = _prop_val; FIsEnabled_Specified = true;  }
  bool __fastcall IsEnabled_Specified(int Index)
  {  return FIsEnabled_Specified;  } 
  void __fastcall SetLicenceSettingId(int Index, __int64 _prop_val)
  {  FLicenceSettingId = _prop_val; FLicenceSettingId_Specified = true;  }
  bool __fastcall LicenceSettingId_Specified(int Index)
  {  return FLicenceSettingId_Specified;  } 
  void __fastcall SetLicenceSettingSetting(int Index, LicenceSetting* _prop_val)
  {  FLicenceSettingSetting = _prop_val; FLicenceSettingSetting_Specified = true;  }
  bool __fastcall LicenceSettingSetting_Specified(int Index)
  {  return FLicenceSettingSetting_Specified;  } 
  void __fastcall SetTerminal(int Index, Terminal* _prop_val)
  {  FTerminal = _prop_val; FTerminal_Specified = true;  }
  bool __fastcall Terminal_Specified(int Index)
  {  return FTerminal_Specified;  } 
  void __fastcall SetTerminalId(int Index, __int64 _prop_val)
  {  FTerminalId = _prop_val; FTerminalId_Specified = true;  }
  bool __fastcall TerminalId_Specified(int Index)
  {  return FTerminalId_Specified;  } 
  void __fastcall SetText(int Index, UnicodeString _prop_val)
  {  FText = _prop_val; FText_Specified = true;  }
  bool __fastcall Text_Specified(int Index)
  {  return FText_Specified;  } 
  void __fastcall SetValue(int Index, UnicodeString _prop_val)
  {  FValue = _prop_val; FValue_Specified = true;  }
  bool __fastcall Value_Specified(int Index)
  {  return FValue_Specified;  } 

public:
  __fastcall ~LicenceSettingMapping();
__published:
  __property bool        IsEnabled = { index=(IS_OPTN), read=FIsEnabled, write=SetIsEnabled, stored = IsEnabled_Specified };
  __property __int64    LicenceSettingId = { index=(IS_OPTN), read=FLicenceSettingId, write=SetLicenceSettingId, stored = LicenceSettingId_Specified };
  __property LicenceSetting* LicenceSettingSetting = { index=(IS_OPTN|IS_NLBL), read=FLicenceSettingSetting, write=SetLicenceSettingSetting, stored = LicenceSettingSetting_Specified };
  __property Terminal*    Terminal = { index=(IS_OPTN|IS_NLBL), read=FTerminal, write=SetTerminal, stored = Terminal_Specified };
  __property __int64    TerminalId = { index=(IS_OPTN), read=FTerminalId, write=SetTerminalId, stored = TerminalId_Specified };
  __property UnicodeString       Text = { index=(IS_OPTN|IS_NLBL), read=FText, write=SetText, stored = Text_Specified };
  __property UnicodeString      Value = { index=(IS_OPTN|IS_NLBL), read=FValue, write=SetValue, stored = Value_Specified };
};




// ************************************************************************ //
// XML       : LicenceSetting, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class LicenceSetting : public TRemotable {
private:
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  bool            FIsEnabledByDefault;
  bool            FIsEnabledByDefault_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  int             FSettingSubType;
  bool            FSettingSubType_Specified;
  LicenceType     FSettingType;
  bool            FSettingType_Specified;
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetIsEnabledByDefault(int Index, bool _prop_val)
  {  FIsEnabledByDefault = _prop_val; FIsEnabledByDefault_Specified = true;  }
  bool __fastcall IsEnabledByDefault_Specified(int Index)
  {  return FIsEnabledByDefault_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetSettingSubType(int Index, int _prop_val)
  {  FSettingSubType = _prop_val; FSettingSubType_Specified = true;  }
  bool __fastcall SettingSubType_Specified(int Index)
  {  return FSettingSubType_Specified;  } 
  void __fastcall SetSettingType(int Index, LicenceType _prop_val)
  {  FSettingType = _prop_val; FSettingType_Specified = true;  }
  bool __fastcall SettingType_Specified(int Index)
  {  return FSettingType_Specified;  } 
__published:
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property bool       IsEnabledByDefault = { index=(IS_OPTN), read=FIsEnabledByDefault, write=SetIsEnabledByDefault, stored = IsEnabledByDefault_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property int        SettingSubType = { index=(IS_OPTN), read=FSettingSubType, write=SetSettingSubType, stored = SettingSubType_Specified };
  __property LicenceType SettingType = { index=(IS_OPTN), read=FSettingType, write=SetSettingType, stored = SettingType_Specified };
};


typedef DynamicArray<Terminal*>   ArrayOfTerminal; /* "http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration"[GblCplx] */


// ************************************************************************ //
// XML       : Site, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class Site : public TRemotable {
private:
  __int64         FCompanyId;
  bool            FCompanyId_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  __int64         FSiteCode;
  bool            FSiteCode_Specified;
  __int64         FSiteProfileId;
  bool            FSiteProfileId_Specified;
  ArrayOfTerminal FTerminals;
  bool            FTerminals_Specified;
  void __fastcall SetCompanyId(int Index, __int64 _prop_val)
  {  FCompanyId = _prop_val; FCompanyId_Specified = true;  }
  bool __fastcall CompanyId_Specified(int Index)
  {  return FCompanyId_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  void __fastcall SetSiteCode(int Index, __int64 _prop_val)
  {  FSiteCode = _prop_val; FSiteCode_Specified = true;  }
  bool __fastcall SiteCode_Specified(int Index)
  {  return FSiteCode_Specified;  } 
  void __fastcall SetSiteProfileId(int Index, __int64 _prop_val)
  {  FSiteProfileId = _prop_val; FSiteProfileId_Specified = true;  }
  bool __fastcall SiteProfileId_Specified(int Index)
  {  return FSiteProfileId_Specified;  } 
  void __fastcall SetTerminals(int Index, ArrayOfTerminal _prop_val)
  {  FTerminals = _prop_val; FTerminals_Specified = true;  }
  bool __fastcall Terminals_Specified(int Index)
  {  return FTerminals_Specified;  } 

public:
  __fastcall ~Site();
__published:
  __property __int64     CompanyId = { index=(IS_OPTN), read=FCompanyId, write=SetCompanyId, stored = CompanyId_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property __int64      SiteCode = { index=(IS_OPTN), read=FSiteCode, write=SetSiteCode, stored = SiteCode_Specified };
  __property __int64    SiteProfileId = { index=(IS_OPTN|IS_NLBL), read=FSiteProfileId, write=SetSiteProfileId, stored = SiteProfileId_Specified };
  __property ArrayOfTerminal  Terminals = { index=(IS_OPTN|IS_NLBL), read=FTerminals, write=SetTerminals, stored = Terminals_Specified };
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
// XML       : Terminal, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class Terminal2 : public Terminal {
private:
__published:
};




// ************************************************************************ //
// XML       : LicenceSettingMapping, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class LicenceSettingMapping2 : public LicenceSettingMapping {
private:
__published:
};




// ************************************************************************ //
// XML       : LicenceSetting, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class LicenceSetting2 : public LicenceSetting {
private:
__published:
};




// ************************************************************************ //
// XML       : Site, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration
// ************************************************************************ //
class Site2 : public Site {
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
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IRegistrationIntegrationWebService/UpdateTerminalRegistrationInfo
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
  virtual RegistrationResponse* UpdateTerminalRegistrationInfo(const UnicodeString inSyndicateCode, const Terminal* terminalRegistrationInfo) = 0; 
};
typedef DelphiInterface<IRegistrationIntegrationWebService> _di_IRegistrationIntegrationWebService;

_di_IRegistrationIntegrationWebService GetIRegistrationIntegrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__RegistrationWSDL

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__RegistrationWSDL;
#endif



#endif // _H
