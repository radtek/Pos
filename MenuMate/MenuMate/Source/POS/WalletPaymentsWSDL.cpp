// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8743/MenumateServices/WalletPayments/?wsdl
//  >Import : http://localhost:8743/MenumateServices/WalletPayments/?wsdl>0
//  >Import : http://localhost:8743/MenumateServices/WalletPayments/?xsd=xsd0
//  >Import : http://localhost:8743/MenumateServices/WalletPayments/?xsd=xsd2
//  >Import : http://localhost:8743/MenumateServices/WalletPayments/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (28/03/2017 12:04:28 a.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "WalletPaymentsWSDL.h"
#endif



namespace NS_WalletPaymentsWSDL {

_di_IWCFServiceWalletPayments GetIWCFServiceWalletPayments(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8743/MenumateServices/WalletPayments/?wsdl";
  static const char* defURL = "http://localhost:8743/MenumateServices/WalletPayments/";
  static const char* defSvc = "WCFServiceWalletPayments";
  static const char* defPrt = "basicHttpBinding_ServiceWalletPayments";
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
  _di_IWCFServiceWalletPayments service;
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
  /* IWCFServiceWalletPayments */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFServiceWalletPayments), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFServiceWalletPayments), L"http://tempuri.org/IWCFServiceWalletPayments/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFServiceWalletPayments), ioDocument);
  /* WalletAccount */
  RemClassRegistry()->RegisterXSClass(__classid(WalletAccount), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments", L"WalletAccount");
  /* WalletActionResponse */
  RemClassRegistry()->RegisterXSClass(__classid(WalletActionResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments", L"WalletActionResponse");
  /* WalletTransaction */
  RemClassRegistry()->RegisterXSClass(__classid(WalletTransaction), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments", L"WalletTransaction");
  /* WalletAccount */
  RemClassRegistry()->RegisterXSClass(__classid(WalletAccount2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments", L"WalletAccount2", L"WalletAccount");
  /* WalletActionResponse */
  RemClassRegistry()->RegisterXSClass(__classid(WalletActionResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments", L"WalletActionResponse2", L"WalletActionResponse");
  /* WalletTransaction */
  RemClassRegistry()->RegisterXSClass(__classid(WalletTransaction2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Wallet_Payments", L"WalletTransaction2", L"WalletTransaction");
}
#pragma startup RegTypes 32

};     // NS__


