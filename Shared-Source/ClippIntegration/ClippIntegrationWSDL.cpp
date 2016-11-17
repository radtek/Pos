// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8888/MenumateServices/ClippService/?wsdl
//  >Import : http://localhost:8888/MenumateServices/ClippService/?wsdl>0
//  >Import : http://localhost:8888/MenumateServices/ClippService/?xsd=xsd0
//  >Import : http://localhost:8888/MenumateServices/ClippService/?xsd=xsd2
//  >Import : http://localhost:8888/MenumateServices/ClippService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (8/09/2015 5:24:22 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "ClippIntegrationWSDL.h"
#endif



namespace NS_ClippIntegrationWSDL {

_di_IClippIntergrationWebService GetIClippIntergrationWebService(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8736/MenumateServices/ClippService/?wsdl";
  static const char* defURL = "http://localhost:8736/MenumateServices/ClippService/";
  static const char* defSvc = "ClippIntergrationWebService";
  static const char* defPrt = "basicHttpBinding_ServiceClipp";
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
  _di_IClippIntergrationWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall ClippMessage::~ClippMessage()
{
  delete FCreatedDate;
  delete FData;
}

__fastcall ClippMessageDetail::~ClippMessageDetail()
{
  delete FDiscountAmount;
  delete FProcessingFeeAmount;
  delete FTabLimit;
  delete FTipAmount;
  delete FTotalPaymentAmount;
  for(int i=0; i<FDiscounts.Length; i++)
    if (FDiscounts[i])
      delete FDiscounts[i];
}

__fastcall Discount::~Discount()
{
  delete FAmount;
}

__fastcall ClippTabDetailRequest::~ClippTabDetailRequest()
{
  delete FAmount;
  delete FTabData;
}

__fastcall ClippTabDetail::~ClippTabDetail()
{
  delete FIncludedTaxAmount;
  delete FOutstandingAmount;
  delete FReceiptDate;
  delete FTabLimit;
  delete FTotalAmount;
  for(int i=0; i<FItems.Length; i++)
    if (FItems[i])
      delete FItems[i];
  for(int i=0; i<FPayments.Length; i++)
    if (FPayments[i])
      delete FPayments[i];
  for(int i=0; i<FSundries.Length; i++)
    if (FSundries[i])
      delete FSundries[i];
}

__fastcall Item::~Item()
{
  delete FDiscountAmount;
  delete FPrice;
  delete FQuantity;
  delete FTotalAmount;
  for(int i=0; i<FSubItems.Length; i++)
    if (FSubItems[i])
      delete FSubItems[i];
}

__fastcall Payment::~Payment()
{
  delete FAmount;
}

__fastcall Sundry::~Sundry()
{
  delete FAmount;
}

__fastcall ClippErrorDetail::~ClippErrorDetail()
{
  delete FTotalAmount;
}

__fastcall PaymentDetailRequest::~PaymentDetailRequest()
{
  delete FAmount;
}

__fastcall PaymentDetailResponse::~PaymentDetailResponse()
{
  delete FProcessingFeeAmount;
  delete FTotalPaymentAmount;
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IClippIntergrationWebService */
  InvRegistry()->RegisterInterface(__delphirtti(IClippIntergrationWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterAllSOAPActions(__delphirtti(IClippIntergrationWebService), L"|http://tempuri.org/IClippIntergrationWebService/GetOpenMessages|http://tempuri.org/IClippIntergrationWebService/GetPreviousMessages|http://tempuri.org/IClippIntergrationWebService/UpdateTabDetails|UpdateTabDetailsOnError|http://tempuri.org/IClippIntergrationWebService/CloseTab|CloseTabOnError|http://tempuri.org/IClippIntergrationWebService/RequestTabPayment|http://tempuri.org/IClippIntergrationWebService/ForceCloseAllTabs");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IClippIntergrationWebService), ioDocument);
  /* ArrayOfClippMessage */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfClippMessage), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfClippMessage");
  /* ClippMessageType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(ClippMessageType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippMessageType");
  /* ClippMessage */
  RemClassRegistry()->RegisterXSClass(__classid(ClippMessage), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippMessage");
  /* ArrayOfDiscount */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscount), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfDiscount");
  /* ClippMessageDetail */
  RemClassRegistry()->RegisterXSClass(__classid(ClippMessageDetail), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippMessageDetail");
  /* Discount */
  RemClassRegistry()->RegisterXSClass(__classid(Discount), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"Discount");
  /* ClippTabDetailRequest */
  RemClassRegistry()->RegisterXSClass(__classid(ClippTabDetailRequest), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippTabDetailRequest");
  /* ArrayOfItem */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItem), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfItem");
  /* ArrayOfPayment */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfPayment), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfPayment");
  /* ArrayOfSundry */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSundry), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfSundry");
  /* ClippTabDetail */
  RemClassRegistry()->RegisterXSClass(__classid(ClippTabDetail), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippTabDetail");
  /* Item */
  RemClassRegistry()->RegisterXSClass(__classid(Item), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"Item");
  /* Payment */
  RemClassRegistry()->RegisterXSClass(__classid(Payment), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"Payment");
  /* Sundry */
  RemClassRegistry()->RegisterXSClass(__classid(Sundry), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"Sundry");
  /* ClippErrorDetail */
  RemClassRegistry()->RegisterXSClass(__classid(ClippErrorDetail), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippErrorDetail");
  /* PaymentDetailRequest */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentDetailRequest), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"PaymentDetailRequest");
  /* PaymentDetailResponse */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentDetailResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"PaymentDetailResponse");
  /* ClippMessage */
  RemClassRegistry()->RegisterXSClass(__classid(ClippMessage2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippMessage2", L"ClippMessage");
  /* ClippMessageDetail */
  RemClassRegistry()->RegisterXSClass(__classid(ClippMessageDetail2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippMessageDetail2", L"ClippMessageDetail");
  /* Discount */
  RemClassRegistry()->RegisterXSClass(__classid(Discount2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"Discount2", L"Discount");
  /* ClippTabDetailRequest */
  RemClassRegistry()->RegisterXSClass(__classid(ClippTabDetailRequest2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippTabDetailRequest2", L"ClippTabDetailRequest");
  /* ClippTabDetail */
  RemClassRegistry()->RegisterXSClass(__classid(ClippTabDetail2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippTabDetail2", L"ClippTabDetail");
  /* Item */
  RemClassRegistry()->RegisterXSClass(__classid(Item2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"Item2", L"Item");
  /* Payment */
  RemClassRegistry()->RegisterXSClass(__classid(Payment2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"Payment2", L"Payment");
  /* Sundry */
  RemClassRegistry()->RegisterXSClass(__classid(Sundry2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"Sundry2", L"Sundry");
  /* ClippErrorDetail */
  RemClassRegistry()->RegisterXSClass(__classid(ClippErrorDetail2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippErrorDetail2", L"ClippErrorDetail");
  /* PaymentDetailRequest */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentDetailRequest2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"PaymentDetailRequest2", L"PaymentDetailRequest");
  /* PaymentDetailResponse */
  RemClassRegistry()->RegisterXSClass(__classid(PaymentDetailResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"PaymentDetailResponse2", L"PaymentDetailResponse");
  /* ArrayOfDiscount */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDiscount), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfDiscount");
  /* ArrayOfItem */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfItem), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfItem");
  /* ArrayOfSundry */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSundry), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfSundry");
  /* ClippMessageType */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(ClippMessageType_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ClippMessageType");
  /* ArrayOfClippMessage */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfClippMessage), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfClippMessage");
  /* ArrayOfPayment */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfPayment), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Clipp", L"ArrayOfPayment");
}
#pragma startup RegTypes 32

};     // NS_ClippIntegrationWSDL
