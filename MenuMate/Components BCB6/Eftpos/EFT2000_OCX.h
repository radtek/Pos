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
// File generated on 3/11/2010 12:59:52 p.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\PROGRA~2\Syncro\EFT2000.exe (1)
// LIBID: {AC17F141-6A16-11D3-ABEA-00105A6CA437}
// LCID: 0
// Helpfile: 
// HelpString: EFT2000 1.3 Type Library
// DepndLst: 
//   (1) v2.0 stdole, (C:\Windows\system32\stdole2.tlb)
// ************************************************************************ //
#ifndef   EFT2000_OCXH
#define   EFT2000_OCXH

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

#include "EFT2000_TLB.h"
namespace Eft2000_tlb
{

// *********************************************************************//
// HelpString: EFT2000 1.3 Type Library
// Version:    1.3
// *********************************************************************//


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TPOSInterface
// Help String      : POSInterface Class
// Default Interface: IPOS4
// Def. Intf. Object: IPOS4Ptr
// Def. Intf. DISP? : No
// Event   Interface: IPOSEvent
// TypeFlags        : (2) CanCreate
// *********************************************************************//
typedef void __fastcall (__closure * TPOSInterfaceEFTPOSStatusChange)(System::TObject * Sender, 
                                                                      Eft2000_tlb::EFTPOSStatusEnum eNewStatus/*[in]*/);
typedef void __fastcall (__closure * TPOSInterfaceEndPrintBanner)(System::TObject * Sender, 
                                                                  Eft2000_tlb::ReceiptTypeEnum eReceiptType/*[in]*/);
typedef void __fastcall (__closure * TPOSInterfaceStartPrintBanner)(System::TObject * Sender, 
                                                                    Eft2000_tlb::ReceiptTypeEnum eReceiptType/*[in]*/);
typedef void __fastcall (__closure * TPOSInterfaceTransactionComplete)(System::TObject * Sender, 
                                                                       long lUniqueID/*[in]*/, 
                                                                       long lResult/*[in]*/);

class PACKAGE TPOSInterface : public Oleserver::TOleServer
{
  IPOS4Ptr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TPOSInterface(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  IPOS4Ptr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(IPOS4Ptr intf);

  virtual void __fastcall InvokeEvent(int DispID, Oleserver::TVariantArray& params);

  BSTR            __fastcall get_ModuleDescription(void);
  BSTR            __fastcall get_ModuleName(void);
  BSTR            __fastcall get_ModuleVersion(void);
  Eft2000_tlb::ResultCodeEnum __fastcall get_ResultCode(void);
  BSTR            __fastcall get_ResultText(void);
  Eft2000_tlb::AccountTypeEnum __fastcall get_AccountType(void);
  BSTR            __fastcall get_CardPANData(void);
  BSTR            __fastcall get_CardType(void);
  long            __fastcall get_DisplayDialogLeft(void);
  void            __fastcall set_DisplayDialogLeft(long pVal/*[in]*/);
  long            __fastcall get_DisplayDialogTop(void);
  void            __fastcall set_DisplayDialogTop(long pVal/*[in]*/);
  BSTR            __fastcall get_ExpiryDate(void);
  BSTR            __fastcall get_InputCustomerTrack2Data(void);
  void            __fastcall set_InputCustomerTrack2Data(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_InputCustomerManualPAN(void);
  void            __fastcall set_InputCustomerManualPAN(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_InputCustomerExpiryDateMonth(void);
  void            __fastcall set_InputCustomerExpiryDateMonth(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_InputCustomerExpiryDateYear(void);
  void            __fastcall set_InputCustomerExpiryDateYear(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_InputMerchantTrack2Data(void);
  void            __fastcall set_InputMerchantTrack2Data(BSTR pVal/*[in]*/);
  long            __fastcall get_SpecialResult(void);
  BSTR            __fastcall get_InputCustomerTrack1Data(void);
  void            __fastcall set_InputCustomerTrack1Data(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_InputCustomerTrack3Data(void);
  void            __fastcall set_InputCustomerTrack3Data(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_InputMerchantTrack1Data(void);
  void            __fastcall set_InputMerchantTrack1Data(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_InputMerchantTrack3Data(void);
  void            __fastcall set_InputMerchantTrack3Data(BSTR pVal/*[in]*/);
  long            __fastcall get_WantProgressBar(void);
  void            __fastcall set_WantProgressBar(long pVal/*[in]*/);
  void            __fastcall set_POSHardware(BSTR Param1/*[in]*/);
  void            __fastcall set_POSSoftwareName(BSTR Param1/*[in]*/);
  void            __fastcall set_POSSoftwareNumber(BSTR Param1/*[in]*/);
  void            __fastcall set_EnablePrintMerchantDetails(long Param1/*[in]*/);
  void            __fastcall set_InputMotoType(Eft2000_tlb::MotoTransTypeEnum Param1/*[in]*/);
  BSTR            __fastcall get_ReceiptData(void);
  void            __fastcall set_AcquirerType(Eft2000_tlb::AcquirerTypeEnum Param1/*[in]*/);
  long            __fastcall get_Stan(void);
  long            __fastcall get_Status(void);
  void            __fastcall ContinuePrinting(void);
  void            __fastcall GetUniqueID(long* lUniqueID/*[in,out]*/);
  void            __fastcall StartAdminFunction(Eft2000_tlb::AdminTypeEnum eAdminType/*[in]*/, 
                                                long lUniqueID/*[in]*/, 
                                                long bRunInBackground/*[in]*/);
  void            __fastcall StartFinancialTransaction(Eft2000_tlb::FinancialTypeEnum eFinancialType/*[in]*/, 
                                                       long lUniqueID/*[in]*/, 
                                                       long bRunInBackground/*[in]*/, 
                                                       long lAmount1/*[in]*/, long lAmount2/*[in]*/);
  void            __fastcall StartUtility(Eft2000_tlb::UtilityTypeEnum eUtilityType/*[in]*/, 
                                          long lUniqueID/*[in]*/, long bRunInBackground/*[in]*/);
  void            __fastcall StartChequeAuthorisation(BSTR bstrChequeSerialNumber/*[in]*/, 
                                                      BSTR bstrChequeBankBranch/*[in]*/, 
                                                      BSTR bstrChequeAccount/*[in]*/, 
                                                      long bRunInBackground/*[in]*/, 
                                                      long lAmount/*[in]*/);
  void            __fastcall SyncroPrint(BSTR PrintData/*[in]*/);
  void            __fastcall EnquireTransaction(long lUniqueID/*[in]*/, 
                                                long bRunInBackground/*[in]*/);
  __property BSTR ModuleDescription={ read=get_ModuleDescription };
  __property BSTR ModuleName={ read=get_ModuleName };
  __property BSTR ModuleVersion={ read=get_ModuleVersion };
  __property Eft2000_tlb::ResultCodeEnum ResultCode={ read=get_ResultCode };
  __property BSTR ResultText={ read=get_ResultText };
  __property Eft2000_tlb::AccountTypeEnum AccountType={ read=get_AccountType };
  __property BSTR CardPANData={ read=get_CardPANData };
  __property BSTR CardType={ read=get_CardType };
  __property BSTR ExpiryDate={ read=get_ExpiryDate };
  __property long SpecialResult={ read=get_SpecialResult };
  __property BSTR POSHardware={ write=set_POSHardware };
  __property BSTR POSSoftwareName={ write=set_POSSoftwareName };
  __property BSTR POSSoftwareNumber={ write=set_POSSoftwareNumber };
  __property long EnablePrintMerchantDetails={ write=set_EnablePrintMerchantDetails };
  __property Eft2000_tlb::MotoTransTypeEnum InputMotoType={ write=set_InputMotoType };
  __property BSTR ReceiptData={ read=get_ReceiptData };
  __property Eft2000_tlb::AcquirerTypeEnum AcquirerType={ write=set_AcquirerType };
  __property long Stan={ read=get_Stan };
  __property long Status={ read=get_Status };
  __property long DisplayDialogLeft={ read=get_DisplayDialogLeft, write=set_DisplayDialogLeft, stored=false };
  __property long DisplayDialogTop={ read=get_DisplayDialogTop, write=set_DisplayDialogTop, stored=false };
  __property BSTR InputCustomerTrack2Data={ read=get_InputCustomerTrack2Data, write=set_InputCustomerTrack2Data, stored=false };
  __property BSTR InputCustomerManualPAN={ read=get_InputCustomerManualPAN, write=set_InputCustomerManualPAN, stored=false };
  __property BSTR InputCustomerExpiryDateMonth={ read=get_InputCustomerExpiryDateMonth, write=set_InputCustomerExpiryDateMonth, stored=false };
  __property BSTR InputCustomerExpiryDateYear={ read=get_InputCustomerExpiryDateYear, write=set_InputCustomerExpiryDateYear, stored=false };
  __property BSTR InputMerchantTrack2Data={ read=get_InputMerchantTrack2Data, write=set_InputMerchantTrack2Data, stored=false };
  __property BSTR InputCustomerTrack1Data={ read=get_InputCustomerTrack1Data, write=set_InputCustomerTrack1Data, stored=false };
  __property BSTR InputCustomerTrack3Data={ read=get_InputCustomerTrack3Data, write=set_InputCustomerTrack3Data, stored=false };
  __property BSTR InputMerchantTrack1Data={ read=get_InputMerchantTrack1Data, write=set_InputMerchantTrack1Data, stored=false };
  __property BSTR InputMerchantTrack3Data={ read=get_InputMerchantTrack3Data, write=set_InputMerchantTrack3Data, stored=false };
  __property long WantProgressBar={ read=get_WantProgressBar, write=set_WantProgressBar, stored=false };
private:
  TPOSInterfaceEFTPOSStatusChange FOnEFTPOSStatusChange;
  TPOSInterfaceEndPrintBanner    FOnEndPrintBanner;
  TPOSInterfaceStartPrintBanner  FOnStartPrintBanner;
  TPOSInterfaceTransactionComplete FOnTransactionComplete;
__published:
  __property TPOSInterfaceEFTPOSStatusChange OnEFTPOSStatusChange={ read=FOnEFTPOSStatusChange, write=FOnEFTPOSStatusChange };
  __property TPOSInterfaceEndPrintBanner OnEndPrintBanner={ read=FOnEndPrintBanner, write=FOnEndPrintBanner };
  __property TPOSInterfaceStartPrintBanner OnStartPrintBanner={ read=FOnStartPrintBanner, write=FOnStartPrintBanner };
  __property TPOSInterfaceTransactionComplete OnTransactionComplete={ read=FOnTransactionComplete, write=FOnTransactionComplete };
};


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TEFTInterface
// Help String      : EFTInterface Class
// Default Interface: IEFT2
// Def. Intf. Object: IEFT2Ptr
// Def. Intf. DISP? : No
// Event   Interface: IEFTEvent
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TEFTInterface : public Oleserver::TOleServer
{
  IEFT2Ptr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TEFTInterface(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  IEFT2Ptr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(IEFT2Ptr intf);

  virtual void __fastcall InvokeEvent(int DispID, Oleserver::TVariantArray& params);

  BSTR            __fastcall get_ModuleDescription(void);
  BSTR            __fastcall get_ModuleName(void);
  BSTR            __fastcall get_ModuleVersion(void);
  Eft2000_tlb::EFTResultCodeEnum __fastcall get_ResultCode(void);
  BSTR            __fastcall get_ResultText(void);
  Eft2000_tlb::AccountTypeEnum __fastcall get_AccountType(void);
  void            __fastcall set_AccountType(Eft2000_tlb::AccountTypeEnum pVal/*[in]*/);
  BSTR            __fastcall get_CardPANData(void);
  void            __fastcall set_CardPANData(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_CardType(void);
  void            __fastcall set_CardType(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_DialogDateDay(void);
  BSTR            __fastcall get_DialogDateMonth(void);
  BSTR            __fastcall get_DialogDateYear(void);
  BSTR            __fastcall get_DialogExpiryDateMonth(void);
  BSTR            __fastcall get_DialogExpiryDateYear(void);
  BSTR            __fastcall get_DialogInput(void);
  BSTR            __fastcall get_DialogManualPAN(void);
  Eft2000_tlb::DialogReturnCodeEnum __fastcall get_DialogReturnCode(void);
  BSTR            __fastcall get_InputCustomerTrack2Data(void);
  BSTR            __fastcall get_ExpiryDate(void);
  void            __fastcall set_ExpiryDate(BSTR pVal/*[in]*/);
  BSTR            __fastcall get_InputCustomerManualPAN(void);
  BSTR            __fastcall get_InputCustomerExpiryDateMonth(void);
  BSTR            __fastcall get_InputCustomerExpiryDateYear(void);
  BSTR            __fastcall get_InputMerchantTrack2Data(void);
  long            __fastcall get_SpecialResult(void);
  void            __fastcall set_SpecialResult(long pVal/*[in]*/);
  BSTR            __fastcall get_InputCustomerTrack1Data(void);
  BSTR            __fastcall get_InputCustomerTrack3Data(void);
  BSTR            __fastcall get_InputMerchantTrack3Data(void);
  BSTR            __fastcall get_InputMerchantTrack1Data(void);
  Eft2000_tlb::DialogUtilityTypeEnum __fastcall get_DialogUtilityType(void);
  long            __fastcall get_POSConnected(void);
  Eft2000_tlb::MotoTransTypeEnum __fastcall get_DialogMOTOType(void);
  Eft2000_tlb::MotoTransTypeEnum __fastcall get_InputMotoType(void);
  Eft2000_tlb::AcquirerTypeEnum __fastcall get_AcquirerType(void);
  Eft2000_tlb::AcquirerTypeEnum __fastcall get_DialogAcquirerType(void);
  BSTR            __fastcall get_DialogChequeBankBranch(void);
  BSTR            __fastcall get_DialogChequeAccount(void);
  BSTR            __fastcall get_DialogChequeSerialNumber(void);
  void            __fastcall set_ReceiptData(BSTR Param1/*[in]*/);
  void            __fastcall set_Stan(long Param1/*[in]*/);
  void            __fastcall set_Status(long Param1/*[in]*/);
  void            __fastcall DestroyActiveDialog(void);
  void            __fastcall SendEFTPOSStatusChangeEvent(Eft2000_tlb::EFTPOSStatusEnum eNewStatus/*[in]*/);
  void            __fastcall SendEndPrintBannerEvent(Eft2000_tlb::ReceiptTypeEnum eReceiptType/*[in]*/);
  void            __fastcall SendStartPrintBannerEvent(Eft2000_tlb::ReceiptTypeEnum eReceiptType/*[in]*/);
  void            __fastcall SendTransactionCompleteEvent(long lUniqueID/*[in]*/, 
                                                          long lResult/*[in]*/);
  void            __fastcall SetResultText(BSTR bstrResultText/*[in]*/);
  void            __fastcall ShowCardSwipeDialog(BSTR bstrTitle/*[in]*/, 
                                                 BSTR bstrPOSPromptText/*[in]*/, 
                                                 Eft2000_tlb::DialogFlagEnum eDialogFlags/*[in]*/);
  void            __fastcall ShowStatusDialog(BSTR bstrTitle/*[in]*/, BSTR bstrPOSPromptText/*[in]*/, 
                                              Eft2000_tlb::DialogFlagEnum eDialogFlags/*[in]*/, 
                                              long bShowProgressBar/*[in]*/);
  void            __fastcall ShowUtilityDialog(void);
  void            __fastcall ShowDateDialog(BSTR bstrTitle/*[in]*/, BSTR bstrPOSPromptText/*[in]*/, 
                                            BSTR bstrDefaultDate/*[in]*/);
  void            __fastcall ShowInputDialog(BSTR bstrTitle/*[in]*/, BSTR bstrPOSPromptText/*[in]*/, 
                                             long bAllowBlank/*[in]*/, long maxLen/*[in]*/);
  void            __fastcall GetUniqueID(long* lUniqueID/*[in,out]*/);
  void            __fastcall ShowMOTODialog(Eft2000_tlb::MotoTransTypeEnum eMOTOTypesAllowed/*[in]*/, 
                                            Eft2000_tlb::MotoTransTypeEnum eDefaultType/*[in]*/);
  void            __fastcall ShowAcquirerSelectionDialog(void);
  void            __fastcall ShowChequeDialog(BSTR bstrTitle/*[in]*/, BSTR bstrPOSPromptText/*[in]*/);
  __property BSTR ModuleDescription={ read=get_ModuleDescription };
  __property BSTR ModuleName={ read=get_ModuleName };
  __property BSTR ModuleVersion={ read=get_ModuleVersion };
  __property Eft2000_tlb::EFTResultCodeEnum ResultCode={ read=get_ResultCode };
  __property BSTR ResultText={ read=get_ResultText };
  __property BSTR DialogDateDay={ read=get_DialogDateDay };
  __property BSTR DialogDateMonth={ read=get_DialogDateMonth };
  __property BSTR DialogDateYear={ read=get_DialogDateYear };
  __property BSTR DialogExpiryDateMonth={ read=get_DialogExpiryDateMonth };
  __property BSTR DialogExpiryDateYear={ read=get_DialogExpiryDateYear };
  __property BSTR DialogInput={ read=get_DialogInput };
  __property BSTR DialogManualPAN={ read=get_DialogManualPAN };
  __property Eft2000_tlb::DialogReturnCodeEnum DialogReturnCode={ read=get_DialogReturnCode };
  __property BSTR InputCustomerTrack2Data={ read=get_InputCustomerTrack2Data };
  __property BSTR InputCustomerManualPAN={ read=get_InputCustomerManualPAN };
  __property BSTR InputCustomerExpiryDateMonth={ read=get_InputCustomerExpiryDateMonth };
  __property BSTR InputCustomerExpiryDateYear={ read=get_InputCustomerExpiryDateYear };
  __property BSTR InputMerchantTrack2Data={ read=get_InputMerchantTrack2Data };
  __property BSTR InputCustomerTrack1Data={ read=get_InputCustomerTrack1Data };
  __property BSTR InputCustomerTrack3Data={ read=get_InputCustomerTrack3Data };
  __property BSTR InputMerchantTrack3Data={ read=get_InputMerchantTrack3Data };
  __property BSTR InputMerchantTrack1Data={ read=get_InputMerchantTrack1Data };
  __property Eft2000_tlb::DialogUtilityTypeEnum DialogUtilityType={ read=get_DialogUtilityType };
  __property long POSConnected={ read=get_POSConnected };
  __property Eft2000_tlb::MotoTransTypeEnum DialogMOTOType={ read=get_DialogMOTOType };
  __property Eft2000_tlb::MotoTransTypeEnum InputMotoType={ read=get_InputMotoType };
  __property Eft2000_tlb::AcquirerTypeEnum AcquirerType={ read=get_AcquirerType };
  __property Eft2000_tlb::AcquirerTypeEnum DialogAcquirerType={ read=get_DialogAcquirerType };
  __property BSTR DialogChequeBankBranch={ read=get_DialogChequeBankBranch };
  __property BSTR DialogChequeAccount={ read=get_DialogChequeAccount };
  __property BSTR DialogChequeSerialNumber={ read=get_DialogChequeSerialNumber };
  __property HRESULT ReceiptData={ write=set_ReceiptData };
  __property HRESULT Stan={ write=set_Stan };
  __property HRESULT Status={ write=set_Status };
private:
  TNotifyEvent                   FOnDialogDismissed;
__published:
  __property TNotifyEvent OnDialogDismissed={ read=FOnDialogDismissed, write=FOnDialogDismissed };
};

};     // namespace Eft2000_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Eft2000_tlb;
#endif

#pragma option pop

#endif // EFT2000_OCXH
