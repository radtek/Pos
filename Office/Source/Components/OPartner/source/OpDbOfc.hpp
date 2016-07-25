// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'OpDbOfc.pas' rev: 6.00

#ifndef OpDbOfcHPP
#define OpDbOfcHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Variants.hpp>	// Pascal unit
#include <OpModels.hpp>	// Pascal unit
#include <DB.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Opdbofc
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS EModelException;
class PASCALIMPLEMENTATION EModelException : public Sysutils::Exception 
{
	typedef Sysutils::Exception inherited;
	
public:
	#pragma option push -w-inl
	/* Exception.Create */ inline __fastcall EModelException(const AnsiString Msg) : Sysutils::Exception(Msg) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmt */ inline __fastcall EModelException(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size) : Sysutils::Exception(Msg, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateRes */ inline __fastcall EModelException(int Ident)/* overload */ : Sysutils::Exception(Ident) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmt */ inline __fastcall EModelException(int Ident, const System::TVarRec * Args, const int Args_Size)/* overload */ : Sysutils::Exception(Ident, Args, Args_Size) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateHelp */ inline __fastcall EModelException(const AnsiString Msg, int AHelpContext) : Sysutils::Exception(Msg, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateFmtHelp */ inline __fastcall EModelException(const AnsiString Msg, const System::TVarRec * Args, const int Args_Size, int AHelpContext) : Sysutils::Exception(Msg, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResHelp */ inline __fastcall EModelException(int Ident, int AHelpContext)/* overload */ : Sysutils::Exception(Ident, AHelpContext) { }
	#pragma option pop
	#pragma option push -w-inl
	/* Exception.CreateResFmtHelp */ inline __fastcall EModelException(System::PResStringRec ResStringRec, const System::TVarRec * Args, const int Args_Size, int AHelpContext)/* overload */ : Sysutils::Exception(ResStringRec, Args, Args_Size, AHelpContext) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Destroy */ inline __fastcall virtual ~EModelException(void) { }
	#pragma option pop
	
};


class DELPHICLASS TOpDataSetModel;
class PASCALIMPLEMENTATION TOpDataSetModel : public Opmodels::TOpUnknownComponent 
{
	typedef Opmodels::TOpUnknownComponent inherited;
	
private:
	bool FWantFullMemos;
	Db::TDataSet* FDataset;
	TOpDataSetModel* FDetailModel;
	bool FInDetail;
	void *FCurrentPos;
	Variant __fastcall GetRowData();
	AnsiString __fastcall GetText(int Index);
	void __fastcall SetDetailModel(const TOpDataSetModel* Value);
	void __fastcall SetDataset(const Db::TDataSet* Value);
	
protected:
	void __fastcall First(void);
	void __fastcall Last(void);
	void __fastcall Next(void);
	void __fastcall Prior(void);
	Opmodels::TOpRetrievalModes __fastcall GetSupportedModes(void);
	bool __fastcall GetEOF(void);
	bool __fastcall GetBOF(void);
	int __fastcall GetColCount(void);
	bool __fastcall GetVariableLengthRows(void);
	int __fastcall GetLevel(void);
	Variant __fastcall GetColHeaders();
	int __fastcall GetCurrentRow(void);
	Variant __fastcall GetData(int Index, Opmodels::TOpRetrievalMode Mode, int &Size);
	void __fastcall BeginRead(void);
	void __fastcall EndRead(void);
	virtual void __fastcall Notification(Classes::TComponent* AComponent, Classes::TOperation Operation);
	
public:
	__fastcall virtual TOpDataSetModel(Classes::TComponent* AOwner);
	
__published:
	__property Db::TDataSet* Dataset = {read=FDataset, write=SetDataset};
	__property TOpDataSetModel* DetailModel = {read=FDetailModel, write=SetDetailModel};
	__property bool WantFullMemos = {read=FWantFullMemos, write=FWantFullMemos, nodefault};
public:
	#pragma option push -w-inl
	/* TComponent.Destroy */ inline __fastcall virtual ~TOpDataSetModel(void) { }
	#pragma option pop
	
private:
	void *__IOpModel;	/* Opmodels::IOpModel */
	
public:
	operator IOpModel*(void) { return (IOpModel*)&__IOpModel; }
	
};


//-- var, const, procedure ---------------------------------------------------

}	/* namespace Opdbofc */
using namespace Opdbofc;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// OpDbOfc
