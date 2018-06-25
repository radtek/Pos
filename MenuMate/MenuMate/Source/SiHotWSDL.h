// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : http://localhost:8742/MenumateServices/SiHotService/?wsdl
//  >Import : http://localhost:8742/MenumateServices/SiHotService/?wsdl>0
//  >Import : http://localhost:8742/MenumateServices/SiHotService/?xsd=xsd0
//  >Import : http://localhost:8742/MenumateServices/SiHotService/?xsd=xsd2
//  >Import : http://localhost:8742/MenumateServices/SiHotService/?xsd=xsd1
// Encoding : utf-8
// Version  : 1.0
// (29/05/2018 6:10:50 p.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   SiHotWSDLH
#define   SiHotWSDLH

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


namespace NS__SiHotWSDL {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS RoomRequest;
class SOAP_REMOTABLE_CLASS RoomDetails;
class SOAP_REMOTABLE_CLASS GuestDetails;
class SOAP_REMOTABLE_CLASS RoomChargeDetails;
class SOAP_REMOTABLE_CLASS SiHotService;
class SOAP_REMOTABLE_CLASS SiHotPayment;
class SOAP_REMOTABLE_CLASS RoomChargeResponse;
class SOAP_REMOTABLE_CLASS RoomRequest2;
class SOAP_REMOTABLE_CLASS RoomDetails2;
class SOAP_REMOTABLE_CLASS GuestDetails2;
class SOAP_REMOTABLE_CLASS RoomChargeDetails2;
class SOAP_REMOTABLE_CLASS SiHotService2;
class SOAP_REMOTABLE_CLASS SiHotPayment2;
class SOAP_REMOTABLE_CLASS RoomChargeResponse2;



// ************************************************************************ //
// XML       : RoomRequest, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class RoomRequest : public TRemotable {
private:
  UnicodeString   FIPAddress;
  bool            FIPAddress_Specified;
  int             FPortNumber;
  bool            FPortNumber_Specified;
  UnicodeString   FRoomNumber;
  bool            FRoomNumber_Specified;
  UnicodeString   FTransactionNumber;
  bool            FTransactionNumber_Specified;
  void __fastcall SetIPAddress(int Index, UnicodeString _prop_val)
  {  FIPAddress = _prop_val; FIPAddress_Specified = true;  }
  bool __fastcall IPAddress_Specified(int Index)
  {  return FIPAddress_Specified;  } 
  void __fastcall SetPortNumber(int Index, int _prop_val)
  {  FPortNumber = _prop_val; FPortNumber_Specified = true;  }
  bool __fastcall PortNumber_Specified(int Index)
  {  return FPortNumber_Specified;  } 
  void __fastcall SetRoomNumber(int Index, UnicodeString _prop_val)
  {  FRoomNumber = _prop_val; FRoomNumber_Specified = true;  }
  bool __fastcall RoomNumber_Specified(int Index)
  {  return FRoomNumber_Specified;  } 
  void __fastcall SetTransactionNumber(int Index, UnicodeString _prop_val)
  {  FTransactionNumber = _prop_val; FTransactionNumber_Specified = true;  }
  bool __fastcall TransactionNumber_Specified(int Index)
  {  return FTransactionNumber_Specified;  } 
__published:
  __property UnicodeString  IPAddress = { index=(IS_OPTN|IS_NLBL), read=FIPAddress, write=SetIPAddress, stored = IPAddress_Specified };
  __property int        PortNumber = { index=(IS_OPTN), read=FPortNumber, write=SetPortNumber, stored = PortNumber_Specified };
  __property UnicodeString RoomNumber = { index=(IS_OPTN|IS_NLBL), read=FRoomNumber, write=SetRoomNumber, stored = RoomNumber_Specified };
  __property UnicodeString TransactionNumber = { index=(IS_OPTN|IS_NLBL), read=FTransactionNumber, write=SetTransactionNumber, stored = TransactionNumber_Specified };
};


typedef DynamicArray<GuestDetails*> ArrayOfGuestDetails; /* "http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : RoomDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class RoomDetails : public TRemotable {
private:
  ArrayOfGuestDetails FGuestDetailsList;
  bool            FGuestDetailsList_Specified;
  bool            FIsSuccessful;
  bool            FIsSuccessful_Specified;
  UnicodeString   FResponseMessage;
  bool            FResponseMessage_Specified;
  UnicodeString   FTransNo;
  bool            FTransNo_Specified;
  void __fastcall SetGuestDetailsList(int Index, ArrayOfGuestDetails _prop_val)
  {  FGuestDetailsList = _prop_val; FGuestDetailsList_Specified = true;  }
  bool __fastcall GuestDetailsList_Specified(int Index)
  {  return FGuestDetailsList_Specified;  } 
  void __fastcall SetIsSuccessful(int Index, bool _prop_val)
  {  FIsSuccessful = _prop_val; FIsSuccessful_Specified = true;  }
  bool __fastcall IsSuccessful_Specified(int Index)
  {  return FIsSuccessful_Specified;  } 
  void __fastcall SetResponseMessage(int Index, UnicodeString _prop_val)
  {  FResponseMessage = _prop_val; FResponseMessage_Specified = true;  }
  bool __fastcall ResponseMessage_Specified(int Index)
  {  return FResponseMessage_Specified;  } 
  void __fastcall SetTransNo(int Index, UnicodeString _prop_val)
  {  FTransNo = _prop_val; FTransNo_Specified = true;  }
  bool __fastcall TransNo_Specified(int Index)
  {  return FTransNo_Specified;  } 

public:
  __fastcall ~RoomDetails();
__published:
  __property ArrayOfGuestDetails GuestDetailsList = { index=(IS_OPTN|IS_NLBL), read=FGuestDetailsList, write=SetGuestDetailsList, stored = GuestDetailsList_Specified };
  __property bool       IsSuccessful = { index=(IS_OPTN), read=FIsSuccessful, write=SetIsSuccessful, stored = IsSuccessful_Specified };
  __property UnicodeString ResponseMessage = { index=(IS_OPTN|IS_NLBL), read=FResponseMessage, write=SetResponseMessage, stored = ResponseMessage_Specified };
  __property UnicodeString    TransNo = { index=(IS_OPTN|IS_NLBL), read=FTransNo, write=SetTransNo, stored = TransNo_Specified };
};




// ************************************************************************ //
// XML       : GuestDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class GuestDetails : public TRemotable {
private:
  UnicodeString   FAccountActive;
  bool            FAccountActive_Specified;
  UnicodeString   FAccountNo;
  bool            FAccountNo_Specified;
  UnicodeString   FAccountOwner;
  bool            FAccountOwner_Specified;
  UnicodeString   FBoard;
  bool            FBoard_Specified;
  UnicodeString   FCardInfo;
  bool            FCardInfo_Specified;
  UnicodeString   FDrink;
  bool            FDrink_Specified;
  UnicodeString   FEMail;
  bool            FEMail_Specified;
  UnicodeString   FExtRef;
  bool            FExtRef_Specified;
  UnicodeString   FFbRemark;
  bool            FFbRemark_Specified;
  UnicodeString   FFirstDayBoard;
  bool            FFirstDayBoard_Specified;
  UnicodeString   FFirstName;
  bool            FFirstName_Specified;
  UnicodeString   FFrom;
  bool            FFrom_Specified;
  UnicodeString   FGeneralRemark;
  bool            FGeneralRemark_Specified;
  UnicodeString   FKitchenRemark;
  bool            FKitchenRemark_Specified;
  UnicodeString   FLastDayBoard;
  bool            FLastDayBoard_Specified;
  UnicodeString   FLastName;
  bool            FLastName_Specified;
  UnicodeString   FLimit;
  bool            FLimit_Specified;
  UnicodeString   FMandator;
  bool            FMandator_Specified;
  UnicodeString   FMarcetCode;
  bool            FMarcetCode_Specified;
  UnicodeString   FMessage;
  bool            FMessage_Specified;
  UnicodeString   FMessageText;
  bool            FMessageText_Specified;
  UnicodeString   FNoPax;
  bool            FNoPax_Specified;
  UnicodeString   FPCIID;
  bool            FPCIID_Specified;
  UnicodeString   FPG;
  bool            FPG_Specified;
  UnicodeString   FPersonType;
  bool            FPersonType_Specified;
  UnicodeString   FResNo;
  bool            FResNo_Specified;
  UnicodeString   FResSubNo;
  bool            FResSubNo_Specified;
  UnicodeString   FRoomBedNo;
  bool            FRoomBedNo_Specified;
  UnicodeString   FRoomNo;
  bool            FRoomNo_Specified;
  UnicodeString   FSalesCategory;
  bool            FSalesCategory_Specified;
  UnicodeString   FSalesCategoryCode;
  bool            FSalesCategoryCode_Specified;
  UnicodeString   FTitle;
  bool            FTitle_Specified;
  UnicodeString   FTo;
  bool            FTo_Specified;
  UnicodeString   FUniqueId;
  bool            FUniqueId_Specified;
  UnicodeString   FVIP;
  bool            FVIP_Specified;
  UnicodeString   FVIP2;
  bool            FVIP2_Specified;
  UnicodeString   FVIP2Code;
  bool            FVIP2Code_Specified;
  UnicodeString   FVIPCode;
  bool            FVIPCode_Specified;
  void __fastcall SetAccountActive(int Index, UnicodeString _prop_val)
  {  FAccountActive = _prop_val; FAccountActive_Specified = true;  }
  bool __fastcall AccountActive_Specified(int Index)
  {  return FAccountActive_Specified;  } 
  void __fastcall SetAccountNo(int Index, UnicodeString _prop_val)
  {  FAccountNo = _prop_val; FAccountNo_Specified = true;  }
  bool __fastcall AccountNo_Specified(int Index)
  {  return FAccountNo_Specified;  } 
  void __fastcall SetAccountOwner(int Index, UnicodeString _prop_val)
  {  FAccountOwner = _prop_val; FAccountOwner_Specified = true;  }
  bool __fastcall AccountOwner_Specified(int Index)
  {  return FAccountOwner_Specified;  } 
  void __fastcall SetBoard(int Index, UnicodeString _prop_val)
  {  FBoard = _prop_val; FBoard_Specified = true;  }
  bool __fastcall Board_Specified(int Index)
  {  return FBoard_Specified;  } 
  void __fastcall SetCardInfo(int Index, UnicodeString _prop_val)
  {  FCardInfo = _prop_val; FCardInfo_Specified = true;  }
  bool __fastcall CardInfo_Specified(int Index)
  {  return FCardInfo_Specified;  } 
  void __fastcall SetDrink(int Index, UnicodeString _prop_val)
  {  FDrink = _prop_val; FDrink_Specified = true;  }
  bool __fastcall Drink_Specified(int Index)
  {  return FDrink_Specified;  } 
  void __fastcall SetEMail(int Index, UnicodeString _prop_val)
  {  FEMail = _prop_val; FEMail_Specified = true;  }
  bool __fastcall EMail_Specified(int Index)
  {  return FEMail_Specified;  } 
  void __fastcall SetExtRef(int Index, UnicodeString _prop_val)
  {  FExtRef = _prop_val; FExtRef_Specified = true;  }
  bool __fastcall ExtRef_Specified(int Index)
  {  return FExtRef_Specified;  } 
  void __fastcall SetFbRemark(int Index, UnicodeString _prop_val)
  {  FFbRemark = _prop_val; FFbRemark_Specified = true;  }
  bool __fastcall FbRemark_Specified(int Index)
  {  return FFbRemark_Specified;  } 
  void __fastcall SetFirstDayBoard(int Index, UnicodeString _prop_val)
  {  FFirstDayBoard = _prop_val; FFirstDayBoard_Specified = true;  }
  bool __fastcall FirstDayBoard_Specified(int Index)
  {  return FFirstDayBoard_Specified;  } 
  void __fastcall SetFirstName(int Index, UnicodeString _prop_val)
  {  FFirstName = _prop_val; FFirstName_Specified = true;  }
  bool __fastcall FirstName_Specified(int Index)
  {  return FFirstName_Specified;  } 
  void __fastcall SetFrom(int Index, UnicodeString _prop_val)
  {  FFrom = _prop_val; FFrom_Specified = true;  }
  bool __fastcall From_Specified(int Index)
  {  return FFrom_Specified;  } 
  void __fastcall SetGeneralRemark(int Index, UnicodeString _prop_val)
  {  FGeneralRemark = _prop_val; FGeneralRemark_Specified = true;  }
  bool __fastcall GeneralRemark_Specified(int Index)
  {  return FGeneralRemark_Specified;  } 
  void __fastcall SetKitchenRemark(int Index, UnicodeString _prop_val)
  {  FKitchenRemark = _prop_val; FKitchenRemark_Specified = true;  }
  bool __fastcall KitchenRemark_Specified(int Index)
  {  return FKitchenRemark_Specified;  } 
  void __fastcall SetLastDayBoard(int Index, UnicodeString _prop_val)
  {  FLastDayBoard = _prop_val; FLastDayBoard_Specified = true;  }
  bool __fastcall LastDayBoard_Specified(int Index)
  {  return FLastDayBoard_Specified;  } 
  void __fastcall SetLastName(int Index, UnicodeString _prop_val)
  {  FLastName = _prop_val; FLastName_Specified = true;  }
  bool __fastcall LastName_Specified(int Index)
  {  return FLastName_Specified;  } 
  void __fastcall SetLimit(int Index, UnicodeString _prop_val)
  {  FLimit = _prop_val; FLimit_Specified = true;  }
  bool __fastcall Limit_Specified(int Index)
  {  return FLimit_Specified;  } 
  void __fastcall SetMandator(int Index, UnicodeString _prop_val)
  {  FMandator = _prop_val; FMandator_Specified = true;  }
  bool __fastcall Mandator_Specified(int Index)
  {  return FMandator_Specified;  } 
  void __fastcall SetMarcetCode(int Index, UnicodeString _prop_val)
  {  FMarcetCode = _prop_val; FMarcetCode_Specified = true;  }
  bool __fastcall MarcetCode_Specified(int Index)
  {  return FMarcetCode_Specified;  } 
  void __fastcall SetMessage(int Index, UnicodeString _prop_val)
  {  FMessage = _prop_val; FMessage_Specified = true;  }
  bool __fastcall Message_Specified(int Index)
  {  return FMessage_Specified;  } 
  void __fastcall SetMessageText(int Index, UnicodeString _prop_val)
  {  FMessageText = _prop_val; FMessageText_Specified = true;  }
  bool __fastcall MessageText_Specified(int Index)
  {  return FMessageText_Specified;  } 
  void __fastcall SetNoPax(int Index, UnicodeString _prop_val)
  {  FNoPax = _prop_val; FNoPax_Specified = true;  }
  bool __fastcall NoPax_Specified(int Index)
  {  return FNoPax_Specified;  } 
  void __fastcall SetPCIID(int Index, UnicodeString _prop_val)
  {  FPCIID = _prop_val; FPCIID_Specified = true;  }
  bool __fastcall PCIID_Specified(int Index)
  {  return FPCIID_Specified;  } 
  void __fastcall SetPG(int Index, UnicodeString _prop_val)
  {  FPG = _prop_val; FPG_Specified = true;  }
  bool __fastcall PG_Specified(int Index)
  {  return FPG_Specified;  } 
  void __fastcall SetPersonType(int Index, UnicodeString _prop_val)
  {  FPersonType = _prop_val; FPersonType_Specified = true;  }
  bool __fastcall PersonType_Specified(int Index)
  {  return FPersonType_Specified;  } 
  void __fastcall SetResNo(int Index, UnicodeString _prop_val)
  {  FResNo = _prop_val; FResNo_Specified = true;  }
  bool __fastcall ResNo_Specified(int Index)
  {  return FResNo_Specified;  } 
  void __fastcall SetResSubNo(int Index, UnicodeString _prop_val)
  {  FResSubNo = _prop_val; FResSubNo_Specified = true;  }
  bool __fastcall ResSubNo_Specified(int Index)
  {  return FResSubNo_Specified;  } 
  void __fastcall SetRoomBedNo(int Index, UnicodeString _prop_val)
  {  FRoomBedNo = _prop_val; FRoomBedNo_Specified = true;  }
  bool __fastcall RoomBedNo_Specified(int Index)
  {  return FRoomBedNo_Specified;  } 
  void __fastcall SetRoomNo(int Index, UnicodeString _prop_val)
  {  FRoomNo = _prop_val; FRoomNo_Specified = true;  }
  bool __fastcall RoomNo_Specified(int Index)
  {  return FRoomNo_Specified;  } 
  void __fastcall SetSalesCategory(int Index, UnicodeString _prop_val)
  {  FSalesCategory = _prop_val; FSalesCategory_Specified = true;  }
  bool __fastcall SalesCategory_Specified(int Index)
  {  return FSalesCategory_Specified;  } 
  void __fastcall SetSalesCategoryCode(int Index, UnicodeString _prop_val)
  {  FSalesCategoryCode = _prop_val; FSalesCategoryCode_Specified = true;  }
  bool __fastcall SalesCategoryCode_Specified(int Index)
  {  return FSalesCategoryCode_Specified;  } 
  void __fastcall SetTitle(int Index, UnicodeString _prop_val)
  {  FTitle = _prop_val; FTitle_Specified = true;  }
  bool __fastcall Title_Specified(int Index)
  {  return FTitle_Specified;  } 
  void __fastcall SetTo(int Index, UnicodeString _prop_val)
  {  FTo = _prop_val; FTo_Specified = true;  }
  bool __fastcall To_Specified(int Index)
  {  return FTo_Specified;  } 
  void __fastcall SetUniqueId(int Index, UnicodeString _prop_val)
  {  FUniqueId = _prop_val; FUniqueId_Specified = true;  }
  bool __fastcall UniqueId_Specified(int Index)
  {  return FUniqueId_Specified;  } 
  void __fastcall SetVIP(int Index, UnicodeString _prop_val)
  {  FVIP = _prop_val; FVIP_Specified = true;  }
  bool __fastcall VIP_Specified(int Index)
  {  return FVIP_Specified;  } 
  void __fastcall SetVIP2(int Index, UnicodeString _prop_val)
  {  FVIP2 = _prop_val; FVIP2_Specified = true;  }
  bool __fastcall VIP2_Specified(int Index)
  {  return FVIP2_Specified;  } 
  void __fastcall SetVIP2Code(int Index, UnicodeString _prop_val)
  {  FVIP2Code = _prop_val; FVIP2Code_Specified = true;  }
  bool __fastcall VIP2Code_Specified(int Index)
  {  return FVIP2Code_Specified;  } 
  void __fastcall SetVIPCode(int Index, UnicodeString _prop_val)
  {  FVIPCode = _prop_val; FVIPCode_Specified = true;  }
  bool __fastcall VIPCode_Specified(int Index)
  {  return FVIPCode_Specified;  } 
__published:
  __property UnicodeString AccountActive = { index=(IS_OPTN|IS_NLBL), read=FAccountActive, write=SetAccountActive, stored = AccountActive_Specified };
  __property UnicodeString  AccountNo = { index=(IS_OPTN|IS_NLBL), read=FAccountNo, write=SetAccountNo, stored = AccountNo_Specified };
  __property UnicodeString AccountOwner = { index=(IS_OPTN|IS_NLBL), read=FAccountOwner, write=SetAccountOwner, stored = AccountOwner_Specified };
  __property UnicodeString      Board = { index=(IS_OPTN|IS_NLBL), read=FBoard, write=SetBoard, stored = Board_Specified };
  __property UnicodeString   CardInfo = { index=(IS_OPTN|IS_NLBL), read=FCardInfo, write=SetCardInfo, stored = CardInfo_Specified };
  __property UnicodeString      Drink = { index=(IS_OPTN|IS_NLBL), read=FDrink, write=SetDrink, stored = Drink_Specified };
  __property UnicodeString      EMail = { index=(IS_OPTN|IS_NLBL), read=FEMail, write=SetEMail, stored = EMail_Specified };
  __property UnicodeString     ExtRef = { index=(IS_OPTN|IS_NLBL), read=FExtRef, write=SetExtRef, stored = ExtRef_Specified };
  __property UnicodeString   FbRemark = { index=(IS_OPTN|IS_NLBL), read=FFbRemark, write=SetFbRemark, stored = FbRemark_Specified };
  __property UnicodeString FirstDayBoard = { index=(IS_OPTN|IS_NLBL), read=FFirstDayBoard, write=SetFirstDayBoard, stored = FirstDayBoard_Specified };
  __property UnicodeString  FirstName = { index=(IS_OPTN|IS_NLBL), read=FFirstName, write=SetFirstName, stored = FirstName_Specified };
  __property UnicodeString       From = { index=(IS_OPTN|IS_NLBL), read=FFrom, write=SetFrom, stored = From_Specified };
  __property UnicodeString GeneralRemark = { index=(IS_OPTN|IS_NLBL), read=FGeneralRemark, write=SetGeneralRemark, stored = GeneralRemark_Specified };
  __property UnicodeString KitchenRemark = { index=(IS_OPTN|IS_NLBL), read=FKitchenRemark, write=SetKitchenRemark, stored = KitchenRemark_Specified };
  __property UnicodeString LastDayBoard = { index=(IS_OPTN|IS_NLBL), read=FLastDayBoard, write=SetLastDayBoard, stored = LastDayBoard_Specified };
  __property UnicodeString   LastName = { index=(IS_OPTN|IS_NLBL), read=FLastName, write=SetLastName, stored = LastName_Specified };
  __property UnicodeString      Limit = { index=(IS_OPTN|IS_NLBL), read=FLimit, write=SetLimit, stored = Limit_Specified };
  __property UnicodeString   Mandator = { index=(IS_OPTN|IS_NLBL), read=FMandator, write=SetMandator, stored = Mandator_Specified };
  __property UnicodeString MarcetCode = { index=(IS_OPTN|IS_NLBL), read=FMarcetCode, write=SetMarcetCode, stored = MarcetCode_Specified };
  __property UnicodeString    Message = { index=(IS_OPTN|IS_NLBL), read=FMessage, write=SetMessage, stored = Message_Specified };
  __property UnicodeString MessageText = { index=(IS_OPTN|IS_NLBL), read=FMessageText, write=SetMessageText, stored = MessageText_Specified };
  __property UnicodeString      NoPax = { index=(IS_OPTN|IS_NLBL), read=FNoPax, write=SetNoPax, stored = NoPax_Specified };
  __property UnicodeString      PCIID = { index=(IS_OPTN|IS_NLBL), read=FPCIID, write=SetPCIID, stored = PCIID_Specified };
  __property UnicodeString         PG = { index=(IS_OPTN|IS_NLBL), read=FPG, write=SetPG, stored = PG_Specified };
  __property UnicodeString PersonType = { index=(IS_OPTN|IS_NLBL), read=FPersonType, write=SetPersonType, stored = PersonType_Specified };
  __property UnicodeString      ResNo = { index=(IS_OPTN|IS_NLBL), read=FResNo, write=SetResNo, stored = ResNo_Specified };
  __property UnicodeString   ResSubNo = { index=(IS_OPTN|IS_NLBL), read=FResSubNo, write=SetResSubNo, stored = ResSubNo_Specified };
  __property UnicodeString  RoomBedNo = { index=(IS_OPTN|IS_NLBL), read=FRoomBedNo, write=SetRoomBedNo, stored = RoomBedNo_Specified };
  __property UnicodeString     RoomNo = { index=(IS_OPTN|IS_NLBL), read=FRoomNo, write=SetRoomNo, stored = RoomNo_Specified };
  __property UnicodeString SalesCategory = { index=(IS_OPTN|IS_NLBL), read=FSalesCategory, write=SetSalesCategory, stored = SalesCategory_Specified };
  __property UnicodeString SalesCategoryCode = { index=(IS_OPTN|IS_NLBL), read=FSalesCategoryCode, write=SetSalesCategoryCode, stored = SalesCategoryCode_Specified };
  __property UnicodeString      Title = { index=(IS_OPTN|IS_NLBL), read=FTitle, write=SetTitle, stored = Title_Specified };
  __property UnicodeString         To = { index=(IS_OPTN|IS_NLBL), read=FTo, write=SetTo, stored = To_Specified };
  __property UnicodeString   UniqueId = { index=(IS_OPTN|IS_NLBL), read=FUniqueId, write=SetUniqueId, stored = UniqueId_Specified };
  __property UnicodeString        VIP = { index=(IS_OPTN|IS_NLBL), read=FVIP, write=SetVIP, stored = VIP_Specified };
  __property UnicodeString       VIP2 = { index=(IS_OPTN|IS_NLBL), read=FVIP2, write=SetVIP2, stored = VIP2_Specified };
  __property UnicodeString   VIP2Code = { index=(IS_OPTN|IS_NLBL), read=FVIP2Code, write=SetVIP2Code, stored = VIP2Code_Specified };
  __property UnicodeString    VIPCode = { index=(IS_OPTN|IS_NLBL), read=FVIPCode, write=SetVIPCode, stored = VIPCode_Specified };
};


typedef DynamicArray<SiHotService*> ArrayOfSiHotService; /* "http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain"[GblCplx] */
typedef DynamicArray<SiHotPayment*> ArrayOfSiHotPayment; /* "http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain"[GblCplx] */


// ************************************************************************ //
// XML       : RoomChargeDetails, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class RoomChargeDetails : public TRemotable {
private:
  UnicodeString   FAccountNumber;
  bool            FAccountNumber_Specified;
  UnicodeString   FCashNo;
  bool            FCashNo_Specified;
  UnicodeString   FCoverType;
  bool            FCoverType_Specified;
  UnicodeString   FCovers;
  bool            FCovers_Specified;
  UnicodeString   FDate;
  bool            FDate_Specified;
  UnicodeString   FIPAddress;
  bool            FIPAddress_Specified;
  ArrayOfSiHotService FItemList;
  bool            FItemList_Specified;
  UnicodeString   FLinkID;
  bool            FLinkID_Specified;
  UnicodeString   FLinkText;
  bool            FLinkText_Specified;
  ArrayOfSiHotPayment FPaymentList;
  bool            FPaymentList_Specified;
  int             FPortNumber;
  bool            FPortNumber_Specified;
  UnicodeString   FRoomNumber;
  bool            FRoomNumber_Specified;
  UnicodeString   FShift;
  bool            FShift_Specified;
  UnicodeString   FTime;
  bool            FTime_Specified;
  UnicodeString   FTotal;
  bool            FTotal_Specified;
  UnicodeString   FTransNo;
  bool            FTransNo_Specified;
  void __fastcall SetAccountNumber(int Index, UnicodeString _prop_val)
  {  FAccountNumber = _prop_val; FAccountNumber_Specified = true;  }
  bool __fastcall AccountNumber_Specified(int Index)
  {  return FAccountNumber_Specified;  } 
  void __fastcall SetCashNo(int Index, UnicodeString _prop_val)
  {  FCashNo = _prop_val; FCashNo_Specified = true;  }
  bool __fastcall CashNo_Specified(int Index)
  {  return FCashNo_Specified;  } 
  void __fastcall SetCoverType(int Index, UnicodeString _prop_val)
  {  FCoverType = _prop_val; FCoverType_Specified = true;  }
  bool __fastcall CoverType_Specified(int Index)
  {  return FCoverType_Specified;  } 
  void __fastcall SetCovers(int Index, UnicodeString _prop_val)
  {  FCovers = _prop_val; FCovers_Specified = true;  }
  bool __fastcall Covers_Specified(int Index)
  {  return FCovers_Specified;  } 
  void __fastcall SetDate(int Index, UnicodeString _prop_val)
  {  FDate = _prop_val; FDate_Specified = true;  }
  bool __fastcall Date_Specified(int Index)
  {  return FDate_Specified;  } 
  void __fastcall SetIPAddress(int Index, UnicodeString _prop_val)
  {  FIPAddress = _prop_val; FIPAddress_Specified = true;  }
  bool __fastcall IPAddress_Specified(int Index)
  {  return FIPAddress_Specified;  } 
  void __fastcall SetItemList(int Index, ArrayOfSiHotService _prop_val)
  {  FItemList = _prop_val; FItemList_Specified = true;  }
  bool __fastcall ItemList_Specified(int Index)
  {  return FItemList_Specified;  } 
  void __fastcall SetLinkID(int Index, UnicodeString _prop_val)
  {  FLinkID = _prop_val; FLinkID_Specified = true;  }
  bool __fastcall LinkID_Specified(int Index)
  {  return FLinkID_Specified;  } 
  void __fastcall SetLinkText(int Index, UnicodeString _prop_val)
  {  FLinkText = _prop_val; FLinkText_Specified = true;  }
  bool __fastcall LinkText_Specified(int Index)
  {  return FLinkText_Specified;  } 
  void __fastcall SetPaymentList(int Index, ArrayOfSiHotPayment _prop_val)
  {  FPaymentList = _prop_val; FPaymentList_Specified = true;  }
  bool __fastcall PaymentList_Specified(int Index)
  {  return FPaymentList_Specified;  } 
  void __fastcall SetPortNumber(int Index, int _prop_val)
  {  FPortNumber = _prop_val; FPortNumber_Specified = true;  }
  bool __fastcall PortNumber_Specified(int Index)
  {  return FPortNumber_Specified;  } 
  void __fastcall SetRoomNumber(int Index, UnicodeString _prop_val)
  {  FRoomNumber = _prop_val; FRoomNumber_Specified = true;  }
  bool __fastcall RoomNumber_Specified(int Index)
  {  return FRoomNumber_Specified;  } 
  void __fastcall SetShift(int Index, UnicodeString _prop_val)
  {  FShift = _prop_val; FShift_Specified = true;  }
  bool __fastcall Shift_Specified(int Index)
  {  return FShift_Specified;  } 
  void __fastcall SetTime(int Index, UnicodeString _prop_val)
  {  FTime = _prop_val; FTime_Specified = true;  }
  bool __fastcall Time_Specified(int Index)
  {  return FTime_Specified;  } 
  void __fastcall SetTotal(int Index, UnicodeString _prop_val)
  {  FTotal = _prop_val; FTotal_Specified = true;  }
  bool __fastcall Total_Specified(int Index)
  {  return FTotal_Specified;  } 
  void __fastcall SetTransNo(int Index, UnicodeString _prop_val)
  {  FTransNo = _prop_val; FTransNo_Specified = true;  }
  bool __fastcall TransNo_Specified(int Index)
  {  return FTransNo_Specified;  } 

public:
  __fastcall ~RoomChargeDetails();
__published:
  __property UnicodeString AccountNumber = { index=(IS_OPTN|IS_NLBL), read=FAccountNumber, write=SetAccountNumber, stored = AccountNumber_Specified };
  __property UnicodeString     CashNo = { index=(IS_OPTN|IS_NLBL), read=FCashNo, write=SetCashNo, stored = CashNo_Specified };
  __property UnicodeString  CoverType = { index=(IS_OPTN|IS_NLBL), read=FCoverType, write=SetCoverType, stored = CoverType_Specified };
  __property UnicodeString     Covers = { index=(IS_OPTN|IS_NLBL), read=FCovers, write=SetCovers, stored = Covers_Specified };
  __property UnicodeString       Date = { index=(IS_OPTN|IS_NLBL), read=FDate, write=SetDate, stored = Date_Specified };
  __property UnicodeString  IPAddress = { index=(IS_OPTN|IS_NLBL), read=FIPAddress, write=SetIPAddress, stored = IPAddress_Specified };
  __property ArrayOfSiHotService   ItemList = { index=(IS_OPTN|IS_NLBL), read=FItemList, write=SetItemList, stored = ItemList_Specified };
  __property UnicodeString     LinkID = { index=(IS_OPTN|IS_NLBL), read=FLinkID, write=SetLinkID, stored = LinkID_Specified };
  __property UnicodeString   LinkText = { index=(IS_OPTN|IS_NLBL), read=FLinkText, write=SetLinkText, stored = LinkText_Specified };
  __property ArrayOfSiHotPayment PaymentList = { index=(IS_OPTN|IS_NLBL), read=FPaymentList, write=SetPaymentList, stored = PaymentList_Specified };
  __property int        PortNumber = { index=(IS_OPTN), read=FPortNumber, write=SetPortNumber, stored = PortNumber_Specified };
  __property UnicodeString RoomNumber = { index=(IS_OPTN|IS_NLBL), read=FRoomNumber, write=SetRoomNumber, stored = RoomNumber_Specified };
  __property UnicodeString      Shift = { index=(IS_OPTN|IS_NLBL), read=FShift, write=SetShift, stored = Shift_Specified };
  __property UnicodeString       Time = { index=(IS_OPTN|IS_NLBL), read=FTime, write=SetTime, stored = Time_Specified };
  __property UnicodeString      Total = { index=(IS_OPTN|IS_NLBL), read=FTotal, write=SetTotal, stored = Total_Specified };
  __property UnicodeString    TransNo = { index=(IS_OPTN|IS_NLBL), read=FTransNo, write=SetTransNo, stored = TransNo_Specified };
};




// ************************************************************************ //
// XML       : SiHotService, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class SiHotService : public TRemotable {
private:
  UnicodeString   FAmount;
  bool            FAmount_Specified;
  UnicodeString   FArticleCategory;
  bool            FArticleCategory_Specified;
  UnicodeString   FArticleCategory_Desc;
  bool            FArticleCategory_Desc_Specified;
  UnicodeString   FArticleNo;
  bool            FArticleNo_Specified;
  UnicodeString   FArticleNo_Desc;
  bool            FArticleNo_Desc_Specified;
  UnicodeString   FBillno;
  bool            FBillno_Specified;
  UnicodeString   FCashier;
  bool            FCashier_Specified;
  UnicodeString   FCashno;
  bool            FCashno_Specified;
  UnicodeString   FMiddleCategory;
  bool            FMiddleCategory_Specified;
  UnicodeString   FMiddleCategory_Desc;
  bool            FMiddleCategory_Desc_Specified;
  UnicodeString   FPricePerUnit;
  bool            FPricePerUnit_Specified;
  UnicodeString   FPriceTotal;
  bool            FPriceTotal_Specified;
  UnicodeString   FSource;
  bool            FSource_Specified;
  UnicodeString   FSuperCategory;
  bool            FSuperCategory_Specified;
  UnicodeString   FSuperCategory_Desc;
  bool            FSuperCategory_Desc_Specified;
  UnicodeString   FVATPercentage;
  bool            FVATPercentage_Specified;
  void __fastcall SetAmount(int Index, UnicodeString _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  } 
  void __fastcall SetArticleCategory(int Index, UnicodeString _prop_val)
  {  FArticleCategory = _prop_val; FArticleCategory_Specified = true;  }
  bool __fastcall ArticleCategory_Specified(int Index)
  {  return FArticleCategory_Specified;  } 
  void __fastcall SetArticleCategory_Desc(int Index, UnicodeString _prop_val)
  {  FArticleCategory_Desc = _prop_val; FArticleCategory_Desc_Specified = true;  }
  bool __fastcall ArticleCategory_Desc_Specified(int Index)
  {  return FArticleCategory_Desc_Specified;  } 
  void __fastcall SetArticleNo(int Index, UnicodeString _prop_val)
  {  FArticleNo = _prop_val; FArticleNo_Specified = true;  }
  bool __fastcall ArticleNo_Specified(int Index)
  {  return FArticleNo_Specified;  } 
  void __fastcall SetArticleNo_Desc(int Index, UnicodeString _prop_val)
  {  FArticleNo_Desc = _prop_val; FArticleNo_Desc_Specified = true;  }
  bool __fastcall ArticleNo_Desc_Specified(int Index)
  {  return FArticleNo_Desc_Specified;  } 
  void __fastcall SetBillno(int Index, UnicodeString _prop_val)
  {  FBillno = _prop_val; FBillno_Specified = true;  }
  bool __fastcall Billno_Specified(int Index)
  {  return FBillno_Specified;  } 
  void __fastcall SetCashier(int Index, UnicodeString _prop_val)
  {  FCashier = _prop_val; FCashier_Specified = true;  }
  bool __fastcall Cashier_Specified(int Index)
  {  return FCashier_Specified;  } 
  void __fastcall SetCashno(int Index, UnicodeString _prop_val)
  {  FCashno = _prop_val; FCashno_Specified = true;  }
  bool __fastcall Cashno_Specified(int Index)
  {  return FCashno_Specified;  } 
  void __fastcall SetMiddleCategory(int Index, UnicodeString _prop_val)
  {  FMiddleCategory = _prop_val; FMiddleCategory_Specified = true;  }
  bool __fastcall MiddleCategory_Specified(int Index)
  {  return FMiddleCategory_Specified;  } 
  void __fastcall SetMiddleCategory_Desc(int Index, UnicodeString _prop_val)
  {  FMiddleCategory_Desc = _prop_val; FMiddleCategory_Desc_Specified = true;  }
  bool __fastcall MiddleCategory_Desc_Specified(int Index)
  {  return FMiddleCategory_Desc_Specified;  } 
  void __fastcall SetPricePerUnit(int Index, UnicodeString _prop_val)
  {  FPricePerUnit = _prop_val; FPricePerUnit_Specified = true;  }
  bool __fastcall PricePerUnit_Specified(int Index)
  {  return FPricePerUnit_Specified;  } 
  void __fastcall SetPriceTotal(int Index, UnicodeString _prop_val)
  {  FPriceTotal = _prop_val; FPriceTotal_Specified = true;  }
  bool __fastcall PriceTotal_Specified(int Index)
  {  return FPriceTotal_Specified;  } 
  void __fastcall SetSource(int Index, UnicodeString _prop_val)
  {  FSource = _prop_val; FSource_Specified = true;  }
  bool __fastcall Source_Specified(int Index)
  {  return FSource_Specified;  } 
  void __fastcall SetSuperCategory(int Index, UnicodeString _prop_val)
  {  FSuperCategory = _prop_val; FSuperCategory_Specified = true;  }
  bool __fastcall SuperCategory_Specified(int Index)
  {  return FSuperCategory_Specified;  } 
  void __fastcall SetSuperCategory_Desc(int Index, UnicodeString _prop_val)
  {  FSuperCategory_Desc = _prop_val; FSuperCategory_Desc_Specified = true;  }
  bool __fastcall SuperCategory_Desc_Specified(int Index)
  {  return FSuperCategory_Desc_Specified;  } 
  void __fastcall SetVATPercentage(int Index, UnicodeString _prop_val)
  {  FVATPercentage = _prop_val; FVATPercentage_Specified = true;  }
  bool __fastcall VATPercentage_Specified(int Index)
  {  return FVATPercentage_Specified;  } 
__published:
  __property UnicodeString     Amount = { index=(IS_OPTN|IS_NLBL), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString ArticleCategory = { index=(IS_OPTN|IS_NLBL), read=FArticleCategory, write=SetArticleCategory, stored = ArticleCategory_Specified };
  __property UnicodeString ArticleCategory_Desc = { index=(IS_OPTN|IS_NLBL), read=FArticleCategory_Desc, write=SetArticleCategory_Desc, stored = ArticleCategory_Desc_Specified };
  __property UnicodeString  ArticleNo = { index=(IS_OPTN|IS_NLBL), read=FArticleNo, write=SetArticleNo, stored = ArticleNo_Specified };
  __property UnicodeString ArticleNo_Desc = { index=(IS_OPTN|IS_NLBL), read=FArticleNo_Desc, write=SetArticleNo_Desc, stored = ArticleNo_Desc_Specified };
  __property UnicodeString     Billno = { index=(IS_OPTN|IS_NLBL), read=FBillno, write=SetBillno, stored = Billno_Specified };
  __property UnicodeString    Cashier = { index=(IS_OPTN|IS_NLBL), read=FCashier, write=SetCashier, stored = Cashier_Specified };
  __property UnicodeString     Cashno = { index=(IS_OPTN|IS_NLBL), read=FCashno, write=SetCashno, stored = Cashno_Specified };
  __property UnicodeString MiddleCategory = { index=(IS_OPTN|IS_NLBL), read=FMiddleCategory, write=SetMiddleCategory, stored = MiddleCategory_Specified };
  __property UnicodeString MiddleCategory_Desc = { index=(IS_OPTN|IS_NLBL), read=FMiddleCategory_Desc, write=SetMiddleCategory_Desc, stored = MiddleCategory_Desc_Specified };
  __property UnicodeString PricePerUnit = { index=(IS_OPTN|IS_NLBL), read=FPricePerUnit, write=SetPricePerUnit, stored = PricePerUnit_Specified };
  __property UnicodeString PriceTotal = { index=(IS_OPTN|IS_NLBL), read=FPriceTotal, write=SetPriceTotal, stored = PriceTotal_Specified };
  __property UnicodeString     Source = { index=(IS_OPTN|IS_NLBL), read=FSource, write=SetSource, stored = Source_Specified };
  __property UnicodeString SuperCategory = { index=(IS_OPTN|IS_NLBL), read=FSuperCategory, write=SetSuperCategory, stored = SuperCategory_Specified };
  __property UnicodeString SuperCategory_Desc = { index=(IS_OPTN|IS_NLBL), read=FSuperCategory_Desc, write=SetSuperCategory_Desc, stored = SuperCategory_Desc_Specified };
  __property UnicodeString VATPercentage = { index=(IS_OPTN|IS_NLBL), read=FVATPercentage, write=SetVATPercentage, stored = VATPercentage_Specified };
};




// ************************************************************************ //
// XML       : SiHotPayment, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class SiHotPayment : public TRemotable {
private:
  UnicodeString   FAmount;
  bool            FAmount_Specified;
  UnicodeString   FBillno;
  bool            FBillno_Specified;
  UnicodeString   FCashier;
  bool            FCashier_Specified;
  UnicodeString   FCashno;
  bool            FCashno_Specified;
  UnicodeString   FDescription;
  bool            FDescription_Specified;
  UnicodeString   FSource;
  bool            FSource_Specified;
  UnicodeString   FType;
  bool            FType_Specified;
  void __fastcall SetAmount(int Index, UnicodeString _prop_val)
  {  FAmount = _prop_val; FAmount_Specified = true;  }
  bool __fastcall Amount_Specified(int Index)
  {  return FAmount_Specified;  } 
  void __fastcall SetBillno(int Index, UnicodeString _prop_val)
  {  FBillno = _prop_val; FBillno_Specified = true;  }
  bool __fastcall Billno_Specified(int Index)
  {  return FBillno_Specified;  } 
  void __fastcall SetCashier(int Index, UnicodeString _prop_val)
  {  FCashier = _prop_val; FCashier_Specified = true;  }
  bool __fastcall Cashier_Specified(int Index)
  {  return FCashier_Specified;  } 
  void __fastcall SetCashno(int Index, UnicodeString _prop_val)
  {  FCashno = _prop_val; FCashno_Specified = true;  }
  bool __fastcall Cashno_Specified(int Index)
  {  return FCashno_Specified;  } 
  void __fastcall SetDescription(int Index, UnicodeString _prop_val)
  {  FDescription = _prop_val; FDescription_Specified = true;  }
  bool __fastcall Description_Specified(int Index)
  {  return FDescription_Specified;  } 
  void __fastcall SetSource(int Index, UnicodeString _prop_val)
  {  FSource = _prop_val; FSource_Specified = true;  }
  bool __fastcall Source_Specified(int Index)
  {  return FSource_Specified;  } 
  void __fastcall SetType(int Index, UnicodeString _prop_val)
  {  FType = _prop_val; FType_Specified = true;  }
  bool __fastcall Type_Specified(int Index)
  {  return FType_Specified;  } 
__published:
  __property UnicodeString     Amount = { index=(IS_OPTN|IS_NLBL), read=FAmount, write=SetAmount, stored = Amount_Specified };
  __property UnicodeString     Billno = { index=(IS_OPTN|IS_NLBL), read=FBillno, write=SetBillno, stored = Billno_Specified };
  __property UnicodeString    Cashier = { index=(IS_OPTN|IS_NLBL), read=FCashier, write=SetCashier, stored = Cashier_Specified };
  __property UnicodeString     Cashno = { index=(IS_OPTN|IS_NLBL), read=FCashno, write=SetCashno, stored = Cashno_Specified };
  __property UnicodeString Description = { index=(IS_OPTN|IS_NLBL), read=FDescription, write=SetDescription, stored = Description_Specified };
  __property UnicodeString     Source = { index=(IS_OPTN|IS_NLBL), read=FSource, write=SetSource, stored = Source_Specified };
  __property UnicodeString       Type = { index=(IS_OPTN|IS_NLBL), read=FType, write=SetType, stored = Type_Specified };
};




// ************************************************************************ //
// XML       : RoomChargeResponse, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class RoomChargeResponse : public TRemotable {
private:
  bool            FIsSuccessful;
  bool            FIsSuccessful_Specified;
  UnicodeString   FResponse;
  bool            FResponse_Specified;
  void __fastcall SetIsSuccessful(int Index, bool _prop_val)
  {  FIsSuccessful = _prop_val; FIsSuccessful_Specified = true;  }
  bool __fastcall IsSuccessful_Specified(int Index)
  {  return FIsSuccessful_Specified;  } 
  void __fastcall SetResponse(int Index, UnicodeString _prop_val)
  {  FResponse = _prop_val; FResponse_Specified = true;  }
  bool __fastcall Response_Specified(int Index)
  {  return FResponse_Specified;  } 
__published:
  __property bool       IsSuccessful = { index=(IS_OPTN), read=FIsSuccessful, write=SetIsSuccessful, stored = IsSuccessful_Specified };
  __property UnicodeString   Response = { index=(IS_OPTN|IS_NLBL), read=FResponse, write=SetResponse, stored = Response_Specified };
};




// ************************************************************************ //
// XML       : RoomRequest, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class RoomRequest2 : public RoomRequest {
private:
__published:
};




// ************************************************************************ //
// XML       : RoomDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class RoomDetails2 : public RoomDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : GuestDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class GuestDetails2 : public GuestDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : RoomChargeDetails, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class RoomChargeDetails2 : public RoomChargeDetails {
private:
__published:
};




// ************************************************************************ //
// XML       : SiHotService, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class SiHotService2 : public SiHotService {
private:
__published:
};




// ************************************************************************ //
// XML       : SiHotPayment, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class SiHotPayment2 : public SiHotPayment {
private:
__published:
};




// ************************************************************************ //
// XML       : RoomChargeResponse, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/SiHotIntegration.Domain
// ************************************************************************ //
class RoomChargeResponse2 : public RoomChargeResponse {
private:
__published:
};



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/ISiHotIntegrationWebService/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_ServiceSiHot
// service   : SiHotIntegrationWebService
// port      : basicHttpBinding_ServiceSiHot
// URL       : http://localhost:8742/MenumateServices/SiHotService/
// ************************************************************************ //
__interface INTERFACE_UUID("{D882000E-B7D1-AD60-7B83-61CBF026BB1B}") ISiHotIntegrationWebService : public IInvokable
{
public:
  virtual RoomDetails*    GetRoomDetails(const RoomRequest* roomRequest, const int timeOut) = 0; 
  virtual RoomChargeResponse* PostRoomCharge(const RoomChargeDetails* roomChargeDetails, const int timeOut) = 0; 
  virtual bool            ValidateCreadentials(const UnicodeString address, const int port, const int transno) = 0; 
};
typedef DelphiInterface<ISiHotIntegrationWebService> _di_ISiHotIntegrationWebService;

_di_ISiHotIntegrationWebService GetISiHotIntegrationWebService(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS__SiHotWSDL

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS__SiHotWSDL;
#endif



#endif // _H
