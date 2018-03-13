// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8744/MenumateServices/FiscalPrinter/?wsdl
//  >Import : http://localhost:8744/MenumateServices/FiscalPrinter/?wsdl>0
//  >Import : http://localhost:8744/MenumateServices/FiscalPrinter/?xsd=xsd0
//  >Import : http://localhost:8744/MenumateServices/FiscalPrinter/?xsd=xsd2
//  >Import : http://localhost:8744/MenumateServices/FiscalPrinter/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (8/03/2018 12:49:47 a.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "FiscalPrinterWSDL.h"
#endif



namespace NS__FIscalPrinterWSDL {

_di_IWCFServiceFiscalPrinter GetIWCFServiceFiscalPrinter(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8744/MenumateServices/FiscalPrinter/?wsdl";
  static const char* defURL = "http://localhost:8744/MenumateServices/FiscalPrinter/";
  static const char* defSvc = "WCFServiceFiscalPrinter";
  static const char* defPrt = "basicHttpBinding_ServiceFiscalPrinter";
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
  _di_IWCFServiceFiscalPrinter service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall FiscalDataDetails::~FiscalDataDetails()
{
  for(int i=0; i<FDiscountList.Length; i++)
    if (FDiscountList[i])
      delete FDiscountList[i];
  for(int i=0; i<FItemList.Length; i++)
    if (FItemList[i])
      delete FItemList[i];
  for(int i=0; i<FPaymentList.Length; i++)
    if (FPaymentList[i])
      delete FPaymentList[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IWCFServiceFiscalPrinter */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFServiceFiscalPrinter), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFServiceFiscalPrinter), L"http://tempuri.org/IWCFServiceFiscalPrinter/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFServiceFiscalPrinter), ioDocument);
  /* ArrayOfDiscountDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscountDetails), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"ArrayOfDiscountDetails");
  /* ArrayOfFiscalService */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfFiscalService), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"ArrayOfFiscalService");
  /* ArrayOfFiscalPayment */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfFiscalPayment), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"ArrayOfFiscalPayment");
  /* FiscalDataDetails */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalDataDetails), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"FiscalDataDetails");
  /* DiscountDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DiscountDetails), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"DiscountDetails");
  /* FiscalService */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalService), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"FiscalService");
  /* FiscalPayment */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalPayment), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"FiscalPayment");
  /* FiscalResponseDetails */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalResponseDetails), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"FiscalResponseDetails");
  /* FiscalDataDetails */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalDataDetails2), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"FiscalDataDetails2", L"FiscalDataDetails");
  /* DiscountDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DiscountDetails2), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"DiscountDetails2", L"DiscountDetails");
  /* FiscalService */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalService2), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"FiscalService2", L"FiscalService");
  /* FiscalPayment */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalPayment2), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"FiscalPayment2", L"FiscalPayment");
  /* FiscalResponseDetails */
  RemClassRegistry()->RegisterXSClass(__classid(FiscalResponseDetails2), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"FiscalResponseDetails2", L"FiscalResponseDetails");
  /* ArrayOfFiscalService */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfFiscalService), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"ArrayOfFiscalService");
  /* ArrayOfDiscountDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscountDetails), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"ArrayOfDiscountDetails");
  /* ArrayOfFiscalPayment */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfFiscalPayment), L"http://schemas.datacontract.org/2004/07/FiscalPrinterIntegration.Domain", L"ArrayOfFiscalPayment");
}
#pragma startup RegTypes 32

};     // NS__FIscalPrinterWSDL

