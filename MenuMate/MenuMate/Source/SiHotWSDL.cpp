// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8742/MenumateServices/SiHotService/?wsdl
//  >Import : http://localhost:8742/MenumateServices/SiHotService/?wsdl>0
//  >Import : http://localhost:8742/MenumateServices/SiHotService/?xsd=xsd0
//  >Import : http://localhost:8742/MenumateServices/SiHotService/?xsd=xsd2
//  >Import : http://localhost:8742/MenumateServices/SiHotService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (29/05/2018 6:10:50 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "SiHotWSDL.h"
#endif



namespace NS__SiHotWSDL {

_di_ISiHotIntegrationWebService GetISiHotIntegrationWebService(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8742/MenumateServices/SiHotService/?wsdl";
  static const char* defURL = "http://localhost:8742/MenumateServices/SiHotService/";
  static const char* defSvc = "SiHotIntegrationWebService";
  static const char* defPrt = "basicHttpBinding_ServiceSiHot";
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
  _di_ISiHotIntegrationWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall RoomDetails::~RoomDetails()
{
  for(int i=0; i<FGuestDetailsList.Length; i++)
    if (FGuestDetailsList[i])
      delete FGuestDetailsList[i];
}

__fastcall RoomChargeDetails::~RoomChargeDetails()
{
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
  /* ISiHotIntegrationWebService */
  InvRegistry()->RegisterInterface(__delphirtti(ISiHotIntegrationWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(ISiHotIntegrationWebService), L"http://tempuri.org/ISiHotIntegrationWebService/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(ISiHotIntegrationWebService), ioDocument);
  /* RoomRequest */
  RemClassRegistry()->RegisterXSClass(__classid(RoomRequest), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"RoomRequest");
  /* ArrayOfGuestDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfGuestDetails), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"ArrayOfGuestDetails");
  /* RoomDetails */
  RemClassRegistry()->RegisterXSClass(__classid(RoomDetails), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"RoomDetails");
  /* GuestDetails */
  RemClassRegistry()->RegisterXSClass(__classid(GuestDetails), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"GuestDetails");
  /* ArrayOfSiHotService */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSiHotService), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"ArrayOfSiHotService");
  /* ArrayOfSiHotPayment */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSiHotPayment), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"ArrayOfSiHotPayment");
  /* RoomChargeDetails */
  RemClassRegistry()->RegisterXSClass(__classid(RoomChargeDetails), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"RoomChargeDetails");
  /* SiHotService */
  RemClassRegistry()->RegisterXSClass(__classid(SiHotService), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"SiHotService");
  /* SiHotPayment */
  RemClassRegistry()->RegisterXSClass(__classid(SiHotPayment), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"SiHotPayment");
  /* RoomChargeResponse */
  RemClassRegistry()->RegisterXSClass(__classid(RoomChargeResponse), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"RoomChargeResponse");
  /* RoomRequest */
  RemClassRegistry()->RegisterXSClass(__classid(RoomRequest2), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"RoomRequest2", L"RoomRequest");
  /* RoomDetails */
  RemClassRegistry()->RegisterXSClass(__classid(RoomDetails2), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"RoomDetails2", L"RoomDetails");
  /* GuestDetails */
  RemClassRegistry()->RegisterXSClass(__classid(GuestDetails2), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"GuestDetails2", L"GuestDetails");
  /* RoomChargeDetails */
  RemClassRegistry()->RegisterXSClass(__classid(RoomChargeDetails2), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"RoomChargeDetails2", L"RoomChargeDetails");
  /* SiHotService */
  RemClassRegistry()->RegisterXSClass(__classid(SiHotService2), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"SiHotService2", L"SiHotService");
  /* SiHotPayment */
  RemClassRegistry()->RegisterXSClass(__classid(SiHotPayment2), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"SiHotPayment2", L"SiHotPayment");
  /* RoomChargeResponse */
  RemClassRegistry()->RegisterXSClass(__classid(RoomChargeResponse2), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"RoomChargeResponse2", L"RoomChargeResponse");
  /* ArrayOfGuestDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfGuestDetails), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"ArrayOfGuestDetails");
  /* ArrayOfSiHotPayment */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSiHotPayment), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"ArrayOfSiHotPayment");
  /* ArrayOfSiHotService */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSiHotService), L"http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain", L"ArrayOfSiHotService");
}
#pragma startup RegTypes 32

};     // NS__SiHotWSDL

