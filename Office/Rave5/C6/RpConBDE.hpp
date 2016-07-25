// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpConBDE.pas' rev: 6.00

#ifndef RpConBDEHPP
#define RpConBDEHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpConDS.hpp>	// Pascal unit
#include <RpCon.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <DBTables.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpconbde
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvTableConnection;
class PASCALIMPLEMENTATION TRvTableConnection : public Rpconds::TRvCustomDataSetConnection 
{
	typedef Rpconds::TRvCustomDataSetConnection inherited;
	
protected:
	Dbtables::TTable* FTable;
	bool FUseSetRange;
	Db::TDataSource* SaveMasterSource;
	AnsiString SaveMasterFields;
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetTable(Dbtables::TTable* Value);
	virtual Db::TDataSet* __fastcall GetDataSet(void);
	
public:
	__fastcall virtual TRvTableConnection(Classes::TComponent* AOwner);
	virtual void __fastcall DoSetFilter(void);
	virtual void __fastcall DoGetSorts(void);
	virtual void __fastcall DoSetSort(void);
	virtual void __fastcall DoOpen(void);
	virtual void __fastcall DoRestore(void);
	
__published:
	__property LocalFilter  = {default=0};
	__property Dbtables::TTable* Table = {read=FTable, write=SetTable, default=0};
	__property bool UseSetRange = {read=FUseSetRange, write=FUseSetRange, default=0};
public:
	#pragma option push -w-inl
	/* TRvCustomConnection.Destroy */ inline __fastcall virtual ~TRvTableConnection(void) { }
	#pragma option pop
	
};


typedef TRvTableConnection TRPTableConnection;
;

class DELPHICLASS TRvQueryConnection;
class PASCALIMPLEMENTATION TRvQueryConnection : public Rpconds::TRvCustomDataSetConnection 
{
	typedef Rpconds::TRvCustomDataSetConnection inherited;
	
protected:
	Dbtables::TQuery* FQuery;
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetQuery(Dbtables::TQuery* Value);
	virtual Db::TDataSet* __fastcall GetDataSet(void);
	
public:
	__fastcall virtual TRvQueryConnection(Classes::TComponent* AOwner);
	virtual void __fastcall DoGetSorts(void);
	virtual void __fastcall DoSetSort(void);
	
__published:
	__property LocalFilter  = {default=0};
	__property Dbtables::TQuery* Query = {read=FQuery, write=SetQuery, default=0};
public:
	#pragma option push -w-inl
	/* TRvCustomConnection.Destroy */ inline __fastcall virtual ~TRvQueryConnection(void) { }
	#pragma option pop
	
};


typedef TRvQueryConnection TRPQueryConnection;
;

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpconbde */
using namespace Rpconbde;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpConBDE
