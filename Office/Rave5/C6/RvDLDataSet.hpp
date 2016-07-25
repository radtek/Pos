// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDLDataSet.pas' rev: 6.00

#ifndef RvDLDataSetHPP
#define RvDLDataSetHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDLBase.hpp>	// Pascal unit
#include <RvDLCommon.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdldataset
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDLDataSetDriver;
class PASCALIMPLEMENTATION TDLDataSetDriver : public Rvdlbase::TDLBaseDriver 
{
	typedef Rvdlbase::TDLBaseDriver inherited;
	
public:
	virtual void __fastcall ResultSetGetRow(Rvdlcommon::PDLResultSetRow Params);
public:
	#pragma option push -w-inl
	/* TDLBaseDriver.Create */ inline __fastcall virtual TDLDataSetDriver(void) : Rvdlbase::TDLBaseDriver() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseDriver.Destroy */ inline __fastcall virtual ~TDLDataSetDriver(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLDataSetConnection;
class PASCALIMPLEMENTATION TDLDataSetConnection : public Rvdlbase::TDLBaseConnection 
{
	typedef Rvdlbase::TDLBaseConnection inherited;
	
public:
	#pragma option push -w-inl
	/* TDLBaseConnection.Create */ inline __fastcall virtual TDLDataSetConnection(void) : Rvdlbase::TDLBaseConnection() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TDLDataSetConnection(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLDataSetResultSet;
class PASCALIMPLEMENTATION TDLDataSetResultSet : public Rvdlbase::TDLBaseResultSet 
{
	typedef Rvdlbase::TDLBaseResultSet inherited;
	
private:
	Db::TDataSet* FDataSet;
	bool FFirstEOFCheck;
	
protected:
	virtual Db::TDataSet* __fastcall OpenDataSet(AnsiString QueryStr) = 0 ;
	
public:
	virtual void __fastcall First(void);
	virtual void __fastcall Next(void);
	virtual bool __fastcall Eof(void);
	virtual void __fastcall GetFields(Rvdlbase::TDLFieldListManager* AFields);
	virtual void __fastcall Open(AnsiString QueryStr);
	virtual void __fastcall Close(void);
	__property Db::TDataSet* DataSet = {read=FDataSet};
public:
	#pragma option push -w-inl
	/* TDLBaseResultSet.Create */ inline __fastcall virtual TDLDataSetResultSet(Rvdlbase::TDLBaseConnection* AConnection) : Rvdlbase::TDLBaseResultSet(AConnection) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseResultSet.Destroy */ inline __fastcall virtual ~TDLDataSetResultSet(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rvdldataset */
using namespace Rvdldataset;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDLDataSet
