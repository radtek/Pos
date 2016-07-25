// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDLBase.pas' rev: 6.00

#ifndef RvDLBaseHPP
#define RvDLBaseHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Controls.hpp>	// Pascal unit
#include <RvDLCommon.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Forms.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdlbase
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TDLFieldListManager;
class PASCALIMPLEMENTATION TDLFieldListManager : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	int FFieldCount;
	Rvdlcommon::TDLFieldItem *FFieldList;
	
public:
	__fastcall virtual ~TDLFieldListManager(void);
	void __fastcall AllocFieldList(int Count);
	void __fastcall SetFieldItem(int Index, AnsiString FieldName, Byte DataType, int Width, AnsiString FullName, AnsiString Description);
	__property int FieldCount = {read=FFieldCount, nodefault};
	__property Rvdlcommon::PDLFieldList FieldList = {read=FFieldList};
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TDLFieldListManager(void) : System::TObject() { }
	#pragma option pop
	
};


class DELPHICLASS TDLConfigForm;
class PASCALIMPLEMENTATION TDLConfigForm : public Forms::TForm 
{
	typedef Forms::TForm inherited;
	
protected:
	void __fastcall TestConnection(Classes::TStrings* TableList);
	
public:
	virtual void __fastcall SetData(AnsiString DataSource, AnsiString UserName, AnsiString Password, Classes::TStringList* OptionList) = 0 ;
	virtual void __fastcall GetData(AnsiString &DataSource, AnsiString &UserName, AnsiString &Password, Classes::TStringList* OptionList) = 0 ;
public:
	#pragma option push -w-inl
	/* TCustomForm.Create */ inline __fastcall virtual TDLConfigForm(Classes::TComponent* AOwner) : Forms::TForm(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.CreateNew */ inline __fastcall virtual TDLConfigForm(Classes::TComponent* AOwner, int Dummy) : Forms::TForm(AOwner, Dummy) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TCustomForm.Destroy */ inline __fastcall virtual ~TDLConfigForm(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TWinControl.CreateParented */ inline __fastcall TDLConfigForm(HWND ParentWindow) : Forms::TForm(ParentWindow) { }
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
	virtual void __fastcall ConnectionClose(Rvdlcommon::PDLConnectionClose Params);
	virtual void __fastcall ConnectionConfigure(Rvdlcommon::PDLConnectionConfigure Params);
	virtual void __fastcall ConnectionOpen(Rvdlcommon::PDLConnectionOpen Params);
	virtual void __fastcall GetErrorText(Rvdlcommon::PDLErrorText Params);
	virtual void __fastcall GetFields(Rvdlcommon::PDLFields Params);
	virtual void __fastcall GetTables(Rvdlcommon::PDLTables Params);
	virtual void __fastcall Information(Rvdlcommon::PDLInformation Params) = 0 ;
	virtual void __fastcall ResultSetClose(Rvdlcommon::PDLResultSetClose Params);
	virtual void __fastcall ResultSetConfigure(Rvdlcommon::PDLResultSetConfigure Params);
	virtual void __fastcall ResultSetEOF(Rvdlcommon::PDLResultSetEOF Params);
	virtual void __fastcall ResultSetFirst(Rvdlcommon::PDLResultSetFirst Params);
	virtual void __fastcall ResultSetGetRow(Rvdlcommon::PDLResultSetRow Params) = 0 ;
	virtual void __fastcall ResultSetNext(Rvdlcommon::PDLResultSetNext Params);
	virtual void __fastcall ResultSetOpen(Rvdlcommon::PDLResultSetOpen Params);
	virtual void __fastcall ResultSetSetFilter(Rvdlcommon::PDLResultSetFilter Params);
	virtual void __fastcall ResultSetSetSort(Rvdlcommon::PDLResultSetSort Params);
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
	Rvdlcommon::TDLBuffer* FBuffer;
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
extern PACKAGE void __fastcall DataConnectionClose(Rvdlcommon::PDLConnectionClose Params);
extern PACKAGE void __fastcall DataConnectionConfigure(Rvdlcommon::PDLConnectionConfigure Params);
extern PACKAGE void __fastcall DataConnectionOpen(Rvdlcommon::PDLConnectionOpen Params);
extern PACKAGE void __fastcall DataGetErrorText(Rvdlcommon::PDLErrorText Params);
extern PACKAGE void __fastcall DataGetFields(Rvdlcommon::PDLFields Params);
extern PACKAGE void __fastcall DataGetTables(Rvdlcommon::PDLTables Params);
extern PACKAGE void __fastcall DataInformation(Rvdlcommon::PDLInformation Params);
extern PACKAGE void __fastcall DataResultSetClose(Rvdlcommon::PDLResultSetClose Params);
extern PACKAGE void __fastcall DataResultSetEOF(Rvdlcommon::PDLResultSetEOF Params);
extern PACKAGE void __fastcall DataResultSetFirst(Rvdlcommon::PDLResultSetFirst Params);
extern PACKAGE void __fastcall DataResultSetGetRow(Rvdlcommon::PDLResultSetRow Params);
extern PACKAGE void __fastcall DataResultSetNext(Rvdlcommon::PDLResultSetNext Params);
extern PACKAGE void __fastcall DataResultSetOpen(Rvdlcommon::PDLResultSetOpen Params);
extern PACKAGE void __fastcall DataResultSetSetFilter(Rvdlcommon::PDLResultSetFilter Params);
extern PACKAGE void __fastcall DataResultSetSetSort(Rvdlcommon::PDLResultSetSort Params);
extern PACKAGE void __fastcall DataResultSetConfigure(Rvdlcommon::PDLResultSetConfigure Params);

}	/* namespace Rvdlbase */
using namespace Rvdlbase;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDLBase
