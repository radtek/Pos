// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDataLink.pas' rev: 6.00

#ifndef RvDataLinkHPP
#define RvDataLinkHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDLBase.hpp>	// Pascal unit
#include <RvDLCommon.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdatalink
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDataConnection;
class DELPHICLASS TRaveDataLink;
typedef void __fastcall (*TRaveDataLinkProc)(void * Params);

class DELPHICLASS TResultSetFieldList;
class PASCALIMPLEMENTATION TRaveDataLink : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	AnsiString FFileName;
	int FLibHandle;
	TRaveDataLinkProc FInformationProc;
	TRaveDataLinkProc FConnectionConfigureProc;
	TRaveDataLinkProc FConnectionOpenProc;
	TRaveDataLinkProc FConnectionCloseProc;
	TRaveDataLinkProc FGetTablesProc;
	TRaveDataLinkProc FGetFieldsProc;
	TRaveDataLinkProc FResultSetOpenProc;
	TRaveDataLinkProc FResultSetCloseProc;
	TRaveDataLinkProc FResultSetConfigureProc;
	TRaveDataLinkProc FResultSetFirstProc;
	TRaveDataLinkProc FResultSetNextProc;
	TRaveDataLinkProc FResultSetEOFProc;
	TRaveDataLinkProc FResultSetGetRowProc;
	TRaveDataLinkProc FResultSetSetFilterProc;
	TRaveDataLinkProc FResultSetSetSortProc;
	TRaveDataLinkProc FErrorTextProc;
	Rvdlbase::TDLBaseDriver* FDriver;
	bool FInstalled;
	AnsiString FInternalName;
	AnsiString FDisplayName;
	int FVerMajor;
	int FVerMinor;
	void __fastcall BindProcs(void);
	void __fastcall CheckInstalled(void);
	void * __fastcall FindProc(const AnsiString Name);
	bool __fastcall GetIsDynamicLink(void);
	void __fastcall Information(void);
	void __fastcall LoadDLL(const AnsiString FileName);
	void __fastcall LogAPIError(const AnsiString APIName, const AnsiString Message);
	void __fastcall LogConnectionError(void * ConnectionHandle, const AnsiString APIName, const AnsiString TableName = "");
	void __fastcall LogGlobalError(const AnsiString APIName);
	void __fastcall LogResultSetError(void * ResultSetHandle, const AnsiString APIName);
	void __fastcall UnloadDLL(void);
	__fastcall virtual ~TRaveDataLink(void);
	void __fastcall ConnectionClose(void * ConnectionHandle);
	bool __fastcall ConnectionConfigure(AnsiString &DataSource, AnsiString &Options, AnsiString &UserName, AnsiString &Password);
	void __fastcall ConnectionOpen(const AnsiString DataSource, const AnsiString Options, const AnsiString UserName, const AnsiString Password, void * &ConnectionHandle);
	void __fastcall GetFieldDefs(void * ConnectionHandle, const AnsiString TableName, Classes::TStrings* FieldDefs);
	void __fastcall GetTableNames(void * ConnectionHandle, Classes::TStrings* TableNames);
	void __fastcall InitDynamic(const AnsiString FileName);
	void __fastcall InitStatic(TMetaClass* ADriverClass);
	void __fastcall ResultSetClose(void * &ResultSetHandle);
	bool __fastcall ResultSetConfigure(AnsiString &QueryCommand, void * ConnectionHandle, AnsiString &AStruct);
	bool __fastcall ResultSetEOF(void * ResultSetHandle);
	void __fastcall ResultSetFirst(void * ResultSetHandle);
	void __fastcall ResultSetGetRow(void * ResultSetHandle, int TransferCount, Rvdlcommon::PDLTransferFieldList TransferList, void * &RowBuffer, int &RowBufSize);
	void __fastcall ResultSetNext(void * ResultSetHandle);
	void __fastcall ResultSetOpen(void * ConnectionHandle, const AnsiString QueryCommand, void * &ResultSetHandle, TResultSetFieldList* FieldList);
	__property AnsiString DisplayName = {read=FDisplayName};
	__property bool Installed = {read=FInstalled, nodefault};
	__property AnsiString InternalName = {read=FInternalName};
	__property bool IsDynamicLink = {read=GetIsDynamicLink, nodefault};
	__property int VerMajor = {read=FVerMajor, nodefault};
	__property int VerMinor = {read=FVerMinor, nodefault};
	__property Rvdlbase::TDLBaseDriver* Driver = {read=FDriver};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveDataLink(void) : System::TObject() { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TDataConnection : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	TRaveDataLink* FDataLink;
	void *FConnectionHandle;
	AnsiString FDataSource;
	AnsiString FOptions;
	AnsiString FUserName;
	AnsiString FPassword;
	
public:
	__fastcall TDataConnection(TRaveDataLink* ADataLink);
	__fastcall virtual ~TDataConnection(void);
	void __fastcall Open(void);
	void __fastcall Close(void);
	void __fastcall GetFieldDefs(const AnsiString TableName, Classes::TStrings* FieldDefs);
	void __fastcall GetTableNames(Classes::TStrings* TableNames);
	bool __fastcall ResultSetConfigure(AnsiString &QueryCommand, AnsiString &AStruct);
	__property AnsiString DataSource = {read=FDataSource, write=FDataSource};
	__property AnsiString Password = {read=FPassword, write=FPassword};
	__property AnsiString Options = {read=FOptions, write=FOptions};
	__property AnsiString UserName = {read=FUserName, write=FUserName};
};


class DELPHICLASS TResultSetField;
class PASCALIMPLEMENTATION TResultSetField : public System::TObject 
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
	/* TObject.Create */ inline __fastcall TResultSetField(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TResultSetField(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TResultSetFieldList : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	TResultSetField* operator[](int Index) { return Items[Index]; }
	
protected:
	Classes::TList* FList;
	int __fastcall GetCount(void);
	TResultSetField* __fastcall GetItem(int Index);
	
public:
	__fastcall TResultSetFieldList(void);
	__fastcall virtual ~TResultSetFieldList(void);
	TResultSetField* __fastcall Add(void);
	void __fastcall Clear(void);
	__property int Count = {read=GetCount, nodefault};
	__property TResultSetField* Items[int Index] = {read=GetItem/*, default*/};
};


class DELPHICLASS TDataResultSet;
class PASCALIMPLEMENTATION TDataResultSet : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	TDataConnection* FConnection;
	AnsiString FCommandText;
	void *FResultSetHandle;
	TResultSetFieldList* FFieldList;
	bool *FTransferList;
	int __fastcall GetFieldCount(void);
	
public:
	__fastcall TDataResultSet(TDataConnection* AConnection);
	__fastcall virtual ~TDataResultSet(void);
	void __fastcall Close(void);
	bool __fastcall Eof(void);
	void __fastcall First(void);
	void __fastcall GetRow(void * &RowBuffer, int &RowBufSize);
	void __fastcall Next(void);
	void __fastcall Open(void);
	__property int FieldCount = {read=GetFieldCount, nodefault};
	__property TResultSetFieldList* Fields = {read=FFieldList};
	__property AnsiString CommandText = {read=FCommandText, write=FCommandText};
};


class DELPHICLASS TRaveDataLinks;
class PASCALIMPLEMENTATION TRaveDataLinks : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	TRaveDataLink* operator[](int Index) { return Items[Index]; }
	
protected:
	Classes::TList* FItems;
	int __fastcall GetCount(void);
	TRaveDataLink* __fastcall GetItem(int Index);
	
public:
	__fastcall TRaveDataLinks(void);
	__fastcall virtual ~TRaveDataLinks(void);
	void __fastcall AddDynamic(const AnsiString FileName);
	void __fastcall AddStatic(TMetaClass* ADriverClass);
	TDataConnection* __fastcall CreateConnection(const AnsiString InternalName);
	TRaveDataLink* __fastcall FindDataLink(const AnsiString InternalName);
	__property int Count = {read=GetCount, nodefault};
	__property TRaveDataLink* Items[int Index] = {read=GetItem/*, default*/};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRaveDataLinks* GDataLinks;
extern PACKAGE void __fastcall (*LogDataLinkError)(AnsiString APIName, AnsiString Message);
extern PACKAGE void __fastcall AddDynamicDataLinks(AnsiString Path);
extern PACKAGE void __fastcall AddStaticDataLinks(void);
extern PACKAGE TRaveDataLink* __fastcall FindDataLink(const AnsiString InternalName);

}	/* namespace Rvdatalink */
using namespace Rvdatalink;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDataLink
