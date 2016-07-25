// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RpDBTabl.pas' rev: 6.00

#ifndef RpDBTablHPP
#define RpDBTablHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RpTable.hpp>	// Pascal unit
#include <RpBase.hpp>	// Pascal unit
#include <RpDefine.hpp>	// Pascal unit
#include <DBTables.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Dialogs.hpp>	// Pascal unit
#include <Graphics.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <Controls.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Messages.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rpdbtabl
{
//-- type declarations -------------------------------------------------------
typedef SmallString<40>  TFieldName;

class DELPHICLASS TDBTableColumn;
class PASCALIMPLEMENTATION TDBTableColumn : public Rptable::TTableColumn 
{
	typedef Rptable::TTableColumn inherited;
	
protected:
	TFieldName FFieldName;
	Db::TDataSet* FLookupDataSet;
	TFieldName FLookupKey;
	TFieldName FLookupDisplay;
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetLookupDataSet(Db::TDataSet* Value);
	virtual void __fastcall FillMemoBuf(void);
	
__published:
	__property TFieldName FieldName = {read=FFieldName, write=FFieldName};
	__property Db::TDataSet* LookupDataSet = {read=FLookupDataSet, write=SetLookupDataSet};
	__property TFieldName LookupKey = {read=FLookupKey, write=FLookupKey};
	__property TFieldName LookupDisplay = {read=FLookupDisplay, write=FLookupDisplay};
public:
	#pragma option push -w-inl
	/* TTableColumn.Create */ inline __fastcall virtual TDBTableColumn(Classes::TComponent* AOwner) : Rptable::TTableColumn(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TTableColumn.Destroy */ inline __fastcall virtual ~TDBTableColumn(void) { }
	#pragma option pop
	
};


class DELPHICLASS TDBTablePrinter;
class PASCALIMPLEMENTATION TDBTablePrinter : public Rptable::TTablePrinter 
{
	typedef Rptable::TTablePrinter inherited;
	
protected:
	Db::TDataSet* FDataSet;
	TFieldName FMasterKey;
	TFieldName FDetailKey;
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	void __fastcall SetDataSet(Db::TDataSet* Value);
	virtual void __fastcall InitMaster(bool &Valid, Rptable::TTablePrinter* Master);
	virtual void __fastcall InitTable(bool &Valid);
	virtual void __fastcall GetNextRow(bool &Valid);
	
public:
	virtual Rptable::TTableColumn* __fastcall CreateColumn(AnsiString Description);
	
__published:
	__property TFieldName MasterKey = {read=FMasterKey, write=FMasterKey};
	__property TFieldName DetailKey = {read=FDetailKey, write=FDetailKey};
	__property Db::TDataSet* DataSet = {read=FDataSet, write=SetDataSet};
public:
	#pragma option push -w-inl
	/* TTablePrinter.Create */ inline __fastcall virtual TDBTablePrinter(Classes::TComponent* AOwner) : Rptable::TTablePrinter(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TTablePrinter.Destroy */ inline __fastcall virtual ~TDBTablePrinter(void) { }
	#pragma option pop
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Rpdbtabl */
using namespace Rpdbtabl;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RpDBTabl
