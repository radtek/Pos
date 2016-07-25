#ifndef ZBDBindH
#define ZBDBindH
/*
  ZBDBind.h - Header file to dynamically bind DLL to component
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2005  Roger Aelbrecht

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License (licence.txt) for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  contact: Zipbuilder AT drehoeksw DOT net
  DelphiZip maillist subscribe at http://www.geocities.com/SiliconValley/Orchard/8607/ 
  updates: http://www.drehoeksw.net

  last modified 2005-12-17
---------------------------------------------------------------------------*/
#include "CondDefines.h"
#ifndef STATIC_LOAD_DELZIP_DLL

#include <vcl\classes.hpp>
#include "ZBDelZip.h"
#include "ZBCore.h"

namespace Zbdbind
{
typedef int __stdcall (*TZipExecFunc)(ZipParms* C);
typedef int __stdcall (*TUnzExecFunc)(UnZipParms* C);
typedef int __stdcall (*TDllPrivVersionFunc)(void);
typedef int __stdcall (*TDllVersionFunc)(void);
typedef int __stdcall (*TAbortOperationFunc)(void* C);
typedef char* __stdcall (*TDllPathFunc)(void);

#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif

class __declspec(delphiclass) TZBDynamicBind : public TZBCore
{
private:
  TZipExecFunc ZipExecFunc;
  TUnzExecFunc UnzExecFunc;
  TAbortOperationFunc AbortFunc;
  TDllPrivVersionFunc PrivFunc;
  TDllVersionFunc VersFunc;
  TDllPathFunc PathFunc;
  String FDllDirectory;
  bool FKillTemp;
	String fPath;
  int fVer;
  HINSTANCE hndl;
  int loadLevel;
	int Priv;
	String TmpFileName;
  int __fastcall GetDll_Build(void);
  bool __fastcall GetDll_Load(void);
	String __fastcall GetPath(void);
  String __fastcall GetResDllPath(void);
  String __fastcall GetVersion(void);
  int __fastcall LoadDll(int level);
  int __fastcall LoadLib(String Fullpath, bool MustExist);
  void __fastcall ReleaseLib(void);
  void __fastcall RemoveTempDll(void);
  void __fastcall SetDllDirectory(const String Value);
  void __fastcall SetDll_Load(const bool Value);
  int __fastcall UnloadDll(int level);
protected:
  unsigned FDllOperKey;
  void __fastcall CheckExec(int RetVal);
  void __fastcall Clear(void);
  int __fastcall DoDllLoad(void);
  void __fastcall DoDllUnload(void);
  int __fastcall DoUnzExec(pUnZipParms Rec);
  int __fastcall DoZipExec(PZipParms Rec);
  void __fastcall Empty(void);
public:
  __fastcall TZBDynamicBind(TObject* ABuilder);
  __fastcall ~TZBDynamicBind(void);
  void __fastcall AbortDll(void);
  __property String DLLDirectory={read=FDllDirectory,write=SetDllDirectory};
  __property int Dll_Build={read=GetDll_Build};
  __property bool Dll_Load={read=GetDll_Load,write=SetDll_Load};
  __property String Dll_Path={read=GetPath};
  __property String Dll_Version={read=GetVersion};
};

#pragma option -a.

}/* EOF namespace Zbdbind */

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
	using namespace Zbdbind;
#endif

#endif

#endif

