// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDriverDataView.pas' rev: 6.00

#ifndef RvDriverDataViewHPP
#define RvDriverDataViewHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDataField.hpp>	// Pascal unit
#include <RvDefine.hpp>	// Pascal unit
#include <RvDatabase.hpp>	// Pascal unit
#include <RvData.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <RvDataLink.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdriverdataview
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveDriverDataView;
class PASCALIMPLEMENTATION TRaveDriverDataView : public Rvdata::TRaveBaseDataView 
{
	typedef Rvdata::TRaveBaseDataView inherited;
	
protected:
	bool FActive;
	Rvdatabase::TRaveDatabase* FDatabase;
	Rvdatalink::TDataConnection* FConnection;
	Rvdatalink::TDataResultSet* FResultSet;
	AnsiString FQuery;
	AnsiString FQueryStruct;
	Classes::TStrings* FQueryParams;
	Rvdatafield::TRaveDataField* __fastcall FindRaveField(AnsiString FieldName);
	virtual void __fastcall GetRow(int EventType);
	HIDESBASE void __fastcall SetActive(const bool AValue);
	virtual void __fastcall Changing(Rvclass::TRaveComponent* OldItem, Rvclass::TRaveComponent* NewItem);
	virtual void __fastcall OverrideProperties(Rvclass::TRaveFiler* Filer);
	void __fastcall ReadSQLProperty(Rvclass::TRaveReader* Reader);
	void __fastcall ReadParamsProperty(Rvclass::TRaveReader* Reader);
	void __fastcall SetQueryParams(Classes::TStrings* Value);
	AnsiString __fastcall PrepareQuery(AnsiString QueryStr);
	
public:
	virtual void __fastcall CreateFields(Classes::TList* AFieldList);
	__fastcall virtual TRaveDriverDataView(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveDriverDataView(void);
	virtual void __fastcall Open(void);
	virtual void __fastcall Close(void);
	virtual void __fastcall SetFilter(Classes::TStringList* FilterList);
	virtual void __fastcall SetRemoteSort(Classes::TStringList* SortList);
	__property bool Active = {read=FActive, write=SetActive, nodefault};
	
__published:
	__property Rvdatabase::TRaveDatabase* Database = {read=FDatabase, write=FDatabase};
	__property AnsiString Query = {read=FQuery, write=FQuery};
	__property Classes::TStrings* QueryParams = {read=FQueryParams, write=SetQueryParams};
	__property AnsiString QueryStruct = {read=FQueryStruct, write=FQueryStruct};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvdriverdataview */
using namespace Rvdriverdataview;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDriverDataView
