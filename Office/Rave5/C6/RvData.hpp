// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvData.pas' rev: 6.00

#ifndef RvDataHPP
#define RvDataHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDataField.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <RvUtil.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <StdCtrls.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <ComCtrls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdata
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveDataFieldInfo;
class PASCALIMPLEMENTATION TRaveDataFieldInfo : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	AnsiString FieldName;
	Rpdefine::TRPDataType DataType;
	int Width;
	AnsiString FullName;
	AnsiString Description;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveDataFieldInfo(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TRaveDataFieldInfo(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveBaseDataView;
class PASCALIMPLEMENTATION TRaveBaseDataView : public Rvclass::TRaveDataObject 
{
	typedef Rvclass::TRaveDataObject inherited;
	
protected:
	bool DataOpened;
	bool FEmpty;
	Classes::TStringList* FieldNameList;
	bool FSaving;
	bool AtEOF;
	Rvdatafield::TRaveDataRow* DataRow;
	Rvdatafield::TRaveDataRow* ValueListHead;
	Rvdatafield::TRaveDataRow* ValueListTail;
	Rvdatafield::TRaveDataRow* ValueListPtr;
	bool StateSaveEOF;
	Rvdatafield::TRaveDataRow* SaveValueListPtr;
	bool SaveAtEOF;
	bool SaveSaving;
	virtual bool __fastcall AcceptChild(TMetaClass* NewChild);
	void __fastcall AddValueListItem(void);
	virtual void __fastcall AfterReport(void);
	void __fastcall GetData(int EventType);
	virtual void __fastcall GetRow(int EventType) = 0 ;
	void __fastcall NextValueListItem(void);
	virtual void __fastcall SetName(const AnsiString NewName);
	
public:
	__fastcall virtual TRaveBaseDataView(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveBaseDataView(void);
	Rvdatafield::TRaveDataRow* __fastcall ActiveDataRow(void);
	virtual void __fastcall Close(void);
	Rvdatafield::TRaveDataField* __fastcall FieldByName(AnsiString FieldName);
	virtual void __fastcall First(void);
	virtual void __fastcall Next(void);
	virtual void __fastcall GetCurrentRow(void);
	virtual bool __fastcall Eof(void);
	virtual void __fastcall CreateFields(Classes::TList* AFieldList) = 0 ;
	virtual void __fastcall SetFilter(Classes::TStringList* FilterList) = 0 ;
	virtual void __fastcall SetRemoteSort(Classes::TStringList* SortList) = 0 ;
	virtual bool __fastcall CheckRowsLeft(int ReqRows);
	virtual void __fastcall SaveState(void);
	virtual void __fastcall RestoreState(void);
	virtual bool __fastcall CheckBeforeEOF(void);
	virtual void __fastcall Peek(int Amount);
	virtual void __fastcall Unpeek(void);
	__property bool Empty = {read=FEmpty, nodefault};
	__property bool Saving = {read=FSaving, nodefault};
};


class DELPHICLASS TRaveDataBuffer;
class PASCALIMPLEMENTATION TRaveDataBuffer : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	char *RowBuf;
	int RowBufSize;
	char *RowBufPtr;
	int FieldIdx;
	int FAddPad;
	int FFieldCount;
	int __fastcall GetBufferSize(void);
	void * __fastcall GetBuffer(void);
	void __fastcall AddingNative(void);
	void __fastcall AddingFormatted(void);
	void __fastcall AllocateSpace(int AddSize);
	void __fastcall WriteStr(AnsiString Value);
	void __fastcall WriteInt(int Value);
	void __fastcall WriteBool(bool Value);
	void __fastcall WriteFloat(Extended Value);
	void __fastcall WriteCurr(System::Currency Value);
	void __fastcall WriteDateTime(System::TDateTime Value);
	void __fastcall WriteBuf(void *Buffer, int Len);
	
public:
	__fastcall TRaveDataBuffer(void);
	__fastcall virtual ~TRaveDataBuffer(void);
	void __fastcall Init(int AFieldCount);
	void __fastcall WriteNullData(void);
	void __fastcall WriteStrData(AnsiString FormatData, AnsiString NativeData);
	void __fastcall WriteIntData(AnsiString FormatData, int NativeData);
	void __fastcall WriteBoolData(AnsiString FormatData, bool NativeData);
	void __fastcall WriteFloatData(AnsiString FormatData, Extended NativeData);
	void __fastcall WriteCurrData(AnsiString FormatData, System::Currency NativeData);
	void __fastcall WriteBCDData(AnsiString FormatData, System::Currency NativeData);
	void __fastcall WriteDateTimeData(AnsiString FormatData, System::TDateTime NativeData);
	void __fastcall WriteBlobData(void *Buffer, int Len);
	__property int AddPad = {read=FAddPad, write=FAddPad, nodefault};
	__property int FieldCount = {read=FFieldCount, nodefault};
	__property int BufferSize = {read=GetBufferSize, nodefault};
	__property void * Buffer = {read=GetBuffer};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE Rvclass::TRaveControl* __fastcall (*CreateTextControlProc)(Classes::TComponent* AOwner, Rvdatafield::TRaveDataField* DataField);
extern PACKAGE Rvclass::TRaveControl* __fastcall (*CreateDataTextControlProc)(Classes::TComponent* AOwner, Rvdatafield::TRaveDataField* DataField);
extern PACKAGE Rvclass::TRaveControl* __fastcall (*CreateDataMemoControlProc)(Classes::TComponent* AOwner, Rvdatafield::TRaveDataField* DataField);
extern PACKAGE AnsiString __fastcall ProcessDataStr(Rvclass::TRaveComponent* Self, TRaveBaseDataView* DefaultDataView, AnsiString Value);
extern PACKAGE AnsiString __fastcall CreateFieldName(AnsiString DataViewName, AnsiString FieldName);
extern PACKAGE void __fastcall CreateFields(TRaveBaseDataView* DataView, Classes::TStringList* DeletedFields, Classes::TStringList* ReplacedFields, bool DoCreate);
extern PACKAGE AnsiString __fastcall PerformLookup(TRaveBaseDataView* LookupDataView, AnsiString LookupValue, Rvdatafield::TRaveDataField* LookupValueField, AnsiString LookupField, AnsiString LookupDisplay, AnsiString LookupInvalid);
extern PACKAGE void __fastcall DataViewFirst(TRaveBaseDataView* DataView, AnsiString DetailKey, TRaveBaseDataView* MasterDataView, AnsiString MasterKey, AnsiString SortKey);

}	/* namespace Rvdata */
using namespace Rvdata;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvData
