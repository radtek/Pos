// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8748/MenumateServices/AustriaService/?wsdl
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?wsdl>0
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?xsd=xsd0
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?xsd=xsd2
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?xsd=xsd3
//  >Import : http://localhost:8748/MenumateServices/AustriaService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (29/11/2018 5:51:35 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   AustriaFiscalWSDLH
#define   AustriaFiscalWSDLH

#include <System.hpp>
#include <InvokeRegistry.hpp>
#include <XSBuiltIns.hpp>
#include <winsock2.h>
#include <SOAPHTTPClient.hpp>

#if !defined(SOAP_REMOTABLE_CLASS)
#define SOAP_REMOTABLE_CLASS __declspec(delphiclass)
#endif
#if !defined(IS_OPTN)
#define IS_OPTN 0x0001
#endif
#if !defined(IS_UNBD)
#define IS_UNBD 0x0002
#endif
#if !defined(IS_NLBL)
#define IS_NLBL 0x0004
#endif
#if !defined(IS_REF)
#define IS_REF 0x0080
#endif


namespace NS__AustriaFiscalWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:dateTime        - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:long            - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS ReceiptRequestLocal;
class SOAP_REMOTABLE_CLASS ChargeItemLocal;
class SOAP_REMOTABLE_CLASS PayItemLocal;
class SOAP_REMOTABLE_CLASS ReceiptReponseLocal;
class SOAP_REMOTABLE_CLASS SignaturItemLocal;
class SOAP_REMOTABLE_CLASS ReceiptRequestLocal2;
class SOAP_REMOTABLE_CLASS ChargeItemLocal2;
class SOAP_REMOTABLE_CLASS PayItemLocal2;
class SOAP_REMOTABLE_CLASS ReceiptReponseLocal2;
class SOAP_REMOTABLE_CLASS SignaturItemLocal2;

typedef DynamicArray<UnicodeString> ArrayOfstring;  /* "http://schemas.microsoft.com/2003/10/Serialization/Arrays"[GblCplx] */
typedef DynamicArray<ChargeItemLocal*> ArrayOfChargeItemLocal; /* "http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain"[GblCplx] */
typedef DynamicArray<PayItemLocal*> ArrayOfPayItemLocal; /* "http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : ReceiptRequestLocal, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class ReceiptRequestLocal : public TRemotable {
private:
  UnicodeString   FArea;
  bool            FArea_Specified;
  ArrayOfChargeItemLocal FChargeItems;
  bool            FChargeItems_Specified;
  UnicodeString   FCustomer;
  bool            FCustomer_Specified;
  ArrayOfPayItemLocal FPayItems;
  bool            FPayItems_Specified;
  UnicodeString   FPreviousReceiptReference;
  bool            FPreviousReceiptReference_Specified;
  double          FReceiptAmount;
  bool            FReceiptAmount_Specified;
  TXSDateTime*    FReceiptMoment;
  bool            FReceiptMoment_Specified;
  UnicodeString   FReceiptReference;
  bool            FReceiptReference_Specified;
  UnicodeString   FSettlement;
  bool            FSettlement_Specified;
  UnicodeString   FTerminalID;
  bool            FTerminalID_Specified;
  UnicodeString   FUser;
  bool            FUser_Specified;
  UnicodeString   FftCashBoxID;
  bool            FftCashBoxID_Specified;
  UnicodeString   FftPosSystemId;
  bool            FftPosSystemId_Specified;
  UnicodeString   FftQueueID;
  bool            FftQueueID_Specified;
  __int64         FftReceiptCase;
  bool            FftReceiptCase_Specified;
  UnicodeString   FftReceiptCaseData;
  bool            FftReceiptCaseData_Specified;
  void __fastcall SetArea(int Index, UnicodeString _prop_val)
  {  FArea = _prop_val; FArea_Specified = true;  }
  bool __fastcall Area_Specified(int Index)
  {  return FArea_Specified;  } 
  void __fastcall SetChargeItems(int Index, ArrayOfChargeItemLocal _prop_val)
  {  FChargeItems = _prop_val; FChargeItems_Specified = true;  }
  bool __fastcall ChargeItems_Specified(int Index)
  {  return FChargeItems_Specified;  } 
  void __fastcall SetCustomer(int Index, UnicodeString _prop_val)
  {  FCustomer = _prop_val; FCustomer_Specified = true;  }
  bool __fastcall Customer_Specified(int Index)
  {  return FCustomer_Specified;  } 
  void __fastcall SetPayItems(int Index, ArrayOfPayItemLocal _prop_val)
  {  FPayItems = _prop_val; FPayItems_Specified = true;  }
  bool __fastcall PayItems_Specified(int Index)
  {  return FPayItems_Specified;  } 
  void __fastcall SetPreviousReceiptReference(int Index, UnicodeString _prop_val)
  {  FPreviousReceiptReference = _prop_val; FPreviousReceiptReference_Specified = true;  }
  bool __fastcall PreviousReceiptReference_Specified(int Index)
  {  return FPreviousReceiptReference_Specified;  } 
  void __fastcall SetReceiptAmount(int Index, double _prop_val)
  {  FReceiptAmount = _prop_val; FReceiptAmount_Specified = true;  }
  bool __fastcall ReceiptAmount_Specified(int Index)
  {  return FReceiptAmount_Specified;  } 
  void __fastcall SetReceiptMoment(int Index, TXSDateTime* _prop_val)
  {  FReceiptMoment = _prop_val; FReceiptMoment_Specified = true;  }
  bool __fastcall ReceiptMoment_Specified(int Index)
  {  return FReceiptMoment_Specified;  } 
  void __fastcall SetReceiptReference(int Index, UnicodeString _prop_val)
  {  FReceiptReference = _prop_val; FReceiptReference_Specified = true;  }
  bool __fastcall ReceiptReference_Specified(int Index)
  {  return FReceiptReference_Specified;  } 
  void __fastcall SetSettlement(int Index, UnicodeString _prop_val)
  {  FSettlement = _prop_val; FSettlement_Specified = true;  }
  bool __fastcall Settlement_Specified(int Index)
  {  return FSettlement_Specified;  } 
  void __fastcall SetTerminalID(int Index, UnicodeString _prop_val)
  {  FTerminalID = _prop_val; FTerminalID_Specified = true;  }
  bool __fastcall TerminalID_Specified(int Index)
  {  return FTerminalID_Specified;  } 
  void __fastcall SetUser(int Index, UnicodeString _prop_val)
  {  FUser = _prop_val; FUser_Specified = true;  }
  bool __fastcall User_Specified(int Index)
  {  return FUser_Specified;  } 
  void __fastcall SetftCashBoxID(int Index, UnicodeString _prop_val)
  {  FftCashBoxID = _prop_val; FftCashBoxID_Specified = true;  }
  bool __fastcall ftCashBoxID_Specified(int Index)
  {  return FftCashBoxID_Specified;  } 
  void __fastcall SetftPosSystemId(int Index, UnicodeString _prop_val)
  {  FftPosSystemId = _prop_val; FftPosSystemId_Specified = true;  }
  bool __fastcall ftPosSystemId_Specified(int Index)
  {  return FftPosSystemId_Specified;  } 
  void __fastcall SetftQueueID(int Index, UnicodeString _prop_val)
  {  FftQueueID = _prop_val; FftQueueID_Specified = true;  }
  bool __fastcall ftQueueID_Specified(int Index)
  {  return FftQueueID_Specified;  } 
  void __fastcall SetftReceiptCase(int Index, __int64 _prop_val)
  {  FftReceiptCase = _prop_val; FftReceiptCase_Specified = true;  }
  bool __fastcall ftReceiptCase_Specified(int Index)
  {  return FftReceiptCase_Specified;  } 
  void __fastcall SetftReceiptCaseData(int Index, UnicodeString _prop_val)
  {  FftReceiptCaseData = _prop_val; FftReceiptCaseData_Specified = true;  }
  bool __fastcall ftReceiptCaseData_Specified(int Index)
  {  return FftReceiptCaseData_Specified;  } 

public:
  __fastcall ~ReceiptRequestLocal();
__published:
  __property UnicodeString       Area = { index=(IS_OPTN|IS_NLBL), read=FArea, write=SetArea, stored = Area_Specified };
  __property ArrayOfChargeItemLocal ChargeItems = { index=(IS_OPTN|IS_NLBL), read=FChargeItems, write=SetChargeItems, stored = ChargeItems_Specified };
  __property UnicodeString   Customer = { index=(IS_OPTN|IS_NLBL), read=FCustomer, write=SetCustomer, stored = Customer_Specified };
  __property ArrayOfPayItemLocal   PayItems = { index=(IS_OPTN|IS_NLBL), read=FPayItems, write=SetPayItems, stored = PayItems_Specified };
  __property UnicodeString PreviousReceiptReference = { index=(IS_OPTN|IS_NLBL), read=FPreviousReceiptReference, write=SetPreviousReceiptReference, stored = PreviousReceiptReference_Specified };
  __property double     ReceiptAmount = { index=(IS_OPTN|IS_NLBL), read=FReceiptAmount, write=SetReceiptAmount, stored = ReceiptAmount_Specified };
  __property TXSDateTime* ReceiptMoment = { index=(IS_OPTN), read=FReceiptMoment, write=SetReceiptMoment, stored = ReceiptMoment_Specified };
  __property UnicodeString ReceiptReference = { index=(IS_OPTN|IS_NLBL), read=FReceiptReference, write=SetReceiptReference, stored = ReceiptReference_Specified };
  __property UnicodeString Settlement = { index=(IS_OPTN|IS_NLBL), read=FSettlement, write=SetSettlement, stored = Settlement_Specified };
  __property UnicodeString TerminalID = { index=(IS_OPTN|IS_NLBL), read=FTerminalID, write=SetTerminalID, stored = TerminalID_Specified };
  __property UnicodeString       User = { index=(IS_OPTN|IS_NLBL), read=FUser, write=SetUser, stored = User_Specified };
  __property UnicodeString ftCashBoxID = { index=(IS_OPTN|IS_NLBL), read=FftCashBoxID, write=SetftCashBoxID, stored = ftCashBoxID_Specified };
  __property UnicodeString ftPosSystemId = { index=(IS_OPTN|IS_NLBL), read=FftPosSystemId, write=SetftPosSystemId, stored = ftPosSystemId_Specified };
  __property UnicodeString  ftQueueID = { index=(IS_OPTN|IS_NLBL), read=FftQueueID, write=SetftQueueID, stored = ftQueueID_Specified };
  __property __int64    ftReceiptCase = { index=(IS_OPTN), read=FftReceiptCase, write=SetftReceiptCase, stored = ftReceiptCase_Specified };
  __property UnicodeString ftReceiptCaseData = { index=(IS_OPTN|IS_NLBL), read=FftReceiptCaseData, write=SetftReceiptCaseData, stored = ftReceiptCaseData_Specified };
};




// ************************************************************************ //
// XML       : ChargeItemLocal, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class ChargeItemLocal : public TRemotable {
private:
  UnicodeString   FAccountNumber;
  bool            FAccountNumber_Specified;
  double          FAmount;
  bool            FAmount_Specified;
  __int64         FChargeItemCase;
  bool            FChargeItemCase_Specified;
  UnicodeString   FChargeItemCaseData;
  bool            FChargeItemCaseData_Specified;
  UnicodeString   FCostCenter;
  bool            FCostCenter_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  TXSDateTime*    FMoment;
  bool            FMoment_Specified;
  __int64         FPosition;
  bool            FPosition_Specified;
  UnicodeString   FProductBarcode;
  bool            FProductBarcode_Specified;
  UnicodeString   FProductGroup;
  bool            FProductGroup_Specified;
  UnicodeString   FProductNumber;
  bool            FProductNumber_Specified;
  double          FQuantity;
  bool            FQuantity_Specified;
  UnicodeString   FUnit;
  bool            FUnit_Specified;
  double          FUnitPrice;
  bool            FUnitPrice_Specified;
  double          FUnitQuantity;
  bool            FUnitQuantity_Specified;
  double          FVATAmount;
  bool            FVATAmount_Specified;
  double          FVATRate;
  bool            FVATRate_Specified;
  void __fastcall SetAccountNumber(int Index, UnicodeString _prop_val)
  {  FAccountNumber = _prop_val; FAccountNumber_Specified = true;  }
  bool __fastcall AccountNumber_Specified(int Index)
  {  return FAccountNumber_Specified;  } 
  void __fastcall SetAmount(int Index, double _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  } 
  void __fastcall SetChargeItemCase(int Index, __int64 _prop_val)
  {  FChargeItemCase = _prop_val; FChargeItemCase_Specified = true;  }
  bool __fastcall ChargeItemCase_Specified(int Index)
  {  return FChargeItemCase_Specified;  } 
  void __fastcall SetChargeItemCaseData(int Index, UnicodeString _prop_val)
  {  FChargeItemCaseData = _prop_val; FChargeItemCaseData_Specified = true;  }
  bool __fastcall ChargeItemCaseData_Specified(int Index)
  {  return FChargeItemCaseData_Specified;  } 
  void __fastcall SetCostCenter(int Index, UnicodeString _prop_val)
  {  FCostCenter = _prop_val; FCostCenter_Specified = true;  }
  bool __fastcall CostCenter_Specified(int Index)
  {  return FCostCenter_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetMoment(int Index, TXSDateTime* _prop_val)
  {  FMoment = _prop_val; FMoment_Specified = true;  }
  bool __fastcall Moment_Specified(int Index)
  {  return FMoment_Specified;  } 
  void __fastcall SetPosition(int Index, __int64 _prop_val)
  {  FPosition = _prop_val; FPosition_Specified = true;  }
  bool __fastcall Position_Specified(int Index)
  {  return FPosition_Specified;  } 
  void __fastcall SetProductBarcode(int Index, UnicodeString _prop_val)
  {  FProductBarcode = _prop_val; FProductBarcode_Specified = true;  }
  bool __fastcall ProductBarcode_Specified(int Index)
  {  return FProductBarcode_Specified;  } 
  void __fastcall SetProductGroup(int Index, UnicodeString _prop_val)
  {  FProductGroup = _prop_val; FProductGroup_Specified = true;  }
  bool __fastcall ProductGroup_Specified(int Index)
  {  return FProductGroup_Specified;  } 
  void __fastcall SetProductNumber(int Index, UnicodeString _prop_val)
  {  FProductNumber = _prop_val; FProductNumber_Specified = true;  }
  bool __fastcall ProductNumber_Specified(int Index)
  {  return FProductNumber_Specified;  } 
  void __fastcall SetQuantity(int Index, double _prop_val)
  {  FQuantity = _prop_val; FQuantity_Specified = true;  }
  bool __fastcall Quantity_Specified(int Index)
  {  return FQuantity_Specified;  } 
  void __fastcall SetUnit(int Index, UnicodeString _prop_val)
  {  FUnit = _prop_val; FUnit_Specified = true;  }
  bool __fastcall Unit_Specified(int Index)
  {  return FUnit_Specified;  } 
  void __fastcall SetUnitPrice(int Index, double _prop_val)
  {  FUnitPrice = _prop_val; FUnitPrice_Specified = true;  }
  bool __fastcall UnitPrice_Specified(int Index)
  {  return FUnitPrice_Specified;  } 
  void __fastcall SetUnitQuantity(int Index, double _prop_val)
  {  FUnitQuantity = _prop_val; FUnitQuantity_Specified = true;  }
  bool __fastcall UnitQuantity_Specified(int Index)
  {  return FUnitQuantity_Specified;  } 
  void __fastcall SetVATAmount(int Index, double _prop_val)
  {  FVATAmount = _prop_val; FVATAmount_Specified = true;  }
  bool __fastcall VATAmount_Specified(int Index)
  {  return FVATAmount_Specified;  } 
  void __fastcall SetVATRate(int Index, double _prop_val)
  {  FVATRate = _prop_val; FVATRate_Specified = true;  }
  bool __fastcall VATRate_Specified(int Index)
  {  return FVATRate_Specified;  } 

public:
  __fastcall ~ChargeItemLocal();
__published:
  __property UnicodeString AccountNumber = { index=(IS_OPTN|IS_NLBL), read=FAccountNumber, write=SetAccountNumber, stored = AccountNumber_Specified };
  __property double         Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property __int64    ChargeItemCase = { index=(IS_OPTN), read=FChargeItemCase, write=SetChargeItemCase, stored = ChargeItemCase_Specified };
  __property UnicodeString ChargeItemCaseData = { index=(IS_OPTN|IS_NLBL), read=FChargeItemCaseData, write=SetChargeItemCaseData, stored = ChargeItemCaseData_Specified };
  __property UnicodeString CostCenter = { index=(IS_OPTN|IS_NLBL), read=FCostCenter, write=SetCostCenter, stored = CostCenter_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property TXSDateTime*     Moment = { index=(IS_OPTN), read=FMoment, write=SetMoment, stored = Moment_Specified };
  __property __int64      Position = { index=(IS_OPTN), read=FPosition, write=SetPosition, stored = Position_Specified };
  __property UnicodeString ProductBarcode = { index=(IS_OPTN|IS_NLBL), read=FProductBarcode, write=SetProductBarcode, stored = ProductBarcode_Specified };
  __property UnicodeString ProductGroup = { index=(IS_OPTN|IS_NLBL), read=FProductGroup, write=SetProductGroup, stored = ProductGroup_Specified };
  __property UnicodeString ProductNumber = { index=(IS_OPTN|IS_NLBL), read=FProductNumber, write=SetProductNumber, stored = ProductNumber_Specified };
  __property double       Quantity = { index=(IS_OPTN), read=FQuantity, write=SetQuantity, stored = Quantity_Specified };
  __property UnicodeString       Unit = { index=(IS_OPTN|IS_NLBL), read=FUnit, write=SetUnit, stored = Unit_Specified };
  __property double      UnitPrice = { index=(IS_OPTN), read=FUnitPrice, write=SetUnitPrice, stored = UnitPrice_Specified };
  __property double     UnitQuantity = { index=(IS_OPTN), read=FUnitQuantity, write=SetUnitQuantity, stored = UnitQuantity_Specified };
  __property double      VATAmount = { index=(IS_OPTN), read=FVATAmount, write=SetVATAmount, stored = VATAmount_Specified };
  __property double        VATRate = { index=(IS_OPTN), read=FVATRate, write=SetVATRate, stored = VATRate_Specified };
};




// ************************************************************************ //
// XML       : PayItemLocal, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class PayItemLocal : public TRemotable {
private:
  UnicodeString   FAccountNumber;
  bool            FAccountNumber_Specified;
  double          FAmount;
  bool            FAmount_Specified;
  UnicodeString   FCostCenter;
  bool            FCostCenter_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  TXSDateTime*    FMoment;
  bool            FMoment_Specified;
  UnicodeString   FMoneyGroup;
  bool            FMoneyGroup_Specified;
  UnicodeString   FMoneyNumber;
  bool            FMoneyNumber_Specified;
  __int64         FPayItemCase;
  bool            FPayItemCase_Specified;
  UnicodeString   FPayItemCaseData;
  bool            FPayItemCaseData_Specified;
  __int64         FPosition;
  bool            FPosition_Specified;
  double          FQuantity;
  bool            FQuantity_Specified;
  void __fastcall SetAccountNumber(int Index, UnicodeString _prop_val)
  {  FAccountNumber = _prop_val; FAccountNumber_Specified = true;  }
  bool __fastcall AccountNumber_Specified(int Index)
  {  return FAccountNumber_Specified;  } 
  void __fastcall SetAmount(int Index, double _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  } 
  void __fastcall SetCostCenter(int Index, UnicodeString _prop_val)
  {  FCostCenter = _prop_val; FCostCenter_Specified = true;  }
  bool __fastcall CostCenter_Specified(int Index)
  {  return FCostCenter_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetMoment(int Index, TXSDateTime* _prop_val)
  {  FMoment = _prop_val; FMoment_Specified = true;  }
  bool __fastcall Moment_Specified(int Index)
  {  return FMoment_Specified;  } 
  void __fastcall SetMoneyGroup(int Index, UnicodeString _prop_val)
  {  FMoneyGroup = _prop_val; FMoneyGroup_Specified = true;  }
  bool __fastcall MoneyGroup_Specified(int Index)
  {  return FMoneyGroup_Specified;  } 
  void __fastcall SetMoneyNumber(int Index, UnicodeString _prop_val)
  {  FMoneyNumber = _prop_val; FMoneyNumber_Specified = true;  }
  bool __fastcall MoneyNumber_Specified(int Index)
  {  return FMoneyNumber_Specified;  } 
  void __fastcall SetPayItemCase(int Index, __int64 _prop_val)
  {  FPayItemCase = _prop_val; FPayItemCase_Specified = true;  }
  bool __fastcall PayItemCase_Specified(int Index)
  {  return FPayItemCase_Specified;  } 
  void __fastcall SetPayItemCaseData(int Index, UnicodeString _prop_val)
  {  FPayItemCaseData = _prop_val; FPayItemCaseData_Specified = true;  }
  bool __fastcall PayItemCaseData_Specified(int Index)
  {  return FPayItemCaseData_Specified;  } 
  void __fastcall SetPosition(int Index, __int64 _prop_val)
  {  FPosition = _prop_val; FPosition_Specified = true;  }
  bool __fastcall Position_Specified(int Index)
  {  return FPosition_Specified;  } 
  void __fastcall SetQuantity(int Index, double _prop_val)
  {  FQuantity = _prop_val; FQuantity_Specified = true;  }
  bool __fastcall Quantity_Specified(int Index)
  {  return FQuantity_Specified;  } 

public:
  __fastcall ~PayItemLocal();
__published:
  __property UnicodeString AccountNumber = { index=(IS_OPTN|IS_NLBL), read=FAccountNumber, write=SetAccountNumber, stored = AccountNumber_Specified };
  __property double         Amount = { index=(IS_OPTN), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString CostCenter = { index=(IS_OPTN|IS_NLBL), read=FCostCenter, write=SetCostCenter, stored = CostCenter_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property TXSDateTime*     Moment = { index=(IS_OPTN), read=FMoment, write=SetMoment, stored = Moment_Specified };
  __property UnicodeString MoneyGroup = { index=(IS_OPTN|IS_NLBL), read=FMoneyGroup, write=SetMoneyGroup, stored = MoneyGroup_Specified };
  __property UnicodeString MoneyNumber = { index=(IS_OPTN|IS_NLBL), read=FMoneyNumber, write=SetMoneyNumber, stored = MoneyNumber_Specified };
  __property __int64    PayItemCase = { index=(IS_OPTN), read=FPayItemCase, write=SetPayItemCase, stored = PayItemCase_Specified };
  __property UnicodeString PayItemCaseData = { index=(IS_OPTN|IS_NLBL), read=FPayItemCaseData, write=SetPayItemCaseData, stored = PayItemCaseData_Specified };
  __property __int64      Position = { index=(IS_OPTN), read=FPosition, write=SetPosition, stored = Position_Specified };
  __property double       Quantity = { index=(IS_OPTN), read=FQuantity, write=SetQuantity, stored = Quantity_Specified };
};


typedef DynamicArray<SignaturItemLocal*> ArrayOfSignaturItemLocal; /* "http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : ReceiptReponseLocal, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class ReceiptReponseLocal : public TRemotable {
private:
  UnicodeString   FCashBoxID;
  bool            FCashBoxID_Specified;
  UnicodeString   FCashBoxIdentification;
  bool            FCashBoxIdentification_Specified;
  ArrayOfChargeItemLocal FChargeItems;
  bool            FChargeItems_Specified;
  ArrayOfstring   FChargeLines;
  bool            FChargeLines_Specified;
  ArrayOfPayItemLocal FPayItems;
  bool            FPayItems_Specified;
  ArrayOfstring   FPayLines;
  bool            FPayLines_Specified;
  UnicodeString   FQueueID;
  bool            FQueueID_Specified;
  UnicodeString   FQueueItemID;
  bool            FQueueItemID_Specified;
  __int64         FQueueRow;
  bool            FQueueRow_Specified;
  ArrayOfstring   FReceiptFooter;
  bool            FReceiptFooter_Specified;
  ArrayOfstring   FReceiptHeader;
  bool            FReceiptHeader_Specified;
  UnicodeString   FReceiptIdentification;
  bool            FReceiptIdentification_Specified;
  TXSDateTime*    FReceiptMoment;
  bool            FReceiptMoment_Specified;
  UnicodeString   FReceiptReference;
  bool            FReceiptReference_Specified;
  ArrayOfSignaturItemLocal FSignatures;
  bool            FSignatures_Specified;
  __int64         FState;
  bool            FState_Specified;
  UnicodeString   FStateData;
  bool            FStateData_Specified;
  UnicodeString   FTerminalID;
  bool            FTerminalID_Specified;
  bool            FUnableToConnectToFiscalService;
  bool            FUnableToConnectToFiscalService_Specified;
  void __fastcall SetCashBoxID(int Index, UnicodeString _prop_val)
  {  FCashBoxID = _prop_val; FCashBoxID_Specified = true;  }
  bool __fastcall CashBoxID_Specified(int Index)
  {  return FCashBoxID_Specified;  } 
  void __fastcall SetCashBoxIdentification(int Index, UnicodeString _prop_val)
  {  FCashBoxIdentification = _prop_val; FCashBoxIdentification_Specified = true;  }
  bool __fastcall CashBoxIdentification_Specified(int Index)
  {  return FCashBoxIdentification_Specified;  } 
  void __fastcall SetChargeItems(int Index, ArrayOfChargeItemLocal _prop_val)
  {  FChargeItems = _prop_val; FChargeItems_Specified = true;  }
  bool __fastcall ChargeItems_Specified(int Index)
  {  return FChargeItems_Specified;  } 
  void __fastcall SetChargeLines(int Index, ArrayOfstring _prop_val)
  {  FChargeLines = _prop_val; FChargeLines_Specified = true;  }
  bool __fastcall ChargeLines_Specified(int Index)
  {  return FChargeLines_Specified;  } 
  void __fastcall SetPayItems(int Index, ArrayOfPayItemLocal _prop_val)
  {  FPayItems = _prop_val; FPayItems_Specified = true;  }
  bool __fastcall PayItems_Specified(int Index)
  {  return FPayItems_Specified;  } 
  void __fastcall SetPayLines(int Index, ArrayOfstring _prop_val)
  {  FPayLines = _prop_val; FPayLines_Specified = true;  }
  bool __fastcall PayLines_Specified(int Index)
  {  return FPayLines_Specified;  } 
  void __fastcall SetQueueID(int Index, UnicodeString _prop_val)
  {  FQueueID = _prop_val; FQueueID_Specified = true;  }
  bool __fastcall QueueID_Specified(int Index)
  {  return FQueueID_Specified;  } 
  void __fastcall SetQueueItemID(int Index, UnicodeString _prop_val)
  {  FQueueItemID = _prop_val; FQueueItemID_Specified = true;  }
  bool __fastcall QueueItemID_Specified(int Index)
  {  return FQueueItemID_Specified;  } 
  void __fastcall SetQueueRow(int Index, __int64 _prop_val)
  {  FQueueRow = _prop_val; FQueueRow_Specified = true;  }
  bool __fastcall QueueRow_Specified(int Index)
  {  return FQueueRow_Specified;  } 
  void __fastcall SetReceiptFooter(int Index, ArrayOfstring _prop_val)
  {  FReceiptFooter = _prop_val; FReceiptFooter_Specified = true;  }
  bool __fastcall ReceiptFooter_Specified(int Index)
  {  return FReceiptFooter_Specified;  } 
  void __fastcall SetReceiptHeader(int Index, ArrayOfstring _prop_val)
  {  FReceiptHeader = _prop_val; FReceiptHeader_Specified = true;  }
  bool __fastcall ReceiptHeader_Specified(int Index)
  {  return FReceiptHeader_Specified;  } 
  void __fastcall SetReceiptIdentification(int Index, UnicodeString _prop_val)
  {  FReceiptIdentification = _prop_val; FReceiptIdentification_Specified = true;  }
  bool __fastcall ReceiptIdentification_Specified(int Index)
  {  return FReceiptIdentification_Specified;  } 
  void __fastcall SetReceiptMoment(int Index, TXSDateTime* _prop_val)
  {  FReceiptMoment = _prop_val; FReceiptMoment_Specified = true;  }
  bool __fastcall ReceiptMoment_Specified(int Index)
  {  return FReceiptMoment_Specified;  } 
  void __fastcall SetReceiptReference(int Index, UnicodeString _prop_val)
  {  FReceiptReference = _prop_val; FReceiptReference_Specified = true;  }
  bool __fastcall ReceiptReference_Specified(int Index)
  {  return FReceiptReference_Specified;  } 
  void __fastcall SetSignatures(int Index, ArrayOfSignaturItemLocal _prop_val)
  {  FSignatures = _prop_val; FSignatures_Specified = true;  }
  bool __fastcall Signatures_Specified(int Index)
  {  return FSignatures_Specified;  } 
  void __fastcall SetState(int Index, __int64 _prop_val)
  {  FState = _prop_val; FState_Specified = true;  }
  bool __fastcall State_Specified(int Index)
  {  return FState_Specified;  } 
  void __fastcall SetStateData(int Index, UnicodeString _prop_val)
  {  FStateData = _prop_val; FStateData_Specified = true;  }
  bool __fastcall StateData_Specified(int Index)
  {  return FStateData_Specified;  } 
  void __fastcall SetTerminalID(int Index, UnicodeString _prop_val)
  {  FTerminalID = _prop_val; FTerminalID_Specified = true;  }
  bool __fastcall TerminalID_Specified(int Index)
  {  return FTerminalID_Specified;  } 
  void __fastcall SetUnableToConnectToFiscalService(int Index, bool _prop_val)
  {  FUnableToConnectToFiscalService = _prop_val; FUnableToConnectToFiscalService_Specified = true;  }
  bool __fastcall UnableToConnectToFiscalService_Specified(int Index)
  {  return FUnableToConnectToFiscalService_Specified;  } 

public:
  __fastcall ~ReceiptReponseLocal();
__published:
  __property UnicodeString  CashBoxID = { index=(IS_OPTN|IS_NLBL), read=FCashBoxID, write=SetCashBoxID, stored = CashBoxID_Specified };
  __property UnicodeString CashBoxIdentification = { index=(IS_OPTN|IS_NLBL), read=FCashBoxIdentification, write=SetCashBoxIdentification, stored = CashBoxIdentification_Specified };
  __property ArrayOfChargeItemLocal ChargeItems = { index=(IS_OPTN|IS_NLBL), read=FChargeItems, write=SetChargeItems, stored = ChargeItems_Specified };
  __property ArrayOfstring ChargeLines = { index=(IS_OPTN|IS_NLBL), read=FChargeLines, write=SetChargeLines, stored = ChargeLines_Specified };
  __property ArrayOfPayItemLocal   PayItems = { index=(IS_OPTN|IS_NLBL), read=FPayItems, write=SetPayItems, stored = PayItems_Specified };
  __property ArrayOfstring   PayLines = { index=(IS_OPTN|IS_NLBL), read=FPayLines, write=SetPayLines, stored = PayLines_Specified };
  __property UnicodeString    QueueID = { index=(IS_OPTN|IS_NLBL), read=FQueueID, write=SetQueueID, stored = QueueID_Specified };
  __property UnicodeString QueueItemID = { index=(IS_OPTN|IS_NLBL), read=FQueueItemID, write=SetQueueItemID, stored = QueueItemID_Specified };
  __property __int64      QueueRow = { index=(IS_OPTN), read=FQueueRow, write=SetQueueRow, stored = QueueRow_Specified };
  __property ArrayOfstring ReceiptFooter = { index=(IS_OPTN|IS_NLBL), read=FReceiptFooter, write=SetReceiptFooter, stored = ReceiptFooter_Specified };
  __property ArrayOfstring ReceiptHeader = { index=(IS_OPTN|IS_NLBL), read=FReceiptHeader, write=SetReceiptHeader, stored = ReceiptHeader_Specified };
  __property UnicodeString ReceiptIdentification = { index=(IS_OPTN|IS_NLBL), read=FReceiptIdentification, write=SetReceiptIdentification, stored = ReceiptIdentification_Specified };
  __property TXSDateTime* ReceiptMoment = { index=(IS_OPTN), read=FReceiptMoment, write=SetReceiptMoment, stored = ReceiptMoment_Specified };
  __property UnicodeString ReceiptReference = { index=(IS_OPTN|IS_NLBL), read=FReceiptReference, write=SetReceiptReference, stored = ReceiptReference_Specified };
  __property ArrayOfSignaturItemLocal Signatures = { index=(IS_OPTN|IS_NLBL), read=FSignatures, write=SetSignatures, stored = Signatures_Specified };
  __property __int64         State = { index=(IS_OPTN), read=FState, write=SetState, stored = State_Specified };
  __property UnicodeString  StateData = { index=(IS_OPTN|IS_NLBL), read=FStateData, write=SetStateData, stored = StateData_Specified };
  __property UnicodeString TerminalID = { index=(IS_OPTN|IS_NLBL), read=FTerminalID, write=SetTerminalID, stored = TerminalID_Specified };
  __property bool       UnableToConnectToFiscalService = { index=(IS_OPTN), read=FUnableToConnectToFiscalService, write=SetUnableToConnectToFiscalService, stored = UnableToConnectToFiscalService_Specified };
};




// ************************************************************************ //
// XML       : SignaturItemLocal, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class SignaturItemLocal : public TRemotable {
private:
  UnicodeString   FCaption;
  bool            FCaption_Specified;
  UnicodeString   FData;
  bool            FData_Specified;
  __int64         FSignatureFormat;
  bool            FSignatureFormat_Specified;
  __int64         FSignatureType;
  bool            FSignatureType_Specified;
  void __fastcall SetCaption(int Index, UnicodeString _prop_val)
  {  FCaption = _prop_val; FCaption_Specified = true;  }
  bool __fastcall Caption_Specified(int Index)
  {  return FCaption_Specified;  } 
  void __fastcall SetData(int Index, UnicodeString _prop_val)
  {  FData = _prop_val; FData_Specified = true;  }
  bool __fastcall Data_Specified(int Index)
  {  return FData_Specified;  } 
  void __fastcall SetSignatureFormat(int Index, __int64 _prop_val)
  {  FSignatureFormat = _prop_val; FSignatureFormat_Specified = true;  }
  bool __fastcall SignatureFormat_Specified(int Index)
  {  return FSignatureFormat_Specified;  } 
  void __fastcall SetSignatureType(int Index, __int64 _prop_val)
  {  FSignatureType = _prop_val; FSignatureType_Specified = true;  }
  bool __fastcall SignatureType_Specified(int Index)
  {  return FSignatureType_Specified;  } 
__published:
  __property UnicodeString    Caption = { index=(IS_OPTN|IS_NLBL), read=FCaption, write=SetCaption, stored = Caption_Specified };
  __property UnicodeString       Data = { index=(IS_OPTN|IS_NLBL), read=FData, write=SetData, stored = Data_Specified };
  __property __int64    SignatureFormat = { index=(IS_OPTN), read=FSignatureFormat, write=SetSignatureFormat, stored = SignatureFormat_Specified };
  __property __int64    SignatureType = { index=(IS_OPTN), read=FSignatureType, write=SetSignatureType, stored = SignatureType_Specified };
};




// ************************************************************************ //
// XML       : ReceiptRequestLocal, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class ReceiptRequestLocal2 : public ReceiptRequestLocal {
private:
__published:
};




// ************************************************************************ //
// XML       : ChargeItemLocal, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class ChargeItemLocal2 : public ChargeItemLocal {
private:
__published:
};




// ************************************************************************ //
// XML       : PayItemLocal, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class PayItemLocal2 : public PayItemLocal {
private:
__published:
};




// ************************************************************************ //
// XML       : ReceiptReponseLocal, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class ReceiptReponseLocal2 : public ReceiptReponseLocal {
private:
__published:
};




// ************************************************************************ //
// XML       : SignaturItemLocal, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/AustriaFiscalIntegration.Domain
// ************************************************************************ //
class SignaturItemLocal2 : public SignaturItemLocal {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IAustriaFiscalIntegrationWebService/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceAustria
// service   : AustriaFiscalIntegrationWebService
// port      : basicHttpBinding_ServiceAustria
// URL       : http://localhost:8748/MenumateServices/AustriaService/
// ************************************************************************ //
__interface INTERFACE_UUID("{DD933A9F-8F97-CA0C-D733-2C6D16D4B6AC}") IAustriaFiscalIntegrationWebService : public IInvokable
{
public:
  virtual UnicodeString   InitAustriaFiscal(const UnicodeString url, const UnicodeString cashBoxId) = 0; 
  virtual ReceiptReponseLocal* PostData(const ReceiptRequestLocal* receiptRequest) = 0; 
  virtual bool            CommissionAustriaFiscal(const UnicodeString url, const UnicodeString cashBoxId) = 0; 
  virtual bool            SendZeroReceipt(const UnicodeString url, const UnicodeString cashBoxId) = 0; 
};
typedef DelphiInterface<IAustriaFiscalIntegrationWebService> _di_IAustriaFiscalIntegrationWebService;

_di_IAustriaFiscalIntegrationWebService GetIAustriaFiscalIntegrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__AustriaFiscalWSDL;
#endif



#endif // _H
