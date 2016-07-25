// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'RvDatabase.pas' rev: 6.00

#ifndef RvDatabaseHPP
#define RvDatabaseHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <RvDefine.hpp>	// Pascal unit
#include <RvClass.hpp>	// Pascal unit
#include <RvDataLink.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Rvdatabase
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TLinkInfo;
class PASCALIMPLEMENTATION TLinkInfo : public System::TObject 
{
	typedef System::TObject inherited;
	
public:
	Rvdatalink::TDataConnection* Link;
	unsigned ThreadID;
	int UseCount;
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TLinkInfo(void) : System::TObject() { }
	#pragma option pop
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~TLinkInfo(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDBAuth;
class PASCALIMPLEMENTATION TRaveDBAuth : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	AnsiString FDatasource;
	AnsiString FOptions;
	AnsiString FPassword;
	AnsiString FUsername;
	
public:
	virtual void __fastcall AssignTo(Classes::TPersistent* ADest);
	void __fastcall Setup(AnsiString ADatasource, AnsiString AOptions, AnsiString APassword, AnsiString AUsername);
	
__published:
	__property AnsiString Datasource = {read=FDatasource, write=FDatasource};
	__property AnsiString Options = {read=FOptions, write=FOptions};
	__property AnsiString Password = {read=FPassword, write=FPassword};
	__property AnsiString Username = {read=FUsername, write=FUsername};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRaveDBAuth(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveDBAuth(void) : Classes::TPersistent() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDatabase;
class PASCALIMPLEMENTATION TRaveDatabase : public Rvclass::TRaveDataObject 
{
	typedef Rvclass::TRaveDataObject inherited;
	
protected:
	TRaveDBAuth* FAuth;
	TRaveDBAuth* FAuthDesign;
	TRaveDBAuth* FAuthRun;
	Classes::TThreadList* FLinkList;
	Classes::TThreadList* FLinkPool;
	int FLinkPoolSize;
	AnsiString FLinkType;
	Rvdatalink::TDataConnection* __fastcall ConstructLink(void);
	void __fastcall SetAuthDesign(TRaveDBAuth* AValue);
	void __fastcall SetAuthRun(TRaveDBAuth* AValue);
	/* virtual class method */ virtual bool __fastcall UseMaster(TMetaClass* vmt);
	
public:
	__fastcall virtual TRaveDatabase(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveDatabase(void);
	TLinkInfo* __fastcall FindLinkForThread(void);
	TLinkInfo* __fastcall FindLink(Rvdatalink::TDataConnection* ALink);
	void __fastcall InitPool(void);
	Rvdatalink::TDataConnection* __fastcall GetLink(void);
	void __fastcall ReleaseLink(Rvdatalink::TDataConnection* ALink);
	
__published:
	__property TRaveDBAuth* AuthDesign = {read=FAuthDesign, write=SetAuthDesign};
	__property TRaveDBAuth* AuthRun = {read=FAuthRun, write=SetAuthRun};
	__property int LinkPoolSize = {read=FLinkPoolSize, write=FLinkPoolSize, nodefault};
	__property AnsiString LinkType = {read=FLinkType, write=FLinkType};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE bool GAuthRunOverride;
extern PACKAGE void __fastcall RaveRegister(void);

}	/* namespace Rvdatabase */
using namespace Rvdatabase;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// RvDatabase
