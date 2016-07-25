// ************************************************************************ //
// The types declared in this file were generated from data read from the
// WSDL File described below:
// WSDL     : C:\Users\michael\dev\5.3\Static Files\XML Examples\FloorPlan wsdl.xml
//  >Import : C:\Users\michael\dev\5.3\Static Files\XML Examples\FloorPlan wsdl.xml>0
//  >Import : http://192.168.42.223:8732/MenumateWinServices/service?xsd=xsd0
//  >Import : http://192.168.42.223:8732/MenumateWinServices/service?xsd=xsd1
//  >Import : http://192.168.42.223:8732/MenumateWinServices/service?xsd=xsd2
//  >Import : http://192.168.42.223:8732/MenumateWinServices/service?xsd=xsd3
// Codegen  : [wfMapArraysToClasses+, wfUseSettersAndGetters+]
// (10/08/2011 8:48:13 a.m. - - $Rev: 25127 $)
// ************************************************************************ //

#ifndef   PosIntegrationH
#define   PosIntegrationH

#include <System.hpp>
#include <XSBuiltIns.hpp>
#include <SOAPHTTPClient.hpp>
#include <InvokeRegistry.hpp>

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


namespace NS_PosIntegration {

// ************************************************************************ //
// The following types, referred to in the WSDL document are not being represented
// in this file. They are either aliases[@] of other types represented or were referred
// to but never[!] declared in the document. The types from the latter category
// typically map to predefined/known XML or Embarcadero types; however, they could also 
// indicate incorrect WSDL documents that failed to declare or import a schema type.
// ************************************************************************ //
// !:int             - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:base64Binary    - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:duration        - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:string          - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:boolean         - "http://www.w3.org/2001/XMLSchema"[Gbl]
// !:double          - "http://www.w3.org/2001/XMLSchema"[Gbl]

class SOAP_REMOTABLE_CLASS OperationResult;
class SOAP_REMOTABLE_CLASS DTOReservable;
class SOAP_REMOTABLE_CLASS DTOLocation;
class SOAP_REMOTABLE_CLASS DTOSession;
class SOAP_REMOTABLE_CLASS DTOLocation2;
class SOAP_REMOTABLE_CLASS DTOReservable2;
class SOAP_REMOTABLE_CLASS DTOSession2;
/*class SOAP_REMOTABLE_CLASS OperationResult2;
class SOAP_REMOTABLE_CLASS ArrayOfDTOLocation;
class SOAP_REMOTABLE_CLASS ArrayOfDTOLocation2;
class SOAP_REMOTABLE_CLASS ArrayOfDTOReservable;
class SOAP_REMOTABLE_CLASS ArrayOfDTOReservable2;
class SOAP_REMOTABLE_CLASS ArrayOfDTOSession;
class SOAP_REMOTABLE_CLASS ArrayOfDTOSession2;*/

typedef TXSDuration duration; /* "http://schemas.microsoft.com/2003/10/Serialization/"[GblSmpl] */
typedef DynamicArray<DTOLocation*> ArrayOfDTOLocation; /* "http://schemas.datacontract.org/2004/07/MenuMate.Reservations.Services.Contracts.DTO"[GblCplx] */
typedef DynamicArray<DTOReservable*> ArrayOfDTOReservable; /* "http://schemas.datacontract.org/2004/07/MenuMate.Reservations.Services.Contracts.DTO"[GblCplx] */
typedef DynamicArray<DTOSession*> ArrayOfDTOSession; /* "http://schemas.datacontract.org/2004/07/MenuMate.Reservations.Services.Contracts.DTO"[GblCplx] */


// ************************************************************************ //
// XML       : OperationResult, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts
// ************************************************************************ //
class OperationResult : public TRemotable {
private:
  UnicodeString   FLongMessage;
  bool            FLongMessage_Specified;
  UnicodeString   FMessage;
  bool            FMessage_Specified;
  int             FObjectAffected;
  bool            FObjectAffected_Specified;
  bool            FSuccess;
  bool            FSuccess_Specified;
  UnicodeString __fastcall GetLongMessage(int Index)
  {  return FLongMessage;  }
  void __fastcall SetLongMessage(int Index, UnicodeString _prop_val)
  {  FLongMessage = _prop_val; FLongMessage_Specified = true;  }
  bool __fastcall LongMessage_Specified(int Index)
  {  return FLongMessage_Specified;  } 
  UnicodeString __fastcall GetMessage(int Index)
  {  return FMessage;  }
  void __fastcall SetMessage(int Index, UnicodeString _prop_val)
  {  FMessage = _prop_val; FMessage_Specified = true;  }
  bool __fastcall Message_Specified(int Index)
  {  return FMessage_Specified;  } 
  int __fastcall GetObjectAffected(int Index)
  {  return FObjectAffected;  }
  void __fastcall SetObjectAffected(int Index, int _prop_val)
  {  FObjectAffected = _prop_val; FObjectAffected_Specified = true;  }
  bool __fastcall ObjectAffected_Specified(int Index)
  {  return FObjectAffected_Specified;  } 
  bool __fastcall GetSuccess(int Index)
  {  return FSuccess;  }
  void __fastcall SetSuccess(int Index, bool _prop_val)
  {  FSuccess = _prop_val; FSuccess_Specified = true;  }
  bool __fastcall Success_Specified(int Index)
  {  return FSuccess_Specified;  } 
__published:
  __property UnicodeString LongMessage = { index=(IS_OPTN|IS_NLBL), read=FLongMessage, write=SetLongMessage, stored = LongMessage_Specified };
  __property UnicodeString    Message = { index=(IS_OPTN|IS_NLBL), read=FMessage, write=SetMessage, stored = Message_Specified };
  __property int        ObjectAffected = { index=(IS_OPTN), read=FObjectAffected, write=SetObjectAffected, stored = ObjectAffected_Specified };
  __property bool          Success = { index=(IS_OPTN), read=FSuccess, write=SetSuccess, stored = Success_Specified };
};




// ************************************************************************ //
// XML       : DTOReservable, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
class DTOReservable : public TRemotable {
private:
  bool            FAvailable;
  bool            FAvailable_Specified;
  int             FCurrentInterestLevel;
  bool            FCurrentInterestLevel_Specified;
  double          FHeight;
  bool            FHeight_Specified;
  int             FId;
  bool            FId_Specified;
  int             FMaxGuests;
  bool            FMaxGuests_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  int             FNumber;
  bool            FNumber_Specified;
  DTOLocation*    FParentLocation;
  bool            FParentLocation_Specified;
  double          FRotationAngle;
  bool            FRotationAngle_Specified;
  UnicodeString   FShape;
  bool            FShape_Specified;
  UnicodeString   FTableColor;
  bool            FTableColor_Specified;
  double          FWidth;
  bool            FWidth_Specified;
  double          FX;
  bool            FX_Specified;
  double          FY;
  bool            FY_Specified;
  bool __fastcall GetAvailable(int Index)
  {  return FAvailable;  }
  void __fastcall SetAvailable(int Index, bool _prop_val)
  {  FAvailable = _prop_val; FAvailable_Specified = true;  }
  bool __fastcall Available_Specified(int Index)
  {  return FAvailable_Specified;  } 
  int __fastcall GetCurrentInterestLevel(int Index)
  {  return FCurrentInterestLevel;  }
  void __fastcall SetCurrentInterestLevel(int Index, int _prop_val)
  {  FCurrentInterestLevel = _prop_val; FCurrentInterestLevel_Specified = true;  }
  bool __fastcall CurrentInterestLevel_Specified(int Index)
  {  return FCurrentInterestLevel_Specified;  } 
  double __fastcall GetHeight(int Index)
  {  return FHeight;  }
  void __fastcall SetHeight(int Index, double _prop_val)
  {  FHeight = _prop_val; FHeight_Specified = true;  }
  bool __fastcall Height_Specified(int Index)
  {  return FHeight_Specified;  } 
  int __fastcall GetId(int Index)
  {  return FId;  }
  void __fastcall SetId(int Index, int _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  int __fastcall GetMaxGuests(int Index)
  {  return FMaxGuests;  }
  void __fastcall SetMaxGuests(int Index, int _prop_val)
  {  FMaxGuests = _prop_val; FMaxGuests_Specified = true;  }
  bool __fastcall MaxGuests_Specified(int Index)
  {  return FMaxGuests_Specified;  } 
  UnicodeString __fastcall GetName(int Index)
  {  return FName;  }
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  int __fastcall GetNumber(int Index)
  {  return FNumber;  }
  void __fastcall SetNumber(int Index, int _prop_val)
  {  FNumber = _prop_val; FNumber_Specified = true;  }
  bool __fastcall Number_Specified(int Index)
  {  return FNumber_Specified;  } 
  DTOLocation* __fastcall GetParentLocation(int Index)
  {  return FParentLocation;  }
  void __fastcall SetParentLocation(int Index, DTOLocation* _prop_val)
  {  FParentLocation = _prop_val; FParentLocation_Specified = true;  }
  bool __fastcall ParentLocation_Specified(int Index)
  {  return FParentLocation_Specified;  } 
  double __fastcall GetRotationAngle(int Index)
  {  return FRotationAngle;  }
  void __fastcall SetRotationAngle(int Index, double _prop_val)
  {  FRotationAngle = _prop_val; FRotationAngle_Specified = true;  }
  bool __fastcall RotationAngle_Specified(int Index)
  {  return FRotationAngle_Specified;  } 
  UnicodeString __fastcall GetShape(int Index)
  {  return FShape;  }
  void __fastcall SetShape(int Index, UnicodeString _prop_val)
  {  FShape = _prop_val; FShape_Specified = true;  }
  bool __fastcall Shape_Specified(int Index)
  {  return FShape_Specified;  } 
  UnicodeString __fastcall GetTableColor(int Index)
  {  return FTableColor;  }
  void __fastcall SetTableColor(int Index, UnicodeString _prop_val)
  {  FTableColor = _prop_val; FTableColor_Specified = true;  }
  bool __fastcall TableColor_Specified(int Index)
  {  return FTableColor_Specified;  } 
  double __fastcall GetWidth(int Index)
  {  return FWidth;  }
  void __fastcall SetWidth(int Index, double _prop_val)
  {  FWidth = _prop_val; FWidth_Specified = true;  }
  bool __fastcall Width_Specified(int Index)
  {  return FWidth_Specified;  } 
  double __fastcall GetX(int Index)
  {  return FX;  }
  void __fastcall SetX(int Index, double _prop_val)
  {  FX = _prop_val; FX_Specified = true;  }
  bool __fastcall X_Specified(int Index)
  {  return FX_Specified;  } 
  double __fastcall GetY(int Index)
  {  return FY;  }
  void __fastcall SetY(int Index, double _prop_val)
  {  FY = _prop_val; FY_Specified = true;  }
  bool __fastcall Y_Specified(int Index)
  {  return FY_Specified;  } 

public:
  __fastcall ~DTOReservable();
__published:
  __property bool        Available = { index=(IS_OPTN), read=FAvailable, write=SetAvailable, stored = Available_Specified };
  __property int        CurrentInterestLevel = { index=(IS_OPTN), read=FCurrentInterestLevel, write=SetCurrentInterestLevel, stored = CurrentInterestLevel_Specified };
  __property double         Height = { index=(IS_OPTN), read=FHeight, write=SetHeight, stored = Height_Specified };
  __property int                Id = { index=(IS_OPTN), read=FId, write=SetId, stored = Id_Specified };
  __property int         MaxGuests = { index=(IS_OPTN), read=FMaxGuests, write=SetMaxGuests, stored = MaxGuests_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property int            Number = { index=(IS_OPTN), read=FNumber, write=SetNumber, stored = Number_Specified };
  __property DTOLocation* ParentLocation = { index=(IS_OPTN|IS_NLBL), read=FParentLocation, write=SetParentLocation, stored = ParentLocation_Specified };
  __property double     RotationAngle = { index=(IS_OPTN), read=FRotationAngle, write=SetRotationAngle, stored = RotationAngle_Specified };
  __property UnicodeString      Shape = { index=(IS_OPTN|IS_NLBL), read=FShape, write=SetShape, stored = Shape_Specified };
  __property UnicodeString TableColor = { index=(IS_OPTN|IS_NLBL), read=FTableColor, write=SetTableColor, stored = TableColor_Specified };
  __property double          Width = { index=(IS_OPTN), read=FWidth, write=SetWidth, stored = Width_Specified };
  __property double              X = { index=(IS_OPTN), read=FX, write=SetX, stored = X_Specified };
  __property double              Y = { index=(IS_OPTN), read=FY, write=SetY, stored = Y_Specified };
};




// ************************************************************************ //
// XML       : DTOLocation, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
class DTOLocation : public TRemotable {
private:
  TByteDynArray   FBackgroundImg;
  bool            FBackgroundImg_Specified;
  duration*       FDefaultDuration;
  bool            FDefaultDuration_Specified;
  double          FHeight;
  bool            FHeight_Specified;
  int             FId;
  bool            FId_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  ArrayOfDTOReservable* FReservableList;
  bool            FReservableList_Specified;
  ArrayOfDTOSession* FSessions;
  bool            FSessions_Specified;
  double          FWidth;
  bool            FWidth_Specified;
  TByteDynArray __fastcall GetBackgroundImg(int Index)
  {  return FBackgroundImg;  }
  void __fastcall SetBackgroundImg(int Index, TByteDynArray _prop_val)
  {  FBackgroundImg = _prop_val; FBackgroundImg_Specified = true;  }
  bool __fastcall BackgroundImg_Specified(int Index)
  {  return FBackgroundImg_Specified;  } 
  duration* __fastcall GetDefaultDuration(int Index)
  {  return FDefaultDuration;  }
  void __fastcall SetDefaultDuration(int Index, duration* _prop_val)
  {  FDefaultDuration = _prop_val; FDefaultDuration_Specified = true;  }
  bool __fastcall DefaultDuration_Specified(int Index)
  {  return FDefaultDuration_Specified;  } 
  double __fastcall GetHeight(int Index)
  {  return FHeight;  }
  void __fastcall SetHeight(int Index, double _prop_val)
  {  FHeight = _prop_val; FHeight_Specified = true;  }
  bool __fastcall Height_Specified(int Index)
  {  return FHeight_Specified;  } 
  int __fastcall GetId(int Index)
  {  return FId;  }
  void __fastcall SetId(int Index, int _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  UnicodeString __fastcall GetName(int Index)
  {  return FName;  }
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  ArrayOfDTOReservable* __fastcall GetReservableList(int Index)
  {  return FReservableList;  }
  void __fastcall SetReservableList(int Index, ArrayOfDTOReservable* _prop_val)
  {  FReservableList = _prop_val; FReservableList_Specified = true;  }
  bool __fastcall ReservableList_Specified(int Index)
  {  return FReservableList_Specified;  } 
  ArrayOfDTOSession* __fastcall GetSessions(int Index)
  {  return FSessions;  }
  void __fastcall SetSessions(int Index, ArrayOfDTOSession* _prop_val)
  {  FSessions = _prop_val; FSessions_Specified = true;  }
  bool __fastcall Sessions_Specified(int Index)
  {  return FSessions_Specified;  } 
  double __fastcall GetWidth(int Index)
  {  return FWidth;  }
  void __fastcall SetWidth(int Index, double _prop_val)
  {  FWidth = _prop_val; FWidth_Specified = true;  }
  bool __fastcall Width_Specified(int Index)
  {  return FWidth_Specified;  } 

public:
  __fastcall ~DTOLocation();
__published:
  __property TByteDynArray BackgroundImg = { index=(IS_OPTN|IS_NLBL), read=FBackgroundImg, write=SetBackgroundImg, stored = BackgroundImg_Specified };
  __property duration*  DefaultDuration = { index=(IS_OPTN), read=FDefaultDuration, write=SetDefaultDuration, stored = DefaultDuration_Specified };
  __property double         Height = { index=(IS_OPTN), read=FHeight, write=SetHeight, stored = Height_Specified };
  __property int                Id = { index=(IS_OPTN), read=FId, write=SetId, stored = Id_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property ArrayOfDTOReservable* ReservableList = { index=(IS_OPTN|IS_NLBL), read=FReservableList, write=SetReservableList, stored = ReservableList_Specified };
  __property ArrayOfDTOSession*   Sessions = { index=(IS_OPTN|IS_NLBL), read=FSessions, write=SetSessions, stored = Sessions_Specified };
  __property double          Width = { index=(IS_OPTN), read=FWidth, write=SetWidth, stored = Width_Specified };
};




// ************************************************************************ //
// XML       : DTOSession, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
class DTOSession : public TRemotable {
private:
  duration*       FEndTime;
  bool            FEndTime_Specified;
  int             FId;
  bool            FId_Specified;
  bool            FIsInSession;
  bool            FIsInSession_Specified;
  UnicodeString   FName;
  bool            FName_Specified;
  duration*       FStartTime;
  bool            FStartTime_Specified;
  duration* __fastcall GetEndTime(int Index)
  {  return FEndTime;  }
  void __fastcall SetEndTime(int Index, duration* _prop_val)
  {  FEndTime = _prop_val; FEndTime_Specified = true;  }
  bool __fastcall EndTime_Specified(int Index)
  {  return FEndTime_Specified;  } 
  int __fastcall GetId(int Index)
  {  return FId;  }
  void __fastcall SetId(int Index, int _prop_val)
  {  FId = _prop_val; FId_Specified = true;  }
  bool __fastcall Id_Specified(int Index)
  {  return FId_Specified;  } 
  bool __fastcall GetIsInSession(int Index)
  {  return FIsInSession;  }
  void __fastcall SetIsInSession(int Index, bool _prop_val)
  {  FIsInSession = _prop_val; FIsInSession_Specified = true;  }
  bool __fastcall IsInSession_Specified(int Index)
  {  return FIsInSession_Specified;  } 
  UnicodeString __fastcall GetName(int Index)
  {  return FName;  }
  void __fastcall SetName(int Index, UnicodeString _prop_val)
  {  FName = _prop_val; FName_Specified = true;  }
  bool __fastcall Name_Specified(int Index)
  {  return FName_Specified;  } 
  duration* __fastcall GetStartTime(int Index)
  {  return FStartTime;  }
  void __fastcall SetStartTime(int Index, duration* _prop_val)
  {  FStartTime = _prop_val; FStartTime_Specified = true;  }
  bool __fastcall StartTime_Specified(int Index)
  {  return FStartTime_Specified;  } 

public:
  __fastcall ~DTOSession();
__published:
  __property duration*     EndTime = { index=(IS_OPTN), read=FEndTime, write=SetEndTime, stored = EndTime_Specified };
  __property int                Id = { index=(IS_OPTN), read=FId, write=SetId, stored = Id_Specified };
  __property bool       IsInSession = { index=(IS_OPTN), read=FIsInSession, write=SetIsInSession, stored = IsInSession_Specified };
  __property UnicodeString       Name = { index=(IS_OPTN|IS_NLBL), read=FName, write=SetName, stored = Name_Specified };
  __property duration*   StartTime = { index=(IS_OPTN), read=FStartTime, write=SetStartTime, stored = StartTime_Specified };
};




// ************************************************************************ //
// XML       : DTOLocation, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
class DTOLocation2 : public DTOLocation {
private:
__published:
};




// ************************************************************************ //
// XML       : DTOReservable, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
class DTOReservable2 : public DTOReservable {
private:
__published:
};




// ************************************************************************ //
// XML       : DTOSession, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
class DTOSession2 : public DTOSession {
private:
__published:
};




// ************************************************************************ //
// XML       : OperationResult, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts
// ************************************************************************ //
class OperationResult2 : public OperationResult {
private:
__published:
};



//typedef DynamicArray<DTOLocation*> Array_Of_DTOLocation; /* "http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO"[GblUbnd] */

// ************************************************************************ //
// XML       : ArrayOfDTOLocation, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
/*
class ArrayOfDTOLocation : public TRemotable {
private:
  Array_Of_DTOLocation FDTOLocation;
  bool            FDTOLocation_Specified;
  Array_Of_DTOLocation __fastcall GetDTOLocation(int Index)
  {  return FDTOLocation;  }
  void __fastcall SetDTOLocation(int Index, Array_Of_DTOLocation _prop_val)
  {  FDTOLocation = _prop_val; FDTOLocation_Specified = true;  }
  bool __fastcall DTOLocation_Specified(int Index)
  {  return FDTOLocation_Specified;  } 

  int  GetLength() const  { return FDTOLocation.get_length(); }
  void SetLength(int len) { FDTOLocation_Specified = true; FDTOLocation.set_length(len); }

public:
  __fastcall ~ArrayOfDTOLocation();

  DTOLocation*& operator[](int index) { FDTOLocation_Specified = true; return FDTOLocation[index]; }
  __property int Length = { read=GetLength, write=SetLength };

__published:
  __property Array_Of_DTOLocation DTOLocation = { index=(IS_OPTN|IS_UNBD|IS_NLBL), read=FDTOLocation, write=SetDTOLocation, stored = DTOLocation_Specified };
};
*/



// ************************************************************************ //
// XML       : ArrayOfDTOLocation, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
/*
class ArrayOfDTOLocation2 : public ArrayOfDTOLocation {
private:
__published:
};


//typedef DynamicArray<DTOReservable*> Array_Of_DTOReservable; /* "http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO"[GblUbnd] */


// ************************************************************************ //
// XML       : ArrayOfDTOReservable, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
/*
class ArrayOfDTOReservable : public TRemotable {
private:
  Array_Of_DTOReservable FDTOReservable;
  bool            FDTOReservable_Specified;
  Array_Of_DTOReservable __fastcall GetDTOReservable(int Index)
  {  return FDTOReservable;  }
  void __fastcall SetDTOReservable(int Index, Array_Of_DTOReservable _prop_val)
  {  FDTOReservable = _prop_val; FDTOReservable_Specified = true;  }
  bool __fastcall DTOReservable_Specified(int Index)
  {  return FDTOReservable_Specified;  } 

  int  GetLength() const  { return FDTOReservable.get_length(); }
  void SetLength(int len) { FDTOReservable_Specified = true; FDTOReservable.set_length(len); }

public:
  __fastcall ~ArrayOfDTOReservable();

  DTOReservable*& operator[](int index) { FDTOReservable_Specified = true; return FDTOReservable[index]; }
  __property int Length = { read=GetLength, write=SetLength };

__published:
  __property Array_Of_DTOReservable DTOReservable = { index=(IS_OPTN|IS_UNBD|IS_NLBL), read=FDTOReservable, write=SetDTOReservable, stored = DTOReservable_Specified };
};

*/


// ************************************************************************ //
// XML       : ArrayOfDTOReservable, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
/*
class ArrayOfDTOReservable2 : public ArrayOfDTOReservable {
private:
__published:
};*/


//typedef DynamicArray<DTOSession*> Array_Of_DTOSession; /* "http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO"[GblUbnd] */


// ************************************************************************ //
// XML       : ArrayOfDTOSession, global, <complexType>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
/*class ArrayOfDTOSession : public TRemotable {
private:
  Array_Of_DTOSession FDTOSession;
  bool            FDTOSession_Specified;
  Array_Of_DTOSession __fastcall GetDTOSession(int Index)
  {  return FDTOSession;  }
  void __fastcall SetDTOSession(int Index, Array_Of_DTOSession _prop_val)
  {  FDTOSession = _prop_val; FDTOSession_Specified = true;  }
  bool __fastcall DTOSession_Specified(int Index)
  {  return FDTOSession_Specified;  } 

  int  GetLength() const  { return FDTOSession.get_length(); }
  void SetLength(int len) { FDTOSession_Specified = true; FDTOSession.set_length(len); }

public:
  __fastcall ~ArrayOfDTOSession();

  DTOSession*& operator[](int index) { FDTOSession_Specified = true; return FDTOSession[index]; }
  __property int Length = { read=GetLength, write=SetLength };

__published:
  __property Array_Of_DTOSession DTOSession = { index=(IS_OPTN|IS_UNBD|IS_NLBL), read=FDTOSession, write=SetDTOSession, stored = DTOSession_Specified };
};

*/


// ************************************************************************ //
// XML       : ArrayOfDTOSession, global, <element>
// Namespace : http://schemas.datacontract.org/2004/07/Menumate.WinServices.Implementation.Contracts.DTO
// ************************************************************************ //
/*
class ArrayOfDTOSession2 : public ArrayOfDTOSession {
private:
__published:
};*/



// ************************************************************************ //
// Namespace : http://tempuri.org/
// soapAction: http://tempuri.org/IPosIntegration/%operationName%
// transport : http://schemas.xmlsoap.org/soap/http
// style     : document
// binding   : basicHttpBinding_PosIntegration
// service   : PosIntegrationService
// port      : basicHttpBinding_PosIntegration
// URL       : http://192.168.42.223:8732/MenumateWinServices/service
// ************************************************************************ //
__interface INTERFACE_UUID("{EB651AF0-4D1D-294F-5131-6E9DBD65CE9E}") IPosIntegration : public IInvokable
{
public:
  virtual ArrayOfDTOLocation GetAllLocations() = 0;
  virtual ArrayOfDTOReservable GetTablesForLocation(const int locationId) = 0;
  virtual TByteDynArray   GetBackgroundImgDataForLocation(const int locationid) = 0;
  virtual OperationResult AddLocation(const DTOLocation* locationDTO) = 0;
  virtual OperationResult AddReservable(const DTOReservable* tableDTO, const DTOLocation* locationDTO) = 0;
  virtual OperationResult DeleteLocation(const DTOLocation* locationDTO) = 0;
  virtual OperationResult DeleteReservable(const DTOReservable* tableDTO, const DTOLocation* locationDTO) = 0;
  virtual OperationResult UpdateLocation(const DTOLocation* location) = 0;
  virtual OperationResult UpdateReservable(const DTOReservable* reservable) = 0;
};
typedef DelphiInterface<IPosIntegration> _di_IPosIntegration;

_di_IPosIntegration GetIPosIntegration(bool useWSDL=false, AnsiString addr="", THTTPRIO* HTTPRIO=0);


};     // NS_PosIntegration

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using  namespace NS_PosIntegration;
#endif



#endif // PosIntegrationH
