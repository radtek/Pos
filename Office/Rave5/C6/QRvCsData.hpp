// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvCsData.pas' rev: 6.00

#ifndef QRvCsDataHPP
#define QRvCsDataHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRvDataField.hpp>	// Pascal unit
#include <QRvCsRpt.hpp>	// Pascal unit
#include <QRvProj.hpp>	// Pascal unit
#include <QRpBase.hpp>	// Pascal unit
#include <QRpDefine.hpp>	// Pascal unit
#include <QRvData.hpp>	// Pascal unit
#include <QRvCsStd.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvDefine.hpp>	// Pascal unit
#include <TypInfo.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QForms.hpp>	// Pascal unit
#include <QStdCtrls.hpp>	// Pascal unit
#include <QButtons.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <QGraphics.hpp>	// Pascal unit
#include <QControls.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvcsdata
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRaveDataText;
class PASCALIMPLEMENTATION TRaveDataText : public Qrvcsstd::TRaveText 
{
	typedef Qrvcsstd::TRaveText inherited;
	
protected:
	AnsiString FDataField;
	Qrvdata::TRaveBaseDataView* FDataView;
	Qrvdata::TRaveBaseDataView* FLookupDataView;
	AnsiString FLookupField;
	AnsiString FLookupDisplay;
	AnsiString FLookupInvalid;
	virtual void __fastcall SetText(AnsiString Value);
	virtual AnsiString __fastcall GetText();
	void __fastcall SetDataField(AnsiString Value);
	void __fastcall SetLookupDataView(Qrvdata::TRaveBaseDataView* Value);
	void __fastcall SetLookupField(AnsiString Value);
	void __fastcall SetLookupDisplay(AnsiString Value);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveDataText(Classes::TComponent* AOwner);
	
__published:
	__property Truncate  = {default=1};
	__property AnsiString DataField = {read=FDataField, write=SetDataField};
	__property Qrvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView};
	__property Qrvdata::TRaveBaseDataView* LookupDataView = {read=FLookupDataView, write=SetLookupDataView, default=0};
	__property AnsiString LookupField = {read=FLookupField, write=SetLookupField};
	__property AnsiString LookupDisplay = {read=FLookupDisplay, write=SetLookupDisplay};
	__property AnsiString LookupInvalid = {read=FLookupInvalid, write=FLookupInvalid};
public:
	#pragma option push -w-inl
	/* TRaveCustomText.Destroy */ inline __fastcall virtual ~TRaveDataText(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDataMemo;
class PASCALIMPLEMENTATION TRaveDataMemo : public Qrvcsstd::TRaveMemo 
{
	typedef Qrvcsstd::TRaveMemo inherited;
	
protected:
	AnsiString FDataField;
	Qrvdata::TRaveBaseDataView* FDataView;
	virtual AnsiString __fastcall GetText();
	void __fastcall SetDataField(AnsiString Value);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	
__published:
	__property ContainsRTF  = {default=0};
	__property AnsiString DataField = {read=FDataField, write=SetDataField};
	__property Qrvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView};
public:
	#pragma option push -w-inl
	/* TRaveMemo.Create */ inline __fastcall virtual TRaveDataMemo(Classes::TComponent* AOwner) : Qrvcsstd::TRaveMemo(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveMemo.Destroy */ inline __fastcall virtual ~TRaveDataMemo(void) { }
	#pragma option pop
	
};


#pragma option push -b-
enum TRaveDisplayType { dtNumericFormat, dtDateTimeFormat };
#pragma option pop

#pragma option push -b-
enum TRaveCalcFunction { cfNone, cfTrunc, cfFrac, cfRound, cfNeg, cfAbs, cfSin, cfCos, cfArcTan, cfSqr, cfSqrt, cfInc, cfDec, cfRandom, cfRound1, cfRound2, cfRound3, cfRound4, cfRound5, cfSecsToTime, cfMinsToTime, cfHoursToTime, cfTimeToSecs, cfTimeToMins, cfTimeToHours, cfPercent };
#pragma option pop

#pragma option push -b-
enum TRaveCalcOperator { coAdd, coSub, coMul, coDiv, coMod, coExp, coGreater, coLesser, coAverage };
#pragma option pop

class DELPHICLASS TRaveCalcText;
class PASCALIMPLEMENTATION TRaveCalcText : public Qrvcsstd::TRaveText 
{
	typedef Qrvcsstd::TRaveText inherited;
	
protected:
	int Count;
	Extended Total;
	Qrvclass::TRaveComponent* FController;
	Qrvclass::TRaveComponent* FInitializer;
	Qrvdata::TRaveBaseDataView* FDataView;
	AnsiString FDataField;
	Qrvdefine::TRaveCalcType FCalcType;
	bool FRunningTotal;
	AnsiString FDisplayFormat;
	TRaveDisplayType FDisplayType;
	bool FCountBlanks;
	bool FCountNulls;
	AnsiString FCountValue;
	Qrvclass::TRaveFloatEvent* FOnCalcValue;
	virtual void __fastcall BeforeReport(void);
	virtual void __fastcall SetText(AnsiString Value);
	virtual AnsiString __fastcall GetText();
	Extended __fastcall GetValue(void);
	void __fastcall SetDataField(AnsiString Value);
	void __fastcall SetController(Qrvclass::TRaveComponent* Value);
	void __fastcall SetInitializer(Qrvclass::TRaveComponent* Value);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	void __fastcall CalcNewDataListen(Qrvclass::TRaveComponent* Speaker, System::TObject* Msg);
	void __fastcall CalcInitDataListen(Qrvclass::TRaveComponent* Speaker, System::TObject* Msg);
	virtual void __fastcall OverrideProperties(Qrvclass::TRaveFiler* Filer);
	void __fastcall ReadControllerBand(Qrvclass::TRaveReader* Reader);
	
public:
	__fastcall virtual TRaveCalcText(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveCalcText(void);
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	__property Extended Value = {read=GetValue};
	
__published:
	__property Qrvdefine::TRaveCalcType CalcType = {read=FCalcType, write=FCalcType, default=1};
	__property Qrvclass::TRaveComponent* Controller = {read=FController, write=SetController, default=0};
	__property bool CountBlanks = {read=FCountBlanks, write=FCountBlanks, default=1};
	__property bool CountNulls = {read=FCountNulls, write=FCountNulls, default=0};
	__property AnsiString CountValue = {read=FCountValue, write=FCountValue};
	__property Qrvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView, default=0};
	__property AnsiString DataField = {read=FDataField, write=SetDataField};
	__property AnsiString DisplayFormat = {read=FDisplayFormat, write=FDisplayFormat};
	__property TRaveDisplayType DisplayType = {read=FDisplayType, write=FDisplayType, default=0};
	__property Qrvclass::TRaveComponent* Initializer = {read=FInitializer, write=SetInitializer, default=0};
	__property bool RunningTotal = {read=FRunningTotal, write=FRunningTotal, default=0};
	__property Qrvclass::TRaveFloatEvent* OnCalcValue = {read=FOnCalcValue, write=FOnCalcValue};
};


class DELPHICLASS TRaveCalcVariable;
class PASCALIMPLEMENTATION TRaveCalcVariable : public Qrvclass::TRaveComponent 
{
	typedef Qrvclass::TRaveComponent inherited;
	
protected:
	AnsiString FDestParam;
	AnsiString FDestPIVar;
	AnsiString FDisplayFormat;
	TRaveDisplayType FDisplayType;
	Qrvclass::TRaveFloatEvent* FOnGetValue;
	virtual Extended __fastcall GetValue(void) = 0 ;
	virtual bool __fastcall GetIsNull(void);
	
public:
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	__property Extended Value = {read=GetValue};
	__property bool IsNull = {read=GetIsNull, nodefault};
	__property AnsiString DestParam = {read=FDestParam, write=FDestParam};
	__property AnsiString DestPIVar = {read=FDestPIVar, write=FDestPIVar};
	__property AnsiString DisplayFormat = {read=FDisplayFormat, write=FDisplayFormat};
	__property TRaveDisplayType DisplayType = {read=FDisplayType, write=FDisplayType, default=0};
	__property Qrvclass::TRaveFloatEvent* OnGetValue = {read=FOnGetValue, write=FOnGetValue};
public:
	#pragma option push -w-inl
	/* TRaveComponent.Create */ inline __fastcall virtual TRaveCalcVariable(Classes::TComponent* AOwner) : Qrvclass::TRaveComponent(AOwner) { }
	#pragma option pop
	#pragma option push -w-inl
	/* TRaveComponent.Destroy */ inline __fastcall virtual ~TRaveCalcVariable(void) { }
	#pragma option pop
	
};


class DELPHICLASS TRaveCalcOp;
class PASCALIMPLEMENTATION TRaveCalcOp : public TRaveCalcVariable 
{
	typedef TRaveCalcVariable inherited;
	
protected:
	TRaveCalcVariable* FSrc1CalcVar;
	AnsiString FSrc1DataField;
	Qrvdata::TRaveBaseDataView* FSrc1DataView;
	Extended FSrc1Value;
	TRaveCalcFunction FSrc1Function;
	TRaveCalcVariable* FSrc2CalcVar;
	AnsiString FSrc2DataField;
	Qrvdata::TRaveBaseDataView* FSrc2DataView;
	Extended FSrc2Value;
	TRaveCalcFunction FSrc2Function;
	TRaveCalcOperator FOperator;
	TRaveCalcFunction FResultFunction;
	virtual bool __fastcall GetIsNull(void);
	virtual Extended __fastcall GetValue(void);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveCalcOp(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveCalcOp(void);
	
__published:
	__property DestParam ;
	__property DestPIVar ;
	__property DisplayFormat ;
	__property DisplayType  = {default=0};
	__property TRaveCalcOperator Operator = {read=FOperator, write=FOperator, default=0};
	__property TRaveCalcFunction ResultFunction = {read=FResultFunction, write=FResultFunction, default=0};
	__property TRaveCalcVariable* Src1CalcVar = {read=FSrc1CalcVar, write=FSrc1CalcVar, default=0};
	__property AnsiString Src1DataField = {read=FSrc1DataField, write=FSrc1DataField};
	__property Qrvdata::TRaveBaseDataView* Src1DataView = {read=FSrc1DataView, write=FSrc1DataView, default=0};
	__property Extended Src1Value = {read=FSrc1Value, write=FSrc1Value};
	__property TRaveCalcFunction Src1Function = {read=FSrc1Function, write=FSrc1Function, default=0};
	__property TRaveCalcVariable* Src2CalcVar = {read=FSrc2CalcVar, write=FSrc2CalcVar, default=0};
	__property AnsiString Src2DataField = {read=FSrc2DataField, write=FSrc2DataField};
	__property Qrvdata::TRaveBaseDataView* Src2DataView = {read=FSrc2DataView, write=FSrc2DataView, default=0};
	__property Extended Src2Value = {read=FSrc2Value, write=FSrc2Value};
	__property TRaveCalcFunction Src2Function = {read=FSrc2Function, write=FSrc2Function, default=0};
	__property OnGetValue ;
};


class DELPHICLASS TRaveCalcTotal;
class PASCALIMPLEMENTATION TRaveCalcTotal : public TRaveCalcVariable 
{
	typedef TRaveCalcVariable inherited;
	
protected:
	int Count;
	Extended Total;
	TRaveCalcVariable* FCalcVar;
	Qrvdata::TRaveBaseDataView* FDataView;
	AnsiString FDataField;
	Qrvclass::TRaveComponent* FController;
	Qrvclass::TRaveComponent* FInitializer;
	Qrvdefine::TRaveCalcType FCalcType;
	bool FCountBlanks;
	bool FCountNulls;
	AnsiString FCountValue;
	bool FRunningTotal;
	Qrvclass::TRaveFloatEvent* FOnCalcValue;
	virtual void __fastcall BeforeReport(void);
	void __fastcall SetController(Qrvclass::TRaveComponent* Value);
	void __fastcall SetInitializer(Qrvclass::TRaveComponent* Value);
	virtual Extended __fastcall GetValue(void);
	void __fastcall CalcNewDataListen(Qrvclass::TRaveComponent* Speaker, System::TObject* Msg);
	void __fastcall CalcInitDataListen(Qrvclass::TRaveComponent* Speaker, System::TObject* Msg);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveCalcTotal(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveCalcTotal(void);
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	
__published:
	__property Qrvdefine::TRaveCalcType CalcType = {read=FCalcType, write=FCalcType, default=1};
	__property TRaveCalcVariable* CalcVar = {read=FCalcVar, write=FCalcVar, default=0};
	__property Qrvclass::TRaveComponent* Controller = {read=FController, write=SetController, default=0};
	__property bool CountBlanks = {read=FCountBlanks, write=FCountBlanks, default=1};
	__property bool CountNulls = {read=FCountNulls, write=FCountNulls, default=0};
	__property AnsiString CountValue = {read=FCountValue, write=FCountValue};
	__property Qrvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView, default=0};
	__property AnsiString DataField = {read=FDataField, write=FDataField};
	__property DestParam ;
	__property DestPIVar ;
	__property DisplayFormat ;
	__property DisplayType  = {default=0};
	__property Qrvclass::TRaveComponent* Initializer = {read=FInitializer, write=SetInitializer, default=0};
	__property bool RunningTotal = {read=FRunningTotal, write=FRunningTotal, default=0};
	__property Qrvclass::TRaveFloatEvent* OnCalcValue = {read=FOnCalcValue, write=FOnCalcValue};
	__property OnGetValue ;
};


class DELPHICLASS TRaveCalcController;
class PASCALIMPLEMENTATION TRaveCalcController : public Qrvclass::TRaveComponent 
{
	typedef Qrvclass::TRaveComponent inherited;
	
protected:
	Qrvclass::TRaveMethodList* CalcNewDataListenList;
	Qrvclass::TRaveMethodList* CalcInitDataListenList;
	TRaveCalcVariable* FInitCalcVar;
	AnsiString FInitDataField;
	Extended FInitValue;
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	
public:
	__fastcall virtual TRaveCalcController(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveCalcController(void);
	virtual void __fastcall AddListener(AnsiString Conversation, Qrvclass::TRaveListenEvent ListenMethod);
	virtual void __fastcall RemoveListener(AnsiString Conversation, Qrvclass::TRaveListenEvent ListenMethod);
	virtual bool __fastcall Habla(AnsiString Conversation);
	virtual void __fastcall Print(Qrpbase::TBaseReport* Report);
	
__published:
	__property TRaveCalcVariable* InitCalcVar = {read=FInitCalcVar, write=FInitCalcVar};
	__property AnsiString InitDataField = {read=FInitDataField, write=FInitDataField};
	__property Extended InitValue = {read=FInitValue, write=FInitValue};
};


class DELPHICLASS TRaveDataMirror;
class PASCALIMPLEMENTATION TRaveDataMirror : public Classes::TPersistent 
{
	typedef Classes::TPersistent inherited;
	
protected:
	AnsiString FDataValue;
	Qrvcsstd::TRaveSection* FSectionMirror;
	bool FIsDefault;
	
__published:
	__property AnsiString DataValue = {read=FDataValue, write=FDataValue};
	__property bool IsDefault = {read=FIsDefault, write=FIsDefault, default=0};
	__property Qrvcsstd::TRaveSection* SectionMirror = {read=FSectionMirror, write=FSectionMirror, default=0};
public:
	#pragma option push -w-inl
	/* TPersistent.Destroy */ inline __fastcall virtual ~TRaveDataMirror(void) { }
	#pragma option pop
	
public:
	#pragma option push -w-inl
	/* TObject.Create */ inline __fastcall TRaveDataMirror(void) : Classes::TPersistent() { }
	#pragma option pop
	
};


class DELPHICLASS TRaveDataMirrorSection;
class PASCALIMPLEMENTATION TRaveDataMirrorSection : public Qrvcsstd::TRaveSection 
{
	typedef Qrvcsstd::TRaveSection inherited;
	
protected:
	bool Initialized;
	bool DirectReference;
	AnsiString FDataField;
	Qrvdata::TRaveBaseDataView* FDataView;
	Qrvclass::TRavePersistentList* FDataMirrors;
	bool FCompareCase;
	Qrvclass::TRaveControl* FSaveMirror;
	Qrvdefine::TRaveAnchor FSaveAnchor;
	bool FSaveCompareCase;
	AnsiString FSaveDataField;
	Qrvclass::TRavePersistentList* FSaveDataMirrors;
	Qrvdata::TRaveBaseDataView* FSaveDataView;
	Qrvdefine::TDisplayOn FSaveDisplayOn;
	int FSaveTag;
	Qrvclass::TRaveStringEvent* FOnMirrorValue;
	void __fastcall SetDataMirrors(Qrvclass::TRavePersistentList* Value);
	virtual void __fastcall Changing(Qrvclass::TRaveComponent* OldItem, Qrvclass::TRaveComponent* NewItem);
	virtual void __fastcall InitData(void);
	virtual void __fastcall DoneData(void);
	virtual void __fastcall SaveDesigned(void);
	virtual void __fastcall RestoreDesigned(void);
	virtual void __fastcall RestoreAdjusted(void);
	
public:
	__fastcall virtual TRaveDataMirrorSection(Classes::TComponent* AOwner);
	__fastcall virtual ~TRaveDataMirrorSection(void);
	virtual void __fastcall PrintAll(Qrpbase::TBaseReport* Report);
	virtual void __fastcall Paint(Qgraphics::TCanvas* Canvas);
	
__published:
	__property bool CompareCase = {read=FCompareCase, write=FCompareCase, default=0};
	__property AnsiString DataField = {read=FDataField, write=FDataField};
	__property Qrvdata::TRaveBaseDataView* DataView = {read=FDataView, write=FDataView};
	__property Qrvclass::TRavePersistentList* DataMirrors = {read=FDataMirrors, write=SetDataMirrors};
	__property Qrvclass::TRaveStringEvent* OnMirrorValue = {read=FOnMirrorValue, write=FOnMirrorValue};
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE Qrvclass::TRaveControl* __fastcall CreateDataTextControl(Classes::TComponent* AOwner, Qrvdatafield::TRaveDataField* DataField);
extern PACKAGE Qrvclass::TRaveControl* __fastcall CreateDataMemoControl(Classes::TComponent* AOwner, Qrvdatafield::TRaveDataField* DataField);
extern PACKAGE void __fastcall RaveRegister(void);
extern PACKAGE Extended __fastcall CalcFunction(Extended Value, TRaveCalcFunction Func);
extern PACKAGE Extended __fastcall CalcVariable(Qrvclass::TRaveComponent* Self, TRaveCalcVariable* CalcVar, Qrvdata::TRaveBaseDataView* DataView, AnsiString DataField, Extended Value, TRaveCalcFunction Func, bool &DoCount);

}	/* namespace Qrvcsdata */
using namespace Qrvcsdata;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvCsData
