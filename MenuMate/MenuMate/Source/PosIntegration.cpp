// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : C:\Users\michael\dev\5.3\Static Files\XML Examples\FloorPlan wsdl.xml
//  >Import : C:\Users\michael\dev\5.3\Static Files\XML Examples\FloorPlan wsdl.xml>0
//  >Import : http://192.168.42.223:8732/MenumateWinServices/service?xsd=xsd0
//  >Import : http://192.168.42.223:8732/MenumateWinServices/service?xsd=xsd1
//  >Import : http://192.168.42.223:8732/MenumateWinServices/service?xsd=xsd2
//  >Import : http://192.168.42.223:8732/MenumateWinServices/service?xsd=xsd3
// Codegen  : [wfMapArraysToClasses+, wfUseSettersAndGetters+]
// (10/08/2011 8:48:13 a.m. - - $Rev: 25127 $)
// ************************************************************************ //

#include <winsock2.h>
#include <vcl.h>
#pragma hdrstop

#if !defined(PosIntegrationH)
#include "PosIntegration.h"
#endif



namespace NS_PosIntegration {

_di_IPosIntegration GetIPosIntegration(bool useWSDL, AnsiString addr, THTTPRIO* HTTPRIO)
{
  static const char* defWSDL= "C:\Users\michael\dev\5.3\Static Files\XML Examples\FloorPlan wsdl.xml";
  static const char* defURL = "http://192.168.42.223:8732/MenumateWinServices/service";
  static const char* defSvc = "PosIntegrationService";
  static const char* defPrt = "basicHttpBinding_PosIntegration";
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
  _di_IPosIntegration service;
  rio->QueryInterface(service);
  if (!service && !HTTPRIO)
    delete rio;
  return service;
}


__fastcall DTOReservable::~DTOReservable()
{
  delete FParentLocation;
}

__fastcall DTOLocation::~DTOLocation()
{
  delete FDefaultDuration;
  delete FReservableList;
  delete FSessions;
}

__fastcall DTOSession::~DTOSession()
{
  delete FEndTime;
  delete FStartTime;
}

/*__fastcall ArrayOfDTOLocation::~ArrayOfDTOLocation()
{
  for(int i=0; i<FDTOLocation.Length; i++)
    if (FDTOLocation[i])
      delete FDTOLocation[i];
}

__fastcall ArrayOfDTOReservable::~ArrayOfDTOReservable()
{
  for(int i=0; i<FDTOReservable.Length; i++)
    if (FDTOReservable[i])
      delete FDTOReservable[i];
}

__fastcall ArrayOfDTOSession::~ArrayOfDTOSession()
{
  for(int i=0; i<FDTOSession.Length; i++)
    if (FDTOSession[i])
      delete FDTOSession[i];
}*/

// ************************************************************************ //
// This routine registers the interfaces and types exposed by the WebService.
// ************************************************************************ //
static void RegTypes()
{
  /* IPosIntegration */
  InvRegistry()->RegisterInterface(__delphirtti(IPosIntegration), L"http://tempuri.org/", L"");
  InvRegistry()->RegisterDefaultSOAPAction(__delphirtti(IPosIntegration), L"http://tempuri.org/IPosIntegration/%operationName%");
  InvRegistry()->RegisterInvokeOptions(__delphirtti(IPosIntegration), ioDocument);
  /* OperationResult */
  RemClassRegistry()->RegisterXSClass(__classid(OperationResult), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts", L"OperationResult");
  /* DTOReservable */
  RemClassRegistry()->RegisterXSClass(__classid(DTOReservable), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"DTOReservable");
  /* DTOLocation */
  RemClassRegistry()->RegisterXSClass(__classid(DTOLocation), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"DTOLocation");
  /* DTOSession */
  RemClassRegistry()->RegisterXSClass(__classid(DTOSession), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"DTOSession");
  /* DTOLocation */
  RemClassRegistry()->RegisterXSClass(__classid(DTOLocation2), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"DTOLocation2", L"DTOLocation");
  /* DTOReservable */
  RemClassRegistry()->RegisterXSClass(__classid(DTOReservable2), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"DTOReservable2", L"DTOReservable");
  /* DTOSession */
  RemClassRegistry()->RegisterXSClass(__classid(DTOSession2), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"DTOSession2", L"DTOSession");
  /* OperationResult */
  RemClassRegistry()->RegisterXSClass(__classid(OperationResult2), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts", L"OperationResult2", L"OperationResult");
  /* Array_Of_DTOLocation */
//  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DTOLocation), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"Array_Of_DTOLocation");
  /* ArrayOfDTOLocation */
//  RemClassRegistry()->RegisterXSClass(__classid(ArrayOfDTOLocation), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"ArrayOfDTOLocation");
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTOLocation), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"ArrayOfDTOLocation");

  /* ArrayOfDTOLocation */
//  RemClassRegistry()->RegisterXSClass(__classid(ArrayOfDTOLocation2), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"ArrayOfDTOLocation2", L"ArrayOfDTOLocation");
  /* Array_Of_DTOReservable */
//  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DTOReservable), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"Array_Of_DTOReservable");
  /* ArrayOfDTOReservable */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTOReservable), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"ArrayOfDTOReservable");
  /* ArrayOfDTOReservable */
//  RemClassRegistry()->RegisterXSClass(__classid(ArrayOfDTOReservable2), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"ArrayOfDTOReservable2", L"ArrayOfDTOReservable");
  /* Array_Of_DTOSession */
//  RemClassRegistry()->RegisterXSInfo(__delphirtti(Array_Of_DTOSession), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"Array_Of_DTOSession");
  /* ArrayOfDTOSession */
  RemClassRegistry()->RegisterXSInfo(__delphirtti(ArrayOfDTOSession), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"ArrayOfDTOSession");
  /* ArrayOfDTOSession */
//  RemClassRegistry()->RegisterXSClass(__classid(ArrayOfDTOSession2), L"http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO", L"ArrayOfDTOSession2", L"ArrayOfDTOSession");
}
#pragma startup RegTypes 32

};     // NS_PosIntegration

