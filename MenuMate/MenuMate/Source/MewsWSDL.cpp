// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8747/MenumateServices/MewsService/?wsdl
//  >Import : http://localhost:8747/MenumateServices/MewsService/?wsdl>0
//  >Import : http://localhost:8747/MenumateServices/MewsService/?xsd=xsd0
//  >Import : http://localhost:8747/MenumateServices/MewsService/?xsd=xsd2
//  >Import : http://localhost:8747/MenumateServices/MewsService/?xsd=xsd3
//  >Import : http://localhost:8747/MenumateServices/MewsService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (17/09/2018 5:33:30 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "MewsWSDL.h"
#endif



namespace NS__MewsWSDL {

_di_IMewsIntegrationWebService GetIMewsIntegrationWebService(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8747/MenumateServices/MewsService/?wsdl";
  static const char* defURL = "http://localhost:8747/MenumateServices/MewsService/";
  static const char* defSvc = "MewsIntegrationWebService";
  static const char* defPrt = "basicHttpBinding_ServiceMews";
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
  _di_IMewsIntegrationWebService service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall BasicInquiry::~BasicInquiry()
{
  delete FExtent;
}

__fastcall Service::~Service()
{
  delete FPromotions;
}

__fastcall Customer::~Customer()
{
  delete FBirthDate;
  delete FCreatedUtc;
  delete FUpdatedUtc;
}

__fastcall SpaceDetails::~SpaceDetails()
{
  for(int i=0; i<FSpaceCategories.Length; i++)
    if (FSpaceCategories[i])
      delete FSpaceCategories[i];
  for(int i=0; i<FSpaces.Length; i++)
    if (FSpaces[i])
      delete FSpaces[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IMewsIntegrationWebService */
  InvRegistry()->RegisterInterface(__delphirtti(IMewsIntegrationWebService), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IMewsIntegrationWebService), L"http://tempuri.org/IMewsIntegrationWebService/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IMewsIntegrationWebService), ioDocument);
  /* ArrayOfstring */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfstring), L"http://schemas.microsoft.com/2003/10/Serialization/Arrays", L"ArrayOfstring");
  /* BasicInquiry */
  RemClassRegistry()->RegisterXSClass(__classid(BasicInquiry), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"BasicInquiry");
  /* Extent */
  RemClassRegistry()->RegisterXSClass(__classid(Extent), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Extent");
  /* ArrayOfService */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfService), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfService");
  /* Service */
  RemClassRegistry()->RegisterXSClass(__classid(Service), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Service");
  /* Promotions */
  RemClassRegistry()->RegisterXSClass(__classid(Promotions), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Promotions");
  /* ArrayOfAccountingCategory */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfAccountingCategory), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfAccountingCategory");
  /* AccountingCategory */
  RemClassRegistry()->RegisterXSClass(__classid(AccountingCategory), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"AccountingCategory");
  /* ArrayOfOutlet */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOutlet), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfOutlet");
  /* Outlet */
  RemClassRegistry()->RegisterXSClass(__classid(Outlet), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Outlet");
  /* CustomerSearch */
  RemClassRegistry()->RegisterXSClass(__classid(CustomerSearch), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"CustomerSearch");
  /* ArrayOfCustomer */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfCustomer), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfCustomer");
  /* Customer */
  RemClassRegistry()->RegisterXSClass(__classid(Customer), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Customer");
  /* ArrayOfSpaceCategory */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSpaceCategory), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfSpaceCategory");
  /* ArrayOfSpace */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSpace), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfSpace");
  /* SpaceDetails */
  RemClassRegistry()->RegisterXSClass(__classid(SpaceDetails), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"SpaceDetails");
  /* SpaceCategory */
  RemClassRegistry()->RegisterXSClass(__classid(SpaceCategory), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"SpaceCategory");
  /* Space */
  RemClassRegistry()->RegisterXSClass(__classid(Space), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Space");
  /* BasicInquiry */
  RemClassRegistry()->RegisterXSClass(__classid(BasicInquiry2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"BasicInquiry2", L"BasicInquiry");
  /* Extent */
  RemClassRegistry()->RegisterXSClass(__classid(Extent2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Extent2", L"Extent");
  /* Service */
  RemClassRegistry()->RegisterXSClass(__classid(Service2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Service2", L"Service");
  /* Promotions */
  RemClassRegistry()->RegisterXSClass(__classid(Promotions2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Promotions2", L"Promotions");
  /* AccountingCategory */
  RemClassRegistry()->RegisterXSClass(__classid(AccountingCategory2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"AccountingCategory2", L"AccountingCategory");
  /* Outlet */
  RemClassRegistry()->RegisterXSClass(__classid(Outlet2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Outlet2", L"Outlet");
  /* CustomerSearch */
  RemClassRegistry()->RegisterXSClass(__classid(CustomerSearch2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"CustomerSearch2", L"CustomerSearch");
  /* Customer */
  RemClassRegistry()->RegisterXSClass(__classid(Customer2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Customer2", L"Customer");
  /* SpaceDetails */
  RemClassRegistry()->RegisterXSClass(__classid(SpaceDetails2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"SpaceDetails2", L"SpaceDetails");
  /* SpaceCategory */
  RemClassRegistry()->RegisterXSClass(__classid(SpaceCategory2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"SpaceCategory2", L"SpaceCategory");
  /* Space */
  RemClassRegistry()->RegisterXSClass(__classid(Space2), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"Space2", L"Space");
  /* ArrayOfstring */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfstring), L"http://schemas.microsoft.com/2003/10/Serialization/Arrays", L"ArrayOfstring");
  /* ArrayOfAccountingCategory */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfAccountingCategory), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfAccountingCategory");
  /* ArrayOfCustomer */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfCustomer), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfCustomer");
  /* ArrayOfSpace */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSpace), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfSpace");
  /* ArrayOfService */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfService), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfService");
  /* ArrayOfSpaceCategory */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfSpaceCategory), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfSpaceCategory");
  /* ArrayOfOutlet */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfOutlet), L"http://schemas.datacontract.org/2004/07/MewsIntegration.Domain", L"ArrayOfOutlet");
}
#pragma startup RegTypes 32

};     // NS__

