// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDLDBX.pas' rev: 6.00

#ifndef RvDLDBXHPP
#define RvDLDBXHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <SqlExpr.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <RvDLDataSet.hpp>	// Pascal unit
#include <RvDLBase.hpp>	// Pascal unit
#include <RvDLCommon.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdldbx
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDLDBXDriver;
class PASCALIMPLEMENTATION TDLDBXDriver : public Rvdldataset::TDLDataSetDriver 
{
	typedef Rvdldataset::TDLDataSetDriver inherited;
	
public:
	virtual Rvdlbase::TDLBaseConnection* __fastcall CreateConnection(void);
	virtual Rvdlbase::TDLBaseResultSet* __fastcall CreateResultSet(Rvdlbase::TDLBaseConnection* AConnection);
	virtual void __fastcall Information(Rvdlcommon::PDLInformation Params);
public:
	#pragma option push -w-inl
	/* TDLBaseDriver.Create */ inline __fastcall virtual TDLDBXDriver(void) : Rvdldataset::TDLDataSetDriver() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseDriver.Destroy */ inline __fastcall virtual ~TDLDBXDriver(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLDBXConnection;
class PASCALIMPLEMENTATION TDLDBXConnection : public Rvdldataset::TDLDataSetConnection 
{
	typedef Rvdldataset::TDLDataSetConnection inherited;
	
protected:
	Sqlexpr::TSQLConnection* FSQLConnection;
	
public:
	virtual void __fastcall Connect(AnsiString DataSource, AnsiString UserName, AnsiString Password, Classes::TStringList* OptionList);
	virtual void __fastcall Disconnect(void);
	virtual void __fastcall GetTableNames(Classes::TStrings* List);
	__property Sqlexpr::TSQLConnection* SQLConnection = {read=FSQLConnection, write=FSQLConnection};
public:
	#pragma option push -w-inl
	/* TDLBaseConnection.Create */ inline __fastcall virtual TDLDBXConnection(void) : Rvdldataset::TDLDataSetConnection() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TDLDBXConnection(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLDBXResultSet;
class PASCALIMPLEMENTATION TDLDBXResultSet : public Rvdldataset::TDLDataSetResultSet 
{
	typedef Rvdldataset::TDLDataSetResultSet inherited;
	
protected:
	virtual Db::TDataSet* __fastcall OpenDataSet(AnsiString QueryStr);
public:
	#pragma option push -w-inl
	/* TDLBaseResultSet.Create */ inline __fastcall virtual TDLDBXResultSet(Rvdlbase::TDLBaseConnection* AConnection) : Rvdldataset::TDLDataSetResultSet(AConnection) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseResultSet.Destroy */ inline __fastcall virtual ~TDLDBXResultSet(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rvdldbx */
using namespace Rvdldbx;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDLDBX
