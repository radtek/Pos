// ************************************************************************ //
// WARNING                                                                    
// -------                                                                    
// The types declared in this file were generated from data read from a       
// Type Library. If this type library is explicitly or indirectly (via        
// another type library referring to this type library) re-imported, or the   
// 'Refresh' command of the Type Library Editor activated while editing the   
// Type Library, the contents of this file will be regenerated and all        
// manual modifications will be lost.                                         
// ************************************************************************ //

// $Rev: 18756 $
// File generated on 3/11/2010 1:29:53 p.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\PROGRA~2\Syncro\EFT2000Engine.exe (1)
// LIBID: {E35B892B-66C9-11D3-A06A-0008C7BC6FEA}
// LCID: 0
// Helpfile: 
// HelpString: EFT2000ETSLEngine 1.0 Type Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\Windows\system32\stdole2.tlb)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#include <oleserver.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "EFT2000ETSLENGINELib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Eft2000etslenginelib_tlb
{

IEFTIFPtr& TEFTIF::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TEFTIF::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TEFTIF::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TEFTIF::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TEFTIF::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TEFTIF::ConnectTo(IEFTIFPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TEFTIF::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_EFTIF;
  sd.IntfIID = __uuidof(IEFTIF);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

void __fastcall TEFTIF::Startup(void)
{
  GetDefaultInterface()->Startup();
}

void __fastcall TEFTIF::StartAdminFunction(long lAdminType/*[in]*/, long lUniqueID/*[in]*/, 
                                           long bRunInBackground/*[in]*/)
{
  GetDefaultInterface()->StartAdminFunction(lAdminType/*[in]*/, lUniqueID/*[in]*/, 
                                            bRunInBackground/*[in]*/);
}

void __fastcall TEFTIF::StartFinancialTransaction(long lTransactionType/*[in]*/, 
                                                  long lUniqueID/*[in]*/, 
                                                  long bRunInBackground/*[in]*/, 
                                                  long lAmount1/*[in]*/, long lAmount2/*[in]*/)
{
  GetDefaultInterface()->StartFinancialTransaction(lTransactionType/*[in]*/, lUniqueID/*[in]*/, 
                                                   bRunInBackground/*[in]*/, lAmount1/*[in]*/, 
                                                   lAmount2/*[in]*/);
}

void __fastcall TEFTIF::StartUtility(long lUtilityType/*[in]*/, long lUniqueID/*[in]*/, 
                                     long bRunInBackground/*[in]*/)
{
  GetDefaultInterface()->StartUtility(lUtilityType/*[in]*/, lUniqueID/*[in]*/, 
                                      bRunInBackground/*[in]*/);
}

void __fastcall TEFTIF::POSConnecting(void)
{
  GetDefaultInterface()->POSConnecting();
}

void __fastcall TEFTIF::StartChequeAuthorisation(BSTR bstrChequeSerialNumber/*[in]*/, 
                                                 BSTR bstrChequeBankBranch/*[in]*/, 
                                                 BSTR bstrChequeAccount/*[in]*/, 
                                                 long bRunInBackground/*[in]*/, long lAmount/*[in]*/)
{
  GetDefaultInterface()->StartChequeAuthorisation(bstrChequeSerialNumber/*[in]*/, 
                                                  bstrChequeBankBranch/*[in]*/, 
                                                  bstrChequeAccount/*[in]*/, 
                                                  bRunInBackground/*[in]*/, lAmount/*[in]*/);
}

void __fastcall TEFTIF::SyncroPrint(BSTR bstrPrintData/*[in]*/)
{
  GetDefaultInterface()->SyncroPrint(bstrPrintData/*[in]*/);
}

BSTR __fastcall TEFTIF::get_ModuleDescription(void)
{
  BSTR pbstrModuleDescription = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleDescription((BSTR*)&pbstrModuleDescription));
  return pbstrModuleDescription;
}

BSTR __fastcall TEFTIF::get_ModuleName(void)
{
  BSTR pbstrModuleName = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleName((BSTR*)&pbstrModuleName));
  return pbstrModuleName;
}

BSTR __fastcall TEFTIF::get_ModuleVersion(void)
{
  BSTR pbstrModuleVersion = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleVersion((BSTR*)&pbstrModuleVersion));
  return pbstrModuleVersion;
}

long __fastcall TEFTIF::get_ResultCode(void)
{
  long plResultCode;
  OLECHECK(GetDefaultInterface()->get_ResultCode((long*)&plResultCode));
  return plResultCode;
}

BSTR __fastcall TEFTIF::get_ResultText(void)
{
  BSTR pbstrResultText = 0;
  OLECHECK(GetDefaultInterface()->get_ResultText((BSTR*)&pbstrResultText));
  return pbstrResultText;
}

void __fastcall TEFTIF::set_POSHardware(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_POSHardware(Param1/*[in]*/);
}

void __fastcall TEFTIF::set_POSSoftwareName(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_POSSoftwareName(Param1/*[in]*/);
}

void __fastcall TEFTIF::set_POSSoftwareNumber(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_POSSoftwareNumber(Param1/*[in]*/);
}

void __fastcall TEFTIF::set_EnablePrintMerchantDetails(long Param1/*[in]*/)
{
  GetDefaultInterface()->set_EnablePrintMerchantDetails(Param1/*[in]*/);
}


};     // namespace Eft2000etslenginelib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Eft2000etslenginelib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass cls_svr[] = {
                              __classid(Eft2000etslenginelib_tlb::TEFTIF)
                           };
  RegisterComponents("MenuMate OCX", cls_svr,
                     sizeof(cls_svr)/sizeof(cls_svr[0])-1);
}

};     // namespace Eft2000etslenginelib_ocx
