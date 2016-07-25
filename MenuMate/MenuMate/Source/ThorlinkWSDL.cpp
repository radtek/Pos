// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8739/MenumateServices/ThorLink/?wsdl
//  >Import : http://localhost:8739/MenumateServices/ThorLink/?wsdl>0
//  >Import : http://localhost:8739/MenumateServices/ThorLink/?xsd=xsd0
//  >Import : http://localhost:8739/MenumateServices/ThorLink/?xsd=xsd2
//  >Import : http://localhost:8739/MenumateServices/ThorLink/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (28/06/2016 7:56:27 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "ThorlinkWSDL.h"
#endif



namespace NS__ThorlinkWSDL {

_di_IWCFServiceThorlink GetIWCFServiceThorlink(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8739/MenumateServices/ThorLink/?wsdl";
  static const char* defURL = "http://localhost:8739/MenumateServices/ThorLink/";
  static const char* defSvc = "WCFServiceThorlink";
  static const char* defPrt = "basicHttpBinding_ServiceThorLink";
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
  _di_IWCFServiceThorlink service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall DTO_TMemberInfo::~DTO_TMemberInfo()
{
  for(int i=0; i<FvoucherDetailsOfThor.Length; i++)
    if (FvoucherDetailsOfThor[i])
      delete FvoucherDetailsOfThor[i];
}

__fastcall DTO_TenderList::~DTO_TenderList()
{
  for(int i=0; i<FitemDetailsList.Length; i++)
    if (FitemDetailsList[i])
      delete FitemDetailsList[i];
  for(int i=0; i<FtenderItemDetails.Length; i++)
    if (FtenderItemDetails[i])
      delete FtenderItemDetails[i];
}

__fastcall DTO_TenderDetails::~DTO_TenderDetails()
{
  for(int i=0; i<FitemDetails.Length; i++)
    if (FitemDetails[i])
      delete FitemDetails[i];
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IWCFServiceThorlink */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFServiceThorlink), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFServiceThorlink), L"http://tempuri.org/IWCFServiceThorlink/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFServiceThorlink), ioDocument);
  /* DTO_ThorAuth */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_ThorAuth), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_ThorAuth");
  /* DTO_ThorlinkInquiry */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_ThorlinkInquiry), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_ThorlinkInquiry");
  /* ArrayOfDTO_VoucherDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTO_VoucherDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"ArrayOfDTO_VoucherDetails");
  /* DTO_TMemberInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_TMemberInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_TMemberInfo");
  /* DTO_VoucherDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_VoucherDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_VoucherDetails");
  /* ArrayOfDTO_ItemDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTO_ItemDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"ArrayOfDTO_ItemDetails");
  /* ArrayOfDTO_TenderDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTO_TenderDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"ArrayOfDTO_TenderDetails");
  /* DTO_TenderList */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_TenderList), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_TenderList");
  /* DTO_ItemDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_ItemDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_ItemDetails");
  /* TenderTypeThor */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(TenderTypeThor_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"TenderTypeThor");
  /* DTO_TenderDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_TenderDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_TenderDetails");
  /* DTO_TPurchaseInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_TPurchaseInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_TPurchaseInfo");
  /* DTO_RefundDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_RefundDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_RefundDetails");
  /* DTO_ThorAuth */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_ThorAuth2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_ThorAuth2", L"DTO_ThorAuth");
  /* DTO_ThorlinkInquiry */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_ThorlinkInquiry2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_ThorlinkInquiry2", L"DTO_ThorlinkInquiry");
  /* DTO_TMemberInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_TMemberInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_TMemberInfo2", L"DTO_TMemberInfo");
  /* DTO_VoucherDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_VoucherDetails2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_VoucherDetails2", L"DTO_VoucherDetails");
  /* DTO_TenderList */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_TenderList2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_TenderList2", L"DTO_TenderList");
  /* DTO_ItemDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_ItemDetails2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_ItemDetails2", L"DTO_ItemDetails");
  /* DTO_TenderDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_TenderDetails2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_TenderDetails2", L"DTO_TenderDetails");
  /* DTO_TPurchaseInfo */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_TPurchaseInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_TPurchaseInfo2", L"DTO_TPurchaseInfo");
  /* DTO_RefundDetails */
  RemClassRegistry()->RegisterXSClass(__classid(DTO_RefundDetails2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"DTO_RefundDetails2", L"DTO_RefundDetails");
  /* ArrayOfDTO_ItemDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTO_ItemDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"ArrayOfDTO_ItemDetails");
  /* TenderTypeThor */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(TenderTypeThor_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"TenderTypeThor");
  /* ArrayOfDTO_TenderDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTO_TenderDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"ArrayOfDTO_TenderDetails");
  /* ArrayOfDTO_VoucherDetails */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTO_VoucherDetails), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.Thorlink", L"ArrayOfDTO_VoucherDetails");
}
#pragma startup RegTypes 32

};     // NS__

