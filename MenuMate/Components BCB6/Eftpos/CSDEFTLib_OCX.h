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
#ifndef   CSDEFTLib_OCXH
#define   CSDEFTLib_OCXH

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

#include "CSDEFTLib_TLB.h"
namespace Csdeftlib_tlb
{

// *********************************************************************//
// HelpString: CsdEft OLE Control module
// Version:    1.0
// *********************************************************************//


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TCsdEft
// Help String      : CsdEft Control
// Default Interface: _DCsdEft
// Def. Intf. Object: _DCsdEftDisp
// Def. Intf. DISP? : Yes
// Event   Interface: _DCsdEftEvents
// TypeFlags        : (34) CanCreate Control
// *********************************************************************//

// *********************************************************************//
// Definition of closures to allow VCL handlers to catch OCX events.      
// *********************************************************************//
typedef void __fastcall (__closure * TCsdEftInProgress)(System::TObject * Sender, 
                                                        VARIANT_BOOL InProgress);
typedef void __fastcall (__closure * TCsdEftRequestCommsEvent)(System::TObject * Sender, 
                                                               BSTR Port, VARIANT_BOOL Requested);
typedef void __fastcall (__closure * TCsdEftPrintReceiptEvent)(System::TObject * Sender, 
                                                               BSTR ReceiptType);
typedef void __fastcall (__closure * TCsdEftKeyDownEvent)(System::TObject * Sender, 
                                                          long KeyCode, long KeyData);
//+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-
// Proxy class to host CsdEft Control in CBuilder IDE/Applications.
//-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
class PACKAGE TCsdEft : public TOleControl
{
  OVERLOADED_PROP_METHODS;
  static TNoParam  OptParam;
  static GUID      DEF_CTL_INTF;

  // Instance of Closures to expose OCX Events as VCL ones
  //
  TNotifyEvent                   FOnSelfTestEvent;
  TNotifyEvent                   FOnSettlementCutoverEvent;
  TNotifyEvent                   FOnSettlementEnquiryEvent;
  TNotifyEvent                   FOnStatusEvent;
  TNotifyEvent                   FOnGetLastTransactionEvent;
  TNotifyEvent                   FOnChequeVerifyEvent;
  TNotifyEvent                   FOnConfigMerchantEvent;
  TNotifyEvent                   FOnDisplayStatusEvent;
  TNotifyEvent                   FOnLogonEvent;
  TNotifyEvent                   FOnQueryCardEvent;
  TNotifyEvent                   FOnReprintReceiptEvent;
  TNotifyEvent                   FOnTransactionEvent;
  TNotifyEvent                   FOnCardSwipeEvent;
  TNotifyEvent                   FOnDisplaySettlementEvent;
  TCsdEftInProgress              FOnInProgress;
  TNotifyEvent                   FOnCsdReserved1;
  TNotifyEvent                   FOnCsdReserved2;
  TNotifyEvent                   FOnCsdReserved3;
  TNotifyEvent                   FOnControlPanelEvent;
  TNotifyEvent                   FOnGetLastReceiptEvent;
  TNotifyEvent                   FOnInvokeAppletEvent;
  TCsdEftRequestCommsEvent       FOnRequestCommsEvent;
  TNotifyEvent                   FOnSettlementReconcileEvent;
  TNotifyEvent                   FOnCsdReserved4;
  TNotifyEvent                   FOnSoftwareDownloadEvent;
  TNotifyEvent                   FOnSoftwareActivateEvent;
  TNotifyEvent                   FOnCsdReserved5;
  TCsdEftPrintReceiptEvent       FOnPrintReceiptEvent;
  TNotifyEvent                   FOnJournalViewerEvent;
  TNotifyEvent                   FOnSetupPinPadEvent;
  TCsdEftKeyDownEvent            FOnKeyDownEvent;
  TNotifyEvent                   FOnShowDialogEvent;
  TNotifyEvent                   FOnSettlementEvent;
  TNotifyEvent                   FOnCsdReserved6;
  TNotifyEvent                   FOnCsdReserved7;
  TNotifyEvent                   FOnDisplayEvent;
  TNotifyEvent                   FOnLicenseEvent;
  TNotifyEvent                   FOnCentralSettlementEvent;
  TNotifyEvent                   FOnSetFocusEvent;
  TNotifyEvent                   FOnSetAutoLogonTimeEvent;
  TNotifyEvent                   FOnCommsTestEvent;
  TNotifyEvent                   FOnTestCommsEvent;
  TNotifyEvent                   FOnGetPendingEvent;

  // Default Interace of OCX
  //
  _DCsdEftDisp m_OCXIntf;

  // VCL Property Getters/Setters which delegate to OCX
  //

  // Static variables used by all instances of OCX proxy
  //
  static int          EventDispIDs[43];
  static TControlData2 CControlData;
  static GUID          CTL_DEF_INTF;

  // Method providing access to interface as __property 
  //
  _DCsdEftDisp __fastcall GetDefaultInterface();
  _DCsdEftDisp __fastcall GetControlInterface()
  { return GetDefaultInterface(); }

protected:
  void     __fastcall CreateControl  ();
  void     __fastcall InitControlData();

public:
  virtual  __fastcall TCsdEft(TComponent* AOwner) : TOleControl(AOwner)
  {};
  virtual  __fastcall TCsdEft(HWND        Parent) : TOleControl(Parent)
  {};

  // OCX methods
  //
  void            __fastcall AboutBox(void);
  void            __fastcall DoGetPending(void);
  void            __fastcall DoTestComms(void);
  void            __fastcall DoSetAutoLogonTime(void);
  void            __fastcall DoSetFocus(VARIANT_BOOL EnableFocus);
  void            __fastcall DoConfigMerchant(void);
  void            __fastcall CsdReservedMethod9(void);
  void            __fastcall CsdReservedMethod8(void);
  void            __fastcall CsdReservedMethod7(void);
  void            __fastcall DoDisplaySettlement(void);
  void            __fastcall DoSettlement(void);
  void            __fastcall DoShowDialog(VARIANT_BOOL Show);
  void            __fastcall DoSetupPinPad(void);
  void            __fastcall DoJournalViewer(void);
  void            __fastcall DoSoftwareActivate(void);
  void            __fastcall DoSoftwareDownload(void);
  void            __fastcall CsdReservedMethod6(void);
  void            __fastcall CsdReservedMethod5(void);
  void            __fastcall DoTransaction(void);
  void            __fastcall DoStatus(void);
  void            __fastcall DoSettlementReconcile(void);
  void            __fastcall DoSettlementEnquiry(void);
  void            __fastcall DoSettlementCutover(void);
  void            __fastcall DoSelfTest(void);
  void            __fastcall CsdReservedMethod4(void);
  void            __fastcall CsdReservedMethod3(void);
  void            __fastcall DoReprintReceipt(void);
  void            __fastcall DoQueryCard(void);
  void            __fastcall DoLogon(void);
  void            __fastcall DoInvokeApplet(BSTR Applet);
  void            __fastcall CsdReservedMethod2(void);
  void            __fastcall CsdReservedMethod1(void);
  void            __fastcall DoGetLastTransaction(void);
  void            __fastcall DoGetLastReceipt(void);
  void            __fastcall DoDisplayStatus(void);
  void            __fastcall DoControlPanel(void);
  void            __fastcall DoChequeVerify(void);
  void            __fastcall DoCardSwipe(void);
  void            __fastcall DoReset(void);

  // OCX properties
  //
  __property _DCsdEftDisp ControlInterface={ read=GetDefaultInterface };

  // Published properties
  //
__published:

  // Standard/Extended properties
  //

  // OCX properties
  //
  __property System::WideString DateSettlement={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=13 };
  __property System::WideString ChqBranch={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=40 };
  __property System::Currency SettleTotalCash={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=67 };
  __property System::WideString DialogType={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=14 };
  __property System::WideString AccountType={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=15 };
  __property int DialogY={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=12 };
  __property int DialogX={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=11 };
  __property bool LastTxnSuccess={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=66 };
  __property System::WideString Filename={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=61 };
  __property System::Currency SettleTotalRefund={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=64 };
  __property System::Currency SettleTotalPurchase={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=63 };
  __property System::WideString Aiic={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=62 };
  __property int SettleCountCash={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=69 };
  __property System::WideString SettleCardTotals={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=68 };
  __property System::WideString AuthCode={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=65 };
  __property int Merchant={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=8 };
  __property System::WideString PanSource={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=5 };
  __property System::WideString ResponseCode={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=2 };
  __property System::WideString Track2={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=9 };
  __property System::WideString Receipt={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=20 };
  __property System::WideString Nii={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=7 };
  __property System::WideString Pan={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=6 };
  __property System::WideString ResponseText={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=1 };
  __property System::WideString TxnRef={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=16 };
  __property System::WideString DataField={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=19 };
  __property System::WideString PinPadSerialNumber={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=18 };
  __property System::WideString PinPadVersion={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=17 };
  __property int Stan={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=4 };
  __property System::WideString TxnType={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=3 };
  __property System::WideString Track1={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=10 };
  __property int MessageType={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=43 };
  __property System::Currency SettleTotal={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=50 };
  __property int VersionMajor={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=57 };
  __property System::WideString EftposNetwork={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=44 };
  __property bool PrintJournal={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=45 };
  __property System::WideString DialogPosition={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=42 };
  __property System::WideString ChqAccount={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=41 };
  __property bool Installed={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=56 };
  __property System::Currency SettleTotalDebit={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=51 };
  __property System::WideString CardType={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=54 };
  __property bool ReceiptAutoPrint={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=53 };
  __property System::Currency SettleTotalCredit={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=52 };
  __property int VersionRevision={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=59 };
  __property int VersionMinor={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=58 };
  __property bool Ready={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=55 };
  __property bool EnableCashoutDuplicate={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=78 };
  __property bool EnableManualPan={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=75 };
  __property int SettleCountDebit={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=72 };
  __property System::WideString PinPadPort={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=79 };
  __property int SettleCount={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=70 };
  __property System::WideString DialogTitle={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=77 };
  __property bool EnableTopmost={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=76 };
  __property int SettleCountCredit={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=71 };
  __property System::WideString PosVendor={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=46 };
  __property System::WideString LicenseId={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=49 };
  __property System::WideString PosVersion={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=48 };
  __property System::WideString PosProductId={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=47 };
  __property int SettleCountRefund={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=74 };
  __property int SettleCountPurchase={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=73 };
  __property bool EnableErrorDialog={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=60 };
  __property System::WideString CsdReservedString2={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=87 };
  __property System::WideString Rrn={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=114 };
  __property System::WideString Address={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=115 };
  __property System::WideString CsdReservedString1={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=86 };
  __property System::WideString PhoneNumber={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=89 };
  __property System::WideString Kvc={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=90 };
  __property System::WideString TotalsData={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=88 };
  __property System::WideString CsdReservedString6={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=113 };
  __property System::Currency AmtCreditLimit={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=118 };
  __property System::WideString CardName={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=116 };
  __property System::Currency AmtCash={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=117 };
  __property System::Currency AmtPurchase={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=120 };
  __property System::WideString CurrencyCode={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=111 };
  __property System::WideString PrintSystem={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=112 };
  __property System::Currency AmtTip={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=119 };
  __property bool CsdReservedBool1={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=92 };
  __property System::WideString SvrPhonePrefix={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=99 };
  __property System::WideString SvrTcpipAddress={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=100 };
  __property System::WideString PurchaseAnalysisData={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=91 };
  __property bool CsdReservedBool3={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=94 };
  __property bool CsdReservedBool4={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=95 };
  __property bool CsdReservedBool2={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=93 };
  __property System::WideString SvrPhoneNumber={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=98 };
  __property int CsdReservedProperty3={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=83 };
  __property int CsdReservedProperty1={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=81 };
  __property int CsdReservedProperty2={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=82 };
  __property int SettleCardCount={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=85 };
  __property bool CsdReservedBool5={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=96 };
  __property bool ResetTotals={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=97 };
  __property int CsdReservedProperty4={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=84 };
  __property bool CutReceipt={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=23 };
  __property int TotalCashCount={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=30 };
  __property bool EnableTip={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=37 };
  __property bool LoggedOn={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=24 };
  __property System::WideString DateExpiry={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=25 };
  __property bool ConfigLocked={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=22 };
  __property int Timeout={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=21 };
  __property System::WideString Time={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=36 };
  __property int TotalRefundCount={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=31 };
  __property bool Success={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=34 };
  __property System::WideString Catid={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=33 };
  __property System::WideString Caid={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=32 };
  __property System::WideString ChqSerialNumber={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=39 };
  __property int ResponseType={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=38 };
  __property System::WideString Date={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=35 };
  __property System::WideString PinPadComPort={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=102 };
  __property System::WideString CsdReservedString5={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=109 };
  __property int TraceFileSizeLimit={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=110 };
  __property System::WideString SvrTcpipPort={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=101 };
  __property System::WideString JournalDirectory={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=104 };
  __property System::WideString JournalFilename={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=105 };
  __property System::WideString PrinterName={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=103 };
  __property System::WideString CsdReservedString4={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=108 };
  __property System::Currency TotalPurchase={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=26 };
  __property int TotalPurchaseCount={ read=GetIntegerProp, write=SetIntegerProp, stored=false, index=29 };
  __property System::Currency TotalRefund={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=28 };
  __property System::Currency TotalCash={ read=GetCurrencyProp, write=SetCurrencyProp, stored=false, index=27 };
  __property bool CsdReservedBool6={ read=GetWordBoolProp, write=SetWordBoolProp, stored=false, index=106 };
  __property System::WideString CsdReservedString3={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=107 };
  __property System::WideString PinPadPortList={ read=GetWideStringProp, write=SetWideStringProp, stored=false, index=80 };

  // OCX Events
  //
  __property TNotifyEvent OnSelfTestEvent={ read=FOnSelfTestEvent, write=FOnSelfTestEvent };
  __property TNotifyEvent OnSettlementCutoverEvent={ read=FOnSettlementCutoverEvent, write=FOnSettlementCutoverEvent };
  __property TNotifyEvent OnSettlementEnquiryEvent={ read=FOnSettlementEnquiryEvent, write=FOnSettlementEnquiryEvent };
  __property TNotifyEvent OnStatusEvent={ read=FOnStatusEvent, write=FOnStatusEvent };
  __property TNotifyEvent OnGetLastTransactionEvent={ read=FOnGetLastTransactionEvent, write=FOnGetLastTransactionEvent };
  __property TNotifyEvent OnChequeVerifyEvent={ read=FOnChequeVerifyEvent, write=FOnChequeVerifyEvent };
  __property TNotifyEvent OnConfigMerchantEvent={ read=FOnConfigMerchantEvent, write=FOnConfigMerchantEvent };
  __property TNotifyEvent OnDisplayStatusEvent={ read=FOnDisplayStatusEvent, write=FOnDisplayStatusEvent };
  __property TNotifyEvent OnLogonEvent={ read=FOnLogonEvent, write=FOnLogonEvent };
  __property TNotifyEvent OnQueryCardEvent={ read=FOnQueryCardEvent, write=FOnQueryCardEvent };
  __property TNotifyEvent OnReprintReceiptEvent={ read=FOnReprintReceiptEvent, write=FOnReprintReceiptEvent };
  __property TNotifyEvent OnTransactionEvent={ read=FOnTransactionEvent, write=FOnTransactionEvent };
  __property TNotifyEvent OnCardSwipeEvent={ read=FOnCardSwipeEvent, write=FOnCardSwipeEvent };
  __property TNotifyEvent OnDisplaySettlementEvent={ read=FOnDisplaySettlementEvent, write=FOnDisplaySettlementEvent };
  __property TCsdEftInProgress OnInProgress={ read=FOnInProgress, write=FOnInProgress };
  __property TNotifyEvent OnCsdReserved1={ read=FOnCsdReserved1, write=FOnCsdReserved1 };
  __property TNotifyEvent OnCsdReserved2={ read=FOnCsdReserved2, write=FOnCsdReserved2 };
  __property TNotifyEvent OnCsdReserved3={ read=FOnCsdReserved3, write=FOnCsdReserved3 };
  __property TNotifyEvent OnControlPanelEvent={ read=FOnControlPanelEvent, write=FOnControlPanelEvent };
  __property TNotifyEvent OnGetLastReceiptEvent={ read=FOnGetLastReceiptEvent, write=FOnGetLastReceiptEvent };
  __property TNotifyEvent OnInvokeAppletEvent={ read=FOnInvokeAppletEvent, write=FOnInvokeAppletEvent };
  __property TCsdEftRequestCommsEvent OnRequestCommsEvent={ read=FOnRequestCommsEvent, write=FOnRequestCommsEvent };
  __property TNotifyEvent OnSettlementReconcileEvent={ read=FOnSettlementReconcileEvent, write=FOnSettlementReconcileEvent };
  __property TNotifyEvent OnCsdReserved4={ read=FOnCsdReserved4, write=FOnCsdReserved4 };
  __property TNotifyEvent OnSoftwareDownloadEvent={ read=FOnSoftwareDownloadEvent, write=FOnSoftwareDownloadEvent };
  __property TNotifyEvent OnSoftwareActivateEvent={ read=FOnSoftwareActivateEvent, write=FOnSoftwareActivateEvent };
  __property TNotifyEvent OnCsdReserved5={ read=FOnCsdReserved5, write=FOnCsdReserved5 };
  __property TCsdEftPrintReceiptEvent OnPrintReceiptEvent={ read=FOnPrintReceiptEvent, write=FOnPrintReceiptEvent };
  __property TNotifyEvent OnJournalViewerEvent={ read=FOnJournalViewerEvent, write=FOnJournalViewerEvent };
  __property TNotifyEvent OnSetupPinPadEvent={ read=FOnSetupPinPadEvent, write=FOnSetupPinPadEvent };
  __property TCsdEftKeyDownEvent OnKeyDownEvent={ read=FOnKeyDownEvent, write=FOnKeyDownEvent };
  __property TNotifyEvent OnShowDialogEvent={ read=FOnShowDialogEvent, write=FOnShowDialogEvent };
  __property TNotifyEvent OnSettlementEvent={ read=FOnSettlementEvent, write=FOnSettlementEvent };
  __property TNotifyEvent OnCsdReserved6={ read=FOnCsdReserved6, write=FOnCsdReserved6 };
  __property TNotifyEvent OnCsdReserved7={ read=FOnCsdReserved7, write=FOnCsdReserved7 };
  __property TNotifyEvent OnDisplayEvent={ read=FOnDisplayEvent, write=FOnDisplayEvent };
  __property TNotifyEvent OnLicenseEvent={ read=FOnLicenseEvent, write=FOnLicenseEvent };
  __property TNotifyEvent OnCentralSettlementEvent={ read=FOnCentralSettlementEvent, write=FOnCentralSettlementEvent };
  __property TNotifyEvent OnSetFocusEvent={ read=FOnSetFocusEvent, write=FOnSetFocusEvent };
  __property TNotifyEvent OnSetAutoLogonTimeEvent={ read=FOnSetAutoLogonTimeEvent, write=FOnSetAutoLogonTimeEvent };
  __property TNotifyEvent OnCommsTestEvent={ read=FOnCommsTestEvent, write=FOnCommsTestEvent };
  __property TNotifyEvent OnTestCommsEvent={ read=FOnTestCommsEvent, write=FOnTestCommsEvent };
  __property TNotifyEvent OnGetPendingEvent={ read=FOnGetPendingEvent, write=FOnGetPendingEvent };
};
typedef TCsdEft  TCsdEftProxy;

};     // namespace Csdeftlib_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Csdeftlib_tlb;
#endif

#pragma option pop

#endif // CSDEFTLib_OCXH
