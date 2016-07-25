/*
  ZBDBind.cpp - Source file to dynamically bind DLL to component
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

  last modified 2008-10-07
---------------------------------------------------------------------------*/
#pragma hdrstop

#include "ZBDBind.h"
#include "ZipUtils.h"
#include "ZipExtrLZ77.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

#ifndef STATIC_LOAD_DELZIP_DLL

const int zldTemp = 1;
const int zldAuto = 2;
const int zldFixed = 4;
const char* DelZipDll_Name = "DelZip179.dll";
const char* DelZipDll_Unzfunc = "DZ_UnzExec";
const char* DelZipDll_Zipfunc = "DZ_ZipExec";
const char* DelZipDll_Abortfunc = "DZ_Abort";
const char* DelZipDll_Versfunc = "DZ_Version";
const char* DelZipDll_Privfunc = "DZ_PrivVersion";
const char* DelZipDll_Pathfunc = "DZ_Path";

const int RDLL_Ver = 11612;   // stored UPX Dll version as string
const int RDLL_Dll  = 11613;  // stored UPX Dll
extern int DELZIPVERSION;
//---------------------------------------------------------------------------
__fastcall TZBDynamicBind::TZBDynamicBind(TObject* ABuilder) : TZBCore(ABuilder)
{
    FDllOperKey = 0;
    FDllDirectory = "";
    FKillTemp = false;
    Empty();
    loadLevel = 0;
    fPath = DelZipDll_Name;
    TmpFileName = "";
}
//---------------------------------------------------------------------------
__fastcall TZBDynamicBind::~TZBDynamicBind(void)
{
    if (hndl)
        ::FreeLibrary(hndl);
    hndl = NULL;
    RemoveTempDll();
}
//---------------------------------------------------------------------------
void __fastcall TZBDynamicBind::AbortDll(void)
{
    Cancel = true;
    if (hndl)
        AbortFunc(reinterpret_cast<void*>(FDllOperKey));
}
/*? TZBDynamicBind::CheckExec-------------------------------------------------
*/
void __fastcall TZBDynamicBind::CheckExec(int retval)
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
//? TZBDynamicBind::CheckExec
//---------------------------------------------------------------------------
void __fastcall TZBDynamicBind::Clear(void)
{
    ReleaseLib();
}
//---------------------------------------------------------------------------
int __fastcall TZBDynamicBind::DoDllLoad(void)
{
    LoadDll(zldAuto);
    if (fVer < DELZIPVERSION)
    {
        UnloadDll(zldFixed | zldAuto | zldTemp);
        throw EZipBuilder(LD_BadDll, fPath);
    }
    return hndl ? fVer : 0;
}
//---------------------------------------------------------------------------
void __fastcall TZBDynamicBind::DoDllUnload(void)
{
    UnloadDll(zldAuto);
}
//---------------------------------------------------------------------------
int __fastcall TZBDynamicBind::DoUnzExec(pUnZipParms Rec)
{
    int Result;
    try
    {
        LoadDll(zldTemp);
        try
        {
            Result = UnzExecFunc(Rec);
        }
        catch (...)
        {
            Result = -7;
        }
    }
    __finally
    {
        FDllOperKey = 0;
        UnloadDll(zldTemp);
    }
    CheckExec(Result);
    return Result;
}
//---------------------------------------------------------------------------
int __fastcall TZBDynamicBind::DoZipExec(PZipParms Rec)
{
    int Result;
    try
    {
        LoadDll(zldTemp);
        try
        {
            Result = ZipExecFunc(Rec);
        }
        catch (...)
        {
            Result = -7;
        }
    }
    __finally
    {
        FDllOperKey = 0;
        UnloadDll(zldTemp);
    }
    CheckExec(Result);
    return Result;
}

//---------------------------------------------------------------------------
void __fastcall TZBDynamicBind::Empty(void)
{
    hndl = NULL;
    ZipExecFunc = NULL;
    UnzExecFunc = NULL;
    VersFunc = NULL;
    PrivFunc = NULL;
    AbortFunc = NULL;
    PathFunc = NULL;
    fVer = 0;
    Priv = 0;
}
//---------------------------------------------------------------------------
int __fastcall TZBDynamicBind::GetDll_Build(void)
{
    LoadDll(zldTemp);
    int result = Priv;
    UnloadDll(zldTemp);
    return result;
}
//---------------------------------------------------------------------------
bool __fastcall TZBDynamicBind::GetDll_Load(void)
{
    return (hndl != 0);
}
//---------------------------------------------------------------------------
String __fastcall TZBDynamicBind::GetPath(void)
{
    LoadDll(zldTemp);
    String result = fPath;
    UnloadDll(zldTemp);
    return result;
}
/*---------------------------------------------------------------------------
1.79.1 27 dec 2005 RP trace messages added
*/
String __fastcall TZBDynamicBind::GetResDllPath(void)
{
    bool done = false;
    int ver = StrToIntDef(LoadStr(RDLL_Ver).SubString(1, 7), 0);
    if (ver > 1790000)
    {
        TmpFileName = MakeTempFileName("DZ_", ".dll");
        TResourceStream* rs = NULL;
        TFileStream* fs = NULL;
        try
        {
            rs = new TResourceStream(reinterpret_cast<int>(HInstance), RDLL_Dll, RT_RCDATA);
            if (rs && (rs->Size > 50000))
            {
                Word w;
                rs->ReadBuffer(&w, sizeof(Word));
                rs->Position = 0;
                fs = new TFileStream(TmpFileName, fmCreate);
                if (w == IMAGE_DOS_SIGNATURE)
                    done = (fs->CopyFrom(rs, rs->Size) == rs->Size);
                else
                    done = LZ77Extract(fs, rs, static_cast<int>(rs->Size)) == 0;
                if (Trace && done)
                {
                    Report(zacMessage, 0, Format("extracted resource dll %s %d", ARRAYOFCONST((TmpFileName, ver))), 0);
                }
            }
        }
        __finally
        {
            delete fs;
            delete rs;
            if (!done && FileExists(TmpFileName))
                DeleteFile(TmpFileName);
            if (!FileExists(TmpFileName))
                TmpFileName = "";
        }
    }
    else
        if (Trace)
            Report(zacMessage, 0, "No dll in resource", 0);
    return TmpFileName;
}
//---------------------------------------------------------------------------
String __fastcall TZBDynamicBind::GetVersion(void)
{
    String fc = " %d.%2.2d.%2.2d.%2.2d ";
    int v = Priv;
    if (!fVer)
    {
        LoadDll(zldTemp);
        fc = "[%d.%2.2d.%2.2d.%2.2d]";
        v = Priv;
        UnloadDll(zldTemp);
    }
    return Format(fc, ARRAYOFCONST((v / 1000000, (v / 10000) % 100,
                                    (v / 100) % 100, v % 100)));
}
//---------------------------------------------------------------------------
int __fastcall TZBDynamicBind::LoadDll(int level)
{
    loadLevel = (loadLevel | level) & 7;
    if (!hndl)
    {
        try
        {
            StartWaitCursor();
            fVer = 0;
            String FullPath;
            if (!TmpFileName.IsEmpty())
                FullPath = TmpFileName;
            // path >< force loading from resource only
            else
                if (DLLDirectory == "><")
                    FullPath = GetResDllPath();
            if (FullPath.IsEmpty())
            {
                if (!DLLDirectory.IsEmpty())
                {
                    FullPath = PathConcat(DLLDirectory, DelZipDll_Name);
                    if (DLLDirectory[1] == '.')
                        FullPath = PathConcat(ExtractFilePath(ParamStr(0)), FullPath);
                    if (!FileExists(FullPath))
                        FullPath = "";
                }
            }
            if (FullPath.IsEmpty())
                FullPath = DelZipDll_Name;  // let Windows search the std dirs
            String RVrs = LoadStr(RDLL_Ver);
            int RVer = StrToIntDef(RVrs.SubString(1, 7), 0);
            if (RVer > LoadLib(FullPath, RVer < 1790000))
                if (LoadLib(GetResDllPath(), false) < 1790000)
                    LoadLib(FullPath, true);
            if (Verbose)
                Report(zacMessage, 0, ZipFmtLoadStr(LD_DllLoaded, ARRAYOFCONST((fPath))), 0);
        }
        __finally
        {
            StopWaitCursor();
        }
    }
    return fVer;
}
//---------------------------------------------------------------------------
int __fastcall TZBDynamicBind::LoadLib(String Fullpath, bool MustExist)
{
    if (hndl)
        FreeLibrary(hndl);
    Empty();
    unsigned oldMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
    try
    {
        if ((hndl = ::LoadLibrary(Fullpath.T_STR())) != 0)
        {
            UnzExecFunc = reinterpret_cast<TUnzExecFunc>(::GetProcAddress(hndl, DelZipDll_Unzfunc));
            if (UnzExecFunc)
                ZipExecFunc = reinterpret_cast<TZipExecFunc>(::GetProcAddress(hndl, DelZipDll_Zipfunc));
            if (ZipExecFunc)
                VersFunc = reinterpret_cast<TDllVersionFunc>(::GetProcAddress(hndl, DelZipDll_Versfunc));
            if (VersFunc)
                PrivFunc = reinterpret_cast<TDllPrivVersionFunc>(::GetProcAddress(hndl, DelZipDll_Privfunc));
            if (PrivFunc)
                AbortFunc = reinterpret_cast<TAbortOperationFunc>(::GetProcAddress(hndl, DelZipDll_Abortfunc));
            if (AbortFunc)
                PathFunc = reinterpret_cast<TDllPathFunc>(::GetProcAddress(hndl, DelZipDll_Pathfunc));
        }
        else
            if (Verbose || Trace)
            {
                int error = GetLastError();
                String msg = SysErrorMessage(error);
                String tmp = Format("Error [%d %s] loading %s " , ARRAYOFCONST((error, msg, Fullpath)));
                Report(zacMessage, 0, tmp , 0);
            }
    }
    __finally
    {
        SetErrorMode(oldMode);
    }
    if (!hndl)
    {
        Empty();
        if (MustExist)
        {
            loadLevel = 0;
            throw EZipBuilder(LD_NoDll, Fullpath);
        }
        return 0;
    }
    if (PathFunc)
    {
        Priv = PrivFunc();
        fVer = VersFunc();
        fPath = String(PathFunc());
    }
    if ((fVer < DELZIPVERSION) || (fVer > 300))
    {
        Fullpath = fPath;
        FreeLibrary(hndl);
        Empty();
        loadLevel = 0;
        if (MustExist)
            throw EZipBuilder(LD_BadDll, Fullpath);
    }
    return Priv;
}
//---------------------------------------------------------------------------
void __fastcall TZBDynamicBind::ReleaseLib(void)
{
    if (hndl)
    {
        if (Verbose)
            Report(zacMessage, 0, ZipFmtLoadStr(LD_DllUnloaded, ARRAYOFCONST((fPath))), 0);
        FreeLibrary(hndl);
        hndl = 0;
    }
    if (!hndl)
    {
        Empty();
        loadLevel = 0;
        if (FKillTemp)
            RemoveTempDll();
    }
}
//---------------------------------------------------------------------------
void __fastcall TZBDynamicBind::RemoveTempDll(void)
{
    FKillTemp = false;
    if ((!TmpFileName.IsEmpty()) && FileExists(TmpFileName))
        DeleteFile(TmpFileName);
    TmpFileName = "";
}
//---------------------------------------------------------------------------
// NOTE: we will allow a dir to be specified that doesn't exist,
// since this is not the only way to locate the DLLs.
void __fastcall TZBDynamicBind::SetDllDirectory(const String Value)
{
    if (Value != FDllDirectory)
    {
        FDllDirectory = DelimitPath(Value, false);
        if (hndl && (!TmpFileName.IsEmpty()))
            FKillTemp = true;
        else
            RemoveTempDll();
    }
}
//---------------------------------------------------------------------------
void __fastcall TZBDynamicBind::SetDll_Load(const bool Value)
{
    if (Value != Dll_Load)
    {
        if (Value)
            LoadDll(zldFixed);
        else
            UnloadDll(zldFixed);
    }
}
//---------------------------------------------------------------------------
int __fastcall TZBDynamicBind::UnloadDll(int level)
{
    loadLevel = (loadLevel & ~level) & 7; // reset bit
    if (!loadLevel)
        ReleaseLib();
    return fVer;
}

#endif
