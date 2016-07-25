// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'QRvLERun.pas' rev: 6.00

#ifndef QRvLERunHPP
#define QRvLERunHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <QRpDefine.hpp>	// Pascal unit
#include <QRvUtil.hpp>	// Pascal unit
#include <QRvLEModule.hpp>	// Pascal unit
#include <QRvLEUtil.hpp>	// Pascal unit
#include <QRvLEDefine.hpp>	// Pascal unit
#include <QRvClass.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Classes.hpp>	// Pascal unit
#include <QDialogs.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Qrvlerun
{
//-- type declarations -------------------------------------------------------
class DELPHICLASS TRCPURunner;
class PASCALIMPLEMENTATION TRCPURunner : public System::TObject 
{
	typedef System::TObject inherited;
	
protected:
	int SP;
	int BP;
	Qrvlemodule::TRCPUModule* MI;
	int CO;
	void *SS;
	void *DS;
	void *CS;
	int StackSize;
	Shortint CompareFlag;
	void __fastcall Push(int Len);
	void __fastcall PushData(void *Data, int Len);
	void __fastcall PushInteger(int &Data);
	void __fastcall PushNull(int Len);
	void __fastcall Pop(int Len);
	void __fastcall PopData(void *Data, int Len);
	int __fastcall PopInteger(void);
	void __fastcall ReadStack(void *Data, int Offset, int Len);
	void __fastcall WriteStack(void *Data, int Offset, int Len);
	void __fastcall ReadData(void *Data, int Offset, int Len);
	void __fastcall WriteData(void *Data, int Offset, int Len);
	Byte __fastcall GetCodeByte(void);
	int __fastcall GetCodeIndex(void);
	int __fastcall GetCodeAddress(void);
	void * __fastcall PushCodeAddress(void);
	void * __fastcall GetMICOPointer(void);
	void __fastcall NewModule(Qrvlemodule::TRCPUModule* AModule);
	
public:
	__fastcall TRCPURunner(void);
	__fastcall virtual ~TRCPURunner(void);
	void __fastcall Execute(Qrvlemodule::TRCPUModule* RCPUModule, Classes::TList* Params, System::TObject* &SuspendState);
};


//-- var, const, procedure ---------------------------------------------------
extern PACKAGE TRCPURunner* RCPURunner;
extern PACKAGE Classes::TStringList* ModuleRegList;

}	/* namespace Qrvlerun */
using namespace Qrvlerun;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// QRvLERun
