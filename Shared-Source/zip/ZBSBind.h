/*
  ZBSBind.h - Header file to statically bind DLL to component
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

  last modified 2005-02-06
---------------------------------------------------------------------------*/

#ifndef ZBSBindH
#define ZBSBindH
#include "CondDefines.h"

#ifdef STATIC_LOAD_DELZIP_DLL
#include <vcl\classes.hpp>
#include "CondDefines.h"
#include "ZBDelZip.h"
#include "ZBCore.h"

extern "C"
{
	BOOL WINAPI  DllEntryPoint( HINSTANCE hinstDll, DWORD fdwRreason,
                                     LPVOID plvReserved );
	long WINAPI DZ_UnzExec( UnZipParms * C );
	long WINAPI DZ_ZipExec( ZipParms * C );
	long WINAPI DZ_Abort( void * C );
	long WINAPI DZ_Version( void );
	long WINAPI DZ_PrivVersion( void );
	char* WINAPI DZ_Path(void);
}


namespace Zbsbind
{



#if __BORLANDC__ < 1344
#pragma option -a4
#else
#pragma option -a8
#endif

class __declspec(delphiclass) TZBStaticBind : public TZBCore
{
private:
  String __fastcall GetDllDirectory(void);
  int __fastcall GetDll_Build(void);
  bool __fastcall GetDll_Load(void);
  String __fastcall GetDllPath(void);
  void __fastcall SetDllDirectory(const String Value) {};
  void __fastcall SetDll_Load(const bool Value) {};
protected:
  unsigned FDllOperKey;
  void __fastcall CheckExec(int RetVal);
  int __fastcall DoDllLoad(void);
  void __fastcall DoDllUnload(void) {};
  int __fastcall DoUnzExec(pUnZipParms Rec);
  int __fastcall DoZipExec(PZipParms Rec);
  String __fastcall GetDllVersion(void);
public:
  __fastcall TZBStaticBind(TObject* ABuilder):TZBCore(ABuilder) {};
  __fastcall ~TZBStaticBind(void) {};
  void __fastcall AbortDll(void);
  __property String DLLDirectory={read=GetDllDirectory,write=SetDllDirectory};
  __property int Dll_Build={read=GetDll_Build};
  __property bool Dll_Load={read=GetDll_Load,write=SetDll_Load};
  __property String Dll_Path={read=GetDllPath};
  __property String Dll_Version={read=GetDllVersion};
};
#pragma option -a.

}/* EOF namespace Zbsbind */

#if !defined(NO_IMPLICIT_NAMESPACE_USE)
	using namespace Zbsbind;
#endif

#endif

#endif

