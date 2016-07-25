// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8740/MenumateServices/PocketVoucherService/?wsdl
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?wsdl>0
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?xsd=xsd0
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?xsd=xsd2
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?xsd=xsd3
//  >Import : http://localhost:8740/MenumateServices/PocketVoucherService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (18/05/2016 11:36:18 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "PocketVoucherWSDL.h"
#endif



namespace NS__PocketVoucherWSDL {

_di_IPocketVoucherWebService GetIPocketVoucherWebService(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8740/MenumateServices/PocketVoucherService/?wsdl";
  static const char* defURL = "http://localhost:8740/MenumateServices/PocketVoucherService/";
  static const char* defSvc = "PocketVoucherWebService";
  static const char* defPrt = "basicHttpBinding_ServicePocketVoucher";
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
  _di_IPocketVoucherWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall PocketVoucherDetail::~PocketVoucherDetail()
{
  delete FDateUpdated;
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IPocketVoucherWebService */
  InvRegistry()->RegisterInterface(__delphirtti(IPocketVoucherWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IPocketVoucherWebService), L"http://tempuri.org/IPocketVoucherWebService/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IPocketVoucherWebService), ioDocument);
  /* PocketVoucherDetail */
  RemClassRegistry()->RegisterXSClass(__classid(PocketVoucherDetail), L"http://schemas.datacontract.org/2004/07/SalesForceCommunication.Domain", L"PocketVoucherDetail");
  /* PocketVoucherDetail */
  RemClassRegistry()->RegisterXSClass(__classid(PocketVoucherDetail2), L"http://schemas.datacontract.org/2004/07/SalesForceCommunication.Domain", L"PocketVoucherDetail2", L"PocketVoucherDetail");
  /* VoucherRedemptionDetails */
  RemClassRegistry()->RegisterXSClass(__classid(VoucherRedemptionDetails), L"http://schemas.datacontract.org/2004/07/PocketVoucherIntegration.RedemptionDetails", L"VoucherRedemptionDetails");
  /* RedemptionResponseDetails */
  RemClassRegistry()->RegisterXSClass(__classid(RedemptionResponseDetails), L"http://schemas.datacontract.org/2004/07/PocketVoucherIntegration.RedemptionDetails", L"RedemptionResponseDetails");
  /* VoucherRedemptionDetails */
  RemClassRegistry()->RegisterXSClass(__classid(VoucherRedemptionDetails2), L"http://schemas.datacontract.org/2004/07/PocketVoucherIntegration.RedemptionDetails", L"VoucherRedemptionDetails2", L"VoucherRedemptionDetails");
  /* RedemptionResponseDetails */
  RemClassRegistry()->RegisterXSClass(__classid(RedemptionResponseDetails2), L"http://schemas.datacontract.org/2004/07/PocketVoucherIntegration.RedemptionDetails", L"RedemptionResponseDetails2", L"RedemptionResponseDetails");
}
#pragma startup RegTypes 32

};     // NS__

