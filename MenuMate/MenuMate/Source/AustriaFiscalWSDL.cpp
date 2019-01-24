// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8748/MenumateServices/AustriaService/?wsdl
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?wsdl>0
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?xsd=xsd0
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?xsd=xsd2
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?xsd=xsd3
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (4/01/2019 6:29:05 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "AustriaFiscalWSDL.h"
#endif



namespace NS__AustriaFiscalWSDL {

_di_IAustriaFiscalIntegrationWebService GetIAustriaFiscalIntegrationWebService(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8748/MenumateServices/AustriaService/?wsdl";
  static const char* defURL = "http://localhost:8748/MenumateServices/AustriaService/";
  static const char* defSvc = "AustriaFiscalIntegrationWebService";
  static const char* defPrt = "basicHttpBinding_ServiceAustria";
  if (addr=="")
    addr = useWSDL ? defWSDL : defURL;
  THTTPRIO* rio = HTTPRIO ? HTTPRIO : new THTTPRIO(0);
  rio->HTTPWebNode->SendTimeout = 200000;
  rio->HTTPWebNode->ReceiveTimeout = 200000;
  if (useWSDL) {
    rio->WSDLLocation = addr;
    rio->Service = defSvc;
    rio->Port = defPrt;
  } else {
    rio->URL = addr;
  }
  _di_IAustriaFiscalIntegrationWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall ReceiptRequestLocal::~ReceiptRequestLocal()
{
  delete FReceiptMoment;
  for(int i=0; i<FChargeItems.Length; i++)
    if (FChargeItems[i])
      delete FChargeItems[i];
  for(int i=0; i<FPayItems.Length; i++)
    if (FPayItems[i])
      delete FPayItems[i];
}

__fastcall ChargeItemLocal::~ChargeItemLocal()
{
  delete FMoment;
}

__fastcall PayItemLocal::~PayItemLocal()
{
  delete FMoment;
}

__fastcall ReceiptReponseLocal::~ReceiptReponseLocal()
{
  delete FReceiptMoment;
  for(int i=0; i<FChargeItems.Length; i++)
    if (FChargeItems[i])
      delete FChargeItems[i];
  for(int i=0; i<FPayItems.Length; i++)
    if (FPayItems[i])
      delete FPayItems[i];
  for(int i=0; i<FSignatures.Length; i++)
    if (FSignatures[i])
      delete FSignatures[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IAustriaFiscalIntegrationWebService */
  InvRegistry()->RegisterInterface(__delphirtti(IAustriaFiscalIntegrationWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IAustriaFiscalIntegrationWebService), L"http://tempuri.org/IAustriaFiscalIntegrationWebService/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IAustriaFiscalIntegrationWebService), ioDocument);
  /* ArrayOfstring */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfstring), L"http://schemas.microsoft.com/2003/10/Serialization/Arrays", L"ArrayOfstring");
  /* ArrayOfChargeItemLocal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfChargeItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ArrayOfChargeItemLocal");
  /* ArrayOfPayItemLocal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfPayItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ArrayOfPayItemLocal");
  /* ReceiptRequestLocal */
  RemClassRegistry()->RegisterXSClass(__classid(ReceiptRequestLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ReceiptRequestLocal");
  /* ChargeItemLocal */
  RemClassRegistry()->RegisterXSClass(__classid(ChargeItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ChargeItemLocal");
  /* PayItemLocal */
  RemClassRegistry()->RegisterXSClass(__classid(PayItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"PayItemLocal");
  /* ArrayOfSignaturItemLocal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSignaturItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ArrayOfSignaturItemLocal");
  /* ReceiptReponseLocal */
  RemClassRegistry()->RegisterXSClass(__classid(ReceiptReponseLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ReceiptReponseLocal");
  /* SignaturItemLocal */
  RemClassRegistry()->RegisterXSClass(__classid(SignaturItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"SignaturItemLocal");
  /* ReceiptRequestLocal */
  RemClassRegistry()->RegisterXSClass(__classid(ReceiptRequestLocal2), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ReceiptRequestLocal2", L"ReceiptRequestLocal");
  /* ChargeItemLocal */
  RemClassRegistry()->RegisterXSClass(__classid(ChargeItemLocal2), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ChargeItemLocal2", L"ChargeItemLocal");
  /* PayItemLocal */
  RemClassRegistry()->RegisterXSClass(__classid(PayItemLocal2), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"PayItemLocal2", L"PayItemLocal");
  /* ReceiptReponseLocal */
  RemClassRegistry()->RegisterXSClass(__classid(ReceiptReponseLocal2), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ReceiptReponseLocal2", L"ReceiptReponseLocal");
  /* SignaturItemLocal */
  RemClassRegistry()->RegisterXSClass(__classid(SignaturItemLocal2), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"SignaturItemLocal2", L"SignaturItemLocal");
  /* ArrayOfstring */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfstring), L"http://schemas.microsoft.com/2003/10/Serialization/Arrays", L"ArrayOfstring");
  /* ArrayOfPayItemLocal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfPayItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ArrayOfPayItemLocal");
  /* ArrayOfChargeItemLocal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfChargeItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ArrayOfChargeItemLocal");
  /* ArrayOfSignaturItemLocal */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSignaturItemLocal), L"http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain", L"ArrayOfSignaturItemLocal");
}
#pragma startup RegTypes 32

};     // NS__

