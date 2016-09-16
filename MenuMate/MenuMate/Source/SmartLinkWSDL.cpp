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

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "SmartLinkWSDL.h"
#endif



namespace NS__SmartLinkWSDL {

_di_IWCFServiceSmartLink GetIWCFServiceSmartLink(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8738/MenumateServices/SmartLink/?wsdl";
  static const char* defURL = "http://localhost:8738/MenumateServices/SmartLink/";
  static const char* defSvc = "WCFServiceSmartLink";
  static const char* defPrt = "basicHttpBinding_ServiceSmartLink";
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
  _di_IWCFServiceSmartLink service;
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
  /* IWCFServiceSmartLink */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFServiceSmartLink), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFServiceSmartLink), L"http://tempuri.org/IWCFServiceSmartLink/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFServiceSmartLink), ioDocument);
  /* DTO_SmartLinkTransactionResponse */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_SmartLinkTransactionResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.SmartLink", L"DTO_SmartLinkTransactionResponse");
  /* DTO_SmartLinkTransactionResponse */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_SmartLinkTransactionResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.SmartLink", L"DTO_SmartLinkTransactionResponse2", L"DTO_SmartLinkTransactionResponse");
}
#pragma startup RegTypes 32

};     // NS__

