// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpModels.pas' rev: 6.00

#ifndef OpModelsHPP
#define OpModelsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Variants.hpp>	// Pascal unit
#include <OpShared.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opmodels
{
//-- type declarations -------------------------------------------------------
typedef Exception EUnsupportedMode;
;

#pragma option push -b-
enum TOpRetrievalMode { rmCell, rmPacket, rmEntire };
#pragma option pop

typedef Set<TOpRetrievalMode, rmCell, rmEntire>  TOpRetrievalModes;

typedef void __fastcall (__closure *TOpModelGetColHeadersEvent)(System::TObject* Sender, Variant &ColHeaders);

typedef void __fastcall (__closure *TOpModelGetColCountEvent)(System::TObject* Sender, int &ColCount);

typedef void __fastcall (__closure *TOpModelGetDataEvent)(System::TObject* Sender, int Index, int Row, TOpRetrievalMode Mode, int &Size, Variant &Data);

__interface IOpModel;
typedef System::DelphiInterface<IOpModel> _di_IOpModel;
__interface INTERFACE_UUID("{AD426B00-2A14-11D3-9703-0000861F6726}") IOpModel  : public IInterface 
{
	
public:
	virtual void __fastcall First(void) = 0 ;
	virtual void __fastcall Last(void) = 0 ;
	virtual void __fastcall Next(void) = 0 ;
	virtual void __fastcall Prior(void) = 0 ;
	virtual TOpRetrievalModes __fastcall GetSupportedModes(void) = 0 ;
	virtual bool __fastcall GetEOF(void) = 0 ;
	virtual bool __fastcall GetBOF(void) = 0 ;
	virtual int __fastcall GetColCount(void) = 0 ;
	virtual int __fastcall GetCurrentRow(void) = 0 ;
	virtual bool __fastcall GetVariableLengthRows(void) = 0 ;
	virtual int __fastcall GetLevel(void) = 0 ;
	virtual Variant __fastcall GetColHeaders(void) = 0 ;
	virtual void __fastcall BeginRead(void) = 0 ;
	virtual void __fastcall EndRead(void) = 0 ;
	__property bool EOF = {read=GetEOF};
	__property bool BOF = {read=GetBOF};
	__property int ColCount = {read=GetColCount};
	__property bool VariableLengthRows = {read=GetVariableLengthRows};
	__property int Level = {read=GetLevel};
	__property Variant ColHeaders = {read=GetColHeaders};
	__property int CurrentRow = {read=GetCurrentRow};
	virtual Variant __fastcall GetData(int Index, TOpRetrievalMode Mode, int &Size) = 0 ;
};

class DELPHICLASS TOpUnknownComponent;
class PASCALIMPLEMENTATION TOpUnknownComponent : public Opshared::TOpBaseComponent 
{
	typedef Opshared::TOpBaseComponent inherited;
	
protected:
	virtual HRESULT __stdcall QueryInterface(const GUID &IID, /* out */ void *Obj);
	HIDESBASE int __stdcall _AddRef(void);
	HIDESBASE int __stdcall _Release(void);
public:
	#pragma option push -w-inl
	/* TComponent.Create */ inline __fastcall virtual TOpUnknownComponent(Classes::TComponent* AOwner) : Opshared::TOpBaseComponent(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TOpUnknownComponent(void) { }
	#pragma option pop
	
private:
	void *__IInterface;	/* System::IInterface */
	
public:
	operator IInterface*(void) { return (IInterface*)&__IInterface; }
	
};


class DELPHICLASS TOpEventModel;
class PASCALIMPLEMENTATION TOpEventModel : public TOpUnknownComponent 
{
	typedef TOpUnknownComponent inherited;
	
private:
	bool FEof;
	bool FBof;
	TOpRetrievalModes FSupportedModes;
	bool FVariableLengthRows;
	int FCurrentRow;
	int FRowCount;
	TOpModelGetDataEvent FOnGetData;
	TOpModelGetColCountEvent FOnGetColCount;
	TOpModelGetColHeadersEvent FOnGetColHeaders;
	
protected:
	void __fastcall First(void);
	void __fastcall Last(void);
	void __fastcall Next(void);
	void __fastcall Prior(void);
	TOpRetrievalModes __fastcall GetSupportedModes(void);
	bool __fastcall GetEOF(void);
	bool __fastcall GetBOF(void);
	int __fastcall GetColCount(void);
	bool __fastcall GetVariableLengthRows(void);
	int __fastcall GetLevel(void);
	Variant __fastcall GetColHeaders();
	int __fastcall GetCurrentRow(void);
	Variant __fastcall GetData(int Index, TOpRetrievalMode Mode, int &Size);
	void __fastcall BeginRead(void);
	void __fastcall EndRead(void);
	
public:
	__fastcall virtual TOpEventModel(Classes::TComponent* AOwner);
	
__published:
	__property int CurrentRow = {read=GetCurrentRow, nodefault};
	__property int RowCount = {read=FRowCount, write=FRowCount, nodefault};
	__property TOpRetrievalModes SupportedModes = {read=FSupportedModes, write=FSupportedModes, nodefault};
	__property bool VariableLengthRows = {read=FVariableLengthRows, write=FVariableLengthRows, nodefault};
	__property TOpModelGetColCountEvent OnGetColCount = {read=FOnGetColCount, write=FOnGetColCount};
	__property TOpModelGetColHeadersEvent OnGetColHeaders = {read=FOnGetColHeaders, write=FOnGetColHeaders};
	__property TOpModelGetDataEvent OnGetData = {read=FOnGetData, write=FOnGetData};
public:
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TOpEventModel(void) { }
	#pragma option pop
	
private:
	void *__IOpModel;	/* Opmodels::IOpModel */
	
public:
	operator IOpModel*(void) { return (IOpModel*)&__IOpModel; }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opmodels */
using namespace Opmodels;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpModels
