// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvDLBase.pas' rev: 6.00

#ifndef QRvDLBaseHPP
#define QRvDLBaseHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QControls.hpp>	// Pascal unit
#include <QRvDLCommon.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvdlbase
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDLFieldListManager;
class PASCALIMPLEMENTATION TDLFieldListManager : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	int FFieldCount;
	Qrvdlcommon::TDLFieldItem *FFieldList;
	
public:
	__fastcall virtual ~TDLFieldListManager(void);
	void __fastcall AllocFieldList(int Count);
	void __fastcall SetFieldItem(int Index, AnsiString FieldName, Byte DataType, int Width, AnsiString FullName, AnsiString Description);
	__property int FieldCount = {read=FFieldCount, nodefault};
	__property Qrvdlcommon::PDLFieldList FieldList = {read=FFieldList};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TDLFieldListManager(void) : System::TObject() { }
	#pragma option pop
	
};


class DELPHICLASS TDLConfigForm;
class PASCALIMPLEMENTATION TDLConfigForm : public Qforms::TForm 
{
	typedef Qforms::TForm inherited;
	
protected:
	void __fastcall TestConnection(Classes::TStrings* TableList);
	
public:
	virtual void __fastcall SetData(AnsiString DataSource, AnsiString UserName, AnsiString Password, Classes::TStringList* OptionList) = 0 ;
	virtual void __fastcall GetData(AnsiString &DataSource, AnsiString &UserName, AnsiString &Password, Classes::TStringList* OptionList) = 0 ;
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TDLConfigForm(Classes::TComponent* AOwner) : Qforms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TDLConfigForm(Classes::TComponent* AOwner, int Dummy) : Qforms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TDLConfigForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWidgetControl.CreateParented */ inline __fastcall TDLConfigForm(Qt::QWidgetH* ParentWidget) : Qforms::TForm(ParentWidget) { }
	#pragma option pop
	
};


typedef TMetaClass*TDLConfigFormClass;

class DELPHICLASS TDLBaseDriver;
class DELPHICLASS TDLBaseConnection;
class DELPHICLASS TDLBaseResultSet;
class PASCALIMPLEMENTATION TDLBaseDriver : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	AnsiString FDataSource;
	AnsiString FUserName;
	AnsiString FPassword;
	Classes::TStringList* FOptionList;
	AnsiString FOptionText;
	TDLFieldListManager* FFields;
	
public:
	__fastcall virtual TDLBaseDriver(void);
	__fastcall virtual ~TDLBaseDriver(void);
	virtual void __fastcall ConnectionClose(Qrvdlcommon::PDLConnectionClose Params);
	virtual void __fastcall ConnectionConfigure(Qrvdlcommon::PDLConnectionConfigure Params);
	virtual void __fastcall ConnectionOpen(Qrvdlcommon::PDLConnectionOpen Params);
	virtual void __fastcall GetErrorText(Qrvdlcommon::PDLErrorText Params);
	virtual void __fastcall GetFields(Qrvdlcommon::PDLFields Params);
	virtual void __fastcall GetTables(Qrvdlcommon::PDLTables Params);
	virtual void __fastcall Information(Qrvdlcommon::PDLInformation Params) = 0 ;
	virtual void __fastcall ResultSetClose(Qrvdlcommon::PDLResultSetClose Params);
	virtual void __fastcall ResultSetConfigure(Qrvdlcommon::PDLResultSetConfigure Params);
	virtual void __fastcall ResultSetEOF(Qrvdlcommon::PDLResultSetEOF Params);
	virtual void __fastcall ResultSetFirst(Qrvdlcommon::PDLResultSetFirst Params);
	virtual void __fastcall ResultSetGetRow(Qrvdlcommon::PDLResultSetRow Params) = 0 ;
	virtual void __fastcall ResultSetNext(Qrvdlcommon::PDLResultSetNext Params);
	virtual void __fastcall ResultSetOpen(Qrvdlcommon::PDLResultSetOpen Params);
	virtual void __fastcall ResultSetSetFilter(Qrvdlcommon::PDLResultSetFilter Params);
	virtual void __fastcall ResultSetSetSort(Qrvdlcommon::PDLResultSetSort Params);
	virtual TDLBaseConnection* __fastcall CreateConnection(void) = 0 ;
	virtual TDLBaseResultSet* __fastcall CreateResultSet(TDLBaseConnection* AConnection) = 0 ;
	__property AnsiString DataSource = {read=FDataSource};
	__property AnsiString UserName = {read=FUserName};
	__property AnsiString Password = {read=FPassword};
	__property Classes::TStringList* OptionList = {read=FOptionList};
	__property TDLFieldListManager* Fields = {read=FFields};
};


typedef TMetaClass*TDLBaseDriverClass;

class PASCALIMPLEMENTATION TDLBaseConnection : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	TDLBaseDriver* FDriver;
	AnsiString FErrorText;
	
public:
	__fastcall virtual TDLBaseConnection(void);
	virtual void __fastcall Connect(AnsiString DataSource, AnsiString UserName, AnsiString Password, Classes::TStringList* OptionList) = 0 ;
	virtual void __fastcall Disconnect(void) = 0 ;
	virtual void __fastcall GetTableNames(Classes::TStrings* List) = 0 ;
	virtual void __fastcall GetFields(AnsiString TableName);
	__property TDLBaseDriver* Driver = {read=FDriver};
	__property AnsiString ErrorText = {read=FErrorText, write=FErrorText};
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TDLBaseConnection(void) { }
	#pragma option pop
	
};


class PASCALIMPLEMENTATION TDLBaseResultSet : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	TDLBaseConnection* FConnection;
	AnsiString FErrorText;
	Qrvdlcommon::TDLBuffer* FBuffer;
	TDLFieldListManager* FFields;
	
public:
	__fastcall virtual TDLBaseResultSet(TDLBaseConnection* AConnection);
	__fastcall virtual ~TDLBaseResultSet(void);
	virtual void __fastcall Open(AnsiString QueryStr) = 0 ;
	virtual void __fastcall GetFields(TDLFieldListManager* AFields) = 0 ;
	virtual void __fastcall Close(void) = 0 ;
	virtual void __fastcall First(void) = 0 ;
	virtual void __fastcall Next(void) = 0 ;
	virtual bool __fastcall Eof(void) = 0 ;
	__property TDLBaseConnection* Connection = {read=FConnection};
	__property AnsiString ErrorText = {read=FErrorText, write=FErrorText};
	__property TDLFieldListManager* Fields = {read=FFields};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TMetaClass*ConnectionConfigureForm;
extern PACKAGE AnsiString SQLEditorName;
extern PACKAGE Classes::TList* GDriverClassList;
extern PACKAGE void __fastcall CreateGlobalTableList(Classes::TStrings* TableNames);
extern PACKAGE void __fastcall FreeGlobalTableList(void);
extern PACKAGE void __fastcall AllocGlobalTableList(int Count);
extern PACKAGE void __fastcall SetGlobalTableItem(int Index, AnsiString TableName);
extern PACKAGE void __fastcall SetGlobalErrorText(AnsiString ErrorText);
extern PACKAGE void __fastcall RegisterDriverClass(TMetaClass* ADriverClass);
extern PACKAGE void __fastcall DataConnectionClose(Qrvdlcommon::PDLConnectionClose Params);
extern PACKAGE void __fastcall DataConnectionConfigure(Qrvdlcommon::PDLConnectionConfigure Params);
extern PACKAGE void __fastcall DataConnectionOpen(Qrvdlcommon::PDLConnectionOpen Params);
extern PACKAGE void __fastcall DataGetErrorText(Qrvdlcommon::PDLErrorText Params);
extern PACKAGE void __fastcall DataGetFields(Qrvdlcommon::PDLFields Params);
extern PACKAGE void __fastcall DataGetTables(Qrvdlcommon::PDLTables Params);
extern PACKAGE void __fastcall DataInformation(Qrvdlcommon::PDLInformation Params);
extern PACKAGE void __fastcall DataResultSetClose(Qrvdlcommon::PDLResultSetClose Params);
extern PACKAGE void __fastcall DataResultSetEOF(Qrvdlcommon::PDLResultSetEOF Params);
extern PACKAGE void __fastcall DataResultSetFirst(Qrvdlcommon::PDLResultSetFirst Params);
extern PACKAGE void __fastcall DataResultSetGetRow(Qrvdlcommon::PDLResultSetRow Params);
extern PACKAGE void __fastcall DataResultSetNext(Qrvdlcommon::PDLResultSetNext Params);
extern PACKAGE void __fastcall DataResultSetOpen(Qrvdlcommon::PDLResultSetOpen Params);
extern PACKAGE void __fastcall DataResultSetSetFilter(Qrvdlcommon::PDLResultSetFilter Params);
extern PACKAGE void __fastcall DataResultSetSetSort(Qrvdlcommon::PDLResultSetSort Params);
extern PACKAGE void __fastcall DataResultSetConfigure(Qrvdlcommon::PDLResultSetConfigure Params);

}	/* namespace Qrvdlbase */
using namespace Qrvdlbase;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvDLBase
