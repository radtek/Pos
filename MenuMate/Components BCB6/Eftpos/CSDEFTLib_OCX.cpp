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

// $Rev: 17252 $
// File generated on 16/06/2009 5:24:45 p.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\PC_EFT\CsdEft.ocx (1)
// LIBID: {D548A361-6579-11D0-913C-444553540000}
// LCID: 0
// Helpfile: 
// HelpString: CsdEft OLE Control module
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

#include "CSDEFTLib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Csdeftlib_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TCsdEft which
// allows "CsdEft Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TCsdEft::EventDispIDs[43] = {
    0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006,
    0x00000007, 0x00000008, 0x00000009, 0x0000000A, 0x0000000B, 0x0000000C,
    0x0000000D, 0x0000000E, 0x0000000F, 0x00000010, 0x00000011, 0x00000012,
    0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000017, 0x00000018,
    0x00000019, 0x0000001A, 0x0000001B, 0x0000001C, 0x0000001D, 0x0000001E,
    0x0000001F, 0x00000020, 0x00000021, 0x00000022, 0x00000023, 0x00000024,
    0x00000025, 0x00000026, 0x00000027, 0x00000028, 0x00000029, 0x0000002A,
    0x0000002B};

TControlData2 TCsdEft::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0xD548A364, 0x6579, 0x11D0,{ 0x91, 0x3C, 0x44,0x45, 0x53, 0x54,0x00, 0x00} }, // CoClass
  {0xD548A363, 0x6579, 0x11D0,{ 0x91, 0x3C, 0x44,0x45, 0x53, 0x54,0x00, 0x00} }, // Events

  // Count of Events and array of their DISPIDs
  43, &EventDispIDs,

  // Pointer to Runtime License string
  NULL,  // HRESULT(0x80004005)

  // Flags for OnChanged PropertyNotification
  0x00000000,
  401,// (IDE Version)

  // Count of Font Prop and array of their DISPIDs
  0, Activex::PDispIDList(NULL),

  // Count of Pict Prop and array of their DISPIDs
  0, Activex::PDispIDList(NULL),
  0, // Reserved
  0, // Instance count (used internally)
  0, // List of Enum descriptions (internal)
};

GUID     TCsdEft::DEF_CTL_INTF = {0xD548A362, 0x6579, 0x11D0,{ 0x91, 0x3C, 0x44,0x45, 0x53, 0x54,0x00, 0x00} };
TNoParam TCsdEft::OptParam;

static inline void ValidCtrCheck(TCsdEft *)
{
   delete new TCsdEft((TComponent*)(0));
};

void __fastcall TCsdEft::InitControlData()
{
  ControlData = (TControlData*)&CControlData;
  CControlData.FirstEventOfs = System::PByte(&FOnSelfTestEvent) - System::PByte(this);
};

void __fastcall TCsdEft::CreateControl()
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

_DCsdEftDisp __fastcall TCsdEft::GetDefaultInterface()
{
  CreateControl();
  return m_OCXIntf;
};

void __fastcall TCsdEft::AboutBox(void)
{
  GetDefaultInterface()->AboutBox();
}

void __fastcall TCsdEft::DoGetPending(void)
{
  GetDefaultInterface()->DoGetPending();
}

void __fastcall TCsdEft::DoTestComms(void)
{
  GetDefaultInterface()->DoTestComms();
}

void __fastcall TCsdEft::DoSetAutoLogonTime(void)
{
  GetDefaultInterface()->DoSetAutoLogonTime();
}

void __fastcall TCsdEft::DoSetFocus(VARIANT_BOOL EnableFocus)
{
  GetDefaultInterface()->DoSetFocus(EnableFocus);
}

void __fastcall TCsdEft::DoConfigMerchant(void)
{
  GetDefaultInterface()->DoConfigMerchant();
}

void __fastcall TCsdEft::CsdReservedMethod9(void)
{
  GetDefaultInterface()->CsdReservedMethod9();
}

void __fastcall TCsdEft::CsdReservedMethod8(void)
{
  GetDefaultInterface()->CsdReservedMethod8();
}

void __fastcall TCsdEft::CsdReservedMethod7(void)
{
  GetDefaultInterface()->CsdReservedMethod7();
}

void __fastcall TCsdEft::DoDisplaySettlement(void)
{
  GetDefaultInterface()->DoDisplaySettlement();
}

void __fastcall TCsdEft::DoSettlement(void)
{
  GetDefaultInterface()->DoSettlement();
}

void __fastcall TCsdEft::DoShowDialog(VARIANT_BOOL Show)
{
  GetDefaultInterface()->DoShowDialog(Show);
}

void __fastcall TCsdEft::DoSetupPinPad(void)
{
  GetDefaultInterface()->DoSetupPinPad();
}

void __fastcall TCsdEft::DoJournalViewer(void)
{
  GetDefaultInterface()->DoJournalViewer();
}

void __fastcall TCsdEft::DoSoftwareActivate(void)
{
  GetDefaultInterface()->DoSoftwareActivate();
}

void __fastcall TCsdEft::DoSoftwareDownload(void)
{
  GetDefaultInterface()->DoSoftwareDownload();
}

void __fastcall TCsdEft::CsdReservedMethod6(void)
{
  GetDefaultInterface()->CsdReservedMethod6();
}

void __fastcall TCsdEft::CsdReservedMethod5(void)
{
  GetDefaultInterface()->CsdReservedMethod5();
}

void __fastcall TCsdEft::DoTransaction(void)
{
  GetDefaultInterface()->DoTransaction();
}

void __fastcall TCsdEft::DoStatus(void)
{
  GetDefaultInterface()->DoStatus();
}

void __fastcall TCsdEft::DoSettlementReconcile(void)
{
  GetDefaultInterface()->DoSettlementReconcile();
}

void __fastcall TCsdEft::DoSettlementEnquiry(void)
{
  GetDefaultInterface()->DoSettlementEnquiry();
}

void __fastcall TCsdEft::DoSettlementCutover(void)
{
  GetDefaultInterface()->DoSettlementCutover();
}

void __fastcall TCsdEft::DoSelfTest(void)
{
  GetDefaultInterface()->DoSelfTest();
}

void __fastcall TCsdEft::CsdReservedMethod4(void)
{
  GetDefaultInterface()->CsdReservedMethod4();
}

void __fastcall TCsdEft::CsdReservedMethod3(void)
{
  GetDefaultInterface()->CsdReservedMethod3();
}

void __fastcall TCsdEft::DoReprintReceipt(void)
{
  GetDefaultInterface()->DoReprintReceipt();
}

void __fastcall TCsdEft::DoQueryCard(void)
{
  GetDefaultInterface()->DoQueryCard();
}

void __fastcall TCsdEft::DoLogon(void)
{
  GetDefaultInterface()->DoLogon();
}

void __fastcall TCsdEft::DoInvokeApplet(BSTR Applet)
{
  GetDefaultInterface()->DoInvokeApplet(Applet);
}

void __fastcall TCsdEft::CsdReservedMethod2(void)
{
  GetDefaultInterface()->CsdReservedMethod2();
}

void __fastcall TCsdEft::CsdReservedMethod1(void)
{
  GetDefaultInterface()->CsdReservedMethod1();
}

void __fastcall TCsdEft::DoGetLastTransaction(void)
{
  GetDefaultInterface()->DoGetLastTransaction();
}

void __fastcall TCsdEft::DoGetLastReceipt(void)
{
  GetDefaultInterface()->DoGetLastReceipt();
}

void __fastcall TCsdEft::DoDisplayStatus(void)
{
  GetDefaultInterface()->DoDisplayStatus();
}

void __fastcall TCsdEft::DoControlPanel(void)
{
  GetDefaultInterface()->DoControlPanel();
}

void __fastcall TCsdEft::DoChequeVerify(void)
{
  GetDefaultInterface()->DoChequeVerify();
}

void __fastcall TCsdEft::DoCardSwipe(void)
{
  GetDefaultInterface()->DoCardSwipe();
}

void __fastcall TCsdEft::DoReset(void)
{
  GetDefaultInterface()->DoReset();
}


};     // namespace Csdeftlib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Csdeftlib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass cls_ocx[] = {
                              __classid(Csdeftlib_tlb::TCsdEft)
                           };
  RegisterComponents("MenuMate OCX", cls_ocx,
                     sizeof(cls_ocx)/sizeof(cls_ocx[0])-1);
}

};     // namespace Csdeftlib_ocx
