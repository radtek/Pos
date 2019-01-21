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

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "RegistrationWSDL.h"
#endif



namespace NS__RegistrationWSDL {

_di_IRegistrationIntegrationWebService GetIRegistrationIntegrationWebService(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8749/MenumateServices/RegistrationService/?wsdl";
  static const char* defURL = "http://localhost:8749/MenumateServices/RegistrationService/";
  static const char* defSvc = "RegistrationIntegrationWebService";
  static const char* defPrt = "basicHttpBinding_ServiceRegistration";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  THTTPRIO* rio = HTTPRIO ? HTTPRIO : new THTTPRIO(0);
  rio->HTTPWebNode->SendTimeout = 120000;
  rio->HTTPWebNode->ReceiveTimeout = 120000;
  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_IRegistrationIntegrationWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall Terminal::~Terminal()
{
  delete FSite;
  for(int i=0; i<FLicenceSettingMappings.Length; i++)
    if (FLicenceSettingMappings[i])
      delete FLicenceSettingMappings[i];
}

__fastcall LicenceSettingMapping::~LicenceSettingMapping()
{
  delete FLicenceSettingSetting;
  delete FTerminal;
}

__fastcall Site::~Site()
{
  for(int i=0; i<FTerminals.Length; i++)
    if (FTerminals[i])
      delete FTerminals[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IRegistrationIntegrationWebService */
  InvRegistry()->RegisterInterface(__delphirtti(IRegistrationIntegrationWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IRegistrationIntegrationWebService), L"http://tempuri.org/IRegistrationIntegrationWebService/UpdateTerminalRegistrationInfo");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IRegistrationIntegrationWebService), ioDocument);
  /* LicenceType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(LicenceType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration.Enums", L"LicenceType");
  /* MMServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(MMServiceResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO", L"MMServiceResponse");
  /* MMServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(MMServiceResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO", L"MMServiceResponse2", L"MMServiceResponse");
  /* ArrayOfLicenceSettingMapping */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfLicenceSettingMapping), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"ArrayOfLicenceSettingMapping");
  /* Terminal */
  RemClassRegistry()->RegisterXSClass(__classid(Terminal), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"Terminal");
  /* LicenceSettingMapping */
  RemClassRegistry()->RegisterXSClass(__classid(LicenceSettingMapping), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"LicenceSettingMapping");
  /* LicenceSetting */
  RemClassRegistry()->RegisterXSClass(__classid(LicenceSetting), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"LicenceSetting");
  /* ArrayOfTerminal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTerminal), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"ArrayOfTerminal");
  /* Site */
  RemClassRegistry()->RegisterXSClass(__classid(Site), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"Site");
  /* RegistrationResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(RegistrationResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"RegistrationResponseCode");
  /* RegistrationResponse */
  RemClassRegistry()->RegisterXSClass(__classid(RegistrationResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"RegistrationResponse");
  /* Terminal */
  RemClassRegistry()->RegisterXSClass(__classid(Terminal2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"Terminal2", L"Terminal");
  /* LicenceSettingMapping */
  RemClassRegistry()->RegisterXSClass(__classid(LicenceSettingMapping2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"LicenceSettingMapping2", L"LicenceSettingMapping");
  /* LicenceSetting */
  RemClassRegistry()->RegisterXSClass(__classid(LicenceSetting2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"LicenceSetting2", L"LicenceSetting");
  /* Site */
  RemClassRegistry()->RegisterXSClass(__classid(Site2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"Site2", L"Site");
  /* RegistrationResponse */
  RemClassRegistry()->RegisterXSClass(__classid(RegistrationResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"RegistrationResponse2", L"RegistrationResponse");
  /* LicenceType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(LicenceType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration.Enums", L"LicenceType");
  /* ArrayOfTerminal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTerminal), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"ArrayOfTerminal");
  /* RegistrationResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(RegistrationResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"RegistrationResponseCode");
  /* ArrayOfLicenceSettingMapping */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfLicenceSettingMapping), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"ArrayOfLicenceSettingMapping");
}
#pragma startup RegTypes 32

};     // NS__RegistrationWSDL

