// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8734/MenumateServices/LoyaltyMate/?wsdl
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?wsdl>0
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd0
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd2
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd3
//  >Import : http://localhost:8734/MenumateServices/LoyaltyMate/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (4/02/2015 9:33:26 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#if !defined(_H)
#include "LoyaltyMateWSDL.h"
#endif

namespace NS_LoyaltyMateWSDL {

_di_IWCFServiceLoyaltyMate GetIWCFServiceLoyaltyMate(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "http://localhost:8734/MenumateServices/LoyaltyMate/?wsdl";
  static const char* defURL = "http://localhost:8734/MenumateServices/LoyaltyMate/";
  static const char* defSvc = "WCFServiceLoyaltyMate";
  static const char* defPrt = "basicHttpBinding_ServiceLoyaltyMate";
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
  _di_IWCFServiceLoyaltyMate service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall MemberInfo::~MemberInfo()
{
  delete FBirthdayBenefitDate;
  delete FDateOfBirth;
  delete FLastModified;
  delete FLastVisitDate;
  delete FYearStartDate;
}

__fastcall LoyaltyMemberResponse::~LoyaltyMemberResponse()
{
  delete FMemberInfo;
}

__fastcall LoyaltyMemberListResponse::~LoyaltyMemberListResponse()
{
  for(int i=0; i<FMemberList.Length; i++)
    if (FMemberList[i])
      delete FMemberList[i];
}

__fastcall TransactionInfo::~TransactionInfo()
{
  delete FTransactionDate;
}

__fastcall LoyaltyTierResponse::~LoyaltyTierResponse()
{
  delete FTierInfo;
}

__fastcall LoyaltyTierListResponse::~LoyaltyTierListResponse()
{
  for(int i=0; i<FTierLevelList.Length; i++)
    if (FTierLevelList[i])
      delete FTierLevelList[i];
}

__fastcall PointsInfo::~PointsInfo()
{
  delete FEndDate;
  delete FStartDate;
}

__fastcall LoyaltyPointsInfoResponse::~LoyaltyPointsInfoResponse()
{
  delete FPointsInfo;
}

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IWCFServiceLoyaltyMate */
  InvRegistry()->RegisterInterface(__delphirtti(IWCFServiceLoyaltyMate), L"http://tempuri.org/", L"utf-8");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IWCFServiceLoyaltyMate), L"http://tempuri.org/IWCFServiceLoyaltyMate/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IWCFServiceLoyaltyMate), ioDocument);
  /* MMServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(MMServiceResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO", L"MMServiceResponse");
  /* MMServiceResponse */
  RemClassRegistry()->RegisterXSClass(__classid(MMServiceResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO", L"MMServiceResponse2", L"MMServiceResponse");
  /* MemberInfo */
  RemClassRegistry()->RegisterXSClass(__classid(MemberInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"MemberInfo");
  /* LoyaltyResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(LoyaltyResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponseCode");
  /* LoyaltyMemberResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyMemberResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyMemberResponse");
  /* LoyaltyResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponse");
  /* ArrayOfMemberInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfMemberInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfMemberInfo");
  /* LoyaltyMemberListResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyMemberListResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyMemberListResponse");
  /* TransactionInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"TransactionInfo");
  /* TierLevelInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TierLevelInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"TierLevelInfo");
  /* LoyaltyTierResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyTierResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyTierResponse");
  /* ArrayOfTierLevelInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTierLevelInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfTierLevelInfo");
  /* LoyaltyTierListResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyTierListResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyTierListResponse");
  /* PointsInfo */
  RemClassRegistry()->RegisterXSClass(__classid(PointsInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"PointsInfo");
  /* LoyaltyPointsInfoResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyPointsInfoResponse), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyPointsInfoResponse");
  /* MemberInfo */
  RemClassRegistry()->RegisterXSClass(__classid(MemberInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"MemberInfo2", L"MemberInfo");
  /* LoyaltyMemberResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyMemberResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyMemberResponse2", L"LoyaltyMemberResponse");
  /* LoyaltyResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponse2", L"LoyaltyResponse");
  /* LoyaltyMemberListResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyMemberListResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyMemberListResponse2", L"LoyaltyMemberListResponse");
  /* TransactionInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TransactionInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"TransactionInfo2", L"TransactionInfo");
  /* TierLevelInfo */
  RemClassRegistry()->RegisterXSClass(__classid(TierLevelInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"TierLevelInfo2", L"TierLevelInfo");
  /* LoyaltyTierResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyTierResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyTierResponse2", L"LoyaltyTierResponse");
  /* LoyaltyTierListResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyTierListResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyTierListResponse2", L"LoyaltyTierListResponse");
  /* PointsInfo */
  RemClassRegistry()->RegisterXSClass(__classid(PointsInfo2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"PointsInfo2", L"PointsInfo");
  /* LoyaltyPointsInfoResponse */
  RemClassRegistry()->RegisterXSClass(__classid(LoyaltyPointsInfoResponse2), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyPointsInfoResponse2", L"LoyaltyPointsInfoResponse");
  /* ArrayOfMemberInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfMemberInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfMemberInfo");
  /* LoyaltyResponseCode */
  RemClassRegistry()->RegisterXSInfo(GetClsMemberTypeInfo(__typeinfo(LoyaltyResponseCode_TypeInfoHolder)), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"LoyaltyResponseCode");
  /* ArrayOfTierLevelInfo */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfTierLevelInfo), L"http://schemas.datacontract.org/2004/07/MenumateServices.DTO.LoyaltyMate", L"ArrayOfTierLevelInfo");
}
#pragma startup RegTypes 32

};     // NS__



