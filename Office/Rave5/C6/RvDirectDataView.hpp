// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDirectDataView.pas' rev: 6.00

#ifndef RvDirectDataViewHPP
#define RvDirectDataViewHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvClass.hpp>	// Pascal unit
#include <RvDataField.hpp>	// Pascal unit
#include <RvData.hpp>	// Pascal unit
#include <RpCon.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdirectdataview
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TRaveDataResult { drContinue, drAbort, drPause };
#pragma option pop

class DELPHICLASS TRaveDataSystem;
typedef void __fastcall (__closure *TTimeoutEvent)(TRaveDataSystem* DataSystem, int Counter, int Timeout, int EventType, AnsiString Connection, bool First, TRaveDataResult &DataResult);

#pragma option push -b-
enum TRaveDataAction { daOpen, daClose };
#pragma option pop

typedef void __fastcall (__closure *TDataActionEvent)(TRaveDataSystem* DataSystem, TRaveDataAction DataAction);

class DELPHICLASS TRaveDataConnection;
class PASCALIMPLEMENTATION TRaveDataConnection : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	int FTag;
	AnsiString FName;
	bool FRuntime;
	HWND FAppHandle;
	Rpcon::TRvCustomConnection* FConnection;
	unsigned FDataEvent;
	int FVersion;
	Rvdata::TRaveBaseDataView* FDataView;
	
public:
	void __fastcall Assign(TRaveDataConnection* Value);
	__property HWND AppHandle = {read=FAppHandle, write=FAppHandle, nodefault};
	__property Rpcon::TRvCustomConnection* Connection = {read=FConnection, write=FConnection};
	__property unsigned DataEvent = {read=FDataEvent, write=FDataEvent, nodefault};
	__property Rvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView};
	__property AnsiString Name = {read=FName, write=FName};
	__property bool Runtime = {read=FRuntime, write=FRuntime, nodefault};
	__property int Tag = {read=FTag, write=FTag, nodefault};
	__property int Version = {read=FVersion, write=FVersion, nodefault};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveDataConnection(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRaveDataConnection(void) { }
	#pragma option pop
	
};


struct TDataSystemEventData;
typedef TDataSystemEventData *PDataSystemEventData;

#pragma pack(push, 4)
struct TDataSystemEventData
{
	TRaveDataConnection* DataCon;
	int EventType;
	bool Handled;
} ;
#pragma pack(pop)

#pragma pack(push, 4)
struct TRaveBufferInfo
{
	int Size;
	Byte FileMap;
	char *FileBuf;
} ;
#pragma pack(pop)

class PASCALIMPLEMENTATION TRaveDataSystem : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	int ControlCount;
	unsigned NormFileMap;
	char *NormFileBuf;
	unsigned AltFileMap;
	char *AltFileBuf;
	char *FileBuf;
	char *FilePtr;
	Byte CurrentFileMap;
	TRaveDataConnection* CurrentDataCon;
	unsigned ControllerMutex;
	unsigned CompletedEvent;
	unsigned ErrorEvent;
	TTimeoutEvent FOnSmallTimeout;
	TTimeoutEvent FOnLargeTimeout;
	TDataActionEvent FOnDataAction;
	Classes::TStringList* FRTConnectList;
	Classes::TStringList* FDTConnectList;
	bool FAutoUpdate;
	
public:
	__fastcall TRaveDataSystem(void);
	__fastcall virtual ~TRaveDataSystem(void);
	bool __fastcall GainControl(void);
	void __fastcall ReleaseControl(void);
	bool __fastcall IsUnique(AnsiString Name);
	void __fastcall UpdateConnections(void);
	AnsiString __fastcall ReadStr();
	int __fastcall ReadInt(void);
	bool __fastcall ReadBool(void);
	Extended __fastcall ReadFloat(void);
	System::Currency __fastcall ReadCurr(void);
	System::TDateTime __fastcall ReadDateTime(void);
	void __fastcall ReadBuf(void *Buffer, int Len);
	void * __fastcall ReadPtr(int Len);
	void __fastcall WriteStr(AnsiString Value);
	void __fastcall WriteInt(int Value);
	void __fastcall WriteInt64(__int64 Value);
	void __fastcall WriteBool(bool Value);
	void __fastcall WriteFloat(Extended Value);
	void __fastcall WriteCurr(System::Currency Value);
	void __fastcall WriteDateTime(System::TDateTime Value);
	void __fastcall WriteBuf(void *Buffer, int Len);
	void __fastcall ClearBuffer(void);
	void __fastcall GetBufferInfo(TRaveBufferInfo &BufferInfo);
	bool __fastcall OpenDataEvent(AnsiString AName, TRaveDataConnection* DataCon);
	void __fastcall CloseDataEvent(TRaveDataConnection* DataCon);
	bool __fastcall CallEvent(int EventType, TRaveDataConnection* DataCon);
	void __fastcall PrepareEvent(int EventType);
	void __fastcall OpenFileMap(TRaveDataConnection* DataCon);
	void __fastcall CloseFileMap(void);
	__property bool AutoUpdate = {read=FAutoUpdate, write=FAutoUpdate, nodefault};
	__property Classes::TStringList* RTConnectList = {read=FRTConnectList};
	__property Classes::TStringList* DTConnectList = {read=FDTConnectList};
	__property TTimeoutEvent OnSmallTimeout = {read=FOnSmallTimeout, write=FOnSmallTimeout};
	__property TTimeoutEvent OnLargeTimeout = {read=FOnLargeTimeout, write=FOnLargeTimeout};
	__property TDataActionEvent OnDataAction = {read=FOnDataAction, write=FOnDataAction};
};


class DELPHICLASS TRaveDataView;
class PASCALIMPLEMENTATION TRaveDataView : public Rvdata::TRaveBaseDataView 
{
	typedef Rvdata::TRaveBaseDataView inherited;
	
protected:
	AnsiString FConnectionName;
	TRaveDataConnection* FDataCon;
	virtual void __fastcall AssignTo(Classes::TPersistent* Dest);
	void __fastcall SetDataCon(TRaveDataConnection* Value);
	virtual void __fastcall GetRow(int EventType);
	
public:
	__fastcall virtual TRaveDataView(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveDataView(void);
	virtual void __fastcall Open(void);
	virtual void __fastcall Close(void);
	virtual void __fastcall AfterReport(void);
	virtual void __fastcall SetFilter(Classes::TStringList* FilterList);
	virtual void __fastcall SetRemoteSort(Classes::TStringList* SortList);
	virtual void __fastcall CreateFields(Classes::TList* AFieldList);
	__property TRaveDataConnection* DataCon = {read=FDataCon, write=SetDataCon};
	
__published:
	__property AnsiString ConnectionName = {read=FConnectionName, write=FConnectionName};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRaveDataSystem* RaveDataSystem;
extern PACKAGE TRaveDataConnection* __fastcall CreateDataCon(Rpcon::TRvCustomConnection* RPConnection);
extern PACKAGE TRaveDataView* __fastcall CreateDataView(TRaveDataConnection* DataCon);
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvdirectdataview */
using namespace Rvdirectdataview;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDirectDataView
