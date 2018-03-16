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
// File generated on 16/03/2018 8:58:05 p.m. from Type Library described below.

// ************************************************************************  //
// Type Lib: C:\MM_Cloud_Git\V6-Master\Runtime\FiscalIntegration.tlb (1)
// LIBID: {3789DF18-77B2-4441-AF37-74CC219EE719}
// LCID: 0
// Helpfile: 
// HelpString: 
// DepndLst: 
//   (1) v2.0 stdole, (C:\WINDOWS\system32\stdole2.tlb)
//   (2) v2.4 mscorlib, (C:\WINDOWS\Microsoft.NET\Framework\v4.0.30319\mscorlib.tlb)
// ************************************************************************ //
#ifndef   FiscalIntegration_OCXH
#define   FiscalIntegration_OCXH

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
#include "mscorlib_TLB.h"

#include "FiscalIntegration_TLB.h"
namespace Fiscalintegration_tlb
{

// *********************************************************************//
// HelpString: 
// Version:    1.0
// *********************************************************************//


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TFiscalLibraryClass
// Help String      : 
// Default Interface: IFiscalLibrary
// Def. Intf. Object: IFiscalLibraryPtr
// Def. Intf. DISP? : Yes
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TFiscalLibraryClass : public Oleserver::TOleServer
{
  IFiscalLibraryPtr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TFiscalLibraryClass(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  IFiscalLibraryPtr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(IFiscalLibraryPtr intf);


  BSTR            __fastcall get_InvoiceNumber(void);
  void            __fastcall set_InvoiceNumber(BSTR Param1/*[in]*/);
  BSTR            __fastcall get_Date(void);
  void            __fastcall set_Date(BSTR Param1/*[in]*/);
  BSTR            __fastcall get_Time(void);
  void            __fastcall set_Time(BSTR Param1/*[in]*/);
  BSTR            __fastcall get_Billno(void);
  void            __fastcall set_Billno(BSTR Param1/*[in]*/);
  BSTR            __fastcall get_Cashno(void);
  void            __fastcall set_Cashno(BSTR Param1/*[in]*/);
  BSTR            __fastcall get_Cashier(void);
  void            __fastcall set_Cashier(BSTR Param1/*[in]*/);
  BSTR            __fastcall get_Source(void);
  void            __fastcall set_Source(BSTR Param1/*[in]*/);
  long            __fastcall PrintReceipt(void);
  void            __fastcall LoadReceiptItemInfo(long identifier/*[in]*/, BSTR InvoiceNumber/*[in]*/);
  void            __fastcall LoadReceiptPaymentInfo(long identifier/*[in]*/, 
                                                    BSTR InvoiceNumber/*[in]*/);
  void            __fastcall LoadReceiptDiscountInfo(long identifier/*[in]*/, 
                                                     BSTR InvoiceNumber/*[in]*/);
  void            __fastcall InitializeItemProperties(void);
  void            __fastcall InitializePaymentProperties(void);
  void            __fastcall InitializeDiscountProperties(void);
  void            __fastcall AddItemToList(void);
  void            __fastcall AddPaymentInfoToList(void);
  void            __fastcall AddDiscountInfoToList(void);
  __property BSTR InvoiceNumber={ read=get_InvoiceNumber, write=set_InvoiceNumber, stored=false };
  __property BSTR Date={ read=get_Date, write=set_Date, stored=false };
  __property BSTR Time={ read=get_Time, write=set_Time, stored=false };
  __property BSTR Billno={ read=get_Billno, write=set_Billno, stored=false };
  __property BSTR Cashno={ read=get_Cashno, write=set_Cashno, stored=false };
  __property BSTR Cashier={ read=get_Cashier, write=set_Cashier, stored=false };
  __property BSTR Source={ read=get_Source, write=set_Source, stored=false };
};


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TDiscountDetails
// Help String      : 
// Default Interface: _DiscountDetails
// Def. Intf. Object: _DiscountDetailsPtr
// Def. Intf. DISP? : No
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TDiscountDetails : public Oleserver::TOleServer
{
  _DiscountDetailsPtr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TDiscountDetails(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  _DiscountDetailsPtr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(_DiscountDetailsPtr intf);


};


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TFiscalDataDetails
// Help String      : 
// Default Interface: _FiscalDataDetails
// Def. Intf. Object: _FiscalDataDetailsPtr
// Def. Intf. DISP? : No
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TFiscalDataDetails : public Oleserver::TOleServer
{
  _FiscalDataDetailsPtr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TFiscalDataDetails(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  _FiscalDataDetailsPtr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(_FiscalDataDetailsPtr intf);


};


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TFiscalPayment
// Help String      : 
// Default Interface: _FiscalPayment
// Def. Intf. Object: _FiscalPaymentPtr
// Def. Intf. DISP? : No
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TFiscalPayment : public Oleserver::TOleServer
{
  _FiscalPaymentPtr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TFiscalPayment(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  _FiscalPaymentPtr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(_FiscalPaymentPtr intf);


};


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TFiscalResponseDetails
// Help String      : 
// Default Interface: _FiscalResponseDetails
// Def. Intf. Object: _FiscalResponseDetailsPtr
// Def. Intf. DISP? : No
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TFiscalResponseDetails : public Oleserver::TOleServer
{
  _FiscalResponseDetailsPtr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TFiscalResponseDetails(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  _FiscalResponseDetailsPtr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(_FiscalResponseDetailsPtr intf);


};


// *********************************************************************//
// COM Component Proxy Class Declaration
// Component Name   : TFiscalService
// Help String      : 
// Default Interface: _FiscalService
// Def. Intf. Object: _FiscalServicePtr
// Def. Intf. DISP? : No
// Event   Interface: 
// TypeFlags        : (2) CanCreate
// *********************************************************************//
class PACKAGE TFiscalService : public Oleserver::TOleServer
{
  _FiscalServicePtr m_DefaultIntf;
  _di_IUnknown __fastcall GetDunk();
public:
  __fastcall TFiscalService(TComponent* owner) : Oleserver::TOleServer(owner)
  {}

  _FiscalServicePtr& GetDefaultInterface();
  void __fastcall InitServerData();
  void __fastcall Connect();
  void __fastcall Disconnect();
  void __fastcall BeforeDestruction();
  void __fastcall ConnectTo(_FiscalServicePtr intf);


};

};     // namespace Fiscalintegration_tlb

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace Fiscalintegration_tlb;
#endif

#pragma option pop

#endif // FiscalIntegration_OCXH
