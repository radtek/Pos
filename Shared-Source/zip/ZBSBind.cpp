/*
  ZBSBind.cpp - Source file to statically bind DLL to component
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

#pragma hdrstop
#include "ZBSBind.h"
#pragma package(smart_init)

#ifdef STATIC_LOAD_DELZIP_DLL
//---------------------------------------------------------------------------
void __fastcall TZBStaticBind::AbortDll(void)
{
    Cancel = true;
    if (FDllOperKey)
        DZ_Abort(reinterpret_cast<void*>(FDllOperKey));
}
/*? TZBStaticBind::CheckExec-------------------------------------------------
*/
void __fastcall TZBStaticBind::CheckExec(int retval)
{
    int x;
    if (retval < 0)
    {
        switch (retval)
        {
        case CRITICAL_CANCELLED:
            x = GE_DLLCancel;
            break;
        case CRITICAL_ABORT:
            x = GE_DLLAbort;
            break;
        case CRITICAL_CALLBACK:
            x = GE_DLLEvent;
            break;
        case CRITICAL_MEMORY:
            x = GE_DLLMem;
            break;
        case CRITICAL_STRUCT:
            x = GE_DLLStruct;
            break;
        default:
            throw EZipBuilder(GE_DLLCritical, ARRAYOFCONST((retval)));
        }
        throw EZipBuilder(x, true);
    }
}
//? TZBStaticBind::CheckExec

//---------------------------------------------------------------------------
int __fastcall TZBStaticBind::DoDllLoad(void)
{
    return DZ_PrivVersion();
}
//---------------------------------------------------------------------------
int __fastcall TZBStaticBind::DoUnzExec(pUnZipParms Rec)
{
    int Result;
    try
    {
        Result = DZ_UnzExec(Rec);
    }
    catch (...)
    {
        Result = -7;
        FDllOperKey = 0;
    }
    CheckExec(Result);
    return Result;
}
//---------------------------------------------------------------------------
int __fastcall TZBStaticBind::DoZipExec(PZipParms Rec)
{
    int Result;
    try
    {
        Result = DZ_ZipExec(Rec);
    }
    catch (...)
    {
        Result = -7;
        FDllOperKey = 0;
    }
    CheckExec(Result);
    return Result;
}
//---------------------------------------------------------------------------
String __fastcall TZBStaticBind::GetDllDirectory(void)
{
    return "";
}
//---------------------------------------------------------------------------
int __fastcall TZBStaticBind::GetDll_Build(void)
{
    return DZ_PrivVersion();
}
//---------------------------------------------------------------------------
bool __fastcall TZBStaticBind::GetDll_Load(void)
{
    return true;
}
//---------------------------------------------------------------------------
String __fastcall TZBStaticBind::GetDllPath(void)
{
    return DZ_Path();
}
//---------------------------------------------------------------------------
String __fastcall TZBStaticBind::GetDllVersion(void)
{
    int ver = DZ_PrivVersion();
    return Format("<%d.%2.2d.%2.2d.%2.2d>", ARRAYOFCONST((ver / 1000000,
                  (ver / 10000) % 100,
                  (ver / 100) % 100, ver % 100)));
}
#endif
