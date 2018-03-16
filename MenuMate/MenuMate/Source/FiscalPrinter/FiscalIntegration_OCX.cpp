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
// File generated on 17/03/2018 12:51:01 a.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\MM_Cloud_Git\V6-Master\Runtime\FiscalIntegration.tlb (1)
// LIBID: {3789DF18-77B2-4441-AF37-74CC219EE719}
// LCID: 0
// Helpfile: 
// HelpString: 
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
//   (2) v2.4 mscorlib, (C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319\mscorlib.tlb)
// Errors:
//   Error creating palette bitmap of (TFiscalLibraryClass) : No Server registered for this CoClass
//   Error creating palette bitmap of (TDiscountDetails) : No Server registered for this CoClass
//   Error creating palette bitmap of (TFiscalDataDetails) : No Server registered for this CoClass
//   Error creating palette bitmap of (TFiscalPayment) : No Server registered for this CoClass
//   Error creating palette bitmap of (TFiscalResponseDetails) : No Server registered for this CoClass
//   Error creating palette bitmap of (TFiscalService) : No Server registered for this CoClass
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#include <oleserver.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "FiscalIntegration_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Fiscalintegration_tlb
{

IFiscalLibraryPtr& TFiscalLibraryClass::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TFiscalLibraryClass::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TFiscalLibraryClass::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TFiscalLibraryClass::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TFiscalLibraryClass::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TFiscalLibraryClass::ConnectTo(IFiscalLibraryPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TFiscalLibraryClass::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_FiscalLibraryClass;
  sd.IntfIID = __uuidof(IFiscalLibrary);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

long __fastcall TFiscalLibraryClass::PrintReceipt(void)
{
  return  GetDefaultInterface()->PrintReceipt();
}

void __fastcall TFiscalLibraryClass::LoadReceiptItemInfo(long identifier/*[in]*/, 
                                                         BSTR InvoiceNumber/*[in]*/)
{
  GetDefaultInterface()->LoadReceiptItemInfo(identifier/*[in]*/, InvoiceNumber/*[in]*/);
}

void __fastcall TFiscalLibraryClass::LoadReceiptPaymentInfo(long identifier/*[in]*/, 
                                                            BSTR InvoiceNumber/*[in]*/)
{
  GetDefaultInterface()->LoadReceiptPaymentInfo(identifier/*[in]*/, InvoiceNumber/*[in]*/);
}

void __fastcall TFiscalLibraryClass::LoadReceiptDiscountInfo(long identifier/*[in]*/, 
                                                             BSTR InvoiceNumber/*[in]*/)
{
  GetDefaultInterface()->LoadReceiptDiscountInfo(identifier/*[in]*/, InvoiceNumber/*[in]*/);
}

void __fastcall TFiscalLibraryClass::InitializeItemProperties(void)
{
  GetDefaultInterface()->InitializeItemProperties();
}

void __fastcall TFiscalLibraryClass::InitializePaymentProperties(void)
{
  GetDefaultInterface()->InitializePaymentProperties();
}

void __fastcall TFiscalLibraryClass::InitializeDiscountProperties(void)
{
  GetDefaultInterface()->InitializeDiscountProperties();
}

void __fastcall TFiscalLibraryClass::AddItemToList(void)
{
  GetDefaultInterface()->AddItemToList();
}

void __fastcall TFiscalLibraryClass::AddPaymentInfoToList(void)
{
  GetDefaultInterface()->AddPaymentInfoToList();
}

void __fastcall TFiscalLibraryClass::AddDiscountInfoToList(void)
{
  GetDefaultInterface()->AddDiscountInfoToList();
}

void __fastcall TFiscalLibraryClass::PrintZReport(void)
{
  GetDefaultInterface()->PrintZReport();
}

BSTR __fastcall TFiscalLibraryClass::get_InvoiceNumber(void)
{
  return  GetDefaultInterface()->get_InvoiceNumber();
}

void __fastcall TFiscalLibraryClass::set_InvoiceNumber(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_InvoiceNumber(Param1/*[in]*/);
}

BSTR __fastcall TFiscalLibraryClass::get_Date(void)
{
  return  GetDefaultInterface()->get_Date();
}

void __fastcall TFiscalLibraryClass::set_Date(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_Date(Param1/*[in]*/);
}

BSTR __fastcall TFiscalLibraryClass::get_Time(void)
{
  return  GetDefaultInterface()->get_Time();
}

void __fastcall TFiscalLibraryClass::set_Time(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_Time(Param1/*[in]*/);
}

BSTR __fastcall TFiscalLibraryClass::get_Billno(void)
{
  return  GetDefaultInterface()->get_Billno();
}

void __fastcall TFiscalLibraryClass::set_Billno(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_Billno(Param1/*[in]*/);
}

BSTR __fastcall TFiscalLibraryClass::get_Cashno(void)
{
  return  GetDefaultInterface()->get_Cashno();
}

void __fastcall TFiscalLibraryClass::set_Cashno(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_Cashno(Param1/*[in]*/);
}

BSTR __fastcall TFiscalLibraryClass::get_Cashier(void)
{
  return  GetDefaultInterface()->get_Cashier();
}

void __fastcall TFiscalLibraryClass::set_Cashier(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_Cashier(Param1/*[in]*/);
}

BSTR __fastcall TFiscalLibraryClass::get_Source(void)
{
  return  GetDefaultInterface()->get_Source();
}

void __fastcall TFiscalLibraryClass::set_Source(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_Source(Param1/*[in]*/);
}

_DiscountDetailsPtr& TDiscountDetails::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TDiscountDetails::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TDiscountDetails::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TDiscountDetails::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TDiscountDetails::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TDiscountDetails::ConnectTo(_DiscountDetailsPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TDiscountDetails::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_DiscountDetails;
  sd.IntfIID = __uuidof(_DiscountDetails);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

_FiscalDataDetailsPtr& TFiscalDataDetails::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TFiscalDataDetails::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TFiscalDataDetails::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TFiscalDataDetails::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TFiscalDataDetails::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TFiscalDataDetails::ConnectTo(_FiscalDataDetailsPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TFiscalDataDetails::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_FiscalDataDetails;
  sd.IntfIID = __uuidof(_FiscalDataDetails);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

_FiscalPaymentPtr& TFiscalPayment::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TFiscalPayment::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TFiscalPayment::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TFiscalPayment::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TFiscalPayment::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TFiscalPayment::ConnectTo(_FiscalPaymentPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TFiscalPayment::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_FiscalPayment;
  sd.IntfIID = __uuidof(_FiscalPayment);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

_FiscalResponseDetailsPtr& TFiscalResponseDetails::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TFiscalResponseDetails::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TFiscalResponseDetails::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TFiscalResponseDetails::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TFiscalResponseDetails::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TFiscalResponseDetails::ConnectTo(_FiscalResponseDetailsPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TFiscalResponseDetails::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_FiscalResponseDetails;
  sd.IntfIID = __uuidof(_FiscalResponseDetails);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

_FiscalServicePtr& TFiscalService::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TFiscalService::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TFiscalService::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TFiscalService::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TFiscalService::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TFiscalService::ConnectTo(_FiscalServicePtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TFiscalService::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_FiscalService;
  sd.IntfIID = __uuidof(_FiscalService);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}


};     // namespace Fiscalintegration_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Fiscalintegration_ocx
{

void __fastcall PACKAGE Register()
{
  // [6]
  TComponentClass cls_svr[] = {
                              __classid(Fiscalintegration_tlb::TFiscalLibraryClass), 
                              __classid(Fiscalintegration_tlb::TDiscountDetails), 
                              __classid(Fiscalintegration_tlb::TFiscalDataDetails), 
                              __classid(Fiscalintegration_tlb::TFiscalPayment), 
                              __classid(Fiscalintegration_tlb::TFiscalResponseDetails), 
                              __classid(Fiscalintegration_tlb::TFiscalService)
                           };
  RegisterComponents("ActiveX", cls_svr,
                     sizeof(cls_svr)/sizeof(cls_svr[0])-1);
}

};     // namespace Fiscalintegration_ocx
