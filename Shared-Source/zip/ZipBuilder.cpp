/*
  ZipBuilder.cpp - Source file of main unit of TZipBuilder
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2009  Roger Aelbrecht

  Name hashing by Bernd Heinemann

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

  last modified 2009-03-15
-------------------------------------------------*/
#include <vcl.h>
#pragma hdrstop

#include "ZipBuilder.h"
#include "ZipMsg.h"
#include "ZipUtils.h"


#if (__BORLANDC__ >= 0x0560)
#pragma link "vcl.bpi"
#endif

//---------------------------------------------------------------------------
#pragma package(smart_init)


// ================== ZipBuilder Version constants==========================
const char* ZIPBUILDERVERSION = "1.79";
const char* ZIPBUILDERBUILD   = "1.79.10.20";
/* file version format changed to 1.79.00.02
           1 - major version number  : needs sync with TZipMaster
          79 - minor                 : needs sync
          00 - update                : no sync
          02 - build number          : no sync  */
const int ZIPBUILDERVER = 179;
const int ZIPBUILDERPRIV = 1791020;
int DELZIPVERSION = 179;   //no const is defined extern in other units

extern AddStoreExts DefAddStoreSuffixes;

//----------------------------------------------------------------------------
static inline TZipBuilder *ValidCtrCheck()
{
    return new TZipBuilder(NULL);
}
//----------------------------------------------------------------------------
namespace Zipbuilder
{
void __fastcall PACKAGE Register()
{
    TComponentClass classes[ 1 ] = {__classid(TZipBuilder) };
    RegisterComponents("ZipComponents", classes, 0);
}
}

//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::AbortDll(void)
{
    FZip->AbortDll();
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::Add(void)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->Add();
        }
    __finally
    {
        Done();
    }
    return Result;
}
//----------------------------------------------------------------------------
#ifndef NO_STREAM
int __fastcall TCustomZipBuilder::AddStreamToFile(const String& Filename,
        const DWORD FileDate, const DWord FileAttr)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->AddStreamToFile(Filename, FileDate, FileAttr);
        }
    __finally
    {
        Done();
    }
    return Result;
}
#endif
//----------------------------------------------------------------------------
#ifndef NO_STREAM
TZipStream* __fastcall TCustomZipBuilder::AddStreamToStream(TMemoryStream* InStream)
{
    TZipStream* Result = NULL;
    if (!Starting())
        try
        {
            Result = FZip->AddStreamToStream(InStream);
        }
    __finally
    {
        Done();
    }
    return Result;
}
#endif
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::AppendSlash(const String sDir)
{
    return DelimitPath(sDir, true);
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::BeforeDestruction(void)
{
    if (FZip)
        FZip->Active = false;
    inherited::BeforeDestruction();
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::Clear(void)
{
    if (ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading) ||
            !(Active && Stopped()))
        return;
    {
        FZip->Clear();
        Done();
        FReentry = false;
    }
}

#ifndef NO_SFX
/*?TCustomZipBuilder::ConvertSFX-----------------------------------------------
1.79 18 Nov 2004 RA Use TZipBuilderSFX as internal SFX
*/
int __fastcall TCustomZipBuilder::ConvertSFX(void)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->ConvertSFX();
        }
    __finally
    {
        Done();
    }
    return Result;
}
//?TCustomZipBuilder::ConvertSFX
#endif

#ifndef NO_SFX
/*?TCustomZipBuilder::ConvertZIP-----------------------------------------------
1.79 18 Nov 2004 RA Use TZipBuilderSFX as internal SFX
*/
int __fastcall TCustomZipBuilder::ConvertZIP(void)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->ConvertZIP();
        }
    __finally
    {
        Done();
    }
    return Result;
}
//?TCustomZipBuilder::ConvertZIP
#endif

//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::CopyZippedFiles(TCustomZipBuilder* DesTCustomZipBuilder, bool DeleteFromSource,
        OvrOpts OverwriteDest)
{
    int Result;
    TCustomZipBuilder* tmp = NULL;
    if ((Result = Starting()) == 0)
        try
        {
            Result = DesTCustomZipBuilder->Starting();
            if (!Result)
            {
                tmp = DesTCustomZipBuilder;
                Result = FZip->CopyZippedFiles(tmp->FZip, DeleteFromSource, OverwriteDest);
            }
        }
    __finally
    {
        if (tmp)
            DesTCustomZipBuilder->Done();
        Done();
    }
    return Result;
}

//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::Copy_File(const String InFileName, const String OutFileName)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->Copy_File(InFileName, OutFileName);
        }
    __finally
    {
        Done();
    }
    return Result;
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::Delete(void)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->Delete();
        }
    __finally
    {
        Done();
    }
    return Result;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::Done(void)
{
    FZip->Done();
    FBusy = false;
    if (FZip->Reentry)
        FReentry = true;
    FFSpecArgs->Assign(FZip->FSpecArgs);
    FFSpecArgsExcl->Assign(FZip->FSpecArgsExcl);
    if (FActive < 0)
    {
        FActive = 0;
        FZip->Active = false;
    }
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::Extract(void)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->Extract();
        }
    __finally
    {
        Done();
    }
    return Result;
}
//----------------------------------------------------------------------------
#ifndef NO_STREAM
TZipStream* __fastcall TCustomZipBuilder::ExtractFileToStream(const String& Filename)
{
    TZipStream* Result = NULL;
    if (!Starting())
        try
        {
            Result = FZip->ExtractFileToStream(Filename);
        }
    __finally
    {
        Done();
    }
    return Result;
}
#endif
//----------------------------------------------------------------------------
#ifndef NO_STREAM
TZipStream* __fastcall TCustomZipBuilder::ExtractStreamToStream(TMemoryStream* InStream, unsigned long OutSize)
{
    TZipStream* Result = NULL;
    if (!Starting())
        try
        {
            Result = FZip->ExtractStreamToStream(InStream, OutSize);
        }
    __finally
    {
        Done();
    }
    return Result;
}
#endif
//----------------------------------------------------------------------------
ZipDirEntry* __fastcall TCustomZipBuilder::Find(const String& fspec, int& idx)
{
    ZipDirEntry* Result;
    if (idx < 0)
        idx = -1;
    int c = Count - 1;
    while (idx < c)
    {
        Result = DirEntry[++idx];
        if (FileNameMatch(fspec, Result->FileName))
            return Result;
    }
    idx = -1;
    return NULL;
}
//----------------------------------------------------------------------------
ZipDirEntry* __fastcall TCustomZipBuilder::Find(const String& FileName)
{
	ZipDirEntry* Result = NULL;
	if(Active)
  {
    int idx = -1;
    if((AnsiPos("*", FileName) > 0) || (AnsiPos("?", FileName) > 0))
      Result = Find(FileName, idx);
    else
      Result = FZip->DirHashs->Find(FileName);
  }
  return Result;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::FullVersionString(void)
{
    String Result = "ZipBuilder " + Version + " "
#ifdef NO_SPAN
                    + " -SPAN "
#endif
#ifdef NO_SFX
                    + " -SFX "
#endif
#ifdef NO_STREAM
                    + " -STREAM "
#endif
                    ;
    return Result + ", DLL " + Dll_Version;
}
//----------------------------------------------------------------------------
bool __fastcall TCustomZipBuilder::GetActive(void)
{
    return FActive;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetAddPassword(void)
{
    TPasswordButton Resp;
    return FZip->GetAddPassword(Resp);
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetAddPassword(TPasswordButton& Response)
{
    return FZip->GetAddPassword(Response);
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetBuild(void)
{
    return ZIPBUILDERPRIV;
}
//----------------------------------------------------------------------------
bool __fastcall TCustomZipBuilder::GetBusy(void)
{
    return FBusy;
}
//----------------------------------------------------------------------------
bool __fastcall TCustomZipBuilder::GetCancel(void)
{
    return FZip->Cancel;
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetCount(void)
{
    if (Active)
        return FZip->Count;
    return 0;
}
//----------------------------------------------------------------------------
ZipDirEntry* __fastcall TCustomZipBuilder::GetDirEntry(int idx)
{
    if (Active)
        return FZip->DirEntry[idx];
    return NULL;
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetDirOnlyCount(void)
{
    return FZip->DirOnlyCount;
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetDll_Build(void)
{
    int Result = 0;
    if (Busy)
        Result = FZip->Dll_Build;
    else
    {
        try
        {
            if (!Starting())
                Result = FZip->Dll_Build;
        }
        __finally
        {
            Done();
        }
    }
    return Result;
}
//----------------------------------------------------------------------------
bool __fastcall TCustomZipBuilder::GetDll_Load(void)
{
    return (ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading)) ? FDllLoad :
           FZip->Dll_Load;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetDll_Path(void)
{
//  return FZip->Dll_Load ? FZip->Dll_Path : AnsiString("");
    String Result;
    if (Busy)
        Result = FZip->Dll_Path;
    else
    {
        try
        {
            if (!Starting())
                Result = FZip->Dll_Path;
        }
        __finally
        {
            Done();
        }
    }
    return Result;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetDll_Version(void)
{
    String Result;
    if (Busy)
        Result = FZip->Dll_Version;
    else
    {
        try
        {
            if (!Starting())
                Result = FZip->Dll_Version;
        }
        __finally
        {
            Done();
        }
    }
    return Result;
}
ZipDirEntry* __fastcall TCustomZipBuilder::GetEntry(const String Name)
{
  return Find(Name);
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetErrCode(void)
{
    if (FReentry)
        return FZip->ErrCode | Reentry_Error;
    if (!Active)
        return GE_Inactive;
    return FZip->ErrCode;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetErrMessage(void)
{
    String Result;
    if (Active)
        Result = FZip->Message;
    else
        Result = ZipLoadStr(GE_Inactive);
    if (FReentry)
        Result = ZipFmtLoadStr(GE_WasBusy, ARRAYOFCONST((Result)));
    return Result;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetExtrPassword(void)
{
    TPasswordButton Resp;
    return FZip->GetExtrPassword(Resp);
}

//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetExtrPassword(TPasswordButton& Response)
{
    return FZip->GetExtrPassword(Response);
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetFullErrCode(void)
{
    return FZip->FullErrCode;
}
//----------------------------------------------------------------------------
bool __fastcall TCustomZipBuilder::GetIsSpanned(void)
{
    return FZip->IsSpanned;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetLanguage(void)
{
    if (ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
        return FLanguage;
    return GetZipMsgLanguage(0);
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetLanguageInfo(int Index, unsigned info)
{
    return GetZipMsgLanguageInfo(Index, info);
}
//----------------------------------------------------------------------------
TCheckTerminateEvent __fastcall TCustomZipBuilder::GetOnCheckTerminate(void)
{
    return FZip->Events->FOnCheckTerminate;
}
//----------------------------------------------------------------------------
TCopyZipOverwriteEvent __fastcall TCustomZipBuilder::GetOnCopyZipOverwrite(void)
{
    return FZip->Events->FOnCopyZipOverwrite;
}
//----------------------------------------------------------------------------
TCRC32ErrorEvent __fastcall TCustomZipBuilder::GetOnCRC32Error(void)
{
    return FZip->Events->FOnCRC32Error;
}
//----------------------------------------------------------------------------
TNotifyEvent __fastcall TCustomZipBuilder::GetOnDirUpdate(void)
{
    return FZip->Events->FOnDirUpdate;
}
//----------------------------------------------------------------------------
TExtractOverwriteEvent __fastcall TCustomZipBuilder::GetOnExtractOverwrite(void)
{
    return FZip->Events->FOnExtractOverwrite;
}
//----------------------------------------------------------------------------
TExtractSkippedEvent __fastcall TCustomZipBuilder::GetOnExtractSkipped(void)
{
    return FZip->Events->FOnExtractSkipped;
}
//----------------------------------------------------------------------------
TFileCommentEvent __fastcall TCustomZipBuilder::GetOnFileComment(void)
{
    return FZip->Events->FOnFileComment;
}
//----------------------------------------------------------------------------
TFileExtraEvent __fastcall TCustomZipBuilder::GetOnFileExtra(void)
{
    return FZip->Events->FOnFileExtra;
}
//----------------------------------------------------------------------------
#ifndef NO_SPAN
TGetNextDiskEvent __fastcall TCustomZipBuilder::GetOnGetNextDisk(void)
{
    return FZip->Events->FOnGetNextDisk;
}
#endif
//----------------------------------------------------------------------------
TItemProgressEvent __fastcall TCustomZipBuilder::GetOnItemProgress(void)
{
    return FZip->Events->FOnItemProgress;
}
//----------------------------------------------------------------------------
TMsgEvent __fastcall TCustomZipBuilder::GetOnMessage(void)
{
    return FZip->Events->FOnMessage;
}
//----------------------------------------------------------------------------
TNewNameEvent __fastcall TCustomZipBuilder::GetOnNewName(void)
{
    return FZip->Events->FOnNewName;
}
//----------------------------------------------------------------------------
TPasswordErrorEvent __fastcall TCustomZipBuilder::GetOnPasswordError(void)
{
    return FZip->Events->FOnPasswordError;
}
//----------------------------------------------------------------------------
Zbcore::TProgressEvent __fastcall TCustomZipBuilder::GetOnProgress(void)
{
    return FZip->Events->FOnProgress;
}
//----------------------------------------------------------------------------
TProgressDetailsEvent __fastcall TCustomZipBuilder::GetOnProgressDetails(void)
{
    return FZip->Events->FOnProgressDetails;
}
//----------------------------------------------------------------------------
#ifndef NO_SPAN
TStatusDiskEvent __fastcall TCustomZipBuilder::GetOnStatusDisk(void)
{
    return FZip->Events->FOnStatusDisk;
}
#endif
//----------------------------------------------------------------------------
TSetAddNameEvent __fastcall TCustomZipBuilder::GetOnSetAddName(void)
{
    return FZip->Events->FOnSetAddName;
}
//----------------------------------------------------------------------------
TSetExtNameEvent __fastcall TCustomZipBuilder::GetOnSetExtName(void)
{
    return FZip->Events->FOnSetExtName;
}
//----------------------------------------------------------------------------
TSetNewNameEvent __fastcall TCustomZipBuilder::GetOnSetNewName(void)
{
    return FZip->Events->FOnSetNewName;
}
//----------------------------------------------------------------------------
TTickEvent __fastcall TCustomZipBuilder::GetOnTick(void)
{
    return FZip->Events->FOnTick;
}
//----------------------------------------------------------------------------
TTotalProgressEvent __fastcall TCustomZipBuilder::GetOnTotalProgress(void)
{
    return FZip->Events->FOnTotalProgress;
}
//----------------------------------------------------------------------------
TZipDialogEvent __fastcall TCustomZipBuilder::GetOnZipDialog(void)
{
    return FZip->Events->FOnZipDialog;
}
//----------------------------------------------------------------------------
TPasswordButton __fastcall TCustomZipBuilder::GetPassword(const String& DialogCaption,
        const String& MsgTxt,
        TPasswordButtons pwb, String& ResultStr)
{
    return FZip->GetPassword(DialogCaption, MsgTxt, pwb, ResultStr);
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetPPassword(void)
{
    return FZip->Password;
}
//----------------------------------------------------------------------------
unsigned __fastcall TCustomZipBuilder::GetRealFileSize(void)
{
    return FZip->ZipFileSize;
}

#ifndef NO_SFX
/*?TCustomZipBuilder::GetSFXLanguage----------------------------------------------
1.79 20 Nov 2004 RA added helper for TZipBuilderSFX as internal SFX
*/
String __fastcall TCustomZipBuilder::GetSFXLanguage(void)
{
    return FSFXSlave->SFXLanguage;
}
//?TCustomZipBuilder::GetSFXLanguage
#endif

//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetSFXOffset(void)
{
    return FZip->SFXOffset;
}
#ifndef NO_SFX
/*?TCustomZipBuilder::GetSFXPath----------------------------------------------
1.79 20 Nov 2004 RA added helper for TZipBuilderSFX as internal SFX
*/
String __fastcall TCustomZipBuilder::GetSFXPath(void)
{
    return FSFXSlave->SFXPath;
}
//?TCustomZipBuilder::GetSFXPath
#endif

//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetSuccessCnt(void)
{
    return FZip->SuccessCnt;
}
//----------------------------------------------------------------------------
__int64 __fastcall TCustomZipBuilder::GetTotalSizeToProcess(void)
{
    return FZip->TotalSizeToProcess;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetVersion(void)
{
    return ZIPBUILDERBUILD;
}
//----------------------------------------------------------------------------
bool __fastcall TCustomZipBuilder::GetUnzBusy(void)
{
    return FZip->UnzBusy;
}
#ifdef KEEP_BACKWARD
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetUnzVers(void)
{
    if (!Busy)
        FZip->DLLDirectory = DLLDirectory;
    return FZip->Dll_Build / 10000;
}
#endif
//----------------------------------------------------------------------------
bool __fastcall TCustomZipBuilder::GetZipBusy(void)
{
    return FZip->ZipBusy;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetZipComment(void)
{
    return FZip->ZipComment;
}
//----------------------------------------------------------------------------
TList* __fastcall TCustomZipBuilder::GetZipContents(void)
{
    return FZip->ZipContents;
}
//----------------------------------------------------------------------------
unsigned __fastcall TCustomZipBuilder::GetZipEOC(void)
{
    return FZip->ZipEOC;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::GetZipFileName(void)
{
    return FZip->ZipFileName;
}
//----------------------------------------------------------------------------
unsigned __fastcall TCustomZipBuilder::GetZipSOC(void)
{
    return FZip->ZipSOC;
}
//----------------------------------------------------------------------------
#ifndef NO_STREAM
TZipStream* __fastcall TCustomZipBuilder::GetZipStream(void)
{
    return FZip->ZipStream;
}
#endif
#ifdef KEEP_BACKWARD
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::GetZipVers(void)
{
    return GetUnzVers();
}
#endif
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::IndexOf(const String& fname)
{
    for (int i = 0; i < Count; ++i)
    {
        if (FileNameMatch(fname, static_cast<ZipDirEntry*>(DirEntry[i])->FileName))
            return i;
    }
    return -1;
}
//----------------------------------------------------------------------------
#ifndef NO_SFX
int __fastcall TCustomZipBuilder::IsZipSFX(const String& SFXExeName)
{
    return FZip->IsZipSFX(SFXExeName);
}
#endif

//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::List(void)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->List();
        }
    __finally
    {
        Done();
    }
    return Result;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::Loaded(void)
{
    inherited::Loaded();
    if (!ComponentState.Contains(csDesigning))
    {
        SetZipMsgLanguage(FLanguage);
        if ((FActive == 2) && !Starting())
        {
            try
            {
                FZip->Active = true;
            }
            __finally
            {
                Done();
            }
        }
        else
            FZip->Active = false;
    }
}

#ifdef KEEP_BACKWARD
/*? TCustomZipBuilder::Load_Unz_Dll------------------------------------------
1.79 29 Nov 2004 RP overloaded version for silent loading added
*/
int __fastcall TCustomZipBuilder::Load_Unz_Dll(void)
{
    return Load_Zip_Dll(false);
}
int __fastcall TCustomZipBuilder::Load_Unz_Dll(bool Silent)
{
    return Load_Zip_Dll(Silent);
}
//?TCustomZipBuilder::Load_Unz_Dll

/*? TCustomZipBuilder::Load_Zip_Dll------------------------------------------
1.79 29 Nov 2004 RP overloaded version for silent loading added
*/
int __fastcall TCustomZipBuilder::Load_Zip_Dll(void)
{
    return Load_Zip_Dll(false);
}
int __fastcall TCustomZipBuilder::Load_Zip_Dll(bool Silent)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            FZip->Dll_Load = true;
            Result = FZip->Dll_Build / 10000;
            if (!Silent && (Result != DELZIPVERSION))
                throw EZipBuilder(FZip, LD_BadDll, Dll_Path);
        }
    __finally
    {
        Done();
    }
    return Result;
}
//? TCustomZipBuilder::Load_Zip_Dll
#endif
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::MakeTempFileName(const String& Prefix,
        const String& Extension)
{
    return FZip->MakeTempFileName(Prefix, Extension);
}

//----------------------------------------------------------------------------
#ifndef NO_SPAN
int __fastcall TCustomZipBuilder::ReadSpan(const String InFileName, String OutFilePath)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->ReadSpan(InFileName, OutFilePath, false);
        }
    __finally
    {
        Done();
    }
    return Result;
}
#endif

//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::Rename(TList& RenameList, int DateTime, TZBRenameOpts How)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->Rename(RenameList, DateTime, How);
        }
    __finally
    {
        Done();
    }
    return Result;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetActive(bool Value)
{
    int was = FActive;
    FActive = 0;
    if (ComponentState.Contains(csDesigning))
    {
        if (Value)
            FActive = 3; // set but ignored
    }
    else
        if (ComponentState.Contains(csLoading))
        {
            if (Value)
                FActive = 2;   // loaded will act on it
        }
        else
            if (Busy)
            {
                FActive = Value ? 1 : -1;  // done will clear it
            }
            else // not busy or loading or designing
            {
                if (Value)
                {
                    FActive = 1;
                    if (!was && !Starting())
                    {
                        try
                        {
                            FZip->Active = true;
                        }
                        __finally
                        {
                            Done();
                        }
                    }
                    else
                        FZip->Active = false;
                }
                else
                    FZip->Active = false;
            }
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetCancel(bool Value)
{
    FZip->Cancel = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetDll_Load(const bool Value)
{
    if (Value != Dll_Load)
    {
        FDllLoad = Value;   // will be applied when Active is set
        if (Active && FBusy)
        {
            FZip->Dll_Load = Value;
            FDllLoad = FZip->Dll_Load;   // true if it loaded
        }
        else
        {
            try
            {
                if (!Starting())
                    FDllLoad = FZip->Dll_Load; // true if loaded
            }
            __finally
            {
                Done();
            }
        }
    }
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetErrCode(int Value)
{
    if (Stopped())
        FZip->ErrCode = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetFSpecArgs(const TStrings* Value)
{
    FFSpecArgs->Assign(const_cast<TPersistent*>(static_cast<const TPersistent*>(Value)));
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetFSpecArgsExcl(const TStrings* Value)
{
    FFSpecArgsExcl->Assign(const_cast<TPersistent*>(static_cast<const TPersistent*>(Value)));
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetLanguage(const String Value)
{
    if (ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
        FLanguage = Value;
    else
        SetZipMsgLanguage(Value);
}
//----------------------------------------------------------------------------
// 1.79.06.04 RP fix won't set in designer
void __fastcall TCustomZipBuilder::SetFileName(const String Value)
{
    if (ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
    {
        FZip->Active = false; // stop any actions
        FZip->ZipFileName = Value;
    }
    else
        if (!Starting())
        {
            try
            {
                FZip->ZipFileName = Value;
            }
            __finally
            {
                Done();
            }
        }
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnCheckTerminate(TCheckTerminateEvent Value)
{
    FZip->Events->FOnCheckTerminate = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnCopyZipOverwrite(TCopyZipOverwriteEvent Value)
{
    FZip->Events->FOnCopyZipOverwrite = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnCRC32Error(TCRC32ErrorEvent Value)
{
    FZip->Events->FOnCRC32Error = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnDirUpdate(TNotifyEvent Value)
{
    FZip->Events->FOnDirUpdate = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnExtractOverwrite(TExtractOverwriteEvent Value)
{
    FZip->Events->FOnExtractOverwrite = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnExtractSkipped(TExtractSkippedEvent Value)
{
    FZip->Events->FOnExtractSkipped = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnFileComment(TFileCommentEvent Value)
{
    FZip->Events->FOnFileComment = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnFileExtra(TFileExtraEvent Value)
{
    FZip->Events->FOnFileExtra = Value;
}
//----------------------------------------------------------------------------
#ifndef NO_SPAN
void __fastcall TCustomZipBuilder::SetOnGetNextDisk(TGetNextDiskEvent Value)
{
    FZip->Events->FOnGetNextDisk = Value;
}
#endif
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnItemProgress(TItemProgressEvent Value)
{
    FZip->Events->FOnItemProgress = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnMessage(TMsgEvent Value)
{
    FZip->Events->FOnMessage = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnNewName(TNewNameEvent Value)
{
    FZip->Events->FOnNewName = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnPasswordError(TPasswordErrorEvent Value)
{
    FZip->Events->FOnPasswordError = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnProgress(Zbcore::TProgressEvent Value)
{
    FZip->Events->FOnProgress = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnProgressDetails(TProgressDetailsEvent Value)
{
    FZip->Events->FOnProgressDetails = Value;
}
//----------------------------------------------------------------------------
#ifndef NO_SPAN
void __fastcall TCustomZipBuilder::SetOnStatusDisk(TStatusDiskEvent Value)
{
    FZip->Events->FOnStatusDisk = Value;
}
#endif
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnSetAddName(TSetAddNameEvent Value)
{
    FZip->Events->FOnSetAddName = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnSetExtName(TSetExtNameEvent Value)
{
    FZip->Events->FOnSetExtName = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnSetNewName(TSetNewNameEvent Value)
{
    FZip->Events->FOnSetNewName = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnTick(TTickEvent Value)
{
    FZip->Events->FOnTick = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnTotalProgress(TTotalProgressEvent Value)
{
    FZip->Events->FOnTotalProgress = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetOnZipDialog(TZipDialogEvent Value)
{
    FZip->Events->FOnZipDialog = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetPassword(const String& Value)
{
    FZip->Password = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetPasswordReqCount(unsigned Value)
{
    if (Value != FPasswordReqCount)
    {
        if (Value > 15)
            Value = 15;
        FPasswordReqCount = Value;
    }
}
#ifndef NO_SFX
//---------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetSFXIcon(TIcon *aIcon)
{
    FSFXIcon->Assign(aIcon);
}
/*?TCustomZipBuilder::SetSFXLanguage----------------------------------------------
1.79 20 Nov 2004 RA added helper for TZipBuilderSFX as internal SFX
*/
void __fastcall TCustomZipBuilder::SetSFXLanguage(const String Value)
{
    FSFXSlave->SFXLanguage = Value;
}
//?TCustomZipBuilder::SetSFXLanguage

/*?TCustomZipBuilder::SetSFXPath----------------------------------------------
1.79 20 Nov 2004 RA added helper for TZipBuilderSFX as internal SFX
*/
void __fastcall TCustomZipBuilder::SetSFXPath(TFileName Value)
{
    FSFXSlave->SFXPath = Value;
}
//?TCustomZipBuilder::SetSFXPath
#endif

/*?TCustomZipBuilder::Starting--------------------------------------------------
*/
int __fastcall TCustomZipBuilder::Starting(void)
{
    if (ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
        return -256;
    if (!(Active && Stopped()))
        return BUSY_ERROR;
    try
    {
        FZip->Builder = this;
        FZip->ErrCode = 0;
        FZip->AddCompLevel = AddCompLevel;
        FZip->AddFrom = AddFrom;
        FZip->AddOptions = AddOptions;
        FZip->AddStoreSuffixes = AddStoreSuffixes;
        FZip->BufSize = BufSize;
        FZip->ExtAddStoreSuffixes = ExtAddStoreSuffixes;
        FZip->CodePage = CodePage;
        FZip->ExtrBaseDir = ExtrBaseDir;
        FZip->ExtrOptions = ExtrOptions;
        FZip->Unattended = Unattended;
        FZip->Verbose = Verbose;
        FZip->Trace = Trace;
        FZip->DLLDirectory = DLLDirectory;
        FZip->NotMainTask = NotMainThread;
#ifndef NO_SPAN
        FZip->ConfirmErase = ConfirmErase;
        FZip->KeepFreeOnAllDisks = KeepFreeOnAllDisks;
        FZip->KeepFreeOnDisk1 = KeepFreeOnDisk1;
        FZip->MaxVolumeSize = MaxVolumeSize;
        FZip->MinFreeVolumeSize = MinFreeVolumeSize;
        FZip->SpanOptions = SpanOptions;
#endif
        FZip->FSpecArgs->Assign(FSpecArgs);
        FZip->FSpecArgsExcl->Assign(FSpecArgsExcl);
        FZip->Handle = Handle;
        FZip->HowToDelete = HowToDelete;
        FZip->Password = Password;
        FZip->PasswordReqCount = PasswordReqCount;
        FZip->RootDir = RootDir;
        FZip->TempDir = TempDir;
        FZip->Unattended = Unattended;
        FZip->UseDirOnlyEntries = UseDirOnlyEntries;
        FZip->Active = true;
        FZip->Starting();
        // if it gets here then starting should be ok
        FBusy = true;
        FReentry = false;
        FZip->Dll_Load = FDllLoad;
    }
    catch (EZipBuilder &ews)
    {
        ShowExceptionError(ews);
        return ews.Ident;
    }
    catch (...)
    {
        return -9;
    }
    return 0;
}
//?TCustomZipBuilder::Starting
//----------------------------------------------------------------------------
bool __fastcall TCustomZipBuilder::Stopped(void)
{
    if (!FBusy)
        return true;
    FReentry = true;
    FZip->Attacked(this);
    return false;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetVersion(const String Value)
{
    //FZip->VersionInfo = Value;
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::SetZipComment(const String Value)
{
    if (ComponentState.Contains(csDesigning) || ComponentState.Contains(csLoading))
        FZip->Active = false; // stop any actions
    if (!Active)
        FZip->ZipComment = Value;
    if (!Starting())
    {
        try
        {
            FZip->ZipComment = Value;
        }
        __finally
        {
            Done();
        }
    }
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::ShowExceptionError(const EZipBuilder &ZBExcept)
{
    FZip->ShowExceptionError(ZBExcept);
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::ShowZipMessage(int Ident, const String UserStr)
{
    FZip->ShowZipMessage(Ident, UserStr);
}
/*.TCustomZipBuilder::TCustomZipBuilder----------------------------------------
1.79 18 Nov 2004 RA Use TZipBuilderSFX as internal SFX
*/
__fastcall TCustomZipBuilder::TCustomZipBuilder(Classes::TComponent *AOwner) : TComponent(AOwner)
{
    FZip = new TZBWorker(this);
    FActive = 0;
    FBusy = false;
    FNotMainThread = false;
    FBufSize = 65536; // 64 kB
    FFSpecArgs = new TStringList();
    FFSpecArgsExcl = new TStringList();
    FAddCompLevel =  9;  // Default to tightest compression.
    FAddStoreSuffixes = DefAddStoreSuffixes;
    FCodePage = cpAuto;
    FEncrypt = false;
    FFromDate = 0;
    FHandle = Application->Handle;
    FHowToDelete = htdAllowUndo;
    FPassword = "";
    FPasswordReqCount = 1;
    FUnattended = false;
    FUseDirOnlyEntries = false;
#ifndef NO_SPAN
    FFreeOnDisk1 =  0;   // Don't leave anything free.
    FFreeOnAllDisks = 0;         // 1.72  use all space
    FMaxVolumeSize =  0;   // Use the maximum disk size.
    FMinFreeVolSize = 65536;  // Reject disks with less bytes than...
    SpanOptions.Clear();
    FConfirmErase   = true;
#endif
#ifndef NO_SFX
    FSFXIcon = new TIcon();
    FSFXOverWriteMode = OvrConfirm;
    FSFXCaption = "Self-extracting Archive";
    FSFXDefaultDir = "";
    FSFXCommandLine = "";
    FSFXOptions = SfxOpts() << SFXCheckSize;
    FSFXSlave = new TZipIntSFX(this);
    FZip->SFXSlave = FSFXSlave;
    FSFXSlave->ZipBuilder = this;
    SFXPath = "";
    SFXLanguage = "US";
#endif
    if (ComponentState.Contains(csDesigning))
        FActive = 3;
    else
        if (ComponentState.Contains(csLoading))
            FActive = 2;
        else
        {
            FActive = 1;
            FZip->Active = true;
        }
}
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::TheErrorCode(int errCode)
{
    return errCode & (Reentry_Error - 1);
}
#ifdef KEEP_BACKWARD
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::Unload_Unz_Dll(void)
{
    Unload_Zip_Dll();
}
//----------------------------------------------------------------------------
void __fastcall TCustomZipBuilder::Unload_Zip_Dll(void)
{
    FZip->Dll_Load = false;;
}
//----------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::UnzDllPath(void)
{
    return FZip->Dll_Path;
}
#endif
#ifndef NO_SPAN
//----------------------------------------------------------------------------
int __fastcall TCustomZipBuilder::WriteSpan(const String InFileName,
        const String OutFileName)
{
    int Result;
    if ((Result = Starting()) == 0)
        try
        {
            Result = FZip->WriteSpan(InFileName, OutFileName, false);
        }
    __finally
    {
        Done();
    }
    return Result;
}
#endif
#ifdef KEEP_BACKWARD
//---------------------------------------------------------------------------
String __fastcall TCustomZipBuilder::ZipDllPath(void)
{
    return FZip->Dll_Path;
}
#endif
//----------------------------------------------------------------------------
__fastcall TCustomZipBuilder::~TCustomZipBuilder(void)
{
    delete FFSpecArgsExcl;
    delete FFSpecArgs;
    delete FZip;
#ifndef NO_SFX
    delete FSFXIcon;
#endif
}








