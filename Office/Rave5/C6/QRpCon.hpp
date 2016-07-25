// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRpCon.pas' rev: 6.00

#ifndef QRpConHPP
#define QRpConHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrpcon
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvCustomConnection;
class DELPHICLASS TDataConnectThread;
class PASCALIMPLEMENTATION TDataConnectThread : public Classes::TThread 
{
	typedef Classes::TThread inherited;
	
protected:
	Classes::TThreadMethod SyncMethod;
	bool SyncRan;
	TRvCustomConnection* FDataCon;
	void __fastcall SyncExecute(void);
	void __fastcall DoEvent(Classes::TThreadMethod AMethod);
	
public:
	virtual void __fastcall Execute(void);
	__property TRvCustomConnection* DataCon = {read=FDataCon, write=FDataCon};
public:
	#pragma option push -w-inl
	/* TThread.Create */ inline __fastcall TDataConnectThread(bool CreateSuspended) : Classes::TThread(CreateSuspended) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TThread.Destroy */ inline __fastcall virtual ~TDataConnectThread(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TRuntimeVisibility { rtNone, rtDeveloper, rtEndUser };
#pragma option pop

typedef void __fastcall (__closure *TRPConnectorEvent)(TRvCustomConnection* Connection);

typedef void __fastcall (__closure *TRPEOFEvent)(TRvCustomConnection* Connection, bool &Eof);

typedef void __fastcall (__closure *TRPValidateRowEvent)(TRvCustomConnection* Connection, bool &ValidRow);

class DELPHICLASS TDataConnectFieldDef;
class PASCALIMPLEMENTATION TRvCustomConnection : public Qrpdefine::TRpComponent 
{
	typedef Qrpdefine::TRpComponent inherited;
	
protected:
	unsigned NormFileMap;
	char *NormFileBuf;
	unsigned AltFileMap;
	char *AltFileBuf;
	int CurrentMapSize;
	Byte CurrentFileMap;
	char *FileBuf;
	char *FilePtr;
	char *RowHeaderPtr;
	bool ErrorResult;
	TDataConnectThread* Thread;
	bool Runtime;
	AnsiString ModeStr;
	bool ThreadDone;
	Classes::TList* FieldDefList;
	int FieldIdx;
	int IncludedFieldIdx;
	int FDataIndex;
	int FDataRows;
	AnsiString FFilterFieldName;
	AnsiString FFilterFieldOp;
	AnsiString FFilterFieldData;
	bool FLocalFilter;
	bool LocalFilterValid;
	TRuntimeVisibility FRTVisible;
	Classes::TStrings* FFieldAliasList;
	TRPConnectorEvent FOnFirst;
	TRPConnectorEvent FOnNext;
	TRPEOFEvent FOnEOF;
	TRPConnectorEvent FOnGetCols;
	TRPConnectorEvent FOnGetRow;
	TRPConnectorEvent FOnSetFilter;
	TRPConnectorEvent FOnGetSorts;
	TRPConnectorEvent FOnSetSort;
	TRPConnectorEvent FOnOpen;
	TRPConnectorEvent FOnRestore;
	TRPValidateRowEvent FOnValidateRow;
	void __fastcall ClearFieldDefs(void);
	void __fastcall ClearFilterData(void);
	void __fastcall CheckDataType(Qrpdefine::TRPDataTypeSet ValidDataTypes);
	void __fastcall AddingNative(void);
	void __fastcall AddingFormatted(void);
	void __fastcall AllocateSpace(int AddSize);
	bool __fastcall LocalFilterAbort(void *FilterData);
	void __fastcall SetRTVisible(TRuntimeVisibility Value);
	void __fastcall ReturnError(AnsiString Msg, Qrpdefine::TRaveConnectionError ErrorType);
	void __fastcall ReadVisible(Classes::TReader* Reader);
	virtual void __fastcall DefineProperties(Classes::TFiler* Filer);
	void __fastcall SetFieldAliasList(Classes::TStrings* Value);
	AnsiString __fastcall GetFieldAlias(AnsiString FieldName);
	void __fastcall NextField(void);
	
public:
	__fastcall virtual TRvCustomConnection(Classes::TComponent* AOwner);
	__fastcall virtual ~TRvCustomConnection(void);
	void __fastcall WriteStr(AnsiString Value);
	void __fastcall WriteInt(int Value);
	void __fastcall WriteInt64(__int64 Value);
	void __fastcall WriteBool(bool Value);
	void __fastcall WriteFloat(Extended Value);
	void __fastcall WriteCurr(System::Currency Value);
	void __fastcall WriteDateTime(System::TDateTime Value);
	void __fastcall WriteBuf(void *Buffer, int Len);
	void __fastcall WriteField(AnsiString Name, Qrpdefine::TRPDataType DataType, int Width, AnsiString FullName, AnsiString Description);
	void __fastcall WriteNullData(void);
	void __fastcall WriteStrData(AnsiString FormatData, AnsiString NativeData);
	void __fastcall WriteIntData(AnsiString FormatData, int NativeData);
	void __fastcall WriteInt64Data(AnsiString FormatData, __int64 NativeData);
	void __fastcall WriteBoolData(AnsiString FormatData, bool NativeData);
	void __fastcall WriteFloatData(AnsiString FormatData, Extended NativeData);
	void __fastcall WriteCurrData(AnsiString FormatData, System::Currency NativeData);
	void __fastcall WriteBCDData(AnsiString FormatData, System::Currency NativeData);
	void __fastcall WriteDateTimeData(AnsiString FormatData, System::TDateTime NativeData);
	void __fastcall WriteBlobData(void *Buffer, int Len);
	AnsiString __fastcall ReadStr();
	int __fastcall ReadInt(void);
	__int64 __fastcall ReadInt64(void);
	bool __fastcall ReadBool(void);
	Extended __fastcall ReadFloat(void);
	System::Currency __fastcall ReadCurr(void);
	System::TDateTime __fastcall ReadDateTime(void);
	void __fastcall ReadBuf(void *Buffer, int Len);
	TDataConnectFieldDef* __fastcall CurrentFieldDef(void);
	TDataConnectFieldDef* __fastcall FindFieldDef(AnsiString FieldName);
	bool __fastcall ReadFilterItem(AnsiString &FieldName, AnsiString &FieldOp, AnsiString &FieldData);
	bool __fastcall GetFilterItem(void);
	void __fastcall ClearBuffer(void);
	void __fastcall FixupBufferHeader(void);
	void __fastcall ClearBufferHeader(void);
	void __fastcall ExecFirst(void);
	void __fastcall ExecNext(void);
	void __fastcall ExecEOF(void);
	bool __fastcall GetEOF(void);
	void __fastcall ExecGetCols(void);
	void __fastcall ExecGetRow(void);
	void __fastcall ExecSetFilter(void);
	void __fastcall ExecGetSorts(void);
	void __fastcall ExecSetSort(void);
	void __fastcall ExecOpen(void);
	void __fastcall ExecRestore(void);
	bool __fastcall ExecValidateRow(void);
	virtual void __fastcall DoFirst(void);
	virtual void __fastcall DoNext(void);
	virtual bool __fastcall DoEOF(void);
	virtual void __fastcall DoGetCols(void);
	virtual void __fastcall DoGetRow(void);
	virtual void __fastcall DoSetFilter(void);
	virtual void __fastcall DoGetSorts(void);
	virtual void __fastcall DoSetSort(void);
	virtual void __fastcall DoOpen(void);
	virtual void __fastcall DoRestore(void);
	virtual bool __fastcall DoValidateRow(void);
	__property int DataIndex = {read=FDataIndex, write=FDataIndex, nodefault};
	__property int DataRows = {read=FDataRows, write=FDataRows, nodefault};
	__property AnsiString FilterFieldName = {read=FFilterFieldName};
	__property AnsiString FilterFieldOp = {read=FFilterFieldOp};
	__property AnsiString FilterFieldData = {read=FFilterFieldData};
	
__published:
	__property Classes::TStrings* FieldAliasList = {read=FFieldAliasList, write=SetFieldAliasList};
	__property bool LocalFilter = {read=FLocalFilter, write=FLocalFilter, default=1};
	__property TRuntimeVisibility RuntimeVisibility = {read=FRTVisible, write=SetRTVisible, stored=true, nodefault};
	__property TRPConnectorEvent OnFirst = {read=FOnFirst, write=FOnFirst};
	__property TRPConnectorEvent OnNext = {read=FOnNext, write=FOnNext};
	__property TRPEOFEvent OnEOF = {read=FOnEOF, write=FOnEOF};
	__property TRPConnectorEvent OnGetCols = {read=FOnGetCols, write=FOnGetCols};
	__property TRPConnectorEvent OnGetRow = {read=FOnGetRow, write=FOnGetRow};
	__property TRPConnectorEvent OnSetFilter = {read=FOnSetFilter, write=FOnSetFilter};
	__property TRPConnectorEvent OnGetSorts = {read=FOnGetSorts, write=FOnGetSorts};
	__property TRPConnectorEvent OnSetSort = {read=FOnSetSort, write=FOnSetSort};
	__property TRPConnectorEvent OnOpen = {read=FOnOpen, write=FOnOpen};
	__property TRPConnectorEvent OnRestore = {read=FOnRestore, write=FOnRestore};
	__property TRPValidateRowEvent OnValidateRow = {read=FOnValidateRow, write=FOnValidateRow};
};

typedef TRvCustomConnection TRPCustomConnection;

class DELPHICLASS TDataConnectManager;
class PASCALIMPLEMENTATION TDataConnectManager : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Classes::TList* FConnectList;
	
public:
	__fastcall TDataConnectManager(void);
	__fastcall virtual ~TDataConnectManager(void);
	void __fastcall AddConnection(TRvCustomConnection* Connection);
	void __fastcall RemoveConnection(TRvCustomConnection* Connection);
	TRvCustomConnection* __fastcall FindConnection(AnsiString ConnectName);
};


class PASCALIMPLEMENTATION TDataConnectFieldDef : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	Qrpdefine::TRPDataType FDataType;
	AnsiString FFieldName;
	bool FIncluded;
	int FFieldIndex;
	AnsiString StrData;
	void *BinData;
	int DataSize;
	
public:
	__fastcall TDataConnectFieldDef(Qrpdefine::TRPDataType ADataType, AnsiString AFieldName, bool AIncluded);
	__fastcall virtual ~TDataConnectFieldDef(void);
	void __fastcall Clear(void);
	void __fastcall SetFilter(void *Value);
	bool __fastcall FilterValid(void *Value);
	__property Qrpdefine::TRPDataType DataType = {read=FDataType, nodefault};
	__property int FieldIndex = {read=FFieldIndex, write=FFieldIndex, nodefault};
	__property AnsiString FieldName = {read=FFieldName};
	__property bool Included = {read=FIncluded, nodefault};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE int DataConVersion;
extern PACKAGE TDataConnectManager* DataConnectManager;
extern PACKAGE bool ManualSync;
extern PACKAGE void __fastcall ExecSyncMethod(void);

}	/* namespace Qrpcon */
using namespace Qrpcon;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRpCon
