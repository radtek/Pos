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
// File generated on 19/06/2009 11:41:35 a.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\PROGRA~1\Syncro\IcePpc.exe (1)
// LIBID: {A728E2C1-0123-11D4-B49B-00C04F078DAE}
// LCID: 0
// Helpfile: 
// HelpString: PinPadControl2 1.4 Type Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
// Errors:
//   Error creating palette bitmap of (TForegroundPinPadControl2) : Server C:\PROGRA~1\Syncro\IcePpc.exe contains no icons
//   Error creating palette bitmap of (TBackgroundPinPadControl2) : Server C:\PROGRA~1\Syncro\IcePpc.exe contains no icons
//   Error creating palette bitmap of (TConfigPinPadControl2) : Server C:\PROGRA~1\Syncro\IcePpc.exe contains no icons
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#include <oleserver.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "PINPADCONTROL2Lib_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Pinpadcontrol2lib_tlb
{

IForegroundPinPadControl3Ptr& TForegroundPinPadControl2::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TForegroundPinPadControl2::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TForegroundPinPadControl2::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TForegroundPinPadControl2::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TForegroundPinPadControl2::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TForegroundPinPadControl2::ConnectTo(IForegroundPinPadControl3Ptr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TForegroundPinPadControl2::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_ForegroundPinPadControl2;
  sd.IntfIID = __uuidof(IForegroundPinPadControl3);
  sd.EventIID= __uuidof(_IForegroundPinPadControlEvents);
  ServerData = &sd;
}

void __fastcall TForegroundPinPadControl2::InvokeEvent(int id, Oleserver::TVariantArray& params)
{
  switch(id)
  {
    case 1: {
      if (OnPINPadData) {
        (OnPINPadData)(this, params[0]);
      }
      break;
      }
    default:
      break;
  }
}

void __fastcall TForegroundPinPadControl2::SelfTest(void)
{
  GetDefaultInterface()->SelfTest();
}

void __fastcall TForegroundPinPadControl2::Startup(void)
{
  GetDefaultInterface()->Startup();
}

void __fastcall TForegroundPinPadControl2::RequestLock(void)
{
  GetDefaultInterface()->RequestLock();
}

void __fastcall TForegroundPinPadControl2::ReleaseLock(void)
{
  GetDefaultInterface()->ReleaseLock();
}

void __fastcall TForegroundPinPadControl2::DisplayText(BSTR bstrDisplayText/*[in]*/)
{
  GetDefaultInterface()->DisplayText(bstrDisplayText/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestAccountSelection(BSTR bstrPromptText/*[in]*/)
{
  GetDefaultInterface()->RequestAccountSelection(bstrPromptText/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestCardSwipe(BSTR bstrPromptText/*[in]*/, 
                                                            long lTracksToRead/*[in]*/)
{
  GetDefaultInterface()->RequestCardSwipe(bstrPromptText/*[in]*/, lTracksToRead/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestPINEntry(BSTR bstrPromptText/*[in]*/, 
                                                           BSTR bstrPAN/*[in]*/, 
                                                           long bUseMasterKey/*[in]*/, 
                                                           long lKeyIndex/*[in]*/, 
                                                           long lAmount/*[in]*/)
{
  GetDefaultInterface()->RequestPINEntry(bstrPromptText/*[in]*/, bstrPAN/*[in]*/, 
                                         bUseMasterKey/*[in]*/, lKeyIndex/*[in]*/, lAmount/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestPromptEntry(long lPromptIndex/*[in]*/, 
                                                              long lMinimumEntryLength/*[in]*/, 
                                                              long lMaximumEntryLength/*[in]*/)
{
  GetDefaultInterface()->RequestPromptEntry(lPromptIndex/*[in]*/, lMinimumEntryLength/*[in]*/, 
                                            lMaximumEntryLength/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestCancel(void)
{
  GetDefaultInterface()->RequestCancel();
}

void __fastcall TForegroundPinPadControl2::Shutdown(void)
{
  GetDefaultInterface()->Shutdown();
}

void __fastcall TForegroundPinPadControl2::RequestSignatureCapture(void)
{
  GetDefaultInterface()->RequestSignatureCapture();
}

void __fastcall TForegroundPinPadControl2::RequestSendRawData(BSTR bstrRawData/*[in]*/)
{
  GetDefaultInterface()->RequestSendRawData(bstrRawData/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestEMVTransaction(long lActionCode/*[in]*/, 
                                                                 long lStatusBitmap/*[in]*/, 
                                                                 BSTR bstrDisplayText/*[in]*/, 
                                                                 VARIANT varTLV/*[in]*/)
{
  GetDefaultInterface()->RequestEMVTransaction(lActionCode/*[in]*/, lStatusBitmap/*[in]*/, 
                                               bstrDisplayText/*[in]*/, varTLV/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestCardData(BSTR bstrPromptText/*[in]*/, 
                                                           BSTR bstrChipCardPromptText/*[in]*/, 
                                                           long lTracksToRead/*[in]*/, 
                                                           long lReadType/*[in]*/)
{
  GetDefaultInterface()->RequestCardData(bstrPromptText/*[in]*/, bstrChipCardPromptText/*[in]*/, 
                                         lTracksToRead/*[in]*/, lReadType/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestEjectCard(BSTR bstrPromptText/*[in]*/)
{
  GetDefaultInterface()->RequestEjectCard(bstrPromptText/*[in]*/);
}

void __fastcall TForegroundPinPadControl2::RequestFGTransaction(BSTR bstrCommandCode/*[in]*/, 
                                                                BSTR bstrExpectedResponseCode/*[in]*/, 
                                                                long lActionCode/*[in]*/, 
                                                                VARIANT varTLV/*[in]*/)
{
  GetDefaultInterface()->RequestFGTransaction(bstrCommandCode/*[in]*/, 
                                              bstrExpectedResponseCode/*[in]*/, lActionCode/*[in]*/, 
                                              varTLV/*[in]*/);
}

BSTR __fastcall TForegroundPinPadControl2::get_ModuleDescription(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleDescription((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_ModuleName(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleName((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_ModuleVersion(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleVersion((BSTR*)&pVal));
  return pVal;
}

long __fastcall TForegroundPinPadControl2::get_ResultCode(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_ResultCode((long*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_ResultText(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ResultText((BSTR*)&pVal));
  return pVal;
}

long __fastcall TForegroundPinPadControl2::get_AccountSelection(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_AccountSelection((long*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_CustomerInput(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_CustomerInput((BSTR*)&pVal));
  return pVal;
}

VARIANT __fastcall TForegroundPinPadControl2::get_PINBlock(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_PINBlock((VARIANT*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_Track1Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_Track1Data((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_Track2Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_Track2Data((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_Track3Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_Track3Data((BSTR*)&pVal));
  return pVal;
}

VARIANT __fastcall TForegroundPinPadControl2::get_SignatureCaptureData(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_SignatureCaptureData((VARIANT*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_SendRawDataResponse(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_SendRawDataResponse((BSTR*)&pVal));
  return pVal;
}

long __fastcall TForegroundPinPadControl2::get_EMVActionCodeResponse(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_EMVActionCodeResponse((long*)&pVal));
  return pVal;
}

long __fastcall TForegroundPinPadControl2::get_EMVStatusBitmapResponse(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_EMVStatusBitmapResponse((long*)&pVal));
  return pVal;
}

VARIANT __fastcall TForegroundPinPadControl2::get_EMVDebugDataResponse(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_EMVDebugDataResponse((VARIANT*)&pVal));
  return pVal;
}

long __fastcall TForegroundPinPadControl2::get_EMVResponseCode(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_EMVResponseCode((long*)&pVal));
  return pVal;
}

VARIANT __fastcall TForegroundPinPadControl2::get_EMVTLVResponse(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_EMVTLVResponse((VARIANT*)&pVal));
  return pVal;
}

BSTR __fastcall TForegroundPinPadControl2::get_SCData(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_SCData((BSTR*)&pVal));
  return pVal;
}

IBackgroundPinPadControl3Ptr& TBackgroundPinPadControl2::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TBackgroundPinPadControl2::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TBackgroundPinPadControl2::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TBackgroundPinPadControl2::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TBackgroundPinPadControl2::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TBackgroundPinPadControl2::ConnectTo(IBackgroundPinPadControl3Ptr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TBackgroundPinPadControl2::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_BackgroundPinPadControl2;
  sd.IntfIID = __uuidof(IBackgroundPinPadControl3);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

void __fastcall TBackgroundPinPadControl2::SelfTest(void)
{
  GetDefaultInterface()->SelfTest();
}

void __fastcall TBackgroundPinPadControl2::Startup(void)
{
  GetDefaultInterface()->Startup();
}

void __fastcall TBackgroundPinPadControl2::RequestLock(void)
{
  GetDefaultInterface()->RequestLock();
}

void __fastcall TBackgroundPinPadControl2::ReleaseLock(void)
{
  GetDefaultInterface()->ReleaseLock();
}

void __fastcall TBackgroundPinPadControl2::RequestMAC(long bTerminalMAC/*[in]*/, 
                                                      long lMACType/*[in]*/, 
                                                      long bUseMasterKey/*[in]*/, 
                                                      long lKeyIndex/*[in]*/, 
                                                      VARIANT varData/*[in]*/)
{
  GetDefaultInterface()->RequestMAC(bTerminalMAC/*[in]*/, lMACType/*[in]*/, bUseMasterKey/*[in]*/, 
                                    lKeyIndex/*[in]*/, varData/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestPINPadDetails(void)
{
  GetDefaultInterface()->RequestPINPadDetails();
}

void __fastcall TBackgroundPinPadControl2::RequestReversalClear(long lKeyIndex/*[in]*/)
{
  GetDefaultInterface()->RequestReversalClear(lKeyIndex/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestReversalRetrieval(long lKeyIndex/*[in]*/)
{
  GetDefaultInterface()->RequestReversalRetrieval(lKeyIndex/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestReversalStatus(long lMasterKeyIndex/*[in]*/)
{
  GetDefaultInterface()->RequestReversalStatus(lMasterKeyIndex/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestReversalStore(long lKeyIndex/*[in]*/, 
                                                                VARIANT varReversalData/*[in]*/)
{
  GetDefaultInterface()->RequestReversalStore(lKeyIndex/*[in]*/, varReversalData/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::VerifyMAC(long bTerminalMAC/*[in]*/, 
                                                     long lMACType/*[in]*/, 
                                                     long bUseMasterKey/*[in]*/, 
                                                     long lKeyIndex/*[in]*/, VARIANT varData/*[in]*/)
{
  GetDefaultInterface()->VerifyMAC(bTerminalMAC/*[in]*/, lMACType/*[in]*/, bUseMasterKey/*[in]*/, 
                                   lKeyIndex/*[in]*/, varData/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::Shutdown(void)
{
  GetDefaultInterface()->Shutdown();
}

void __fastcall TBackgroundPinPadControl2::StoreWorkingKeys(long lKeyIndex/*[in]*/, 
                                                            VARIANT varMACKeyData/*[in]*/, 
                                                            VARIANT varPINKeyData/*[in]*/)
{
  GetDefaultInterface()->StoreWorkingKeys(lKeyIndex/*[in]*/, varMACKeyData/*[in]*/, 
                                          varPINKeyData/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestImageLoad(long lImageNumber/*[in]*/, 
                                                            VARIANT varImageData/*[in]*/)
{
  GetDefaultInterface()->RequestImageLoad(lImageNumber/*[in]*/, varImageData/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestSendRawData(BSTR bstrRawData/*[in]*/)
{
  GetDefaultInterface()->RequestSendRawData(bstrRawData/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestPINPadLogon(long lKeyIndex/*[in]*/, 
                                                              VARIANT varCurrentPassword/*[in]*/, 
                                                              VARIANT varNewPassword/*[in]*/)
{
  GetDefaultInterface()->RequestPINPadLogon(lKeyIndex/*[in]*/, varCurrentPassword/*[in]*/, 
                                            varNewPassword/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestPINPadMKLoad(long lKeyIndex/*[in]*/, 
                                                               long lDecryptingKeyIndex/*[in]*/, 
                                                               VARIANT varMasterKeyData/*[in]*/, 
                                                               VARIANT varNewPINPadID/*[in]*/)
{
  GetDefaultInterface()->RequestPINPadMKLoad(lKeyIndex/*[in]*/, lDecryptingKeyIndex/*[in]*/, 
                                             varMasterKeyData/*[in]*/, varNewPINPadID/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestPINPadLogoff(void)
{
  GetDefaultInterface()->RequestPINPadLogoff();
}

void __fastcall TBackgroundPinPadControl2::Store3DESWorkingKeys(long lKeyIndex/*[in]*/, 
                                                                VARIANT varMACKeyData/*[in]*/, 
                                                                VARIANT varPINKeyData/*[in]*/)
{
  GetDefaultInterface()->Store3DESWorkingKeys(lKeyIndex/*[in]*/, varMACKeyData/*[in]*/, 
                                              varPINKeyData/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestEMVTransaction(long lActionCode/*[in]*/, 
                                                                 long StatusBitmap/*[in]*/, 
                                                                 VARIANT varTLV/*[in]*/)
{
  GetDefaultInterface()->RequestEMVTransaction(lActionCode/*[in]*/, StatusBitmap/*[in]*/, 
                                               varTLV/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestEMVAdmin(long lActionCode/*[in]*/, 
                                                           long lStatusBitmap/*[in]*/, 
                                                           long lTableID/*[in]*/, 
                                                           long lEntryCount/*[in]*/, 
                                                           VARIANT varTableData/*[in]*/)
{
  GetDefaultInterface()->RequestEMVAdmin(lActionCode/*[in]*/, lStatusBitmap/*[in]*/, 
                                         lTableID/*[in]*/, lEntryCount/*[in]*/, varTableData/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestEMVReadList(VARIANT varTLV/*[in]*/)
{
  GetDefaultInterface()->RequestEMVReadList(varTLV/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestEMVWriteList(VARIANT varTLV/*[in]*/)
{
  GetDefaultInterface()->RequestEMVWriteList(varTLV/*[in]*/);
}

void __fastcall TBackgroundPinPadControl2::RequestBGTransaction(BSTR bstrRequestCode/*[in]*/, 
                                                                BSTR bstrExpectedResponse/*[in]*/, 
                                                                long lActionCode/*[in]*/, 
                                                                VARIANT varTLV/*[in]*/)
{
  GetDefaultInterface()->RequestBGTransaction(bstrRequestCode/*[in]*/, bstrExpectedResponse/*[in]*/, 
                                              lActionCode/*[in]*/, varTLV/*[in]*/);
}

BSTR __fastcall TBackgroundPinPadControl2::get_ModuleDescription(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleDescription((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TBackgroundPinPadControl2::get_ModuleName(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleName((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TBackgroundPinPadControl2::get_ModuleVersion(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleVersion((BSTR*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_ResultCode(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_ResultCode((long*)&pVal));
  return pVal;
}

BSTR __fastcall TBackgroundPinPadControl2::get_ResultText(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ResultText((BSTR*)&pVal));
  return pVal;
}

VARIANT __fastcall TBackgroundPinPadControl2::get_MACBlock(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_MACBlock((VARIANT*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_MACVerifyResult(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_MACVerifyResult((long*)&pVal));
  return pVal;
}

VARIANT __fastcall TBackgroundPinPadControl2::get_PINPadDetails(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_PINPadDetails((VARIANT*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_ReversalStatus(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_ReversalStatus((long*)&pVal));
  return pVal;
}

VARIANT __fastcall TBackgroundPinPadControl2::get_ReversalData(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_ReversalData((VARIANT*)&pVal));
  return pVal;
}

BSTR __fastcall TBackgroundPinPadControl2::get_SendRawDataResponse(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_SendRawDataResponse((BSTR*)&pVal));
  return pVal;
}

VARIANT __fastcall TBackgroundPinPadControl2::get_PINPadID(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_PINPadID((VARIANT*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_KeyLength(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_KeyLength((long*)&pVal));
  return pVal;
}

void __fastcall TBackgroundPinPadControl2::set_KeyLength(long pVal/*[in]*/)
{
  GetDefaultInterface()->set_KeyLength(pVal/*[in]*/);
}

long __fastcall TBackgroundPinPadControl2::get_MACType(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_MACType((long*)&pVal));
  return pVal;
}

void __fastcall TBackgroundPinPadControl2::set_MACType(long pVal/*[in]*/)
{
  GetDefaultInterface()->set_MACType(pVal/*[in]*/);
}

VARIANT __fastcall TBackgroundPinPadControl2::get_KVC(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_KVC((VARIANT*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_Uses3DES(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_Uses3DES((long*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_EMVActionCodeResponse(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_EMVActionCodeResponse((long*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_EMVStatusBitmapResponse(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_EMVStatusBitmapResponse((long*)&pVal));
  return pVal;
}

VARIANT __fastcall TBackgroundPinPadControl2::get_EMVDebugDataResponse(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_EMVDebugDataResponse((VARIANT*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_EMVResponseCode(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_EMVResponseCode((long*)&pVal));
  return pVal;
}

VARIANT __fastcall TBackgroundPinPadControl2::get_EMVTLVResponse(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_EMVTLVResponse((VARIANT*)&pVal));
  return pVal;
}

VARIANT __fastcall TBackgroundPinPadControl2::get_EMVResponseData(void)
{
  VARIANT pVal;
  OLECHECK(GetDefaultInterface()->get_EMVResponseData((VARIANT*)&pVal));
  return pVal;
}

long __fastcall TBackgroundPinPadControl2::get_PINPadDeviceType(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_PINPadDeviceType((long*)&pVal));
  return pVal;
}

IConfigPinPadControlPtr& TConfigPinPadControl2::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TConfigPinPadControl2::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TConfigPinPadControl2::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TConfigPinPadControl2::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TConfigPinPadControl2::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TConfigPinPadControl2::ConnectTo(IConfigPinPadControlPtr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TConfigPinPadControl2::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_ConfigPinPadControl2;
  sd.IntfIID = __uuidof(IConfigPinPadControl);
  sd.EventIID= GUID_NULL;
  ServerData = &sd;
}

void __fastcall TConfigPinPadControl2::Reset(long Immediate)
{
  GetDefaultInterface()->Reset(Immediate);
}


};     // namespace Pinpadcontrol2lib_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Pinpadcontrol2lib_ocx
{

void __fastcall PACKAGE Register()
{
  // [3]
  TComponentClass cls_svr[] = {
                              __classid(Pinpadcontrol2lib_tlb::TForegroundPinPadControl2), 
                              __classid(Pinpadcontrol2lib_tlb::TBackgroundPinPadControl2), 
                              __classid(Pinpadcontrol2lib_tlb::TConfigPinPadControl2)
                           };
  RegisterComponents("MenuMate OCX", cls_svr,
                     sizeof(cls_svr)/sizeof(cls_svr[0])-1);
}

};     // namespace Pinpadcontrol2lib_ocx
