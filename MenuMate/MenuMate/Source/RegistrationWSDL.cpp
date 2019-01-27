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
// (27/01/2019 7:33:38 p.m. - - $Rev: 25127 $)
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


__fastcall TerminalModel::~TerminalModel()
{
  for(int i=0; i<FLicenceSettingsModel.Length; i++)
    if (FLicenceSettingsModel[i])
      delete FLicenceSettingsModel[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IRegistrationIntegrationWebService */
  InvRegistry()->RegisterInterface(__delphirtti(IRegistrationIntegrationWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IRegistrationIntegrationWebService), L"http://tempuri.org/IRegistrationIntegrationWebService/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IRegistrationIntegrationWebService), ioDocument);
  /* MMServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(MMServiceResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO", L"MMServiceResponse");
  /* MMServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(MMServiceResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO", L"MMServiceResponse2", L"MMServiceResponse");
  /* ArrayOfLicenceSettingModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfLicenceSettingModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"ArrayOfLicenceSettingModel");
  /* TerminalModel */
  RemClassRegistry()->RegisterXSClass(__classid(TerminalModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"TerminalModel");
  /* LicenceSettingModel */
  RemClassRegistry()->RegisterXSClass(__classid(LicenceSettingModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"LicenceSettingModel");
  /* RegistrationResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(RegistrationResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"RegistrationResponseCode");
  /* RegistrationResponse */
  RemClassRegistry()->RegisterXSClass(__classid(RegistrationResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"RegistrationResponse");
  /* CompanySiteModelResponse */
  RemClassRegistry()->RegisterXSClass(__classid(CompanySiteModelResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"CompanySiteModelResponse");
  /* TerminalModel */
  RemClassRegistry()->RegisterXSClass(__classid(TerminalModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"TerminalModel2", L"TerminalModel");
  /* LicenceSettingModel */
  RemClassRegistry()->RegisterXSClass(__classid(LicenceSettingModel2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"LicenceSettingModel2", L"LicenceSettingModel");
  /* RegistrationResponse */
  RemClassRegistry()->RegisterXSClass(__classid(RegistrationResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"RegistrationResponse2", L"RegistrationResponse");
  /* CompanySiteModelResponse */
  RemClassRegistry()->RegisterXSClass(__classid(CompanySiteModelResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"CompanySiteModelResponse2", L"CompanySiteModelResponse");
  /* ArrayOfLicenceSettingModel */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfLicenceSettingModel), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"ArrayOfLicenceSettingModel");
  /* RegistrationResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(RegistrationResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.MenumateRegistration", L"RegistrationResponseCode");
}
#pragma startup RegTypes 32

};     // NS__

