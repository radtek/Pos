// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvDLDataSet.pas' rev: 6.00

#ifndef QRvDLDataSetHPP
#define QRvDLDataSetHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvDLBase.hpp>	// Pascal unit
#include <QRvDLCommon.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvdldataset
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDLDataSetDriver;
class PASCALIMPLEMENTATION TDLDataSetDriver : public Qrvdlbase::TDLBaseDriver 
{
	typedef Qrvdlbase::TDLBaseDriver inherited;
	
public:
	virtual void __fastcall ResultSetGetRow(Qrvdlcommon::PDLResultSetRow Params);
public:
	#pragma option push -w-inl
	/* TDLBaseDriver.Create */ inline __fastcall virtual TDLDataSetDriver(void) : Qrvdlbase::TDLBaseDriver() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseDriver.Destroy */ inline __fastcall virtual ~TDLDataSetDriver(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLDataSetConnection;
class PASCALIMPLEMENTATION TDLDataSetConnection : public Qrvdlbase::TDLBaseConnection 
{
	typedef Qrvdlbase::TDLBaseConnection inherited;
	
public:
	#pragma option push -w-inl
	/* TDLBaseConnection.Create */ inline __fastcall virtual TDLDataSetConnection(void) : Qrvdlbase::TDLBaseConnection() { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TDLDataSetConnection(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDLDataSetResultSet;
class PASCALIMPLEMENTATION TDLDataSetResultSet : public Qrvdlbase::TDLBaseResultSet 
{
	typedef Qrvdlbase::TDLBaseResultSet inherited;
	
private:
	Db::TDataSet* FDataSet;
	bool FFirstEOFCheck;
	
protected:
	virtual Db::TDataSet* __fastcall OpenDataSet(AnsiString QueryStr) = 0 ;
	
public:
	virtual void __fastcall First(void);
	virtual void __fastcall Next(void);
	virtual bool __fastcall Eof(void);
	virtual void __fastcall GetFields(Qrvdlbase::TDLFieldListManager* AFields);
	virtual void __fastcall Open(AnsiString QueryStr);
	virtual void __fastcall Close(void);
	__property Db::TDataSet* DataSet = {read=FDataSet};
public:
	#pragma option push -w-inl
	/* TDLBaseResultSet.Create */ inline __fastcall virtual TDLDataSetResultSet(Qrvdlbase::TDLBaseConnection* AConnection) : Qrvdlbase::TDLBaseResultSet(AConnection) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TDLBaseResultSet.Destroy */ inline __fastcall virtual ~TDLDataSetResultSet(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Qrvdldataset */
using namespace Qrvdldataset;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvDLDataSet
