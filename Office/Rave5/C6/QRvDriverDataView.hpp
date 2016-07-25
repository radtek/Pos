// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvDriverDataView.pas' rev: 6.00

#ifndef QRvDriverDataViewHPP
#define QRvDriverDataViewHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvDataField.hpp>	// Pascal unit
#include <QRvDefine.hpp>	// Pascal unit
#include <QRvDatabase.hpp>	// Pascal unit
#include <QRvData.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <QRvDataLink.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvdriverdataview
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveDriverDataView;
class PASCALIMPLEMENTATION TRaveDriverDataView : public Qrvdata::TRaveBaseDataView 
{
	typedef Qrvdata::TRaveBaseDataView inherited;
	
protected:
	bool FActive;
	Qrvdatabase::TRaveDatabase* FDatabase;
	Qrvdatalink::TDataConnection* FConnection;
	Qrvdatalink::TDataResultSet* FResultSet;
	AnsiString FQuery;
	AnsiString FQueryStruct;
	Classes::TStrings* FQueryParams;
	Qrvdatafield::TRaveDataField* __fastcall FindRaveField(AnsiString FieldName);
	virtual void __fastcall GetRow(int EventType);
	HIDESBASE void __fastcall SetActive(const bool AValue);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	virtual void __fastcall OverrideProperties(Qrvclass::TRaveFiler* Filer);
	void __fastcall ReadSQLProperty(Qrvclass::TRaveReader* Reader);
	void __fastcall ReadParamsProperty(Qrvclass::TRaveReader* Reader);
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
	__property Qrvdatabase::TRaveDatabase* Database = {read=FDatabase, write=FDatabase};
	__property AnsiString Query = {read=FQuery, write=FQuery};
	__property Classes::TStrings* QueryParams = {read=FQueryParams, write=SetQueryParams};
	__property AnsiString QueryStruct = {read=FQueryStruct, write=FQueryStruct};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Qrvdriverdataview */
using namespace Qrvdriverdataview;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvDriverDataView
