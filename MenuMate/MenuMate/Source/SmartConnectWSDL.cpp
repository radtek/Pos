// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8744/MenumateServices/SmartConnect/?wsdl
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?wsdl>0
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?xsd=xsd0
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?xsd=xsd2
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?xsd=xsd3
//  >Import : http://localhost:8744/MenumateServices/SmartConnect/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (12/10/2017 9:42:58 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "SmartConnectWSDL.h"
#endif



namespace NS__SmartConnnectWSDL {

_di_IWCFSmartConnect GetIWCFSmartConnect(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8744/MenumateServices/SmartConnect/?wsdl";
  static const char* defURL = "http://localhost:8744/MenumateServices/SmartConnect/";
  static const char* defSvc = "WCFServiceSmartConnect";
  static const char* defPrt = "basicHttpBinding_ServiceSmartConnect";
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
  _di_IWCFSmartConnect service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall DateTimeOffset::~DateTimeOffset()
{
  delete FDateTime;
}

__fastcall SmartConnectResponse::~SmartConnectResponse()
{
  delete FSmartConnectData;
  delete FTransactionTime;
}

__fastcall SmartConnectDataObject::~SmartConnectDataObject()
{
  delete FTimestamp;
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IWCFSmartConnect */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFSmartConnect), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFSmartConnect), L"http://tempuri.org/IWCFSmartConnect/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFSmartConnect), ioDocument);
  /* DateTimeOffset */
  RemClassRegistry()->RegisterXSClass(__classid(DateTimeOffset), L"http://schemas.datacontract.org/2004/07/System", L"DateTimeOffset");
  /* DateTimeOffset */
  RemClassRegistry()->RegisterXSClass(__classid(DateTimeOffset2), L"http://schemas.datacontract.org/2004/07/System", L"DateTimeOffset2", L"DateTimeOffset");
  /* POSDetails */
  RemClassRegistry()->RegisterXSClass(__classid(POSDetails), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"POSDetails");
  /* PairingTerminal */
  RemClassRegistry()->RegisterXSClass(__classid(PairingTerminal), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"PairingTerminal");
  /* SmartConnectResponse */
  RemClassRegistry()->RegisterXSClass(__classid(SmartConnectResponse), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"SmartConnectResponse");
  /* SmartConnectDataObject */
  RemClassRegistry()->RegisterXSClass(__classid(SmartConnectDataObject), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"SmartConnectDataObject");
  /* TransactionTypes */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionTypes), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"TransactionTypes");
  /* PairingTerminal */
  RemClassRegistry()->RegisterXSClass(__classid(PairingTerminal2), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"PairingTerminal2", L"PairingTerminal");
  /* POSDetails */
  RemClassRegistry()->RegisterXSClass(__classid(POSDetails2), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"POSDetails2", L"POSDetails");
  /* SmartConnectResponse */
  RemClassRegistry()->RegisterXSClass(__classid(SmartConnectResponse2), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"SmartConnectResponse2", L"SmartConnectResponse");
  /* SmartConnectDataObject */
  RemClassRegistry()->RegisterXSClass(__classid(SmartConnectDataObject2), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"SmartConnectDataObject2", L"SmartConnectDataObject");
  /* TransactionTypes */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionTypes2), L"http://schemas.datacontract.org/2004/07/SmartConnectIntegration.Domain", L"TransactionTypes2", L"TransactionTypes");
}
#pragma startup RegTypes 32

};     // NS__

