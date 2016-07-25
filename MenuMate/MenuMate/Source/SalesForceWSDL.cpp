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

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "SalesForceWSDL.h"
#endif



namespace NS__SalesForceWSDL {

_di_ISalesForceIntegrationWebService GetISalesForceIntegrationWebService(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8741/MenumateServices/SalesForceService/?wsdl";
  static const char* defURL = "http://localhost:8741/MenumateServices/SalesForceService/";
  static const char* defSvc = "SalesForceIntegrationWebService";
  static const char* defPrt = "basicHttpBinding_ServiceSalesForce";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  THTTPRIO* rio = HTTPRIO ? HTTPRIO : new THTTPRIO(0);
  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_ISalesForceIntegrationWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* ISalesForceIntegrationWebService */
  InvRegistry()->RegisterInterface(__delphirtti(ISalesForceIntegrationWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(ISalesForceIntegrationWebService), L"http://tempuri.org/ISalesForceIntegrationWebService/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(ISalesForceIntegrationWebService), ioDocument);
  /* SalesForceModuleStatus */
  RemClassRegistry()->RegisterXSClass(__classid(SalesForceModuleStatus), L"http://schemas.datacontract.org/2004/07/SalesForceCommunication.Domain", L"SalesForceModuleStatus");
  /* SalesForceModuleStatus */
  RemClassRegistry()->RegisterXSClass(__classid(SalesForceModuleStatus2), L"http://schemas.datacontract.org/2004/07/SalesForceCommunication.Domain", L"SalesForceModuleStatus2", L"SalesForceModuleStatus");
}
#pragma startup RegTypes 32

};     // NS__

