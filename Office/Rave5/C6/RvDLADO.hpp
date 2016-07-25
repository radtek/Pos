// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDLADO.pas' rev: 6.00

#ifndef RvDLADOHPP
#define RvDLADOHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ADODB.hpp>	// Pascal unit
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

namespace Rvdlado
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDLADODriver;
class PASCALIMPLEMENTATION TDLADODriver : public Rvdldataset::TDLDataSetDriver 
{
	typedef Rvdldataset::TDLDataSetDriver inherited;
	
public:
	virtual Rvdlbase::TDLBaseConnection* __fastcall CreateConnection(void);
	virtual Rvdlbase::TDLBaseResultSet* __fastcall CreateResultSet(Rvdlbase::TDLBaseConnection* AConnection);
	virtual void __fastcall Information(Rvdlcommon::PDLInformation Params);
public:
	#pragma option push -w-inl
	/* TDLBaseDriver.Create */ inline __fastcall virtual TDLADODriver(void) : Rvdldataset::TDLDataSetDriver() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseDriver.Destroy */ inline __fastcall virtual ~TDLADODriver(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLAdoConnection;
class PASCALIMPLEMENTATION TDLAdoConnection : public Rvdldataset::TDLDataSetConnection 
{
	typedef Rvdldataset::TDLDataSetConnection inherited;
	
protected:
	Adodb::TADOConnection* FADOConnection;
	
public:
	virtual void __fastcall Connect(AnsiString DataSource, AnsiString UserName, AnsiString Password, Classes::TStringList* OptionList);
	virtual void __fastcall Disconnect(void);
	virtual void __fastcall GetTableNames(Classes::TStrings* List);
	__property Adodb::TADOConnection* ADOConnection = {read=FADOConnection, write=FADOConnection};
public:
	#pragma option push -w-inl
	/* TDLBaseConnection.Create */ inline __fastcall virtual TDLAdoConnection(void) : Rvdldataset::TDLDataSetConnection() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TDLAdoConnection(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLADOResultSet;
class PASCALIMPLEMENTATION TDLADOResultSet : public Rvdldataset::TDLDataSetResultSet 
{
	typedef Rvdldataset::TDLDataSetResultSet inherited;
	
public:
	virtual Db::TDataSet* __fastcall OpenDataSet(AnsiString QueryStr);
public:
	#pragma option push -w-inl
	/* TDLBaseResultSet.Create */ inline __fastcall virtual TDLADOResultSet(Rvdlbase::TDLBaseConnection* AConnection) : Rvdldataset::TDLDataSetResultSet(AConnection) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseResultSet.Destroy */ inline __fastcall virtual ~TDLADOResultSet(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rvdlado */
using namespace Rvdlado;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDLADO
