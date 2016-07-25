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
// Errors:
//   Error creating palette bitmap of (TPOSInterface) : Server C:\PROGRA~2\Syncro\EFT2000.exe contains no icons
//   Error creating palette bitmap of (TEFTInterface) : Server C:\PROGRA~2\Syncro\EFT2000.exe contains no icons
// ************************************************************************ //

#include <vcl.h>
#pragma hdrstop

#include <olectrls.hpp>
#include <oleserver.hpp>
#if defined(USING_ATL)
#include <atl\atlvcl.h>
#endif

#include "EFT2000_OCX.h"

#if !defined(__PRAGMA_PACKAGE_SMART_INIT)
#define      __PRAGMA_PACKAGE_SMART_INIT
#pragma package(smart_init)
#endif

namespace Eft2000_tlb
{

IPOS4Ptr& TPOSInterface::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TPOSInterface::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TPOSInterface::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TPOSInterface::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TPOSInterface::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TPOSInterface::ConnectTo(IPOS4Ptr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TPOSInterface::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_POSInterface;
  sd.IntfIID = __uuidof(IPOS4);
  sd.EventIID= __uuidof(IPOSEvent);
  ServerData = &sd;
}

void __fastcall TPOSInterface::InvokeEvent(int id, Oleserver::TVariantArray& params)
{
  switch(id)
  {
    case 1: {
      if (OnEFTPOSStatusChange) {
        (OnEFTPOSStatusChange)(this, (Eft2000_tlb::EFTPOSStatusEnum)(int)params[0]);
      }
      break;
      }
    case 2: {
      if (OnEndPrintBanner) {
        (OnEndPrintBanner)(this, (Eft2000_tlb::ReceiptTypeEnum)(int)params[0]);
      }
      break;
      }
    case 3: {
      if (OnStartPrintBanner) {
        (OnStartPrintBanner)(this, (Eft2000_tlb::ReceiptTypeEnum)(int)params[0]);
      }
      break;
      }
    case 4: {
      if (OnTransactionComplete) {
        (OnTransactionComplete)(this, params[0], params[1]);
      }
      break;
      }
    default:
      break;
  }
}

void __fastcall TPOSInterface::ContinuePrinting(void)
{
  GetDefaultInterface()->ContinuePrinting();
}

void __fastcall TPOSInterface::GetUniqueID(long* lUniqueID/*[in,out]*/)
{
  GetDefaultInterface()->GetUniqueID(lUniqueID/*[in,out]*/);
}

void __fastcall TPOSInterface::StartAdminFunction(Eft2000_tlb::AdminTypeEnum eAdminType/*[in]*/, 
                                                  long lUniqueID/*[in]*/, 
                                                  long bRunInBackground/*[in]*/)
{
  GetDefaultInterface()->StartAdminFunction(eAdminType/*[in]*/, lUniqueID/*[in]*/, 
                                            bRunInBackground/*[in]*/);
}

void __fastcall TPOSInterface::StartFinancialTransaction(Eft2000_tlb::FinancialTypeEnum eFinancialType/*[in]*/, 
                                                         long lUniqueID/*[in]*/, 
                                                         long bRunInBackground/*[in]*/, 
                                                         long lAmount1/*[in]*/, 
                                                         long lAmount2/*[in]*/)
{
  GetDefaultInterface()->StartFinancialTransaction(eFinancialType/*[in]*/, lUniqueID/*[in]*/, 
                                                   bRunInBackground/*[in]*/, lAmount1/*[in]*/, 
                                                   lAmount2/*[in]*/);
}

void __fastcall TPOSInterface::StartUtility(Eft2000_tlb::UtilityTypeEnum eUtilityType/*[in]*/, 
                                            long lUniqueID/*[in]*/, long bRunInBackground/*[in]*/)
{
  GetDefaultInterface()->StartUtility(eUtilityType/*[in]*/, lUniqueID/*[in]*/, 
                                      bRunInBackground/*[in]*/);
}

void __fastcall TPOSInterface::StartChequeAuthorisation(BSTR bstrChequeSerialNumber/*[in]*/, 
                                                        BSTR bstrChequeBankBranch/*[in]*/, 
                                                        BSTR bstrChequeAccount/*[in]*/, 
                                                        long bRunInBackground/*[in]*/, 
                                                        long lAmount/*[in]*/)
{
  GetDefaultInterface()->StartChequeAuthorisation(bstrChequeSerialNumber/*[in]*/, 
                                                  bstrChequeBankBranch/*[in]*/, 
                                                  bstrChequeAccount/*[in]*/, 
                                                  bRunInBackground/*[in]*/, lAmount/*[in]*/);
}

void __fastcall TPOSInterface::SyncroPrint(BSTR PrintData/*[in]*/)
{
  GetDefaultInterface()->SyncroPrint(PrintData/*[in]*/);
}

void __fastcall TPOSInterface::EnquireTransaction(long lUniqueID/*[in]*/, 
                                                  long bRunInBackground/*[in]*/)
{
  GetDefaultInterface()->EnquireTransaction(lUniqueID/*[in]*/, bRunInBackground/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_ModuleDescription(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleDescription((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TPOSInterface::get_ModuleName(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleName((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TPOSInterface::get_ModuleVersion(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleVersion((BSTR*)&pVal));
  return pVal;
}

Eft2000_tlb::ResultCodeEnum __fastcall TPOSInterface::get_ResultCode(void)
{
  Eft2000_tlb::ResultCodeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_ResultCode((Eft2000_tlb::ResultCodeEnum*)&pVal));
  return pVal;
}

BSTR __fastcall TPOSInterface::get_ResultText(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ResultText((BSTR*)&pVal));
  return pVal;
}

Eft2000_tlb::AccountTypeEnum __fastcall TPOSInterface::get_AccountType(void)
{
  Eft2000_tlb::AccountTypeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_AccountType((Eft2000_tlb::AccountTypeEnum*)&pVal));
  return pVal;
}

BSTR __fastcall TPOSInterface::get_CardPANData(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_CardPANData((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TPOSInterface::get_CardType(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_CardType((BSTR*)&pVal));
  return pVal;
}

long __fastcall TPOSInterface::get_DisplayDialogLeft(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_DisplayDialogLeft((long*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_DisplayDialogLeft(long pVal/*[in]*/)
{
  GetDefaultInterface()->set_DisplayDialogLeft(pVal/*[in]*/);
}

long __fastcall TPOSInterface::get_DisplayDialogTop(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_DisplayDialogTop((long*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_DisplayDialogTop(long pVal/*[in]*/)
{
  GetDefaultInterface()->set_DisplayDialogTop(pVal/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_ExpiryDate(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ExpiryDate((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TPOSInterface::get_InputCustomerTrack2Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerTrack2Data((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputCustomerTrack2Data(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputCustomerTrack2Data(pVal/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_InputCustomerManualPAN(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerManualPAN((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputCustomerManualPAN(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputCustomerManualPAN(pVal/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_InputCustomerExpiryDateMonth(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerExpiryDateMonth((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputCustomerExpiryDateMonth(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputCustomerExpiryDateMonth(pVal/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_InputCustomerExpiryDateYear(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerExpiryDateYear((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputCustomerExpiryDateYear(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputCustomerExpiryDateYear(pVal/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_InputMerchantTrack2Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputMerchantTrack2Data((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputMerchantTrack2Data(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputMerchantTrack2Data(pVal/*[in]*/);
}

long __fastcall TPOSInterface::get_SpecialResult(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_SpecialResult((long*)&pVal));
  return pVal;
}

BSTR __fastcall TPOSInterface::get_InputCustomerTrack1Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerTrack1Data((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputCustomerTrack1Data(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputCustomerTrack1Data(pVal/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_InputCustomerTrack3Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerTrack3Data((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputCustomerTrack3Data(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputCustomerTrack3Data(pVal/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_InputMerchantTrack1Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputMerchantTrack1Data((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputMerchantTrack1Data(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputMerchantTrack1Data(pVal/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_InputMerchantTrack3Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputMerchantTrack3Data((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_InputMerchantTrack3Data(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_InputMerchantTrack3Data(pVal/*[in]*/);
}

long __fastcall TPOSInterface::get_WantProgressBar(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_WantProgressBar((long*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_WantProgressBar(long pVal/*[in]*/)
{
  GetDefaultInterface()->set_WantProgressBar(pVal/*[in]*/);
}

void __fastcall TPOSInterface::set_POSHardware(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_POSHardware(Param1/*[in]*/);
}

void __fastcall TPOSInterface::set_POSSoftwareName(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_POSSoftwareName(Param1/*[in]*/);
}

void __fastcall TPOSInterface::set_POSSoftwareNumber(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_POSSoftwareNumber(Param1/*[in]*/);
}

void __fastcall TPOSInterface::set_EnablePrintMerchantDetails(long Param1/*[in]*/)
{
  GetDefaultInterface()->set_EnablePrintMerchantDetails(Param1/*[in]*/);
}

void __fastcall TPOSInterface::set_InputMotoType(Eft2000_tlb::MotoTransTypeEnum Param1/*[in]*/)
{
  GetDefaultInterface()->set_InputMotoType(Param1/*[in]*/);
}

BSTR __fastcall TPOSInterface::get_ReceiptData(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ReceiptData((BSTR*)&pVal));
  return pVal;
}

void __fastcall TPOSInterface::set_AcquirerType(Eft2000_tlb::AcquirerTypeEnum Param1/*[in]*/)
{
  GetDefaultInterface()->set_AcquirerType(Param1/*[in]*/);
}

long __fastcall TPOSInterface::get_Stan(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_Stan((long*)&pVal));
  return pVal;
}

long __fastcall TPOSInterface::get_Status(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_Status((long*)&pVal));
  return pVal;
}

IEFT2Ptr& TEFTInterface::GetDefaultInterface()
{
  if (!m_DefaultIntf)
    Connect();
  return m_DefaultIntf;
}

_di_IUnknown __fastcall TEFTInterface::GetDunk()
{
  _di_IUnknown diUnk;
  if (m_DefaultIntf) {
    IUnknownPtr punk = m_DefaultIntf;
    diUnk = LPUNKNOWN(punk);
  }
  return diUnk;
}

void __fastcall TEFTInterface::Connect()
{
  if (!m_DefaultIntf) {
    _di_IUnknown punk = GetServer();
    m_DefaultIntf = punk;
    if (ServerData->EventIID != GUID_NULL)
      ConnectEvents(GetDunk());
  }
}

void __fastcall TEFTInterface::Disconnect()
{
  if (m_DefaultIntf) {

    if (ServerData->EventIID != GUID_NULL)
      DisconnectEvents(GetDunk());
    m_DefaultIntf.Reset();
  }
}

void __fastcall TEFTInterface::BeforeDestruction()
{
  Disconnect();
}

void __fastcall TEFTInterface::ConnectTo(IEFT2Ptr intf)
{
  Disconnect();
  m_DefaultIntf = intf;
  if (ServerData->EventIID != GUID_NULL)
    ConnectEvents(GetDunk());
}

void __fastcall TEFTInterface::InitServerData()
{
  static Oleserver::TServerData sd;
  sd.ClassID = CLSID_EFTInterface;
  sd.IntfIID = __uuidof(IEFT2);
  sd.EventIID= __uuidof(IEFTEvent);
  ServerData = &sd;
}

void __fastcall TEFTInterface::InvokeEvent(int id, Oleserver::TVariantArray& params)
{
  switch(id)
  {
    case 1: {
      if (OnDialogDismissed) {
        (OnDialogDismissed)(this);
      }
      break;
      }
    default:
      break;
  }
}

void __fastcall TEFTInterface::DestroyActiveDialog(void)
{
  GetDefaultInterface()->DestroyActiveDialog();
}

void __fastcall TEFTInterface::SendEFTPOSStatusChangeEvent(Eft2000_tlb::EFTPOSStatusEnum eNewStatus/*[in]*/)
{
  GetDefaultInterface()->SendEFTPOSStatusChangeEvent(eNewStatus/*[in]*/);
}

void __fastcall TEFTInterface::SendEndPrintBannerEvent(Eft2000_tlb::ReceiptTypeEnum eReceiptType/*[in]*/)
{
  GetDefaultInterface()->SendEndPrintBannerEvent(eReceiptType/*[in]*/);
}

void __fastcall TEFTInterface::SendStartPrintBannerEvent(Eft2000_tlb::ReceiptTypeEnum eReceiptType/*[in]*/)
{
  GetDefaultInterface()->SendStartPrintBannerEvent(eReceiptType/*[in]*/);
}

void __fastcall TEFTInterface::SendTransactionCompleteEvent(long lUniqueID/*[in]*/, 
                                                            long lResult/*[in]*/)
{
  GetDefaultInterface()->SendTransactionCompleteEvent(lUniqueID/*[in]*/, lResult/*[in]*/);
}

void __fastcall TEFTInterface::SetResultText(BSTR bstrResultText/*[in]*/)
{
  GetDefaultInterface()->SetResultText(bstrResultText/*[in]*/);
}

void __fastcall TEFTInterface::ShowCardSwipeDialog(BSTR bstrTitle/*[in]*/, 
                                                   BSTR bstrPOSPromptText/*[in]*/, 
                                                   Eft2000_tlb::DialogFlagEnum eDialogFlags/*[in]*/)
{
  GetDefaultInterface()->ShowCardSwipeDialog(bstrTitle/*[in]*/, bstrPOSPromptText/*[in]*/, 
                                             eDialogFlags/*[in]*/);
}

void __fastcall TEFTInterface::ShowStatusDialog(BSTR bstrTitle/*[in]*/, 
                                                BSTR bstrPOSPromptText/*[in]*/, 
                                                Eft2000_tlb::DialogFlagEnum eDialogFlags/*[in]*/, 
                                                long bShowProgressBar/*[in]*/)
{
  GetDefaultInterface()->ShowStatusDialog(bstrTitle/*[in]*/, bstrPOSPromptText/*[in]*/, 
                                          eDialogFlags/*[in]*/, bShowProgressBar/*[in]*/);
}

void __fastcall TEFTInterface::ShowUtilityDialog(void)
{
  GetDefaultInterface()->ShowUtilityDialog();
}

void __fastcall TEFTInterface::ShowDateDialog(BSTR bstrTitle/*[in]*/, BSTR bstrPOSPromptText/*[in]*/, 
                                              BSTR bstrDefaultDate/*[in]*/)
{
  GetDefaultInterface()->ShowDateDialog(bstrTitle/*[in]*/, bstrPOSPromptText/*[in]*/, 
                                        bstrDefaultDate/*[in]*/);
}

void __fastcall TEFTInterface::ShowInputDialog(BSTR bstrTitle/*[in]*/, 
                                               BSTR bstrPOSPromptText/*[in]*/, 
                                               long bAllowBlank/*[in]*/, long maxLen/*[in]*/)
{
  GetDefaultInterface()->ShowInputDialog(bstrTitle/*[in]*/, bstrPOSPromptText/*[in]*/, 
                                         bAllowBlank/*[in]*/, maxLen/*[in]*/);
}

void __fastcall TEFTInterface::GetUniqueID(long* lUniqueID/*[in,out]*/)
{
  GetDefaultInterface()->GetUniqueID(lUniqueID/*[in,out]*/);
}

void __fastcall TEFTInterface::ShowMOTODialog(Eft2000_tlb::MotoTransTypeEnum eMOTOTypesAllowed/*[in]*/, 
                                              Eft2000_tlb::MotoTransTypeEnum eDefaultType/*[in]*/)
{
  GetDefaultInterface()->ShowMOTODialog(eMOTOTypesAllowed/*[in]*/, eDefaultType/*[in]*/);
}

void __fastcall TEFTInterface::ShowAcquirerSelectionDialog(void)
{
  GetDefaultInterface()->ShowAcquirerSelectionDialog();
}

void __fastcall TEFTInterface::ShowChequeDialog(BSTR bstrTitle/*[in]*/, 
                                                BSTR bstrPOSPromptText/*[in]*/)
{
  GetDefaultInterface()->ShowChequeDialog(bstrTitle/*[in]*/, bstrPOSPromptText/*[in]*/);
}

BSTR __fastcall TEFTInterface::get_ModuleDescription(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleDescription((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_ModuleName(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleName((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_ModuleVersion(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ModuleVersion((BSTR*)&pVal));
  return pVal;
}

Eft2000_tlb::EFTResultCodeEnum __fastcall TEFTInterface::get_ResultCode(void)
{
  Eft2000_tlb::EFTResultCodeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_ResultCode((Eft2000_tlb::EFTResultCodeEnum*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_ResultText(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ResultText((BSTR*)&pVal));
  return pVal;
}

Eft2000_tlb::AccountTypeEnum __fastcall TEFTInterface::get_AccountType(void)
{
  Eft2000_tlb::AccountTypeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_AccountType((Eft2000_tlb::AccountTypeEnum*)&pVal));
  return pVal;
}

void __fastcall TEFTInterface::set_AccountType(Eft2000_tlb::AccountTypeEnum pVal/*[in]*/)
{
  GetDefaultInterface()->set_AccountType(pVal/*[in]*/);
}

BSTR __fastcall TEFTInterface::get_CardPANData(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_CardPANData((BSTR*)&pVal));
  return pVal;
}

void __fastcall TEFTInterface::set_CardPANData(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_CardPANData(pVal/*[in]*/);
}

BSTR __fastcall TEFTInterface::get_CardType(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_CardType((BSTR*)&pVal));
  return pVal;
}

void __fastcall TEFTInterface::set_CardType(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_CardType(pVal/*[in]*/);
}

BSTR __fastcall TEFTInterface::get_DialogDateDay(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogDateDay((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogDateMonth(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogDateMonth((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogDateYear(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogDateYear((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogExpiryDateMonth(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogExpiryDateMonth((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogExpiryDateYear(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogExpiryDateYear((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogInput(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogInput((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogManualPAN(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogManualPAN((BSTR*)&pVal));
  return pVal;
}

Eft2000_tlb::DialogReturnCodeEnum __fastcall TEFTInterface::get_DialogReturnCode(void)
{
  Eft2000_tlb::DialogReturnCodeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_DialogReturnCode((Eft2000_tlb::DialogReturnCodeEnum*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_InputCustomerTrack2Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerTrack2Data((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_ExpiryDate(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_ExpiryDate((BSTR*)&pVal));
  return pVal;
}

void __fastcall TEFTInterface::set_ExpiryDate(BSTR pVal/*[in]*/)
{
  GetDefaultInterface()->set_ExpiryDate(pVal/*[in]*/);
}

BSTR __fastcall TEFTInterface::get_InputCustomerManualPAN(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerManualPAN((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_InputCustomerExpiryDateMonth(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerExpiryDateMonth((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_InputCustomerExpiryDateYear(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerExpiryDateYear((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_InputMerchantTrack2Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputMerchantTrack2Data((BSTR*)&pVal));
  return pVal;
}

long __fastcall TEFTInterface::get_SpecialResult(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_SpecialResult((long*)&pVal));
  return pVal;
}

void __fastcall TEFTInterface::set_SpecialResult(long pVal/*[in]*/)
{
  GetDefaultInterface()->set_SpecialResult(pVal/*[in]*/);
}

BSTR __fastcall TEFTInterface::get_InputCustomerTrack1Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerTrack1Data((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_InputCustomerTrack3Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputCustomerTrack3Data((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_InputMerchantTrack3Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputMerchantTrack3Data((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_InputMerchantTrack1Data(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_InputMerchantTrack1Data((BSTR*)&pVal));
  return pVal;
}

Eft2000_tlb::DialogUtilityTypeEnum __fastcall TEFTInterface::get_DialogUtilityType(void)
{
  Eft2000_tlb::DialogUtilityTypeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_DialogUtilityType((Eft2000_tlb::DialogUtilityTypeEnum*)&pVal));
  return pVal;
}

long __fastcall TEFTInterface::get_POSConnected(void)
{
  long pVal;
  OLECHECK(GetDefaultInterface()->get_POSConnected((long*)&pVal));
  return pVal;
}

Eft2000_tlb::MotoTransTypeEnum __fastcall TEFTInterface::get_DialogMOTOType(void)
{
  Eft2000_tlb::MotoTransTypeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_DialogMOTOType((Eft2000_tlb::MotoTransTypeEnum*)&pVal));
  return pVal;
}

Eft2000_tlb::MotoTransTypeEnum __fastcall TEFTInterface::get_InputMotoType(void)
{
  Eft2000_tlb::MotoTransTypeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_InputMotoType((Eft2000_tlb::MotoTransTypeEnum*)&pVal));
  return pVal;
}

Eft2000_tlb::AcquirerTypeEnum __fastcall TEFTInterface::get_AcquirerType(void)
{
  Eft2000_tlb::AcquirerTypeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_AcquirerType((Eft2000_tlb::AcquirerTypeEnum*)&pVal));
  return pVal;
}

Eft2000_tlb::AcquirerTypeEnum __fastcall TEFTInterface::get_DialogAcquirerType(void)
{
  Eft2000_tlb::AcquirerTypeEnum pVal;
  OLECHECK(GetDefaultInterface()->get_DialogAcquirerType((Eft2000_tlb::AcquirerTypeEnum*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogChequeBankBranch(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogChequeBankBranch((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogChequeAccount(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogChequeAccount((BSTR*)&pVal));
  return pVal;
}

BSTR __fastcall TEFTInterface::get_DialogChequeSerialNumber(void)
{
  BSTR pVal = 0;
  OLECHECK(GetDefaultInterface()->get_DialogChequeSerialNumber((BSTR*)&pVal));
  return pVal;
}

void __fastcall TEFTInterface::set_ReceiptData(BSTR Param1/*[in]*/)
{
  GetDefaultInterface()->set_ReceiptData(Param1/*[in]*/);
}

void __fastcall TEFTInterface::set_Stan(long Param1/*[in]*/)
{
  GetDefaultInterface()->set_Stan(Param1/*[in]*/);
}

void __fastcall TEFTInterface::set_Status(long Param1/*[in]*/)
{
  GetDefaultInterface()->set_Status(Param1/*[in]*/);
}


};     // namespace Eft2000_tlb


// *********************************************************************//
// The Register function is invoked by the IDE when this module is 
// installed in a Package. It provides the list of Components (including
// OCXes) implemented by this module. The following implementation
// informs the IDE of the OCX proxy classes implemented here.
// *********************************************************************//
namespace Eft2000_ocx
{

void __fastcall PACKAGE Register()
{
  // [2]
  TComponentClass cls_svr[] = {
                              __classid(Eft2000_tlb::TPOSInterface), 
                              __classid(Eft2000_tlb::TEFTInterface)
                           };
  RegisterComponents("MenuMate OCX", cls_svr,
                     sizeof(cls_svr)/sizeof(cls_svr[0])-1);
}

};     // namespace Eft2000_ocx
