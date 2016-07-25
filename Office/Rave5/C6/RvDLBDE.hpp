// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDLBDE.pas' rev: 6.00

#ifndef RvDLBDEHPP
#define RvDLBDEHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <DBTables.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <RvDLDataSet.hpp>	// Pascal unit
#include <RvDLBase.hpp>	// Pascal unit
#include <RvDLCommon.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdlbde
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDLBDEDriver;
class PASCALIMPLEMENTATION TDLBDEDriver : public Rvdldataset::TDLDataSetDriver 
{
	typedef Rvdldataset::TDLDataSetDriver inherited;
	
public:
	virtual Rvdlbase::TDLBaseConnection* __fastcall CreateConnection(void);
	virtual Rvdlbase::TDLBaseResultSet* __fastcall CreateResultSet(Rvdlbase::TDLBaseConnection* AConnection);
	virtual void __fastcall Information(Rvdlcommon::PDLInformation Params);
public:
	#pragma option push -w-inl
	/* TDLBaseDriver.Create */ inline __fastcall virtual TDLBDEDriver(void) : Rvdldataset::TDLDataSetDriver() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseDriver.Destroy */ inline __fastcall virtual ~TDLBDEDriver(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLBDEConnection;
class PASCALIMPLEMENTATION TDLBDEConnection : public Rvdldataset::TDLDataSetConnection 
{
	typedef Rvdldataset::TDLDataSetConnection inherited;
	
protected:
	Dbtables::TSession* FSession;
	Dbtables::TDatabase* FDatabase;
	
public:
	virtual void __fastcall Connect(AnsiString DataSource, AnsiString UserName, AnsiString Password, Classes::TStringList* OptionList);
	virtual void __fastcall Disconnect(void);
	virtual void __fastcall GetTableNames(Classes::TStrings* List);
	__property Dbtables::TSession* Session = {read=FSession};
	__property Dbtables::TDatabase* Database = {read=FDatabase};
public:
	#pragma option push -w-inl
	/* TDLBaseConnection.Create */ inline __fastcall virtual TDLBDEConnection(void) : Rvdldataset::TDLDataSetConnection() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TDLBDEConnection(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLBDEResultSet;
class PASCALIMPLEMENTATION TDLBDEResultSet : public Rvdldataset::TDLDataSetResultSet 
{
	typedef Rvdldataset::TDLDataSetResultSet inherited;
	
protected:
	virtual Db::TDataSet* __fastcall OpenDataSet(AnsiString QueryStr);
public:
	#pragma option push -w-inl
	/* TDLBaseResultSet.Create */ inline __fastcall virtual TDLBDEResultSet(Rvdlbase::TDLBaseConnection* AConnection) : Rvdldataset::TDLDataSetResultSet(AConnection) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseResultSet.Destroy */ inline __fastcall virtual ~TDLBDEResultSet(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE AnsiString __fastcall EncodeDataSource(const AnsiString AliasName, const AnsiString DriverName, Classes::TStrings* DriverParams);
extern PACKAGE void __fastcall DecodeDataSource(const AnsiString DataSource, AnsiString &AliasName, AnsiString &DriverName, Classes::TStrings* DriverParams);

}	/* namespace Rvdlbde */
using namespace Rvdlbde;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDLBDE
