// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpConDS.pas' rev: 6.00

#ifndef RpConDSHPP
#define RpConDSHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpCon.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpconds
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRvCustomDataSetConnection;
class PASCALIMPLEMENTATION TRvCustomDataSetConnection : public Rpcon::TRvCustomConnection 
{
	typedef Rpcon::TRvCustomConnection inherited;
	
protected:
	bool StateSaved;
	bool InitActive;
	AnsiString SaveFilter;
	bool SaveFiltered;
	void *SaveBookmark;
	bool FirstEOFCheck;
	bool FDisableDataSource;
	bool FRestoreDataSet;
	virtual Db::TDataSet* __fastcall GetDataSet(void) = 0 ;
	
public:
	__fastcall virtual TRvCustomDataSetConnection(Classes::TComponent* AOwner);
	virtual void __fastcall DoFirst(void);
	virtual void __fastcall DoNext(void);
	virtual bool __fastcall DoEOF(void);
	virtual void __fastcall DoGetCols(void);
	virtual void __fastcall DoGetRow(void);
	virtual void __fastcall DoSetFilter(void);
	virtual void __fastcall DoOpen(void);
	virtual void __fastcall DoRestore(void);
	
__published:
	__property bool DisableDataSource = {read=FDisableDataSource, write=FDisableDataSource, default=1};
	__property bool RestoreDataSet = {read=FRestoreDataSet, write=FRestoreDataSet, default=1};
public:
	#pragma option push -w-inl
	/* TRvCustomConnection.Destroy */ inline __fastcall virtual ~TRvCustomDataSetConnection(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRvDataSetConnection;
class PASCALIMPLEMENTATION TRvDataSetConnection : public TRvCustomDataSetConnection 
{
	typedef TRvCustomDataSetConnection inherited;
	
protected:
	Db::TDataSet* FDataSet;
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetDataSet(Db::TDataSet* Value);
	virtual Db::TDataSet* __fastcall GetDataSet(void);
	
__published:
	__property Db::TDataSet* DataSet = {read=FDataSet, write=SetDataSet, default=0};
public:
	#pragma option push -w-inl
	/* TRvCustomDataSetConnection.Create */ inline __fastcall virtual TRvDataSetConnection(Classes::TComponent* AOwner) : TRvCustomDataSetConnection(AOwner) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TRvCustomConnection.Destroy */ inline __fastcall virtual ~TRvDataSetConnection(void) { }
	#pragma option pop
	
};


typedef TRvDataSetConnection TRPDataSetConnection;
;

//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpconds */
using namespace Rpconds;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpConDS
