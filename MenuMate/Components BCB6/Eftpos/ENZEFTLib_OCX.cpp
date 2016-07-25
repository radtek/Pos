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
// File generated on 14/12/2010 2:51:56 p.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\Windows\SysWow64\ENZEFT.ocx (1)
// LIBID: {4D7EB042-6114-4092-B1FF-BCD4B2939D91}
// LCID: 0
// Helpfile: 
// HelpString: ENZEFT OLE Control module
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

#include "ENZEFTLib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Enzeftlib_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TENZEFT which
// allows "ENZEFT Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TENZEFT::EventDispIDs[43] = {
    0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006,
    0x00000007, 0x00000008, 0x00000009, 0x0000000A, 0x0000000B, 0x0000000C,
    0x0000000D, 0x0000000E, 0x0000000F, 0x00000010, 0x00000011, 0x00000012,
    0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000017, 0x00000018,
    0x00000019, 0x0000001A, 0x0000001B, 0x0000001C, 0x0000001D, 0x0000001E,
    0x0000001F, 0x00000020, 0x00000021, 0x00000022, 0x00000023, 0x00000024,
    0x00000025, 0x00000026, 0x00000027, 0x00000028, 0x00000029, 0x0000002A,
    0x0000002B};

TControlData2 TENZEFT::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0xC495607D, 0x4428, 0x4085,{ 0xAB, 0x88, 0x92,0x98, 0x4D, 0x5E,0x15, 0x93} }, // CoClass
  {0x5E7AD773, 0x28B0, 0x4A3F,{ 0x9E, 0xC8, 0x6F,0x21, 0xB9, 0x1F,0x66, 0x2E} }, // Events

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

GUID     TENZEFT::DEF_CTL_INTF = {0x74481918, 0xEBF7, 0x4D38,{ 0x8D, 0x64, 0xD2,0x96, 0x27, 0x75,0xC5, 0x29} };
TNoParam TENZEFT::OptParam;

static inline void ValidCtrCheck(TENZEFT *)
{
   delete new TENZEFT((TComponent*)(0));
};

void __fastcall TENZEFT::InitControlData()
{
  ControlData = (TControlData*)&CControlData;
  CControlData.FirstEventOfs = System::PByte(&FOnSelfTestEvent) - System::PByte(this);
};

void __fastcall TENZEFT::CreateControl()
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

_DENZEFTDisp __fastcall TENZEFT::GetDefaultInterface()
{
  CreateControl();
  return m_OCXIntf;
};

void __fastcall TENZEFT::AboutBox(void)
{
  GetDefaultInterface()->AboutBox();
}

void __fastcall TENZEFT::DoGetPending(void)
{
  GetDefaultInterface()->DoGetPending();
}

void __fastcall TENZEFT::DoTestComms(void)
{
  GetDefaultInterface()->DoTestComms();
}

void __fastcall TENZEFT::DoSetAutoLogonTime(void)
{
  GetDefaultInterface()->DoSetAutoLogonTime();
}

void __fastcall TENZEFT::DoSetFocus(VARIANT_BOOL EnableFocus)
{
  GetDefaultInterface()->DoSetFocus(EnableFocus);
}

void __fastcall TENZEFT::DoConfigMerchant(void)
{
  GetDefaultInterface()->DoConfigMerchant();
}

void __fastcall TENZEFT::CsdReservedMethod6(void)
{
  GetDefaultInterface()->CsdReservedMethod6();
}

void __fastcall TENZEFT::CsdReservedMethod5(void)
{
  GetDefaultInterface()->CsdReservedMethod5();
}

void __fastcall TENZEFT::DoAddTip(void)
{
  GetDefaultInterface()->DoAddTip();
}

void __fastcall TENZEFT::DoUploadTip(void)
{
  GetDefaultInterface()->DoUploadTip();
}

void __fastcall TENZEFT::DoGetMerchants(void)
{
  GetDefaultInterface()->DoGetMerchants();
}

void __fastcall TENZEFT::DoShowDialog(VARIANT_BOOL Show)
{
  GetDefaultInterface()->DoShowDialog(Show);
}

void __fastcall TENZEFT::DoVoidTip(void)
{
  GetDefaultInterface()->DoVoidTip();
}

void __fastcall TENZEFT::DoJournalViewer(void)
{
  GetDefaultInterface()->DoJournalViewer();
}

void __fastcall TENZEFT::DoSoftwareActivate(void)
{
  GetDefaultInterface()->DoSoftwareActivate();
}

void __fastcall TENZEFT::DoSoftwareDownload(void)
{
  GetDefaultInterface()->DoSoftwareDownload();
}

void __fastcall TENZEFT::DoLastSettlement(void)
{
  GetDefaultInterface()->DoLastSettlement();
}

void __fastcall TENZEFT::DoSubTotals(void)
{
  GetDefaultInterface()->DoSubTotals();
}

void __fastcall TENZEFT::DoTransaction(void)
{
  GetDefaultInterface()->DoTransaction();
}

void __fastcall TENZEFT::DoStatus(void)
{
  GetDefaultInterface()->DoStatus();
}

void __fastcall TENZEFT::DoSettlementReconcile(void)
{
  GetDefaultInterface()->DoSettlementReconcile();
}

void __fastcall TENZEFT::DoSettlementEnquiry(void)
{
  GetDefaultInterface()->DoSettlementEnquiry();
}

void __fastcall TENZEFT::DoSettlementCutover(void)
{
  GetDefaultInterface()->DoSettlementCutover();
}

void __fastcall TENZEFT::DoSelfTest(void)
{
  GetDefaultInterface()->DoSelfTest();
}

void __fastcall TENZEFT::DoReprintReceiptFromTxnRef(void)
{
  GetDefaultInterface()->DoReprintReceiptFromTxnRef();
}

void __fastcall TENZEFT::DoReprintReceiptFromStan(void)
{
  GetDefaultInterface()->DoReprintReceiptFromStan();
}

void __fastcall TENZEFT::DoReprintReceipt(void)
{
  GetDefaultInterface()->DoReprintReceipt();
}

void __fastcall TENZEFT::DoQueryCard(void)
{
  GetDefaultInterface()->DoQueryCard();
}

void __fastcall TENZEFT::DoLogon(void)
{
  GetDefaultInterface()->DoLogon();
}

void __fastcall TENZEFT::DoInvokeApplet(BSTR Applet)
{
  GetDefaultInterface()->DoInvokeApplet(Applet);
}

void __fastcall TENZEFT::DoGetReceiptFromTxnRef(void)
{
  GetDefaultInterface()->DoGetReceiptFromTxnRef();
}

void __fastcall TENZEFT::DoSearchTip(void)
{
  GetDefaultInterface()->DoSearchTip();
}

void __fastcall TENZEFT::DoGetLastTransaction(void)
{
  GetDefaultInterface()->DoGetLastTransaction();
}

void __fastcall TENZEFT::DoGetLastReceipt(void)
{
  GetDefaultInterface()->DoGetLastReceipt();
}

void __fastcall TENZEFT::DoDisplayStatus(void)
{
  GetDefaultInterface()->DoDisplayStatus();
}

void __fastcall TENZEFT::DoControlPanel(void)
{
  GetDefaultInterface()->DoControlPanel();
}

void __fastcall TENZEFT::DoChequeVerify(void)
{
  GetDefaultInterface()->DoChequeVerify();
}

void __fastcall TENZEFT::DoCardSwipe(void)
{
  GetDefaultInterface()->DoCardSwipe();
}

void __fastcall TENZEFT::DoReset(void)
{
  GetDefaultInterface()->DoReset();
}


};     // namespace Enzeftlib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Enzeftlib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass cls_ocx[] = {
                              __classid(Enzeftlib_tlb::TENZEFT)
                           };
  RegisterComponents("MenuMate OCX", cls_ocx,
                     sizeof(cls_ocx)/sizeof(cls_ocx[0])-1);
}

};     // namespace Enzeftlib_ocx
