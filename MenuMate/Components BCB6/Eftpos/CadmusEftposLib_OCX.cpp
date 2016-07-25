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

// C++ TLBWRTR : $Revision:   1.151.1.0.1.27  $
// File generated on 8/11/2007 10:06:54 a.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\Program Files\Cadmus\Cronos\CadmusEftpos.tlb (1)
// LIBID: {2C17B0C4-BDF2-4921-884C-ECC72D5030FA}
// LCID: 0
// Helpfile: 
// HelpString: CadmusEftpos 1.0 Type Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#include <oleserver.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "CadmusEftposLib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Cadmuseftposlib_tlb
{

IPOSInterfacePtr& TCronosInterface::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TCronosInterface::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TCronosInterface::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TCronosInterface::Disconnect()
{
  if (m_DefaultIntf) {
    
    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TCronosInterface::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TCronosInterface::ConnectTo(IPOSInterfacePtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TCronosInterface::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_POSInterface;
  sd.IntfIID = __uuidof(IPOSInterface);
  sd.EventIID= __uuidof(_IPOSInterfaceEvents);
  ServerData = &sd;
}

void __fastcall TCronosInterface::InvokeEvent(int id, Oleserver::TVariantArray& params)
{
  switch(id)
  {
    case 1: {
      if (OnEFT2POSMessage) {
		  (OnEFT2POSMessage)(this, &params[0]);
		}
		break;
      }
    default:
      break;
  }
}


};     // namespace Cadmuseftposlib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
/*namespace Cadmuseftposlib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass cls_svr[] = {
                              __classid(Cadmuseftposlib_tlb::TCronosInterface)
                           };
  RegisterComponents("ActiveX", cls_svr,
                     sizeof(cls_svr)/sizeof(cls_svr[0])-1);
}

}; */    // namespace Cadmuseftposlib_ocx
