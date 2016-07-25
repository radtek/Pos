// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvDLCommon.pas' rev: 6.00

#ifndef QRvDLCommonHPP
#define QRvDLCommonHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvdlcommon
{
//-- type declarations -------------------------------------------------------
typedef void *TConnectionHandle;

typedef void *TResultSetHandle;

struct TDLInformation;
typedef TDLInformation *PDLInformation;

#pragma pack(push, 1)
struct TDLInformation
{
	int Resultcode;
	bool Installed;
	Word VerMajor;
	Word VerMinor;
	char InternalName[33];
	char DisplayName[256];
} ;
#pragma pack(pop)

struct TDLConnectionConfigure;
typedef TDLConnectionConfigure *PDLConnectionConfigure;

#pragma pack(push, 1)
struct TDLConnectionConfigure
{
	int ResultCode;
	char *DataSource;
	char *Options;
	char *UserName;
	char *PassWord;
} ;
#pragma pack(pop)

struct TDLConnectionOpen;
typedef TDLConnectionOpen *PDLConnectionOpen;

#pragma pack(push, 1)
struct TDLConnectionOpen
{
	int Resultcode;
	char *DataSource;
	char *Options;
	char *UserName;
	char *Password;
	void *ConnectionHandle;
} ;
#pragma pack(pop)

struct TDLConnectionClose;
typedef TDLConnectionClose *PDLConnectionClose;

#pragma pack(push, 1)
struct TDLConnectionClose
{
	int ResultCode;
	void *ConnectionHandle;
} ;
#pragma pack(pop)

#pragma pack(push, 1)
struct TDLTableItem
{
	char *Name;
} ;
#pragma pack(pop)

typedef TDLTableItem TDLTableList[10000];

typedef TDLTableItem *PDLTableList;

struct TDLTables;
typedef TDLTables *PDLTables;

#pragma pack(push, 1)
struct TDLTables
{
	int ResultCode;
	void *ConnectionHandle;
	int TableCount;
	TDLTableItem *TableList;
} ;
#pragma pack(pop)

#pragma pack(push, 1)
struct TDLFieldItem
{
	char *FieldName;
	Byte DataType;
	int Width;
	char *FullName;
	char *Description;
} ;
#pragma pack(pop)

typedef TDLFieldItem TDLFieldList[10000];

typedef TDLFieldItem *PDLFieldList;

struct TDLFields;
typedef TDLFields *PDLFields;

#pragma pack(push, 1)
struct TDLFields
{
	int ResultCode;
	void *ConnectionHandle;
	char *TableName;
	int FieldCount;
	TDLFieldItem *FieldList;
} ;
#pragma pack(pop)

struct TDLResultSetOpen;
typedef TDLResultSetOpen *PDLResultSetOpen;

#pragma pack(push, 1)
struct TDLResultSetOpen
{
	int ResultCode;
	void *ConnectionHandle;
	char *QueryCommand;
	void *ResultSetHandle;
	int FieldCount;
	TDLFieldItem *FieldList;
} ;
#pragma pack(pop)

struct TDLResultSetClose;
typedef TDLResultSetClose *PDLResultSetClose;

#pragma pack(push, 1)
struct TDLResultSetClose
{
	int ResultCode;
	void *ResultSetHandle;
} ;
#pragma pack(pop)

struct TDLResultSetFirst;
typedef TDLResultSetFirst *PDLResultSetFirst;

#pragma pack(push, 1)
struct TDLResultSetFirst
{
	int ResultCode;
	void *ResultSetHandle;
} ;
#pragma pack(pop)

struct TDLResultSetNext;
typedef TDLResultSetNext *PDLResultSetNext;

#pragma pack(push, 1)
struct TDLResultSetNext
{
	int ResultCode;
	void *ResultSetHandle;
} ;
#pragma pack(pop)

struct TDLResultSetEOF;
typedef TDLResultSetEOF *PDLResultSetEOF;

#pragma pack(push, 1)
struct TDLResultSetEOF
{
	int ResultCode;
	void *ResultSetHandle;
	bool Eof;
} ;
#pragma pack(pop)

typedef bool TDLTransferFieldList[10000];

typedef bool *PDLTransferFieldList;

struct TDLResultSetRow;
typedef TDLResultSetRow *PDLResultSetRow;

#pragma pack(push, 1)
struct TDLResultSetRow
{
	int ResultCode;
	void *ResultSetHandle;
	int TransferCount;
	bool *TransferList;
	char *RowBuffer;
	int RowBufSize;
} ;
#pragma pack(pop)

struct TResultSetFilter;
typedef TResultSetFilter *PDLResultSetFilter;

#pragma pack(push, 1)
struct TResultSetFilter
{
	int ResultCode;
	void *ResultSetHandle;
} ;
#pragma pack(pop)

struct TDLResultSetSort;
typedef TDLResultSetSort *PDLResultSetSort;

#pragma pack(push, 1)
struct TDLResultSetSort
{
	int ResultCode;
	void *ResultSetHandle;
} ;
#pragma pack(pop)

struct TDLErrorText;
typedef TDLErrorText *PDLErrorText;

#pragma pack(push, 1)
struct TDLErrorText
{
	void *ConnectionHandle;
	void *ResultSetHandle;
	char *ErrorText;
} ;
#pragma pack(pop)

typedef void __fastcall (*TGetTablesProc)(PDLTables Params);

typedef void __fastcall (*TGetFieldsProc)(PDLFields Params);

typedef void __fastcall (*TConnectionCloseProc)(PDLConnectionClose Params);

typedef void __fastcall (*TConnectionConfigureProc)(PDLConnectionConfigure Params);

typedef void __fastcall (*TConnectionOpenProc)(PDLConnectionOpen Params);

typedef void __fastcall (*TGetErrorTextProc)(PDLErrorText Params);

typedef void __fastcall (*TInformationProc)(PDLInformation Params);

typedef void __fastcall (*TResultSetCloseProc)(PDLResultSetClose Params);

typedef void __fastcall (*TResultSetEOFProc)(PDLResultSetEOF Params);

typedef void __fastcall (*TResultSetFirstProc)(PDLResultSetFirst Params);

typedef void __fastcall (*TResultSetGetRowProc)(PDLResultSetRow Params);

typedef void __fastcall (*TResultSetNextProc)(PDLResultSetNext Params);

typedef void __fastcall (*TResultSetOpenProc)(PDLResultSetOpen Params);

typedef void __fastcall (*TResultSetSetFilterProc)(PDLResultSetFilter Params);

typedef void __fastcall (*TResultSetSetSortProc)(PDLResultSetSort Params);

struct TDLQueryEditor;
typedef TDLQueryEditor *PDLQueryEditor;

#pragma pack(push, 1)
struct TDLQueryEditor
{
	int ResultCode;
	void *ConnectionHandle;
	char *QueryCommand;
	char *QueryStruct;
	TGetTablesProc GetTablesProc;
	TGetFieldsProc GetFieldsProc;
	TConnectionCloseProc ConnectionCloseProc;
	TConnectionConfigureProc ConnectionConfigureProc;
	TConnectionOpenProc ConnectionOpenProc;
	TGetErrorTextProc GetErrorTextProc;
	TInformationProc InformationProc;
	TResultSetCloseProc ResultSetCloseProc;
	TResultSetEOFProc ResultSetEOFProc;
	TResultSetFirstProc ResultSetFirstProc;
	TResultSetGetRowProc ResultSetGetRowProc;
	TResultSetNextProc ResultSetNextProc;
	TResultSetOpenProc ResultSetOpenProc;
	TResultSetSetFilterProc ResultSetSetFilterProc;
	TResultSetSetSortProc ResultSetSetSortProc;
} ;
#pragma pack(pop)

struct TDLResultSetConfigure;
typedef TDLResultSetConfigure *PDLResultSetConfigure;

#pragma pack(push, 1)
struct TDLResultSetConfigure
{
	int ResultCode;
	void *ConnectionHandle;
	char *QueryCommand;
	char *QueryStruct;
} ;
#pragma pack(pop)

class DELPHICLASS TDLBuffer;
class PASCALIMPLEMENTATION TDLBuffer : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	char *FRowBuf;
	int FRowBufSize;
	char *FRowBufPtr;
	int FFieldIndex;
	int FAddPad;
	int FFieldCount;
	int __fastcall GetBufferSize(void);
	void * __fastcall GetBuffer(void);
	void __fastcall AddingNative(void);
	void __fastcall AddingFormatted(void);
	void __fastcall AllocateSpace(int AddSize);
	void __fastcall WriteStr(AnsiString Value);
	void __fastcall WriteLong(int Value);
	void __fastcall WriteBool(bool Value);
	void __fastcall WriteFloat(Extended Value);
	void __fastcall WriteCurr(System::Currency Value);
	void __fastcall WriteDateTime(System::TDateTime Value);
	void __fastcall WriteBuf(void *Buffer, int Len);
	
public:
	__fastcall TDLBuffer(void);
	__fastcall virtual ~TDLBuffer(void);
	void __fastcall Init(int AFieldCount);
	void __fastcall WriteNullData(void);
	void __fastcall WriteStrData(AnsiString FormatData, AnsiString NativeData);
	void __fastcall WriteLongData(AnsiString FormatData, int NativeData);
	void __fastcall WriteInt64Data(AnsiString FormatData, __int64 NativeData);
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
static const Shortint dt_String = 0x0;
static const Shortint dt_Integer = 0x1;
static const Shortint dt_Boolean = 0x2;
static const Shortint dt_Float = 0x3;
static const Shortint dt_Currency = 0x4;
static const Shortint dt_BCD = 0x5;
static const Shortint dt_Date = 0x6;
static const Shortint dt_Time = 0x7;
static const Shortint dt_DateTime = 0x8;
static const Shortint dt_Blob = 0x9;
static const Shortint dt_Memo = 0xa;
static const Shortint dt_Graphic = 0xb;
static const Shortint dt_Int64 = 0xc;

}	/* namespace Qrvdlcommon */
using namespace Qrvdlcommon;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvDLCommon
