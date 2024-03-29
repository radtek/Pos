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
// ************************************************************************ //
#ifndef   PINPADCONTROL2Lib_OCXH
#define   PINPADCONTROL2Lib_OCXH

#pragma option push -b -a4 -w-inl

#include <olectrls.hpp>
#include <oleserver.hpp>
#if !defined(__UTILCLS_H)
#include <utilcls.h>
#endif
#if !defined(__UTILCLS_H_VERSION) || (__UTILCLS_H_VERSION < 0x0700)
//
// The code generated by the TLIBIMP utility or the Import|TypeLibrary 
// and Import|ActiveX feature of C++Builder rely on specific versions of
// the header file UTILCLS.H found in the INCLUDE\VCL directory. If an 
// older version of the file is detected, you probably need an update/patch.
//
#error "This file requires a newer version of the header UTILCLS.H" \
       "You need to apply an update/patch to your copy of C++Builder"
#endif
#include <olectl.h>
#include <ocidl.h>
#if !defined(_NO_VCL)
#include <stdvcl.hpp>
#endif  //   _NO_VCL
#include <ocxproxy.h>

#include "PINPADCONTROL2Lib_TLB.h"
namespace Pinpadcontrol2lib_tlb
{

// *********************************************************************//
// HelpString: PinPadControl2 1.4 Type Library
// Version:    1.4
// *********************************************************************//


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TForegroundPinPadControl2
// Help String      : ForegroundPinPadControl Class
// Default Interface: IForegroundPinPadControl3
// Def. Intf. Object: IForegroundPinPadControl3Ptr
// Def. Intf. DISP? : No
// Event   Interface: _IForegroundPinPadControlEvents
// TypeFlags        : (2) CanCreate
// *********************************************************************//
typedef void __fastcall (__closure * TForegroundPinPadControl2PINPadData)(System::TObject * Sender, 
                                                                          long lEventType/*[in]*/);

class PACKAGE TForegroundPinPadControl2 : public Oleserver::TOleServer
{
  IForegroundPinPadControl3Ptr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TForegroundPinPadControl2(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  IForegroundPinPadControl3Ptr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(IForegroundPinPadControl3Ptr intf);

  virtual void __fastcall InvokeEvent(int DispID, Oleserver::TVariantArray& params);

  BSTR            __fastcall get_ModuleDescription(void);
  BSTR            __fastcall get_ModuleName(void);
  BSTR            __fastcall get_ModuleVersion(void);
  long            __fastcall get_ResultCode(void);
  BSTR            __fastcall get_ResultText(void);
  long            __fastcall get_AccountSelection(void);
  BSTR            __fastcall get_CustomerInput(void);
  VARIANT         __fastcall get_PINBlock(void);
  BSTR            __fastcall get_Track1Data(void);
  BSTR            __fastcall get_Track2Data(void);
  BSTR            __fastcall get_Track3Data(void);
  VARIANT         __fastcall get_SignatureCaptureData(void);
  BSTR            __fastcall get_SendRawDataResponse(void);
  long            __fastcall get_EMVActionCodeResponse(void);
  long            __fastcall get_EMVStatusBitmapResponse(void);
  VARIANT         __fastcall get_EMVDebugDataResponse(void);
  long            __fastcall get_EMVResponseCode(void);
  VARIANT         __fastcall get_EMVTLVResponse(void);
  BSTR            __fastcall get_SCData(void);
  void            __fastcall SelfTest(void);
  void            __fastcall Startup(void);
  void            __fastcall RequestLock(void);
  void            __fastcall ReleaseLock(void);
  void            __fastcall DisplayText(BSTR bstrDisplayText/*[in]*/);
  void            __fastcall RequestAccountSelection(BSTR bstrPromptText/*[in]*/);
  void            __fastcall RequestCardSwipe(BSTR bstrPromptText/*[in]*/, 
                                              long lTracksToRead/*[in]*/);
  void            __fastcall RequestPINEntry(BSTR bstrPromptText/*[in]*/, BSTR bstrPAN/*[in]*/, 
                                             long bUseMasterKey/*[in]*/, long lKeyIndex/*[in]*/, 
                                             long lAmount/*[in]*/);
  void            __fastcall RequestPromptEntry(long lPromptIndex/*[in]*/, 
                                                long lMinimumEntryLength/*[in]*/, 
                                                long lMaximumEntryLength/*[in]*/);
  void            __fastcall RequestCancel(void);
  void            __fastcall Shutdown(void);
  void            __fastcall RequestSignatureCapture(void);
  void            __fastcall RequestSendRawData(BSTR bstrRawData/*[in]*/);
  void            __fastcall RequestEMVTransaction(long lActionCode/*[in]*/, 
                                                   long lStatusBitmap/*[in]*/, 
                                                   BSTR bstrDisplayText/*[in]*/, 
                                                   VARIANT varTLV/*[in]*/);
  void            __fastcall RequestCardData(BSTR bstrPromptText/*[in]*/, 
                                             BSTR bstrChipCardPromptText/*[in]*/, 
                                             long lTracksToRead/*[in]*/, long lReadType/*[in]*/);
  void            __fastcall RequestEjectCard(BSTR bstrPromptText/*[in]*/);
  void            __fastcall RequestFGTransaction(BSTR bstrCommandCode/*[in]*/, 
                                                  BSTR bstrExpectedResponseCode/*[in]*/, 
                                                  long lActionCode/*[in]*/, VARIANT varTLV/*[in]*/);
  __property BSTR ModuleDescription={ read=get_ModuleDescription };
  __property BSTR ModuleName={ read=get_ModuleName };
  __property BSTR ModuleVersion={ read=get_ModuleVersion };
  __property long ResultCode={ read=get_ResultCode };
  __property BSTR ResultText={ read=get_ResultText };
  __property long AccountSelection={ read=get_AccountSelection };
  __property BSTR CustomerInput={ read=get_CustomerInput };
  __property VARIANT PINBlock={ read=get_PINBlock };
  __property BSTR Track1Data={ read=get_Track1Data };
  __property BSTR Track2Data={ read=get_Track2Data };
  __property BSTR Track3Data={ read=get_Track3Data };
  __property VARIANT SignatureCaptureData={ read=get_SignatureCaptureData };
  __property BSTR SendRawDataResponse={ read=get_SendRawDataResponse };
  __property long EMVActionCodeResponse={ read=get_EMVActionCodeResponse };
  __property long EMVStatusBitmapResponse={ read=get_EMVStatusBitmapResponse };
  __property VARIANT EMVDebugDataResponse={ read=get_EMVDebugDataResponse };
  __property long EMVResponseCode={ read=get_EMVResponseCode };
  __property VARIANT EMVTLVResponse={ read=get_EMVTLVResponse };
  __property BSTR SCData={ read=get_SCData };
private:
  TForegroundPinPadControl2PINPadData FOnPINPadData;
__published:
  __property TForegroundPinPadControl2PINPadData OnPINPadData={ read=FOnPINPadData, write=FOnPINPadData };
};


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TBackgroundPinPadControl2
// Help String      : BackgroundPinPadControl Class
// Default Interface: IBackgroundPinPadControl3
// Def. Intf. Object: IBackgroundPinPadControl3Ptr
// Def. Intf. DISP? : No
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TBackgroundPinPadControl2 : public Oleserver::TOleServer
{
  IBackgroundPinPadControl3Ptr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TBackgroundPinPadControl2(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  IBackgroundPinPadControl3Ptr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(IBackgroundPinPadControl3Ptr intf);


  BSTR            __fastcall get_ModuleDescription(void);
  BSTR            __fastcall get_ModuleName(void);
  BSTR            __fastcall get_ModuleVersion(void);
  long            __fastcall get_ResultCode(void);
  BSTR            __fastcall get_ResultText(void);
  VARIANT         __fastcall get_MACBlock(void);
  long            __fastcall get_MACVerifyResult(void);
  VARIANT         __fastcall get_PINPadDetails(void);
  long            __fastcall get_ReversalStatus(void);
  VARIANT         __fastcall get_ReversalData(void);
  BSTR            __fastcall get_SendRawDataResponse(void);
  VARIANT         __fastcall get_PINPadID(void);
  long            __fastcall get_KeyLength(void);
  void            __fastcall set_KeyLength(long pVal/*[in]*/);
  long            __fastcall get_MACType(void);
  void            __fastcall set_MACType(long pVal/*[in]*/);
  VARIANT         __fastcall get_KVC(void);
  long            __fastcall get_Uses3DES(void);
  long            __fastcall get_EMVActionCodeResponse(void);
  long            __fastcall get_EMVStatusBitmapResponse(void);
  VARIANT         __fastcall get_EMVDebugDataResponse(void);
  long            __fastcall get_EMVResponseCode(void);
  VARIANT         __fastcall get_EMVTLVResponse(void);
  VARIANT         __fastcall get_EMVResponseData(void);
  long            __fastcall get_PINPadDeviceType(void);
  void            __fastcall SelfTest(void);
  void            __fastcall Startup(void);
  void            __fastcall RequestLock(void);
  void            __fastcall ReleaseLock(void);
  void            __fastcall RequestMAC(long bTerminalMAC/*[in]*/, long lMACType/*[in]*/, 
                                        long bUseMasterKey/*[in]*/, long lKeyIndex/*[in]*/, 
                                        VARIANT varData/*[in]*/);
  void            __fastcall RequestPINPadDetails(void);
  void            __fastcall RequestReversalClear(long lKeyIndex/*[in]*/);
  void            __fastcall RequestReversalRetrieval(long lKeyIndex/*[in]*/);
  void            __fastcall RequestReversalStatus(long lMasterKeyIndex/*[in]*/);
  void            __fastcall RequestReversalStore(long lKeyIndex/*[in]*/, 
                                                  VARIANT varReversalData/*[in]*/);
  void            __fastcall VerifyMAC(long bTerminalMAC/*[in]*/, long lMACType/*[in]*/, 
                                       long bUseMasterKey/*[in]*/, long lKeyIndex/*[in]*/, 
                                       VARIANT varData/*[in]*/);
  void            __fastcall Shutdown(void);
  void            __fastcall StoreWorkingKeys(long lKeyIndex/*[in]*/, VARIANT varMACKeyData/*[in]*/, 
                                              VARIANT varPINKeyData/*[in]*/);
  void            __fastcall RequestImageLoad(long lImageNumber/*[in]*/, 
                                              VARIANT varImageData/*[in]*/);
  void            __fastcall RequestSendRawData(BSTR bstrRawData/*[in]*/);
  void            __fastcall RequestPINPadLogon(long lKeyIndex/*[in]*/, 
                                                VARIANT varCurrentPassword/*[in]*/, 
                                                VARIANT varNewPassword/*[in]*/);
  void            __fastcall RequestPINPadMKLoad(long lKeyIndex/*[in]*/, 
                                                 long lDecryptingKeyIndex/*[in]*/, 
                                                 VARIANT varMasterKeyData/*[in]*/, 
                                                 VARIANT varNewPINPadID/*[in]*/);
  void            __fastcall RequestPINPadLogoff(void);
  void            __fastcall Store3DESWorkingKeys(long lKeyIndex/*[in]*/, 
                                                  VARIANT varMACKeyData/*[in]*/, 
                                                  VARIANT varPINKeyData/*[in]*/);
  void            __fastcall RequestEMVTransaction(long lActionCode/*[in]*/, 
                                                   long StatusBitmap/*[in]*/, VARIANT varTLV/*[in]*/);
  void            __fastcall RequestEMVAdmin(long lActionCode/*[in]*/, long lStatusBitmap/*[in]*/, 
                                             long lTableID/*[in]*/, long lEntryCount/*[in]*/, 
                                             VARIANT varTableData/*[in]*/);
  void            __fastcall RequestEMVReadList(VARIANT varTLV/*[in]*/);
  void            __fastcall RequestEMVWriteList(VARIANT varTLV/*[in]*/);
  void            __fastcall RequestBGTransaction(BSTR bstrRequestCode/*[in]*/, 
                                                  BSTR bstrExpectedResponse/*[in]*/, 
                                                  long lActionCode/*[in]*/, VARIANT varTLV/*[in]*/);
  __property BSTR ModuleDescription={ read=get_ModuleDescription };
  __property BSTR ModuleName={ read=get_ModuleName };
  __property BSTR ModuleVersion={ read=get_ModuleVersion };
  __property long ResultCode={ read=get_ResultCode };
  __property BSTR ResultText={ read=get_ResultText };
  __property VARIANT MACBlock={ read=get_MACBlock };
  __property long MACVerifyResult={ read=get_MACVerifyResult };
  __property VARIANT PINPadDetails={ read=get_PINPadDetails };
  __property long ReversalStatus={ read=get_ReversalStatus };
  __property VARIANT ReversalData={ read=get_ReversalData };
  __property BSTR SendRawDataResponse={ read=get_SendRawDataResponse };
  __property VARIANT PINPadID={ read=get_PINPadID };
  __property VARIANT KVC={ read=get_KVC };
  __property long Uses3DES={ read=get_Uses3DES };
  __property long EMVActionCodeResponse={ read=get_EMVActionCodeResponse };
  __property long EMVStatusBitmapResponse={ read=get_EMVStatusBitmapResponse };
  __property VARIANT EMVDebugDataResponse={ read=get_EMVDebugDataResponse };
  __property long EMVResponseCode={ read=get_EMVResponseCode };
  __property VARIANT EMVTLVResponse={ read=get_EMVTLVResponse };
  __property VARIANT EMVResponseData={ read=get_EMVResponseData };
  __property long PINPadDeviceType={ read=get_PINPadDeviceType };
  __property long KeyLength={ read=get_KeyLength, write=set_KeyLength, stored=false };
  __property long MACType={ read=get_MACType, write=set_MACType, stored=false };
};


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TConfigPinPadControl2
// Help String      : ConfigPinPadControl Class
// Default Interface: IConfigPinPadControl
// Def. Intf. Object: IConfigPinPadControlPtr
// Def. Intf. DISP? : No
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TConfigPinPadControl2 : public Oleserver::TOleServer
{
  IConfigPinPadControlPtr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TConfigPinPadControl2(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  IConfigPinPadControlPtr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(IConfigPinPadControlPtr intf);


  void            __fastcall Reset(long Immediate);
};

};     // namespace Pinpadcontrol2lib_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Pinpadcontrol2lib_tlb;
#endif

#pragma option pop

#endif // PINPADCONTROL2Lib_OCXH
