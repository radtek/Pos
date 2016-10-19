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
// File generated on 15/06/2009 11:35:39 a.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\Program Files\DPS\Eftpos\DPSEFTX.OCX (1)
// LIBID: {A9EA3083-C360-4817-B2FD-0527DC103656}
// LCID: 0
// Helpfile: C:\Program Files\DPS\Eftpos\DpsEftX.hlp
// HelpString: DpsEftX ActiveX Control module
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

#include "DPSEFTXLib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Dpseftxlib_tlb
{



// *********************************************************************//
// OCX PROXY CLASS IMPLEMENTATION
// (The following variables/methods implement the class TDpsEftX which
// allows "DpsEftX Control" to be hosted in CBuilder IDE/apps).
// *********************************************************************//
int   TDpsEftX::EventDispIDs[24] = {
    0x00000001, 0x00000002, 0x00000003, 0x00000004, 0x00000005, 0x00000006,
    0x00000007, 0x00000008, 0x00000009, 0x0000000A, 0x0000000B, 0x0000000C,
    0x0000000D, 0x0000000E, 0x0000000F, 0x00000010, 0x00000011, 0x00000012,
    0x00000013, 0x00000014, 0x00000015, 0x00000016, 0x00000017, 0x00000018};

int TDpsEftX::TFontIDs[1] = {
    0xFFFFFE00
};

TControlData2 TDpsEftX::CControlData =
{
  // GUID of CoClass and Event Interface of Control
  {0x92EDD80C, 0xB50B, 0x4B85,{ 0x9A, 0xE9, 0x9B,0x3E, 0x6C, 0x63,0xDE, 0xD9} }, // CoClass
  {0x07C85829, 0xF18B, 0x47B5,{ 0x9C, 0x23, 0x12,0x29, 0x39, 0x69,0x8B, 0x4C} }, // Events

  // Count of Events and array of their DISPIDs
  24, &EventDispIDs,

  // Pointer to Runtime License string
  NULL,  // HRESULT(0x80004005)

  // Flags for OnChanged PropertyNotification
  0x0000001F,
  401,// (IDE Version)

  // Count of Font Prop and array of their DISPIDs
  1, Activex::PDispIDList(TFontIDs),

  // Count of Pict Prop and array of their DISPIDs
  0, Activex::PDispIDList(NULL),
  0, // Reserved
  0, // Instance count (used internally)
  0, // List of Enum descriptions (internal)
};

GUID     TDpsEftX::DEF_CTL_INTF = {0x64DFACC5, 0x67A9, 0x4C67,{ 0x82, 0x2A, 0x33,0x3E, 0xCD, 0xE4,0xB4, 0xC9} };
TNoParam TDpsEftX::OptParam;

static inline void ValidCtrCheck(TDpsEftX *)
{
   delete new TDpsEftX((TComponent*)(0));
};

void __fastcall TDpsEftX::InitControlData()
{
  ControlData = (TControlData*)&CControlData;
  CControlData.FirstEventOfs = System::PByte(&FOnPinpadDisplayStartEvent) - System::PByte(this);
};

void __fastcall TDpsEftX::CreateControl()
{
  if (!m_OCXIntf)
  {
    _ASSERTE(DefaultDispatch);
    DefaultDispatch->QueryInterface(DEF_CTL_INTF, (LPVOID*)&m_OCXIntf);
  }
};

_DDpsEftXDisp __fastcall TDpsEftX::GetDefaultInterface()
{
  CreateControl();
  return m_OCXIntf;
};

void __fastcall TDpsEftX::DoLogEvent(BSTR ReCo, BSTR ExtraData)
{
  GetDefaultInterface()->DoLogEvent(ReCo, ExtraData);
}

void __fastcall TDpsEftX::DoAddLineItem(BSTR ProductCode, BSTR Amount, long Quantity)
{
  GetDefaultInterface()->DoAddLineItem(ProductCode, Amount, Quantity);
}

void __fastcall TDpsEftX::AboutBox(void)
{
  GetDefaultInterface()->AboutBox();
}

void __fastcall TDpsEftX::DoSetIdlePrompt(void)
{
  GetDefaultInterface()->DoSetIdlePrompt();
}

void __fastcall TDpsEftX::DoEnterData(void)
{
  GetDefaultInterface()->DoEnterData();
}

void __fastcall TDpsEftX::DoReadCard(void)
{
  GetDefaultInterface()->DoReadCard();
}

void __fastcall TDpsEftX::DoSettlement(void)
{
  GetDefaultInterface()->DoSettlement();
}

void __fastcall TDpsEftX::DoUserInterfaceAction(void)
{
  GetDefaultInterface()->DoUserInterfaceAction();
}

void __fastcall TDpsEftX::DoLogon(void)
{
  GetDefaultInterface()->DoLogon();
}

void __fastcall TDpsEftX::DoInitialize(void)
{
  GetDefaultInterface()->DoInitialize();
}

void __fastcall TDpsEftX::DoEditTender(void)
{
  GetDefaultInterface()->DoEditTender();
}

void __fastcall TDpsEftX::DoPrintPendingReceipt(void)
{
  GetDefaultInterface()->DoPrintPendingReceipt();
}

void __fastcall TDpsEftX::DoAddProductLine(void)
{
  GetDefaultInterface()->DoAddProductLine();
}

/*void __fastcall TDpsEftX::DoreservedMethod11(void)
{
  GetDefaultInterface()->DoreservedMethod11();
}*/

void __fastcall TDpsEftX::DoReservedMethod5(void)
{
  GetDefaultInterface()->DoReservedMethod5();
}

void __fastcall TDpsEftX::ResetLineItems(void)
{
  GetDefaultInterface()->ResetLineItems();
}

void __fastcall TDpsEftX::DoPinPadDisplay(void)
{
  GetDefaultInterface()->DoPinPadDisplay();
}

void __fastcall TDpsEftX::DoRefreshDpsEmvConfig(void)
{
  GetDefaultInterface()->DoRefreshDpsEmvConfig();
}

void __fastcall TDpsEftX::DoReservedMethod6(void)
{
  GetDefaultInterface()->DoReservedMethod6();
}

void __fastcall TDpsEftX::DoReservedMethod4(void)
{
  GetDefaultInterface()->DoReservedMethod4();
}

void __fastcall TDpsEftX::DoReservedMethod3(void)
{
  GetDefaultInterface()->DoReservedMethod3();
}

void __fastcall TDpsEftX::DoReservedMethod2(void)
{
  GetDefaultInterface()->DoReservedMethod2();
}

void __fastcall TDpsEftX::DoVoidLastTransaction(void)
{
  GetDefaultInterface()->DoVoidLastTransaction();
}

void __fastcall TDpsEftX::DoGetLastTransaction(void)
{
  GetDefaultInterface()->DoGetLastTransaction();
}

void __fastcall TDpsEftX::DoGetLastReceipt(void)
{
  GetDefaultInterface()->DoGetLastReceipt();
}

void __fastcall TDpsEftX::DoMaint(void)
{
  GetDefaultInterface()->DoMaint();
}

void __fastcall TDpsEftX::DoConfig(void)
{
  GetDefaultInterface()->DoConfig();
}

void __fastcall TDpsEftX::DoAuthorize(void)
{
  GetDefaultInterface()->DoAuthorize();
}


};     // namespace Dpseftxlib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Dpseftxlib_ocx
{

void __fastcall PACKAGE Register()
{
  // [1]
  TComponentClass cls_ocx[] = {
                              __classid(Dpseftxlib_tlb::TDpsEftX)
                           };
  RegisterComponents("MenuMate OCX", cls_ocx,
                     sizeof(cls_ocx)/sizeof(cls_ocx[0])-1);
}

};     // namespace Dpseftxlib_ocx
