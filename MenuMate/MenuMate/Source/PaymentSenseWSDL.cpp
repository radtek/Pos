// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?wsdl
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?wsdl>0
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd0
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd2
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd3
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd4
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd5
//  >Import : http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (8/06/2018 11:09:31 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "PaymentSenseWSDL.h"
#endif



namespace NS__PaymentSenseWSDL {

_di_IWCFServicePaymentSense GetIWCFServicePaymentSense(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/?wsdl";
  static const char* defURL = "http://localhost:8746/MenumateServices.WCFServices/WCFServicePaymentSense/";
  static const char* defSvc = "WCFServicePaymentSense";
  static const char* defPrt = "basicHttpBinding_ServicePaymentSense";
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
  _di_IWCFServicePaymentSense service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall PACTerminalWrapper::~PACTerminalWrapper()
{
  for(int i=0; i<FTerminals.Length; i++)
    if (FTerminals[i])
      delete FTerminals[i];
}

__fastcall TransactionDataResponse::~TransactionDataResponse()
{
  delete FReceiptLines;
}

__fastcall ReceiptLines::~ReceiptLines()
{
  for(int i=0; i<FCustomer.Length; i++)
    if (FCustomer[i])
      delete FCustomer[i];
  for(int i=0; i<FMerchant.Length; i++)
    if (FMerchant[i])
      delete FMerchant[i];
}

__fastcall ReportResponseData::~ReportResponseData()
{
  for(int i=0; i<FreportLines.Length; i++)
    if (FreportLines[i])
      delete FreportLines[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IWCFServicePaymentSense */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFServicePaymentSense), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFServicePaymentSense), L"http://tempuri.org/IWCFServicePaymentSense/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFServicePaymentSense), ioDocument);
  /* ArrayOfstring */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfstring), L"http://schemas.microsoft.com/2003/10/Serialization/Arrays", L"ArrayOfstring");
  /* AuthorizationDetails */
  RemClassRegistry()->RegisterXSClass(__classid(AuthorizationDetails), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"AuthorizationDetails");
  /* ArrayOfPACTerminal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfPACTerminal), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"ArrayOfPACTerminal");
  /* PACTerminalWrapper */
  RemClassRegistry()->RegisterXSClass(__classid(PACTerminalWrapper), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"PACTerminalWrapper");
  /* PACTerminal */
  RemClassRegistry()->RegisterXSClass(__classid(PACTerminal), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"PACTerminal");
  /* TransactionRequest */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionRequest), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"TransactionRequest");
  /* PostRequestResponse */
  RemClassRegistry()->RegisterXSClass(__classid(PostRequestResponse), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"PostRequestResponse");
  /* Reports */
  RemClassRegistry()->RegisterXSClass(__classid(Reports), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"Reports");
  /* ArrayOfReceiptData */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfReceiptData), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"ArrayOfReceiptData");
  /* ReceiptData */
  RemClassRegistry()->RegisterXSClass(__classid(ReceiptData), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"ReceiptData");
  /* TransactionDataResponse */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionDataResponse), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"TransactionDataResponse");
  /* ReceiptLines */
  RemClassRegistry()->RegisterXSClass(__classid(ReceiptLines), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"ReceiptLines");
  /* AuthorizationDetails */
  RemClassRegistry()->RegisterXSClass(__classid(AuthorizationDetails2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"AuthorizationDetails2", L"AuthorizationDetails");
  /* PACTerminalWrapper */
  RemClassRegistry()->RegisterXSClass(__classid(PACTerminalWrapper2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"PACTerminalWrapper2", L"PACTerminalWrapper");
  /* PACTerminal */
  RemClassRegistry()->RegisterXSClass(__classid(PACTerminal2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"PACTerminal2", L"PACTerminal");
  /* TransactionRequest */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionRequest2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"TransactionRequest2", L"TransactionRequest");
  /* PostRequestResponse */
  RemClassRegistry()->RegisterXSClass(__classid(PostRequestResponse2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"PostRequestResponse2", L"PostRequestResponse");
  /* Reports */
  RemClassRegistry()->RegisterXSClass(__classid(Reports2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"Reports2", L"Reports");
  /* ReceiptData */
  RemClassRegistry()->RegisterXSClass(__classid(ReceiptData2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"ReceiptData2", L"ReceiptData");
  /* TransactionDataResponse */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionDataResponse2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"TransactionDataResponse2", L"TransactionDataResponse");
  /* ReceiptLines */
  RemClassRegistry()->RegisterXSClass(__classid(ReceiptLines2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"ReceiptLines2", L"ReceiptLines");
  /* ReportResponseData */
  RemClassRegistry()->RegisterXSClass(__classid(ReportResponseData), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain.ResponseData", L"ReportResponseData");
  /* ReportResponseData */
  RemClassRegistry()->RegisterXSClass(__classid(ReportResponseData2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain.ResponseData", L"ReportResponseData2", L"ReportResponseData");
  /* SignatureRequest */
  RemClassRegistry()->RegisterXSClass(__classid(SignatureRequest), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain.SignatureRequest", L"SignatureRequest");
  /* SignatureRequest */
  RemClassRegistry()->RegisterXSClass(__classid(SignatureRequest2), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain.SignatureRequest", L"SignatureRequest2", L"SignatureRequest");
  /* ArrayOfPACTerminal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfPACTerminal), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"ArrayOfPACTerminal");
  /* ArrayOfstring */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfstring), L"http://schemas.microsoft.com/2003/10/Serialization/Arrays", L"ArrayOfstring");
  /* ArrayOfReceiptData */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfReceiptData), L"http://schemas.datacontract.org/2004/07/PaymentSenseIntegration.Domain", L"ArrayOfReceiptData");
}
#pragma startup RegTypes 32

};     // NS__PaymentSenseWSDL
