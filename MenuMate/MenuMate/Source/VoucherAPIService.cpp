// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : C:\Code\5.5\Static Files\wsdl schema\VoucherAPIService.wsdl
//  >Import : C:\Code\5.5\Static Files\wsdl schema\VoucherAPIService.wsdl>0
// Encoding : utf-8
// Version  : 1.0
// (14/03/2012 11:51:19 a.m. - - $Rev: 25127 $)
// ************************************************************************ //
#include <winsock2.h>
#include <vcl.h>
#pragma hdrstop

#if !defined(VoucherAPIServiceH)
#include "VoucherAPIService.h"
#endif



namespace NS_VoucherAPIService {

_di_VoucherAPI GetVoucherAPI(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "C:\Code\5.5\Static Files\wsdl schema\VoucherAPIService.wsdl";
  static const char* defURL = "http://pos.host.pocketvouchers.com/menumate.aspx";
  static const char* defSvc = "VoucherAPIImplementation";
  static const char* defPrt = "BasicHttpBinding_VoucherAPI";
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
  _di_VoucherAPI service;
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
  /* VoucherAPI */
  InvRegistry()->RegisterInterface(__delphirtti(VoucherAPI), L"http://voucher.menumate.com/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(VoucherAPI), L"http://voucher.menumate.com/VoucherAPI/%operationName%Request");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(VoucherAPI), ioDocument);
  InvRegistry()->RegisterExternalParamName(__delphirtti(VoucherAPI), L"AuthToken", L"return_", L"return");
  InvRegistry()->RegisterExternalParamName(__delphirtti(VoucherAPI), L"RedeemVoucher", L"return_", L"return");
  InvRegistry()->RegisterExternalParamName(__delphirtti(VoucherAPI), L"CreditVoucher", L"return_", L"return");
  /* voucherResponse */
  RemClassRegistry()->RegisterXSClass(__classid(voucherResponse), L"http://voucher.menumate.com/", L"voucherResponse");
}
#pragma startup RegTypes 32

};     // NS_VoucherAPIService

