/*
  ZBWrkr.cpp - Source file of worker unit of TZipBuilder
  TZipBuilder VCL by Chris Vleghert and Eric W. Engler
  v1.79
  Copyright (C) 2009  Roger Aelbrecht

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

  last modified 2009-05-18
---------------------------------------------------------------------------*/
#include <vcl\vcl.h>
#pragma hdrstop

#include "ZBWrkr.h"
#include "ZipMsg.h"
#include "ZBDlg.h"
#include "ZBCtx.h"
#include "ZBCompat.h"

#ifdef UNICODE
#include <shlobj.h>
#endif

#pragma package(smart_init) // Used in BCB 3,4,5 ignored in BCB 1

#if (__BORLANDC__ >= 0x0560)
#define STRMSIZ __int64
#else
#define STRMSIZ int
#endif

// ===============================  constants =============================
extern const char* ZIPBUILDERBUILD;
extern int DELZIPVERSION;
AddStoreExts DefAddStoreSuffixes = AddStoreExts() << assGIF << assPNG << assZ << assZIP << assZOO << assARC
                                   << assLZH << assARJ << assTAZ << assTGZ << assLHA
                                   << assRAR << assACE << assCAB << assGZ << assGZIP << assJAR << assEXE
                                   << assJPG << assJPEG << ass7ZP << assMP3 << assWMA << assWMV
                                   << assDVRMS << assAVI;
const char extend[256] = {0};
const int SE_GeneralError = -9;
const int FS_FAT_ = 0;
const int FS_HPFS_ = 6;
const int FS_NTFS_ = 11;
const int DEF_HOST_VER = 0x0B16;    // SysNo = 11 : NTFS; Verno = 22 :2.2
// constants for external attribute from stas.h
const int S_IFREG = 0x8000; // regular file
const int S_IFDIR = 0x4000; // directory
const int S_IREAD = 0x0100; // owner may read
const int S_IWRITE = 0x0080; // owner may write
const int S_IEXEC = 0x0040; // owner may execute (dir search)
// constants for QueryZip results
const int ZB_EXE_STUB = zqbStartEXE + zqbGoodComment + zqbHasEOC;  // Empty sfx stub
const int ZB_EXE_DET_HEAD = zqbStartEXE + zqbGoodComment + zqbHasCentral + zqbHasEOC; // sfx detached header

TSysCharSet Lchars;
TSysCharSet Schars;
void initsets(void)
{
    for (char c = 'A'; c <= 'Z'; ++c)
        Lchars << c;
    for (char c = 'a'; c <= 'z'; ++c)
        Lchars << c;
    for(char c = 0; c < 32; ++c) Schars << c;
    Schars << '<' << '>' << ':' << '|' << '*' << '?' ;
}
#pragma startup initsets
//==============================ZipStream====================================
#ifndef NO_STREAM
//---------------------------------------------------------------------------
void __fastcall TZipStream::SetPointer(void *Ptr, int Size)
{
    TMemoryStream::SetPointer(Ptr, Size);
}
#endif
//==============================TMDZipData====================================
String __fastcall TMDZipData::
GetFileName(void)
{
	return String(FileNameA);
}
void __fastcall TMDZipData::SetFileName(const String& Value)
{
	FileNameA = AnsiString(Value);
}
void __fastcall TMDZipData::SetName(const char* src, int len)
{
	char* p = const_cast<char*>(src);
  FFileNameA.SetLength(len);
  memcpy(FFileNameA.c_str(), p, len);
}
//==============================ZipDataList====================================
__fastcall TMZipDataList::TMZipDataList(int TotalEntries) : TList()
{
    Capacity = TotalEntries;
    for (int i = 0; i < TotalEntries; i++)
    {
        TMDZipData* MDZD = new TMDZipData();
        Add(MDZD);
    }
}

__fastcall TMZipDataList::~TMZipDataList(void)
{
    if (Count > 0)
    {
        for (int i = Count - 1; i >= 0; i--)
        {
            TMDZipData* MDZD = Items[i];
            delete MDZD; // free the memory pointed-to by this entry
        }
        Clear(); // delete all pointers from the list
    }
}

TMDZipData* TMZipDataList::GetItems(int Index)
{
    if (Index > Count)
        throw ERangeError("Index out of range");
    return static_cast<TMDZipData*>(TList::Items[Index]);
}
//---------------------------------------------------------------------------
int TMZipDataList::IndexOf(const String& fname)
{
    int Result;
    bool Found = false;
    for (Result = 0; Result < Count; Result++)
    {
        TMDZipData* MDZD = Items[Result];
        if (CompareText(fname, MDZD->FileName) == 0) // case insensitive compare
        {
            Found = true;
            break;// case insensitive compare
        }
    }
    // Should not happen, but maybe in a bad archive..
    if(!Found)
        throw EZipBuilder("A local header not found in internal structure");
    return Result;
}

/* TMZipDataList.IndexOf
  find the index of an entry holding a header name*/
int TMZipDataList::IndexOf(const AnsiChar* hname, int len)
{
    bool Found = false;
    int Result;
    for (Result = 0; Result < Count; ++Result)
    {
        TMDZipData* MDZD = Items[Result];
        if(!MDZD || (len != MDZD->FileNameA.Length()))
            continue;
        if(!memcmp(hname, MDZD->FileNameA.c_str(), len))
    		{
      		Found = true;
      		break;
    		}
    }
    // Should not happen, but maybe in a bad archive...
    if(!Found)
        throw EZipBuilder("A local header not found in internal structure");
    return Result;
}



//==============================ZBWorker====================================
int __fastcall TZBWorker::Add(void)
{
    ExtAdd(0, 0, 0, NULL);
    return FErrCode;
}

/*? TZBWorker::AddStoreExtStr--------------------------------------------------
*/
String __fastcall TZBWorker::AddStoreExtStr(const AddStoreExts SufOption)
{
    static char *SuffixStrings[] = { "gif", "png", "z", "zip", "zoo", "arc", "lzh", "arj", "taz",
                                     "tgz", "lha", "rar", "ace", "cab", "gz", "gzip", "jar", "exe", "",
                                     "jpg", "jpeg", "7zp", "mp3", "wmv", "wma", "dvr-ms", "avi"
                                   };
    String Result;
    for (int i = assGIF; i <= assAVI; ++i)
        if ((i != assEXT) && SufOption.Contains(static_cast<AddStoreSuffix>(i)))
            Result += "." +  String(SuffixStrings[i]) + ":";
    if (SufOption.Contains(assEXT))
        Result += FExtAddStoreSuffixes;
    return Result;
}

#ifndef NO_STREAM
/*?TZBWorker::AddStreamToFile--------------------------------------------------
1.79 5 Dec 2004 RA added testing empty stream
1.76 10 June 2004 RA fix access to FSpecArgs
 1.73 (29 July 2003) RA changed trace messages
 FileAttr are set to 0 as default.
 FileAttr can be one or a logical combination of the following types:
 FILE_ATTRIBUTE_ARCHIVE, FILE_ATTRIBUTE_HIDDEN, FILE_ATTRIBUTE_READONLY, FILE_ATTRIBUTE_SYSTEM.
 FileName is as default an empty string.
 FileDate is default the system date.*/
int __fastcall TZBWorker::AddStreamToFile(const String& FileName, DWORD FileDate,
        DWORD FileAttr)
{
    if (FileName.Length() && ZipStream->Size) // does stream have data
    {
        FFSpecArgs->Clear();
        FFSpecArgs->Add(FileName);
    }
    if (!FileDate)
    {
        SYSTEMTIME st;
        FILETIME   ft;
        WORD       FatDate, FatTime;
        ::GetLocalTime(&st);
        ::SystemTimeToFileTime(&st, &ft);
        ::FileTimeToDosDateTime(&ft, &FatDate, &FatTime);
        FileDate = ((DWORD)FatDate << 16) + FatTime;
    }
    FSuccessCnt = 0;
    // Check if wildcards are set.
    if (FFSpecArgs->Count)
    {
        if (IsWild(FFSpecArgs->Strings[0]))
            ShowZipMessage(AD_InvalidName, "");
        else
            ExtAdd(1, FileDate, FileAttr, NULL);
    }
    else
        ShowZipMessage(AD_NothingToZip, "");
    return FErrCode;
}
// TZBWorker::AddStreamToFile
#endif

#ifndef NO_STREAM
// TZBWorker::AddStreamToStream-----------------------------------------------
// 1.73( 21 June 2003) RA FSuccessCnt set to 0 in case of error previous count remained
TZipStream * __fastcall TZBWorker::AddStreamToStream(TMemoryStream *InStream)
{
    FSuccessCnt = 0;
    if (InStream == FZipStream)
    {
        ShowZipMessage(AD_InIsOutStream, "");
        return NULL;
    }
    if (InStream->Size)
    {
        FZipStream->Size = InStream->Size + 6;
        ExtAdd(2, 0, 0, InStream);
        // The size of the output stream is reset by the dll in ZipParms2 in fOutStreamSize.
        // Also the size is 6 bytes more than the actual output size because:
        // - the first two bytes are used as flag, STORED=0 or DEFLATED=8.
        //   short int Flag = *(short int *)ZipBuilder1->FZipStream->Memory;
        // - the next four bytes are set to the calculated CRC value.
        //   unsigned long Crc = *(unsigned long *)((char *)ZipBuilder1->FZipStream->Memory + 2);
        // The size is reset from Inputsize +6 to the actual data size +6.
        // (you do not have to set the size yourself, in fact it won't be taken into account.
        // The start of the stream is set to the actual data start.
        if (FSuccessCnt == 1)
            FZipStream->Position = 6;
        else
            FZipStream->Size = 0;
    }
    else
        ShowZipMessage(AD_NothingToZip, "");
    return FZipStream;
}
// TZBWorker::AddStreamToStream
#endif

/*? TZBWorker.BeforeDestruction ---------------------------------------------
1.73 3 July 2003 RP stop callbacks
*/
void __fastcall TZBWorker::BeforeDestruction(void)
{
    fIsDestructing = true; // stop callbacks
    inherited::BeforeDestruction();
}

#ifndef NO_SPAN
/*? TZBWorker::CheckForDisk-------------------------------------------------
1.77 31 August 2004 RP - allow cancel on hard drive
1.77 16 August 2004 RA allow unattended split on harddisk
*/
void __fastcall TZBWorker::CheckForDisk(bool Writing)
{
    if ((FDriveFixed = IsFixedDrive(FDrive)) != 0)
    {  // If it is a fixed disk we don't want a new one.
        FNewDisk = false;
        if (FCancel)
            throw EZipAbort();
        return;
    }
    Report(zacTick, 0, "", 0);              // just ProcessMessages

    int Res = IDOK;
    int MsgFlag = zmtWarning + DHC_SpanNxtW;   // or error?
    String MsgStr;

    // First check if we want a new one or if there is a disk (still) present.
    while (FNewDisk || (Res == IDOK && !GetDriveProps()))
    {
        if (FUnattended && !FDriveFixed)
            throw EZipBuilder(DS_NoUnattSpan);
        if (FDiskNr < 0)
        {  // -1=ReadSpan(), 0=WriteSpan()
            MsgStr = ZipLoadStr(DS_InsertDisk);
            MsgFlag = zmtError + DHC_SpanNxtR;//MsgFlag or MB_ICONERROR;
        }
        else
        {
            if (Writing)
            {  // Are we from ReadSpan() or WriteSpan()?
                // This is an estimate, we can't know if every future disk has the same space available and
                // if there is no disk present we can't determine the size unless it's set by MaxVolumeSize.
                __int64 SizeOfDisk = FSizeOfDisk - FFreeOnAllDisks;
                if (FMaxVolumeSize && (FMaxVolumeSize < FSizeOfDisk))
                    SizeOfDisk = FMaxVolumeSize;
                if (SizeOfDisk > 0)
                {
                    FTotalDisks = static_cast<unsigned short>(max(static_cast<__int64>(FDiskNr),
                                  (FFileSize + 4 + FFreeOnDisk1) / SizeOfDisk)); //v1.52j
                    MsgStr = ZipFmtLoadStr(DS_InsertVolume, ARRAYOFCONST((FDiskNr + 1, FTotalDisks + 1)));
                }
                else
                {
                    FTotalDisks = -1;
                    MsgStr = ZipFmtLoadStr(DS_InsertAVolume, ARRAYOFCONST((FDiskNr + 1)));
                }
            }
            else
                MsgStr = ZipFmtLoadStr(DS_InsertVolume,  ARRAYOFCONST((FDiskNr + 1, FTotalDisks + 1)));
            MsgStr += ZipFmtLoadStr(DS_InDrive, ARRAYOFCONST((FDrive.SubString(1, 1))));
        }
        if (!(FDiskNr == 0 && GetDriveProps() && Writing)) // don't ask for 1st disk if one is present
        {
            TGetNextDiskEvent tmpGetNextDisk = FEvents.FOnGetNextDisk;
            if (tmpGetNextDisk)
            {
                bool AbortAction = false;
                tmpGetNextDisk(Builder, FDiskNr + 1, FTotalDisks + 1, FDrive.SubString(1, 1),
                               AbortAction);
                Res = AbortAction ? IDABORT : IDOK;
            }
            else
                Res = ZipMessageDlg(String(""), MsgStr, MsgFlag,
                                    TMsgDlgButtons() << mbOK << mbCancel);

        }
        FNewDisk = false;
    }
    // Check if user pressed Cancel or Mem is running out.
    if (Res != IDOK)
        throw EZipAbort();
    if (!Res)
        throw EZipBuilder(DS_NoMem);
}
#endif

/*?TZBWorker::Clear------------------------------------------------------------
1.79 18 Nov 2004 RA - Use TZipBuilderSFX as internal sfx
1.77 17 August 2004 RA default for FBufSize to 64K
1.76 26 May 2004 expanded
1.74 14 January 2004 RP added
*/
void __fastcall TZBWorker::Clear(void)
{
    fIsDestructing = true;                   // stop callbacks
    AbortDll();
    FCancel = true;
    FreeZipDirEntryRecords();
    FReenter = false;
    FDelaying.Clear();
    FRealFileSize = 0;
    FZipSOC = 0;
    FSFXOffset = 0;
    FZipComment = "";
    FIsSpanned = false;
    FFSpecArgs->Clear();
    FFSpecArgsExcl->Clear();
    FZipBusy = false;
    FUnzBusy = false;
    FZipFileName = "";
    FPassword = "";
    FPasswordReqCount = 1;
    FEncrypt = false;
    FSuccessCnt = 0;
    FAddCompLevel = 9;
    FUnattended = false;
    FFreeOnDisk1   =  0;   // Don't leave anything free.
    FFreeOnAllDisks = 0;         // use all space
    FMaxVolumeSize   =  0;   // Use the maximum disk size.
    FMinFreeVolSize  = 65536;  // Reject disks with less bytes than...
    FCodePage          = cpAuto;
    FIsSpanned         = false;
    FDriveFixed = false;
    FZipComment = "";
    FHowToDelete   = htdAllowUndo;
    FAddStoreSuffixes = DefAddStoreSuffixes;
    FUseDirOnlyEntries = false;
    FDirOnlyCount   = 0;
    FBufSize = 65536;
#ifndef NO_SPAN
    FSpanOptions.Clear();
    FConfirmErase = true;
#endif
    inherited::Clear();
}

#ifndef NO_SPAN
/*? TZBWorker::ClearFloppy-----------------------------------------------------
1.79 12 Dec 2004 RP added progress messages
1.73 (10 July 2003) RP changed trace messages
*/
void __fastcall TZBWorker::ClearFloppy(String Dir)
{
    TSearchRec SRec;
    String Dirname;
    if (FindFirst(Dir + "*.*", faAnyFile, SRec) == 0)
    {
        do
        {
            if (SRec.Attr == faDirectory && SRec.Name != "." && SRec.Name != "..")
            {
                Dirname = Dir + SRec.Name + (String)"\\" ;
                ClearFloppy(Dirname);
                if (FTrace)
                    Report(zacMessage, 0, ZipFmtLoadStr(TM_Erasing, ARRAYOFCONST((Dirname))), 0);
                else
                    Report(zacTick, 0, "", 0);
                RemoveDir(Dirname);
            }
            else
            {
                if (FTrace)
                    Report(zacMessage, 0, ZipFmtLoadStr(TM_Deleting, ARRAYOFCONST((Dirname))), 0);
                else
                    Report(zacTick, 0, "", 0);
                DeleteFile(Dir + SRec.Name);
            }
        }
        while (FindNext(SRec) == 0);
        FindClose(SRec);
    }
}
#endif

// TZBWorker::ConvertOEM--------------------------------------------------
// 1.73 (24 June 2003) RA HostVer parameter now struct zipHostInfo
// 1.73 (22 June 2003) RA added Host Ver parameters
// 1.73 (26 May 2003) RP use String for buffer
// new 1.73 (15 May 2003) replacement function that should be able to handle MBCS
/* Convert filename (and file comment string) into "internal" charset (ISO).
 * This function assumes that Zip entry filenames are coded in OEM (IBM DOS)
 * codepage when made on:
 *  -> DOS (this includes 16-bit Windows 3.1)  (FS_FAT_  {0} )
 *  -> OS/2                                    (FS_HPFS_ {6} )
 *  -> Win95/WinNT with Nico Mak's WinZip      (FS_NTFS_ {11} && hostver == "5.0" {50} )
 *
 * All other ports are assumed to code zip entry filenames in ISO 8859-1.
 * But norton Zip v1.0 sets the host byte as OEM(0) but does use the ISO set,
 * thus archives made by NortonZip are not recognized as being ISO.
 * (In this case you need to set the CodePage property manualy to cpNone.)
 * When ISO is used in the zip archive there is no need for translation
 * and so we call this cpNone.
 */
String __fastcall TZBWorker::ConvertOEM(const String Source, CodePageDirection Direction)
{
    String  Result = Source;
    if (FCodePage == cpAuto &&
            (FHostNum == FS_FAT_ || FHostNum == FS_HPFS_ || (FHostNum == FS_NTFS_ && FHostVer == 50))
            || FCodePage == cpOEM)
    {
        String buf;
#ifdef UNICODE
        AnsiString bufA;
        if (Direction == cpdOEM2ISO)
        {
            buf.SetLength(Source.Length() + 4);
            bufA = AnsiString(Source);
            OemToChar(bufA.c_str(), &buf[1]);
            Result = &buf[1];
        }
        else
        {
            bufA.SetLength(Source.Length() + 4);
            bufA = AnsiString(Source);
            CharToOem(Source.T_STR(), bufA.c_str());
            Result = String(bufA.c_str());
        }
#else
        buf.SetLength(2 * Source.Length() + 1); // allow worst case - all double
        if (Direction == cpdOEM2ISO)
            OemToChar(Source.T_STR(), buf.T_STR());
        else
            CharToOem(Source.T_STR(), buf.T_STR());
        Result = buf.T_STR();
#endif
    }
    return Result;
}
// TZBWorker::ConvertOEM

#ifndef NO_SFX
/*? TZBWorker::ConvertSFX------------------------------------------------------
1.79 12 June 2005 RA - added test if destiantion is writable
1.79 18 Nov 2004 RA - Use TZipBuilderSFX as internal sfx =>AutoExeViaAdd not used
1.73( 30 July 2003) RA use TInternalSFX class
1.73( 21 June 2003) RA FSuccessCnt set to 0
*/
int __fastcall TZBWorker::ConvertSFX(void)
{
    ErrCode = 0;
    FSuccessCnt = 1;
    int Result;
    try
    {
        CheckDestWritable(FZipFileName, false);
        Result = FSFXSlave->ConvertToSFX();
        // set zipfilename to converted filename, slave cannot do it
        if (!Result)
            FZipFileName = ChangeFileExt(FZipFileName, ".exe");
    }
    catch (EZipBuilder &ews)
    {
        ErrCode = ews.Ident;
        if (!Unattended)
            ZipMessageDlg(ews.Message, DHC_ExSFX2EXE);
        TMsgEvent tmpMessage = FEvents.FOnMessage;
        if (tmpMessage)
            tmpMessage(Builder, ErrCode, ews.Message);
        FSuccessCnt = 0;
        Result = SE_GeneralError;
    }
    catch (...)
    {
        ShowZipMessage(GE_NoMem, "");
        FSuccessCnt = 0;
        Result = SE_GeneralError;
    }
    return Result;
}
#endif


#ifndef NO_SFX
/*? TZBWorker::ConvertZIP------------------------------------------------------
1.79 12 June 2005 RA - added test if destiantion is writable
1.79 18 Nov 2004 RA - Use TZipBuilderSFX as internal sfx
1.73( 30 July 2003) RA use TInternalSFX class
1.73( 21 June 2003) RA FSuccessCnt set to 0
 Convert an .EXE archive to a .ZIP archive.
 Returns 0 if good, or else a negative error code.
*/
int __fastcall TZBWorker::ConvertZIP(void)
{
    ErrCode = 0;
    FSuccessCnt = 1;
    int Result;
    try
    {
        CheckDestWritable(FZipFileName, false);
        Result = FSFXSlave->ConvertToZip();
        // set zipfilename to converted filename, slave cannot do it
        if (!Result)
            FZipFileName = ChangeFileExt(FZipFileName, ".zip");
    }
    catch (EZipBuilder &ews)
    {
        ErrCode = ews.Ident;
        if (!Unattended)
            ZipMessageDlg(ews.Message, DHC_ExSFX2Zip);
        TMsgEvent tmpMessage = FEvents.FOnMessage;
        if (tmpMessage)
            tmpMessage(Builder, ErrCode, ews.Message);
        FSuccessCnt = 0;
        Result = SE_GeneralError;
    }
    return Result;
}
#endif

/* TZBWorker::CopyBuffer------------------------------------------------------
1.79 4 March 2006 RA corrected error for ReadLen -1
1.79 3 January 2006 RP copy > 2G
1.73 (16 May 2003) RA changed to use callback
Used by CopyFile, ConvertSFX, ConvertZip and CopyZippedFiles.
If ReadLen == -1 the remaining part of InFile is copied.
If ReadLen >= 0 than ReadLen bytes are copied.
*/
int __fastcall TZBWorker::CopyBuffer(int InFile, int OutFile, __int64 ReadLen)
{
    const SE_CopyError = -2; // Write error or no memory during copy.
    int SizeR;
    unsigned ToRead = FBufSize;
    int  Result = 0;
    char *Buffer = NULL;

    try
    {
        if (!ReadLen)
            return Result;
        Buffer = new char[FBufSize];
        do
        {
            if (ReadLen >= 0)
            {
                ToRead = FBufSize;
                if (ReadLen < ToRead)
                    ToRead = static_cast<unsigned>(ReadLen);
            }
            SizeR = FileRead(InFile, Buffer, ToRead);
            if ((SizeR < 0) || (FileWrite(OutFile, Buffer, SizeR) != SizeR))
            {
                Result = SE_CopyError;
                break;
            }
            if (FShowProgress != zspNone)
                Report(zacProgress, 0, "", SizeR);
            else
                Report(zacTick, 0, "", 0);
            if (ReadLen > 0)
                ReadLen -= static_cast<unsigned>(SizeR);
        }
        while (static_cast<unsigned>(SizeR) == ToRead && ReadLen);
    }
    __finally
    {
        delete[] Buffer;
    }
    return Result;
}
// TZBWorker::CopyBuffer


/*? TZBWorker::CopyZippedFiles-------------------------------------------------
1.79 4 December 2006 RP allow to create new dest
1.79 2 December 2006 RP clean all Flag first
1.79 23 Dec 2004 RA added use test if destination is writable
1.76 9 June 2004 RA properly assign not done files to FSpecArgs
1.75 18 February 2004 allow >2G
1.74 14 January 2004 RP allow create destination file
1.73 1 Oct 2003 RA corrected slashes in central dir
1.73 8 August 2003 RA close InFileHandle after error, set to -1 in non-error case
1.73 (31 July 2003) RA close InFileHandle for Spanerror
1.73 (24 July 2003) RA init OutFileHandle
1.73 (12 July 2003) RP string extra data
1.73 (2 June 2003) RA Use of ConvertOEM
Function to copy one or more zipped files from the zip archive to another zip archive
FSpecArgs in source is used to hold the filename(s) to be copied.
When this function is ready FSpecArgs contains the file(s) that where not copied.
Return values:
 0            All Ok.
 -6           CopyZippedFiles Busy
 -7           CopyZippedFiles errors. See ZipMsgXX.rc
 -8           Memory allocation error.
 -9           General unknown CopyZippedFiles error.
*/
int __fastcall TZBWorker::CopyZippedFiles(TZBWorker *DesTZBWorker, bool DeleteFromSource,
        OvrOpts OverwriteDest)
{
    ZipEndOfCentral  EOC;
    ZipCentralHeader CEH;
    ZipDirEntry *zde, *pzd;
    String OutFilePath;
    char* Buffer = NULL;
    int  Result = 0;
    int  In2FileHandle = -1; //to avoid external exception in case of error
    TStringList* NotCopiedFiles = NULL;
    bool Found;
    int  DestMemCount;
    TMZipDataList* MDZD = NULL;
    StartWaitCursor();
    FZipBusy = true;
    FShowProgress = zspNone;
    FOutFileHandle = -1;
    FDateStamp = 0;
    try
    {
        // Are source and destination different?
        if (DesTZBWorker == this || !AnsiStrIComp(ZipFileName.T_STR(),
                DesTZBWorker->ZipFileName.T_STR()))
            throw EZipBuilder(CF_SourceIsDest);
        // test if destination is writable
        DesTZBWorker->CheckDestWritable(DesTZBWorker->ZipFileName, false);
        //testing for diskspan

        // The following function a.o. opens the input file no. 1.
        OpenEOC(EOC, false);
        if (DesTZBWorker->IsSpanned || FIsSpanned)
            throw EZipBuilder(CF_NoCopyOnSpan, true);

        Buffer = new char[FBufSize];

        // Now check for every source file if it is in the destination archive and determine what to do.
        // we use the three most significant bits from the Flag field from ZipDirEntry to specify the action
        // None           = 000xxxxx, Destination no change. Action: Copy old Dest to New Dest
        // Add            = 001xxxxx (New).                  Action: Copy Source to New Dest
        // Overwrite      = 010xxxxx (OvrAlways)             Action: Copy Source to New Dest
        // AskToOverwrite = 011xxxxx (OvrConfirm) Action to perform: Overwrite or NeverOverwrite
        // NeverOverwrite = 100xxxxx (OvrNever)      Action: Copy old Dest to New Dest
        // Clean Flag first
        for (int d = 0; d < DesTZBWorker->Count; ++d)
        {
            zde = static_cast<ZipDirEntry*>(DesTZBWorker->ZipContents->Items[d]);
            zde->Flag &= 0x1FFFui16;
        }
        for (int s = 0; s < FSpecArgs->Count; s++)
        {
            String FSpec = FSpecArgs->Strings[s];
            RemoveDriveSpec(FSpec); //allow drive name in FSpecArg
            Found = false;
            for (int d = 0; d < DesTZBWorker->Count; ++d)
            {
                zde = static_cast<ZipDirEntry*>(DesTZBWorker->ZipContents->Items[d]);
                if (!AnsiStrIComp(FSpec.T_STR(), zde->FileName.T_STR()))
                {
                    Found = true;
                    Word k;
                    switch (OverwriteDest)
                    {
                    case OvrAlways:
                        k = 0x4000ui16;
                        break;
                    case OvrNever:
                        k = 0x8000ui16;
                        break;
                    case OvrConfirm:
                        k = 0x6000ui16;
                        break;
                    }
                    zde->Flag = (zde->Flag & 0x1FFFui16) | k; // Clear the three upper bits.
                    break;
                }
            }
            if (!Found)
            { // Add the Filename to the list and set flag
                zde = new ZipDirEntry;
                DesTZBWorker->FZipContents->Add(zde);
                FDirHashs->Add(zde); // ignore duplicates
                zde->FileName       = FSpec;
                zde->FileName.Unique();
                zde->FileNameLength = (Word)FSpecArgs->Strings[s].Length();
                zde->Flag = (zde->Flag & 0x1FFFui16) | 0x2000ui16;  // (a new entry)
                zde->ExtraData = NULL; //Needed when deleting zde
            }
        }
        // Make a temporary filename like: C:\...\zipxxxx.zip for the new destination
        if ((OutFilePath = MakeTempFileName("", "")).IsEmpty())
            throw EZipBuilder(DS_NoTempFile);

        // Create the output file.
        FOutFileHandle = FileCreate(OutFilePath);
        if (FOutFileHandle == -1)
            throw EZipBuilder(DS_NoOutFile);

        // Open the second input archive, i.e. the original destination.
        if ((In2FileHandle = FileOpen(DesTZBWorker->ZipFileName, fmShareDenyWrite | fmOpenRead)) > -1)
        {
            // Get the date-time stamp and save for later.
            FDateStamp = FileGetDate(In2FileHandle);

            // Write the SFX header if present.
            if (CopyBuffer(In2FileHandle, FOutFileHandle, DesTZBWorker->SFXOffset))
                throw EZipBuilder(CF_SFXCopyError);
        }
        NotCopiedFiles = new TStringList();
        // Now walk trough the destination, copying and replacing
        DestMemCount = DesTZBWorker->FZipContents->Count;
        MDZD = new TMZipDataList(DestMemCount);  // create class

        // Copy the local data and save central header info for later use.
        for (int d = 0; d < DestMemCount; d++)
        {
            zde = (ZipDirEntry *)DesTZBWorker->ZipContents->Items[d];
            if ((zde->Flag & 0xE000ui16) == 0x6000ui16)
            { // Ask first if we may overwrite.
                bool Overwrite = false;
                // Do we have a event assigned for this then don't ask.
                TCopyZipOverwriteEvent tmpCopyZipOverwrite = FEvents.FOnCopyZipOverwrite;
                if (tmpCopyZipOverwrite)
                    tmpCopyZipOverwrite(DesTZBWorker, zde->FileName, Overwrite);
                else
                    Overwrite = (ZipMessageDlg("",
                                               Format(ZipLoadStr(CF_OverwriteYN),
                                                      ARRAYOFCONST((zde->FileName, DesTZBWorker->ZipFileName))),
                                               zmtConfirmation + DHC_CpyZipOvr,
                                               TMsgDlgButtons() << mbYes << mbNo) == IDYES);
                zde->Flag &= 0x1FFFui16; // Clear the three upper bits.
                zde->Flag |= (Overwrite) ? 0x4000ui16 : 0x8000ui16;
            }
            // Change info for later while writing the central dir in new Dest.
            MDZD->Items[d]->RelOffLocal = FileSeek(FOutFileHandle, 0, 1);

            if ((zde->Flag & 0x6000ui16) == 0x0000ui16)
            { // Copy from original dest to new dest.
                // Set the file pointer to the start of the local header.
                FileSeek(In2FileHandle, static_cast<__int64>(zde->RelOffLocalHdr), 0);
                if (CopyBuffer(In2FileHandle, FOutFileHandle, sizeof(ZipLocalHeader) +
                               zde->FileNameLength +
                               zde->ExtraFieldLength +
                               zde->CompressedSize))
                    throw EZipBuilder(CF_CopyFailed, DesTZBWorker->ZipFileName, DesTZBWorker->ZipFileName);
                if (zde->Flag & 0x8000ui16)
                {
                    NotCopiedFiles->Add(zde->FileName);
                    // Delete also from FSpecArgs, should not be deleted from source later.
                    FSpecArgs->Delete(FSpecArgs->IndexOf(zde->FileName));
                }
            }
            else
            { // Copy from source to new dest.
                // Find the filename in the source archive and position the file pointer.
                for (int s = 0; s < Count; s++)
                {
                    pzd = (ZipDirEntry *)ZipContents->Items[s];
                    if (!AnsiStrIComp(pzd->FileName.T_STR(), zde->FileName.T_STR()))
                    {
                        FileSeek(FInFileHandle, static_cast<__int64>(pzd->RelOffLocalHdr), 0);
                        if (CopyBuffer(FInFileHandle, FOutFileHandle, sizeof(ZipLocalHeader) +
                                       pzd->FileNameLength +
                                       pzd->ExtraFieldLength +
                                       pzd->CompressedSize))
                            throw EZipBuilder(CF_CopyFailed, ZipFileName, DesTZBWorker->ZipFileName);
                        break;
                    }
                }
            }
            // Save the file name info in the MDZD structure.
             MDZD->Items[d]->FileName = zde->FileName;
        } // Now we have written al entries.

        // Now write the central directory with possibly changed offsets.
        // Remember the EOC we are going to use is from the wrong input file!
        EOC.CentralSize = 0;
        for (int d = 0; d < DestMemCount; d++)
        {
            zde = (ZipDirEntry *)DesTZBWorker->ZipContents->Items[d];
            Found = false;
            // Rebuild the CEH structure.
            if ((zde->Flag & 0x6000ui16) == 0ui16)
            { // Copy from original dest to new dest.
                pzd = (ZipDirEntry *)DesTZBWorker->ZipContents->Items[d];
                Found = true;
            }
            else
            { // Copy from source to new dest.
                // Find the filename in the source archive and position the file pointer.
                for (int s = 0; s < Count; s++)
                {
                    pzd = (ZipDirEntry *)ZipContents->Items[s];
                    if (!AnsiStrIComp(pzd->FileName.T_STR(), zde->FileName.T_STR()))
                    {
                        Found = true;
                        break;
                    }
                }
            }
            if (!Found)
                throw EZipBuilder(CF_SourceNotFound, zde->FileName, ZipFileName);
            memcpy(&CEH.VersionMadeBy0, pzd, sizeof(ZipCentralHeader) - 4);
            CEH.HeaderSig = CentralFileHeaderSig;
            CEH.Flag &= 0x1FFFui16;
            CEH.RelOffLocal = static_cast<unsigned long>(MDZD->Items[d]->RelOffLocal);
            // Save the first Central directory offset for use in EOC record.
            if (!d)
                EOC.CentralOffset = FileSeek(FOutFileHandle, 0, 1);
            EOC.CentralSize += (sizeof(CEH) + CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen);

            // Write this changed central header to disk
            WriteJoin(&CEH, sizeof(CEH), DS_CEHBadWrite);
            //if filename was converted OEM2ISO then we have to reconvert before copying
            FHostNum = CEH.VersionMadeBy1;
            FHostVer = CEH.VersionMadeBy0;
						MDZD->Items[d]->FileName = SetSlash(ConvertOEM(MDZD->Items[d]->FileName, cpdISO2OEM), psdInternal);
            // Write to destination the central filename.
            WriteJoin(MDZD->Items[d]->FileNameA.c_str(), CEH.FileNameLen, DS_CEHBadWrite);

            // And the extra field from zde or pzd.
            if (CEH.ExtraLen)
                WriteJoin(pzd->ExtraData.c_str(), CEH.ExtraLen, DS_CEExtraLen);

            // And the file comment.
            if (CEH.FileComLen)
            {
                AnsiString StrA = AnsiString(pzd->FileComment);
                WriteJoin(StrA.c_str(), CEH.FileComLen, DS_CECommentLen);
            }
        }
        EOC.TotalEntries = EOC.CentralEntries = static_cast<unsigned short>(DestMemCount);
        EOC.ZipCommentLen = (In2FileHandle > -1) ?
                            static_cast<unsigned short>(DesTZBWorker->ZipComment.Length()) : 0i16;

        // Write the changed EndOfCentral directory record.
        WriteJoin(&EOC, sizeof(EOC), DS_EOCBadWrite);
        if (In2FileHandle > -1)
        {
            // And finally the archive comment
            FileSeek(In2FileHandle, static_cast<__int64>(DesTZBWorker->ZipEOC + sizeof(EOC)), 0);
            if (CopyBuffer(In2FileHandle, FOutFileHandle, DesTZBWorker->ZipComment.Length()))
                throw EZipBuilder(DS_EOArchComLen);
        }
        CloseFileHandle(FInFileHandle);
        // Now delete all copied files from the source when deletion is wanted.
        if (DeleteFromSource && FSpecArgs->Count > 0)
        {
            FZipBusy = false;
            Delete(); // Delete files specified in FSpecArgs and update the contents.
            FInFileHandle = -1; // closed by Delete and List
        }
        FSpecArgs->Assign(NotCopiedFiles); // Info for the caller.
    }
    catch (EZipBuilder &ers)
    { // All CopyZippedFiles specific errors..
        ShowExceptionError(ers);
        Result = -7;
    }
    catch (const MEMEXCEPT &me)
    {  // All memory allocation errors.
        ShowZipMessage(GE_NoMem, "");
        Result = -8;
    }
    catch (const Exception &E)
    {
        ShowZipMessage(DS_ErrorUnknown, "\n" + E.Message);
        Result = -9;
    }
    catch (...)
    {        // The remaining errors, should not occur.
        ShowZipMessage(DS_ErrorUnknown, "");
        Result = -9;
    }
    if (MDZD)
        delete MDZD;
    delete NotCopiedFiles;
    delete[] Buffer;
    CloseFileHandle(FInFileHandle);
    CloseFileHandle(In2FileHandle);
    if (FOutFileHandle != -1)
    {
        if (FDateStamp)
            FileSetDate(FOutFileHandle, FDateStamp);
        FileClose(FOutFileHandle);
        if (Result)
            DeleteFile(OutFilePath);  // An error somewhere, OutFile is not reliable.
        else
        {
            EraseFile(DesTZBWorker->FZipFileName, DesTZBWorker->HowToDelete);
            if (!RenameFile(OutFilePath, DesTZBWorker->FZipFileName))
                EraseFile(OutFilePath, DesTZBWorker->HowToDelete);
        }
    }
    DesTZBWorker->List(); // Update the old(possibly some entries were added temporarily) or new destination.
    StopWaitCursor();
    FZipBusy = false;
    return Result;
}
// TZBWorker::CopyZippedFiles

/*? TZBWorker::Copy_file-------------------------------------------------------
1.79 10 December 2004 RP trap dest = source
1.75 18 February 2004 Allow >2G
 Returns 0 if good copy, or a negative error code.
*/
int __fastcall TZBWorker::Copy_File(const String InFileName, const String OutFileName)
{
    const
    SE_CreateError   = -1, // Error in open or creation of OutFile.
    SE_OpenReadError = -3, // Error in open or Seek of InFile.
    SE_SetDateError  = -4, // Error setting date/time of OutFile.
    SE_GeneralError  = -9;
    int InFile, OutFile, Result = SE_OpenReadError;
    __int64 InSize = -1, OutSize = -1;
    FShowProgress = zspNone;
    if (!FileExists(InFileName))
        return Result;

    if ((InFile = FileOpen(InFileName, fmOpenRead | fmShareDenyWrite)) != -1)
    {
        if (FileExists(OutFileName))
        {
            if ((OutFile = FileOpen(OutFileName, fmOpenWrite | fmShareExclusive)) == -1)
            {
                CloseFileHandle(InFile);
                return SE_CreateError;  // might be read-only or source
            }
            CloseFileHandle(OutFile);
            EraseFile(OutFileName, htdFinal);
        }
        StartWaitCursor();
        if ((OutFile = FileCreate(OutFileName)) != -1)
        {
            Result = CopyBuffer(InFile, OutFile, -1);
            if (!Result && FileSetDate(OutFile, FileGetDate(InFile)) != 0)
                Result = SE_SetDateError;
            OutSize = FileSeek(OutFile, 0i64, 2);
            CloseFileHandle(OutFile);
        }
        else
            Result = SE_CreateError;
        InSize = FileSeek(InFile, 0i64, 2);
        CloseFileHandle(InFile);
    }
    // An extra check if the filesizes are the same.
    if (!Result && (InSize == -1 || OutSize == -1 || InSize != OutSize))
        Result = SE_GeneralError;
    // Don't leave a corrupted outfile lying around. (SetDateError is not fatal!)
    if (Result && Result != SE_SetDateError)
        DeleteFile(OutFileName);
    StopWaitCursor();
    return Result;
}

#ifndef NO_SPAN
/*? TZBWorker::CreatMVFileName---------------------------------------------------
1.79 12 Dec 2004 RP added parameter
1.77 19 August 2004 RA Allow creation of >99 parts with compatnames
*/
void __fastcall TZBWorker::CreateMVFileName(String& FileName, bool StripPartNbr,
        bool Compat)
{  // changes filename into multi volume filename
    if (Compat)
    {
        String ext = ".zip";
        if (FDiskNr != FTotalDisks)
        {
            if (FDiskNr < 9)
                ext = ".z0";
            else
                ext = ".z";
            ext += IntToStr(FDiskNr + 1);
        }
        FileName = ChangeFileExt(FileName, ext);
    }
    else
    {
        int StripLen = StripPartNbr ? 3 : 0;
        FileName = FileName.SubString(1, FileName.Length() - ExtractFileExt(FileName).Length() - StripLen)
                   + IntToStr(1001 + FDiskNr).SubString(2, 3)
                   + ExtractFileExt(FileName);
    }
}
#endif

/*? TZBWorker::Delete---------------------------------------------------------
1.79 13 March 2009 RP clear FullErrCode
1.79 25 August 2006 RP do not delete orig file on failure
1.79 12 June 2005 RA changed error handling using exceptions.
1.79 23 Dec 2004 RA added use test if destination is writable
1.79 20 Dec 2004 RA changed exception handling during opening + corected if nothing to do
1.79 8 Dec 2004 RA Allow delete in MV archive
1.73 6 September 2003 RA throw exception if dll returns negative value
1.73 2 August 2003 RA removed loading of dll will be done in Exec
1.73 (16 July 2003) RA catch and display dll load errors
1.73 (13 July 2003) RA for spanned archive no exception but show mesage
*/
int __fastcall TZBWorker::Delete(void)
{
    ZipEndOfCentral  EOC;
    FSuccessCnt = FErrCode = FFullErrCode = 0;
    FMessage = "";
    String TmpZipName;
    PZipParms ZipRecP = NULL;
    // Make sure we can't get back in here while work is going on
    if (FZipBusy)
        return FErrCode;
    try
    {
        if (!FFSpecArgs->Count)
            throw EZipBuilder(DL_NothingToDel);
#ifndef NO_SPAN
        if (FileExists(FZipFileName) && QueryZip(FZipFileName) == ZB_EXE_DET_HEAD)
#ifdef NO_SFX
            throw EZipBuilder(SF_NoSFXSupport);
#else
            FInFileName = ChangeFileExt(FZipFileName, ".zip");
#endif
        else
            FInFileName = FZipFileName;
        // Locate last of multi volume or last disk of spanned
        if (GetLastVolume(FInFileName, EOC, false) == 1)
            throw EZipAbort();
#else
        if (!FileExists(FZipFileName))
            throw EZipBuilder(GE_NoZipSpecified);
        OpenEOC(EOC, false);
        if (FIsSpanned)  //function not allowed in spanned archive
            throw EZipBuilder(DL_NoDelOnSpan);
#endif
        if (FInFileHandle == -1)
            return FErrCode;    // was problem
        CloseFileHandle(FInFileHandle);
        CheckDestWritable(FInFileName, false);
        DoDllLoad();
    }
    catch (EZipBuilder &ews)
    {
        ShowExceptionError(ews);
        return FErrCode;
    }
    FZipBusy = true;     // delete uses the ZIPDLL, so it shares the FZipBusy flag
    Cancel = false;

    if (!FIsSpanned)
        TmpZipName = FZipFileName; //not spanned create the outfile directly
    else
        if (RejoinMVArchive(TmpZipName, true))
        {
            FZipBusy = false;
            Cancel = false;
            DoDllUnload();
            return FErrCode;
        }
    try
    {
        try
        {
            ZipRecP = new ZipParms;
            memset(ZipRecP, 0, sizeof(*ZipRecP));
            SetZipSwitches(TmpZipName, ZipRecP);
            // Override "Add" behavior assumed by SetZipSwitches.
            ZipRecP->fDeleteEntries = true;
            ZipRecP->fGrow = false;
            ZipRecP->fJunkDir = false;
            ZipRecP->fMove = false;
            ZipRecP->fFreshen = false;
            ZipRecP->fUpdate = false;
            ZipRecP->fRecurse = false;  // Bug fix per Angus Johnson.
            ZipRecP->fEncrypt = false;   // You don't need the password to delete a file.

            ZipRecP->fFDS = new FileData[FFSpecArgs->Count];
            for (int i = 0; i < FFSpecArgs->Count; i++)
            {
                memset(ZipRecP->fFDS + i, 0, sizeof(FileData));
                ZipRecP->fFDS[i].fFileSpec = MakeStrP(FFSpecArgs->Strings[i]);
            }
            ZipRecP->fSeven = 7; // used to QC the data structure passed to DLL
            // argc is now the no. of filespecs we want deleted.
            ZipRecP->fArgc = FFSpecArgs->Count;
            FEventErr = "";
            FSuccessCnt = DoZipExec(ZipRecP);
            if (FIsSpanned)
            {
                if (FSuccessCnt)
                    RecreateMVArchive(TmpZipName, true);
                else
                    DeleteFile(TmpZipName);
            }
        }
        catch (EZipBuilder &ers)
        {
            if (FEventErr != "")
                ers.Message += FEventErr;
            ShowExceptionError(ers);
        }
        catch (...)
        {
            ShowZipMessage(GE_FatalZip, "");
        }
    }
    __finally
    {
        FFSpecArgs->Clear();
        FFSpecArgsExcl->Clear();
        StrDispose(ZipRecP->pSuffix);
        StrDispose(ZipRecP->pZipFN);
        StrDispose(ZipRecP->pZipPassword);
        StrDispose(ZipRecP->fTempPath);
        StrDispose(ZipRecP->fArchComment);
        for (int i = ZipRecP->fArgc - 1; i >= 0; i--)
            StrDispose(ZipRecP->fFDS[i].fFileSpec);
        delete[] ZipRecP->fFDS;
        for (int i = ZipRecP->fTotExFileSpecs - 1; i >= 0; i--)
            StrDispose(ZipRecP->fExFiles[i].fFileSpec);
        delete[] ZipRecP->fExFiles;
        delete ZipRecP;
        DoDllUnload();
        FZipBusy = false;
        Cancel = false;
        if ((FSuccessCnt > 0) && (QueryZip(FZipFileName) != ZB_EXE_DET_HEAD))
            List();  // Update the Zip Directory by calling List() function.
    }               // not needed for spanned exe
    return FErrCode;
}

//---------------------------------------------------------------------------
// Replacement for the functions DiskFree and DiskSize.
// This should solve problems with drives > 2Gb and UNC filenames.
// Path FDrive ends with a backslash.
// Action=1 FreeOnDisk, 2=SizeOfDisk, 3=Both
void __fastcall TZBWorker::DiskFreeAndSize(int Action)
{
    DWORD     SectorsPCluster, BytesPSector, FreeClusters, TotalClusters;
    __int64    LDiskFree = -1, LSizeOfDisk = -1;

#if __BORLANDC__ >= 0x0600
    if (!::GetDiskFreeSpaceEx(FDrive.T_STR(), (_ULARGE_INTEGER*)&LDiskFree, (_ULARGE_INTEGER*)&LSizeOfDisk, NULL))
        LDiskFree = LSizeOfDisk = -1;
#else
    if (HMODULE Lib = ::GetModuleHandle(TEXT("kernel32")))
    {
        GDiskFreeSpaceEx GetDiskFreeSpaceEx;
        if ((GetDiskFreeSpaceEx = (GDiskFreeSpaceEx)::GetProcAddress(Lib, "GetDiskFreeSpaceExA")) != 0)
            // We probably have W95+OSR2 or better.
            if (!GetDiskFreeSpaceEx(FDrive.c_str(), &LDiskFree, &LSizeOfDisk, NULL))
                LDiskFree = LSizeOfDisk = -1;
        ::FreeLibrary(Lib);
    }
#endif
    // Do we have W95 original or W95+OSR1 or an error?
    if (LDiskFree == -1)
    { // We use this because DiskFree/Size don't support UNC drive names.
        if (GetDiskFreeSpace(FDrive.T_STR(), &SectorsPCluster, &BytesPSector, &FreeClusters, &TotalClusters))
        {
            LDiskFree   = static_cast<__int64>(BytesPSector * SectorsPCluster * FreeClusters);
            LSizeOfDisk = static_cast<__int64>(BytesPSector * SectorsPCluster * TotalClusters);
        }
    }
    if (Action & 1)
        FFreeOnDisk = LDiskFree;
    if (Action & 2)
        FSizeOfDisk = LSizeOfDisk;
}

/*? TZBWorker::DllCallBack---------------------------------------------------------
1.77 4 July 2004 changed callbackStruct + bytes written
1.76 25 April 2004 changes to GetXXXPasswordd
1.76 24 April 2004
*/
void __fastcall TZBWorker::DllCallBack(PZCallBackStruct cbr)
{
    if (fIsDestructing)
        return;
    String CMsg;
    unsigned XIdent = cbr->ActionCode;
    ActionCodes ActionCode = static_cast<ActionCodes>(XIdent & 63);

    /*ActionCode = 18, zacKey
     *  set or clear operation key
     *   (O) Arg1 = key     */
    if (ActionCode == zacKey)
    {
        FDllOperKey = cbr->Arg1;
        return;
    }
    try
    {
        __int64 FileSize;
        int ErrorCode = 0;
        bool IsZip = cbr->IsOperationZip;
        if ((ActionCode <= zacSize) || (ActionCode == zacXItem) || (ActionCode == zacXProgress))
        {
            FileSize = 0;
            String m;
            if (ActionCode != zacTick)
                switch (ActionCode)
                {
                case zacSize:
                    FileSize = cbr->FileSize;
                    break;
                case zacItem:
                case zacProgress:
                case zacEndOfBatch:
                    FileSize = cbr->FileSize;
                    if (ActionCode == zacItem)
                        m = ExtMsg(CMsg, cbr);
                    if (IsZip)
                        static_cast<TProgDetails*>(FProgDetails)->Written(cbr->Written);
                    break;
                case zacCount:
                    FileSize = cbr->Arg1;
                    break;
                case zacMessage:
                    m = ExtMsg(CMsg, cbr);
                    ErrorCode = cbr->Arg1;
                    break;
                case zacXItem:
                    FileSize = cbr->FileSize;
                    m = ExtMsg(CMsg, cbr);
                    ErrorCode = cbr->Arg1;
                    break;
                case zacXProgress:
                    FileSize = cbr->FileSize;
                    ErrorCode = cbr->Arg1;
                    break;
                }
            Report(ActionCode, ErrorCode, m, FileSize);
            return;
        }
        switch (ActionCode)
        {
            /*   ActionCode = 7, zacNewName,
             *  the internal filename is being passed.
             *   (O) Msg2P = pointer to full filespec
             *   (O) MsgP = the internal filename as the dll thinks it should be.
             *   (I) ActionCode = -1 if changed
             *   (I) Msg = the new internal filename       */
        case zacNewName:
        {
            // request for a new path+name just before zipping or extracting
            TSetNewNameEvent tmpSetNewName = FEvents.FOnSetNewName;
            TSetAddNameEvent tmpSetAddName = FEvents.FOnSetAddName;
            if (tmpSetAddName || tmpSetNewName)
            {
                String m = ExtMsg(CMsg, cbr); // saves OldFileName
                char* tp = const_cast<char*>(cbr->MsgP2);
                if (tmpSetNewName)
                {
                    String OldFileName = m;
                    bool IsChanged = false;
                    tmpSetNewName(Builder, OldFileName, IsChanged);
                    if (IsChanged)
                    {
                        StrPLCopy(cbr->Msg, OldFileName, 512);
                        cbr->ActionCode = -1;
                    }
                }
                if (tp && tmpSetAddName)
                {
                    String OrigName = SetSlash(String(tp), psdExternal);
                    String OldFileName = m;
                    bool IsChanged = false;
                    tmpSetAddName(Builder, OldFileName, OrigName, IsChanged);
                    if (IsChanged)
                    {
                        StrPLCopy(cbr->Msg, OldFileName, 512);
                        cbr->ActionCode = -1;
                    }
                }
            }
        }
        break;
        /*   ActionCode = 8, zacPassword,
         *  get Password
         *   (O) Arg1 = request count
         *   (O) MsgP = filename
         *   (I) ActionCode = -1 if has password
         *       (I) Msg = password
         *       (I) Arg1 = request count
         *   (I) ActionCode = -2 if responded 'no'
         *       (I) Arg1 = request count       */
        case zacPassword:
        { // New or other password needed during Extract() or Add().
            String  pwd;
            unsigned RptCount = cbr->Arg1;
            TPasswordButton Response = mbOK;
            TPasswordErrorEvent tmpPasswordError = FEvents.FOnPasswordError;
            if (tmpPasswordError)
            {
                tmpPasswordError(Builder, IsZip, pwd,
                                 ExtMsg(CMsg, cbr), RptCount, Response);
                if (Response != mbOK)
                    pwd = "";
            }
            else
                pwd = IsZip ? GetAddPassword(Response) : GetExtrPassword(Response);
            if (!pwd.IsEmpty())
            {
                StrPLCopy(cbr->Msg, String(pwd), PWLEN);
                cbr->ActionCode = -1;
            }
            else
            {
                RptCount = 0;
                cbr->ActionCode = -2;
            }
            cbr->Arg1 = min(RptCount, 15U);
            if (Response == mbCancel)
                cbr->ActionCode = -2; // Cancel
            if (Response == mbNoToAll)
                cbr->ActionCode = -3; // Cancel all
            if (Response == mbAbort)
                Cancel = true;  // Abort
        }
        break;
        /*   Actioncode = 9, zacCRCError,
         *  CRC32 error during Extract
         *   (O) Arg1 = Found CRC
         *   (O) Arg2 = Stored/Expected CRC
         *   (O) MsgP = File for which the CRC check went wrong.
         *   (I) ActionCode -1 = delete the file
         *         -2 = warn only          */
        case zacCRCError:
        { // CRC32 error, (default action is extract/test the file).
            bool DoExtract = true;
            TCRC32ErrorEvent tmpCRC32Error = FEvents.FOnCRC32Error;
            if (tmpCRC32Error)
            {
                tmpCRC32Error(Builder, ExtMsg(CMsg, cbr), cbr->Arg1,
                              cbr->Arg2, DoExtract);
                cbr->ActionCode = DoExtract ? -1 : -3;
            }
        }
        break;
        /*   Actioncode = 10, zacOverwrite,
         *  Extract(UnZip) Overwrite ask.
         *   (O) Arg3 = 'older'
         *   (O) Arg2 = Index
         *   (O) Arg1 = Overwrite_All
         *   (O) MsgP = filename
         *   (I) ActionCode -1 = overwrite
         *      -2 = don't overwrite                */
        case zacOverwrite:
        {
            TExtractOverwriteEvent tmpExtractOverwrite = FEvents.FOnExtractOverwrite;
            if (tmpExtractOverwrite)
            {
                bool DoOverwrite = (cbr->Arg1 != 0);
                tmpExtractOverwrite(Builder, ExtMsg(CMsg, cbr),
                                    cbr->Arg3 != 2,
                                    DoOverwrite, cbr->Arg2);
                cbr->ActionCode = DoOverwrite ? -1 : -2;
            }
        }
        break;
        /*   Actioncode = 11, zacSkipped,
         *   Skipped during Extract
         *   (O) Arg1 = error code
         *   (O) Arg2 = type
         *   (O) MsgP = filename     */
        case zacSkipped:
        {
            int ErrorCode = cbr->Arg1;
            int type = cbr->Arg2;
            if (ErrorCode)
            {
                FFullErrCode = ErrorCode;
                ErrorCode = static_cast<int>(ErrorCode & 0xFF);
            }
            TExtractSkippedEvent tmpExtractSkipped = FEvents.FOnExtractSkipped;
            if (tmpExtractSkipped)
                tmpExtractSkipped(Builder, ExtMsg(CMsg, cbr),
                                  static_cast<UnZipSkipTypes>((type & 255) - 1), ErrorCode);
            else
            {
                TMsgEvent tmpMessage = FEvents.FOnMessage;
                if (tmpMessage)
                    tmpMessage(Builder, GE_Unknown, ZipFmtLoadStr(GE_Skipped,
                               ARRAYOFCONST((ExtMsg(CMsg, cbr),
                                             (type & 0xFF) - 1))));
            }
        }
        break;
        /*   Actioncode = 12, zacComment,
         *  FileComment while adding ( ZipDll only )
         *   (O) MsgP = filename
         *   (O) MsgP2 = old comment
         *   (I) ActionCode = -1 if comment changed
         *       (I) Arg1 = length of new comment
         *       (I) Msg = new comment              */
        case zacComment:
        {
            TFileCommentEvent tmpFileComment = FEvents.FOnFileComment;
            if (tmpFileComment)
            {
                String FileComment = String(cbr->MsgP2);
                bool IsChanged = false;
                tmpFileComment(Builder, ExtMsg(CMsg, cbr), FileComment, IsChanged);
                if (IsChanged)
                {
                 	AnsiString tmp = FileComment;
                  int ti = tmp.Length();
                  if(ti > 511) ti = 511;
                    if (!FileComment.IsEmpty())
                        StrPLCopy(cbr->Msg, String(FileComment), ti);
                    else
                        cbr->Msg[0] = '\0';
                    cbr->ActionCode = -1;
                    cbr->Arg1 = ti;
                }
            }
        }
        break;
        /*   Actioncode = 13, zacStream,
         *  Adjust unzip stream size
         *   (O) FileSize (u64) = required size
         *   (I) ActionCode = -1 if ok
         *       (I)  = Memory                   */
#ifndef NO_STREAM
        case zacStream:
            try
            {
                FZipStream->Size =  static_cast<STRMSIZ>(cbr->FileSize);
                cbr->MsgP2 = reinterpret_cast<char*>(FZipStream->Memory);
                cbr->ActionCode = -1; // ok
            }
            catch (...)
            {
                cbr->ActionCode = -2;
                cbr->FileSize = NULL;
                break;
            }
            break;
#endif
            /*    Actioncode = 14, zacData,
             *  Set Extra Data
             *   (O) Arg1 = size of data
             *   (O) MsgP = filename
             *   (O) MsgP2 = pointer to data
             *   (I) ActionCode = -1 if changed
             *       (I) Arg1 = new size of data
             *       (I) Msg = new data (< 512 bytes)
             *       (I) MsgP2 = new data (> 512 bytes) (data must remain until next callback) */
        case zacData:
        {
            TFileExtraEvent tmpFileExtra = FEvents.FOnFileExtra;
            if (tmpFileExtra)
            {
                int FileSize = cbr->Arg1; // old size
                FStoredExtraData.SetLength(FileSize);
                if (FileSize)
                    memcpy(FStoredExtraData.c_str(), cbr->MsgP2, FileSize);
                bool IsChanged = false;
                tmpFileExtra(Builder, ExtMsg(CMsg, cbr), FStoredExtraData,
                             IsChanged);
                if (IsChanged)
                {
                    unsigned xlen = FStoredExtraData.Length();
                    if (xlen && xlen < 512)
                        memcpy(cbr->Msg, FStoredExtraData.c_str(), xlen);
                    else
                        cbr->MsgP2 = FStoredExtraData.c_str();
                    cbr->Arg1 = xlen;
                    cbr->ActionCode = -1;
                }
            }
        }
        break;
        /*   ActionCode = 17, zacExtName,
         *  change extract name
         *   (O) MsgP2 = ? null
         *   (O) MsgP = filename
         *   (I) ActionCode = -1 if changed
         *      (I) Msg = new name              */
        case zacExtName:
        {
            TSetExtNameEvent tmpSetExtName = FEvents.FOnSetExtName;
            if (tmpSetExtName)
            {
                String OldFileName = ExtMsg(CMsg, cbr);
                bool IsChanged = false;
                tmpSetExtName(Builder, OldFileName, IsChanged);
                String Msg = ExtMsg(CMsg, cbr);
                if (IsChanged && (OldFileName != Msg) && (IsPathOnly(OldFileName) == IsPathOnly(Msg)))
                {
                    StrPLCopy(cbr->Msg, String(OldFileName), 512);
                    cbr->ActionCode = -1;
                }
            }
        }
        break;
        }
        Report(zacNone, 0, "", 0);            // process messages
    }
    catch (const Exception &ezl)
    {
        if (FEventErr.IsEmpty())
            FEventErr = " #" + IntToStr(ActionCode) + " \"" + ezl.Message + "\"";
        Cancel = true;
    }
}

/*? TZBWorker::Done---------------------------------------------------------
1.76 31 May 2004 New function
*/
void __fastcall TZBWorker::Done(void)
{
    inherited::Done();
}

/*? TZBWorker::ExtAdd---------------------------------------------------------
1.79 13 March 2009 RP clear FullErrCode
1.79 7 Feb 2007 RP allow unattended encrypt if event handled
1.79 17 May 2005 RA AddForceDest needs to be before IsWritable
1.79 23 Dec 2004 RA added use test if destination is writable
1.79 6 Dec 2004 RA allow freshen/update MV SFX
1.79 5 Dec 2004 RA - Load dll before joining mult-parts, stream to MV file
1.79 3 Dec 2004 RA - Allow freshen/update for multi volume archives
1.79 18 Nov 2004 RA - Use TZipBuilderSFX as internal sfx + allow spanned sfx
1.77 5 August 2004 RA parameter added in writespan
1.76 26 May 2004 RP use shielded FSpecArgs
1.76 13 May 2004 RP change RootDir support and allow Freshen/Update with no args
1.76 24 April 2004 RP change checking no files
1.76 17 April 2004 RP change exception reporting
1.75 16 March 2004 RA only forceDir on hard drive
1.73 7 September 2003 RA allow empty FSpecArgs for Update and Freshen
1.73 6 September 2003 RA throw exception if dll returns negative value
1.73 4 August 2003 RA bug fix for ' ' as password + Add Dir test moved inside try catch
1.73 2 August 2003 RA removed loading of dll will be done in Exec
1.73 (17 July 2003) RP reject '< ' as password ' '
1.73 (16 July 2003) RA load Dll in try except
1.73 (16 July 2003) RP trim filenames
1.73 (15 July 2003) RA remove '<' from filename
1.73 (14 June 2003) RP changed dll handling
UseStream = 0 ==> Add file to zip archive file.
UseStream = 1 ==> Add stream to zip archive file.
UseStream = 2 ==> Add stream to another (zipped) stream.
*/
#ifndef NO_STREAM
void __fastcall TZBWorker::ExtAdd(int UseStream, DWORD StrFileDate, DWORD StrFileAttr, TMemoryStream *MemStream)
#else
void __fastcall TZBWorker::ExtAdd(int UseStream, DWORD /*StrFileDate*/, DWORD /*StrFileAttr*/, TMemoryStream* /*MemStream*/)
#endif
{
    // Make sure we can't get back in here while work is going on.
    if (FZipBusy)
        return;
    String TmpZipName;
    FSuccessCnt = FErrCode = FFullErrCode = 0;
    FMessage = "";
    PZipParms ZipRecP = NULL;;
    try
    {
        if (!UseStream && !FFSpecArgs->Count)
        {
            if (!(FAddOptions.Contains(AddFreshen) || FAddOptions.Contains(AddUpdate)))
                throw EZipBuilder(AD_NothingToZip);
            FAddOptions = FAddOptions >> AddUpdate;
            FAddOptions = FAddOptions << AddFreshen; // convert update to freshen
            FFSpecArgs->Add("*.*");// do freshen all
        }
        if (FAddOptions.Contains(AddDiskSpanErase))
        {
            FAddOptions = FAddOptions << AddDiskSpan; // to make certain
#ifndef NO_SPAN
            FSpanOptions = FSpanOptions << spWipeFiles;
#endif
        }
#ifdef NO_SPAN
        if (FAddOptions.Contains(AddDiskSpan))
            throw EZipBuilder(DS_NoDiskSpan);
#endif
        /* We must allow a zipfile to be specified that doesn't already exist,
        * so don't check here for existence. */
        // We can not do an Unattended Add if we don't have a password.
        if (FUnattended && FAddOptions.Contains(AddEncrypt) && (FPassword.IsEmpty()) && !FEvents.FOnPasswordError)
            throw EZipBuilder(AD_UnattPassword);
        if (UseStream < 2)
        {
            // We must allow a zipfile to be specified that doesn't already exist,
            // so don't check here for existence.
            if (FZipFileName.IsEmpty()) // Make sure we have a zip filename.
                throw EZipBuilder(GE_NoZipSpecified);
            FDrive = ExtractFileDrive(ExpandUNCFileName(FZipFileName)) + "\\";
            GetDriveProps();
            // unattended span to removable disc not allowed
            if (FUnattended && !FDriveFixed && FAddOptions.Contains(AddDiskSpan))
                throw EZipBuilder(DS_NoUnattSpan);
            // make certain destination can exist
            if (FDriveFixed || !FAddOptions.Contains(AddDiskSpan))
            {
                if (FAddOptions.Contains(AddForceDest))
                    ForceDirectory(ExtractFilePath(FZipFileName));
                if (!DirExists(ExtractFilePath(FZipFileName)))
                    throw EZipBuilder(AD_NoDestDir, ARRAYOFCONST((ExtractFilePath(FZipFileName))));
            }
            // do not test if dest is removeable and AddDiskSpan - drive may be empty
            if (FDriveFixed || !FAddOptions.Contains(AddDiskSpan))
                CheckDestWritable(FZipFileName, false);
        }
        if (DoDllLoad() <= 0)
            return;
#ifndef NO_SFX
        if (UseStream < 2 && UpperCase(ExtractFileExt(FZipFileName)) == ".EXE" &&
                !FileExists(FZipFileName)
                && !FAddOptions.Contains(AddDiskSpan)) // do not create sfx yet
        {
            // This is the first 'Add' operation following creation of a new
            // .EXE archive.  We need to add the SFX code now, before we add
            // the files.
            int SFXResult;
            if ((SFXResult = FSFXSlave->CreateNewSFX(FZipFileName)) != 0)
                throw EZipBuilder(AD_AutoSFXWrong, SFXResult);
#else
        if (UseStream < 2 && (UpperCase(ExtractFileExt(FZipFileName)) == ".EXE" &&
                              (FAddOptions.Contains(AddDiskSpan) || QueryZip(FZipFileName) != ZB_EXE_STUB)))
        {
            throw EZipBuilder(SF_NoSFXSupport);
#endif
        }
        // If we are using disk spanning, first create a temporary file to do the Add.
        if (UseStream < 2 && FAddOptions.Contains(AddDiskSpan))
        {
            if (RejoinMVArchive(TmpZipName, FAddOptions.Contains(AddFreshen) ||
                                FAddOptions.Contains(AddUpdate) ||
                                (UseStream == 1)))
                return; // probably should give reason
        }
        else
            TmpZipName = FZipFileName; //not spanned create the outfile directly
    }
    catch (EZipBuilder &ews)
    {
        ShowExceptionError(ews);
        return;
    }
    catch (...)
    {
        return;
    }
    Cancel = false;
    FZipBusy = true;
    StartWaitCursor();
    try
    {
        try
        {
            ZipRecP = new ZipParms;
            memset(ZipRecP, 0, sizeof(*ZipRecP));
            SetZipSwitches(TmpZipName, ZipRecP);
#ifndef NO_STREAM
            if (UseStream == 1)
            {
                ZipRecP->fUseInStream  = true;
                ZipRecP->fInStream   = FZipStream->Memory;
                ZipRecP->fInStreamSize = static_cast<int>(FZipStream->Size);
                ZipRecP->fStrFileAttr  = StrFileAttr;
                ZipRecP->fStrFileDate  = StrFileDate;
            }
            if (UseStream == 2)
            {
                ZipRecP->fUseOutStream = true;
                ZipRecP->fOutStream  = FZipStream->Memory;
                ZipRecP->fOutStreamSize = static_cast<int>(MemStream->Size + 6);
                ZipRecP->fUseInStream  = true;
                ZipRecP->fInStream   = MemStream->Memory;
                ZipRecP->fInStreamSize = static_cast<int>(MemStream->Size);
            }
#endif
            if (!FRootDir.IsEmpty())
            {
                String rdir = ExpandFileName(FRootDir); // allow relative root
                ZipRecP->fGRootDir = MakeStrP(rdir);
            }
            int FDSCnt = FFSpecArgs->Count;
            if (!FDSCnt && (FAddOptions.Contains(AddFreshen) || FAddOptions.Contains(AddUpdate)))
                FDSCnt = 1;
            ZipRecP->fArgc = FDSCnt;
            ZipRecP->fFDS = new FileData[FDSCnt];
            memset(ZipRecP->fFDS, 0, FFSpecArgs->Count * sizeof(FileData));
            for (int i = 0; i < FDSCnt; i++)
            {
                String FDSSpec;
                if (!FFSpecArgs->Count)
                    FDSSpec = "";
                else
                    FDSSpec = FFSpecArgs->Strings[i];
                int len = FDSSpec.Length(), b, p = 1;
                // Regardless of what AddRecurseDirs is set to, a '>' will force recursion, and a '|' will stop recursion.
                ZipRecP->fFDS[i].fRecurse = ZipRecP->fRecurse;    // Set default
                if (FDSSpec[1] == '>')
                {
                    ZipRecP->fFDS[i].fRecurse = true;
                    p++;
                }
                if (FDSSpec[1] == '|')
                {
                    ZipRecP->fFDS[i].fRecurse = false;
                    p++;
                }
                // Further it is possible the specify a password after the FFileSpec, separated by a '<'
                // If there is no other text after the '<' then, an existing password, is temporarily canceled.
                ZipRecP->fFDS[i].fEncrypt = ZipRecP->fEncrypt;   // Set default
                if (ZipRecP->pZipPassword) // set default password if exists
                    ZipRecP->fFDS[i].fPassword = MakeStrP(String(ZipRecP->pZipPassword));
                if ((b = AnsiPos("<", FDSSpec)) != 0)
                { // Found...
                    ZipRecP->fFDS[i].fEncrypt = true;  // the new default, but...
                    delete[] ZipRecP->fFDS[i].fPassword; // delete default pasword first
                    ZipRecP->fFDS[i].fPassword = NULL;
                    String tmp = FDSSpec.SubString(b + 1, 1);
                    if ((tmp.IsEmpty()) || (tmp == " "))
                    {
                        ZipRecP->fFDS[i].fEncrypt = false; // No password, so cancel for this FFspecArg
                        len -= tmp.Length() + 1;
                    }
                    else
                    {
                        ZipRecP->fFDS[i].fPassword = MakeStrP(FDSSpec.SubString(b + 1, len - b));
                        len = b - 1;
                    }
                }
                ZipRecP->fFDS[i].fRootDir = NULL;
                String tmp = FFSpecArgs->Strings[i].SubString(p, len - p + 1).Trim();
                if (!tmp.IsEmpty())
                    ZipRecP->fFDS[i].fFileSpec = MakeStrP(tmp);
                else
                    ZipRecP->fFDS[i].fFileSpec = NULL;
            }
            ZipRecP->fSeven = 7;
            FEventErr = "";
            FSuccessCnt = DoZipExec(ZipRecP);
            // If Add was successfull and we want spanning copy the temporary file to the destination.
            if (UseStream < 2 && (FAddOptions.Contains(AddDiskSpan) ||
                                  FAddOptions.Contains(AddDiskSpanErase)))
            {
                if (FSuccessCnt)
                    RecreateMVArchive(TmpZipName, FAddOptions.Contains(AddFreshen) ||
                                      FAddOptions.Contains(AddUpdate) ||
                                      (UseStream == 1));
                else
                    DeleteFile(TmpZipName);
            }

#ifndef NO_STREAM
            if (UseStream == 2 && FSuccessCnt == 1)
                FZipStream->Size = static_cast<STRMSIZ>(ZipRecP->fOutStreamSize);
#endif
        }
        catch (EZipBuilder &ers)
        {
            if (!FEventErr.IsEmpty())
                ers.Message += FEventErr;
            ShowExceptionError(ers);
        }
        catch (...)
        {
            ShowZipMessage(GE_FatalZip, "");
        }
    }
    __finally
    {
        FFSpecArgs->Clear();
        FFSpecArgsExcl->Clear();
        /* Free the memory for the zipfilename and parameters
         * we know we had a filename, so we'll dispose it's space. */
        if (ZipRecP)
        {
            StrDispose(ZipRecP->pSuffix);
            StrDispose(ZipRecP->pZipFN);
            StrDispose(ZipRecP->pZipPassword);
            StrDispose(ZipRecP->fTempPath);
            StrDispose(ZipRecP->fArchComment);
            StrDispose(ZipRecP->fGRootDir);
            for (int i = ZipRecP->fArgc - 1; i >= 0; i--)
            {
                StrDispose(ZipRecP->fFDS[i].fFileSpec);
                StrDispose(ZipRecP->fFDS[i].fPassword);
                StrDispose(ZipRecP->fFDS[i].fRootDir);
            }
            delete[] ZipRecP->fFDS;
            for (int i = ZipRecP->fTotExFileSpecs - 1; i >= 0; i--)
                StrDispose(ZipRecP->fExFiles[i].fFileSpec);
            delete[] ZipRecP->fExFiles;
            delete ZipRecP;
        }
        ZipRecP = NULL;
        StopWaitCursor();
        DoDllUnload();
        FZipBusy = false;
        Cancel = false;
        FStoredExtraData = "";                   // release held data
        if ((FSuccessCnt > 0) && (QueryZip(FZipFileName) != ZB_EXE_DET_HEAD))
            List();  // Update the Zip Directory by calling List() function.
    }               // not needed for spanned exe
}

/*? TZBWorker::ExtExtract------------------------------------------------------
1.79 13 March 2009 RP clear FullErrCode
1.79 3 July 2008 RA removed writeabilty check for destination dir
1.79 26 July 2005 RA changed error handling
1.77 5 August 2004 RA parameter added in readspan
1.76 28 April 2004 do not check destination when testing (ExtrTest is active)
1.73 17 September 2003 RA avoid same exception shown twice
1.73 6 September 2003 RA throw exception if dll returns negative value
1.73 4 August 2003 RA removed loading of dll will be done in Exec
1.73 (22 July 2003) RA exception handling for EZipMaster + fUnzBusy := False when dll load error
1.73 (16 July 2003) RA catch and display dll load errors
1.73 (18 June 2003) RA bug fix UnZipParms->fExtractDir when FExtrBaseDir not used
1.73 (14 June 2003) RP new dll handling
1.73 (22 May 2003) RA use of DelimitPah
1.73 (20 May 2003 RA added ForceDirectories and ExtrForceDir option
 UseStream = 0 ==> Extract file from zip archive file.
 UseStream = 1 ==> Extract stream from zip archive file.
 UseStream = 2 ==> Extract (zipped) stream from another stream.*/
#ifndef NO_STREAM
void __fastcall TZBWorker::ExtExtract(int UseStream, TMemoryStream *MemStream)
#else
void __fastcall TZBWorker::ExtExtract(int UseStream, TMemoryStream* /*MemStream*/)
#endif
{
    String TmpZipName;
    int  OldPRC = FPasswordReqCount;
    FSuccessCnt = FErrCode = FFullErrCode = 0;
    FMessage = "";
    int DllVers = 0;
    // Make sure we can't get back in here while work is going on
    if (FUnzBusy)
        return;
    FUnzBusy = true;
    Cancel = false;
    String TmpBaseDir;
    pUnZipParms UnzRecP = NULL;
    try
    {
        if (UseStream < 2)
        {
            if (FZipFileName.IsEmpty())
                throw EZipBuilder(GE_NoZipSpecified);
            if (Count == 0)
                List(); // List was probably aborted try again
            if (Count == 0)
            {
                FUnzBusy = false; // stop from doing anything
                if (!FErrCode)
                    throw EZipBuilder(DS_FileOpen);
            }
            Cancel = false;     // might have been set in List
            if (FUnzBusy)
            {
#ifdef NO_SPAN
                if (FTotalDisks > 0)
                    throw EZipBuilder(DS_NoDiskSpan);
#endif
                // expand and check ExtrBaseDir
                if (!UseStream && (!FExtrBaseDir.IsEmpty()) && !FExtrOptions.Contains(ExtrTest))
                {
                    TmpBaseDir = ExpandUNCFileName(DelimitPath(FExtrBaseDir, true));
                    if (FExtrOptions.Contains(ExtrForceDirs))
                        ForceDirectory(TmpBaseDir);
                    if (!DirExists(TmpBaseDir))
                        throw EZipBuilder(EX_NoExtrDir, TmpBaseDir);
                }
                TmpZipName = FZipFileName;
                // We do a check if we need UnSpanning first, this depends on
                // The number of the disk the EOC record was found on. ( provided by List() )
                // If we have a spanned set consisting of only one disk we don't use ReadSpan().
#ifndef NO_SPAN
                if (FTotalDisks > 0)
                {
                    if (FTempDir.IsEmpty())
                    {
                        TCHAR NewName[MAX_PATH];
                        ::GetTempPath(MAX_PATH, NewName);
                        TmpZipName = NewName;
                    }
                    else
                        TmpZipName = DelimitPath(FTempDir, true);
                    FUnzBusy = !ReadSpan(FZipFileName, TmpZipName, true);
                    // We returned without an error, now  TmpZipName contains a real name.
                }
#endif
            }
        }
        if (FUnzBusy)
            DllVers = DoDllLoad();
        if (DllVers > 0)
        {
            try   // Pass in a ptr to parms.
            {
                UnzRecP = new UnZipParms;
                memset(UnzRecP, 0, sizeof(UnZipParms));
                SetUnZipSwitches(TmpZipName, UnzRecP);
                if (!TmpBaseDir.IsEmpty())
                    UnzRecP->fExtractDir = MakeStrP(TmpBaseDir);
                else
                    UnzRecP->fExtractDir = NULL;

                UnzRecP->fUFDS = new UnzFileData[FFSpecArgs->Count];
                memset(UnzRecP->fUFDS, 0, sizeof(UnzFileData) * FFSpecArgs->Count);
                if (FFSpecArgs->Count && (FFSpecArgs->Strings[0]) != "*.*")
                {
                    for (int i = 0; i < FFSpecArgs->Count; i++)
                        UnzRecP->fUFDS[i].fFileSpec = MakeStrP(FFSpecArgs->Strings[i]);
                    UnzRecP->fArgc = FFSpecArgs->Count;
                }
                else
                    UnzRecP->fArgc = 0;
#ifndef NO_STREAM
                if (UseStream == 1)
                    {
                      ZipDirEntry* TEntry = FDirHashs->Find(FFSpecArgs->Strings[0]);
                      if(TEntry)
                      {
                          FZipStream->Size = static_cast<STRMSIZ>(TEntry->UncompressedSize);
                          UnzRecP->fUseOutStream = true;
                          UnzRecP->fOutStream = FZipStream->Memory;
                          UnzRecP->fOutStreamSize = TEntry->UncompressedSize;
                          UnzRecP->fArgc = 1;
                      }
                    }
                if (UseStream == 2)
                {
                    UnzRecP->fUseInStream   = true;
                    UnzRecP->fInStream    = MemStream->Memory;
                    UnzRecP->fInStreamSize  = static_cast<int>(MemStream->Size);
                    UnzRecP->fUseOutStream  = true;
                    UnzRecP->fOutStream     = FZipStream->Memory;
                    UnzRecP->fOutStreamSize = static_cast<int>(FZipStream->Size);
                }
#endif
                UnzRecP->fSeven = 7;
                FEventErr = "";
                // We have to be carefull doing an unattended Extract when a password is needed
                // for some file in the archive.
                if ((UseStream < 2) && FUnattended && FPassword.IsEmpty() && !FEvents.FOnPasswordError)
                {
                    FPasswordReqCount = 0;
                    Report(zacMessage, 0, ZipLoadStr(EX_UnAttPassword), 0);
                }
                // fArgc is now the no. of filespecs we want extracted.
                if (!UseStream || (UseStream && UnzRecP->fUseOutStream))
                    FSuccessCnt = DoUnzExec(UnzRecP);
            }
            __finally
            {
                DoDllUnload();
                FFSpecArgs->Clear();
#ifndef NO_STREAM
                // Remove from memory if stream is not Ok.
                if (UseStream && FSuccessCnt != 1)
                    FZipStream->Clear();
#endif
                // If UnSpanned we still have this temporary file hanging around.
                if (FTotalDisks > 0)
                    DeleteFile(TmpZipName);
                // Free all allocated memory.
                if (UnzRecP)
                {
                    StrDispose(UnzRecP->pZipFN);
                    StrDispose(UnzRecP->pZipPassword);
                    if (!UseStream)
                        SetCurrentDir(UnzRecP->fExtractDir);
//                    delete[] UnzRecP->fExtractDir;
                    StrDispose(UnzRecP->fExtractDir);
                    for (int i = UnzRecP->fArgc - 1; i >= 0; i--)
                        StrDispose(UnzRecP->fUFDS[i].fFileSpec);
                    delete[] UnzRecP->fUFDS;
                    delete UnzRecP;
                }
                if (FUnattended && FPassword.IsEmpty() && !FEvents.FOnPasswordError)
                    PasswordReqCount = OldPRC;
                FUnzBusy = false;
            }
        }
    }
    catch (EZipBuilder &ezl)
    {
        if (!FEventErr.IsEmpty())
            ezl.Message += FEventErr;
        ShowExceptionError(ezl);
        FSuccessCnt = 0;
        FUnzBusy = false;
    }
    // No need to call the List() function contents was unchanged
}
// TZBWorker::ExtExtract

/*? TZBWorker::ExtMsg----------------------------------------------------------
1.77 27 July 2004 RP New function
*/
String __fastcall TZBWorker::ExtMsg(String CMsg, PZCallBackStruct cbr)
{
    if (CMsg.IsEmpty())
    {
        CMsg = String(cbr->MsgP);
        CMsg = SetSlash(CMsg, psdExternal);
    }
    return CMsg;
}
//---------------------------------------------------------------------------
int __fastcall TZBWorker::Extract(void)
{
    ExtExtract(0, NULL);
    return FErrCode;
}

#ifndef NO_STREAM
// TZBWorker::ExtractFileToStream---------------------------------------------
// 1.73( 21 June 2003) RA FSuccessCnt set to 0 in case of error previous count remained
TZipStream * __fastcall TZBWorker::ExtractFileToStream(const String& FileName)
{
    FSuccessCnt = 0;
    // Use FileName if set, if not expect the filename in the FFSpecArgs.
    if (!FileName.IsEmpty())
    {
        FFSpecArgs->Clear();
        FFSpecArgs->Add(FileName);
    }
    if (FFSpecArgs->Count)
    {
        FZipStream->Clear();
        ExtExtract(1, NULL);
        return (FSuccessCnt != 1) ? NULL : FZipStream;
    }
    ShowZipMessage(AD_NothingToZip, "");
    return NULL;
}
// TZBWorker::ExtractFileToStream
#endif

#ifndef NO_STREAM
// TZBWorker::ExtractStreamToStream-------------------------------------------
// 1.73( 21 June 2003) RA FSuccessCnt set to 0 in case of error previous count remained
TZipStream * __fastcall TZBWorker::ExtractStreamToStream(TMemoryStream *InStream,
        unsigned long OutSize)
{
    FSuccessCnt = 0;
    if (InStream == FZipStream)
    {
        ShowZipMessage(AD_InIsOutStream, "");
        return NULL;
    }
    FZipStream->Clear();
    FZipStream->Size = static_cast<STRMSIZ>(OutSize);
    ExtExtract(2, InStream);
    return (FSuccessCnt != 1) ? NULL : FZipStream;
}
// TZBWorker::ExtractStreamToStream
#endif

/*? TZBWorker::FreeZipDirEntryRecords------------------------------------------
1.73 (12 July 2003 RP string ExtraData
 Free the storage used for dir entries and Empty FZipContents.
*/
void __fastcall TZBWorker::FreeZipDirEntryRecords(void)
{
    if (!ZipContents->Count)
        return;
    // Dispose of the memory pointed-to by all entries.
    for (int i = 0; i < FZipContents->Count; i++)
        if (FZipContents->Items[i])
            delete reinterpret_cast<ZipDirEntry *>(FZipContents->Items[i]);
    // Delete all pointers from the TList itself.
    FZipContents->Clear();
  FDirHashs->Clear();
    // The FZipContents TList will be deleted when the component destructor is called.
}
// TZBWorker::FreeZipDirEntryRecords

/*? TZBWorker::GetAddPassword --------------------------------------------------
1.76 25 May 2004 changed
1.76 10 May 2004 change loading strings
*/
String __fastcall TZBWorker::GetAddPassword(TPasswordButton& Response)
{
    String p1, p2;

    if (FUnattended)
        ShowZipMessage(PW_UnatAddPWMiss, "");
    else
    {
        if ((Response = GetPassword(ZipLoadStr(PW_Caption), ZipLoadStr(PW_MessageEnter),
                                    DHC_AddPwrd1, TMsgDlgButtons() << mbOK << mbCancel, p1))
                == mbOK && !p1.IsEmpty())
        {
            if ((Response = GetPassword(ZipLoadStr(PW_Caption), ZipLoadStr(PW_MessageConfirm),
                                        DHC_AddPwrd2, TMsgDlgButtons() << mbOK << mbCancel, p2))
                    == mbOK && !p2.IsEmpty())
            {
                if (AnsiCompareStr(p1, p2))
                {
                    ShowZipMessage(GE_WrongPassword, "");
                    p2 = "";
                }
            }
        }
    }
    return p2;
}

//---------------------------------------------------------------------------
int __fastcall TZBWorker::GetCount(void)
{
    return (!FZipFileName.IsEmpty()) ? FZipContents->Count : 0;
}
//---------------------------------------------------------------------------
ZipDirEntry* __fastcall TZBWorker::GetDirEntry(int idx)
{
    return reinterpret_cast<ZipDirEntry *>(FZipContents->Items[idx]);
}
/*? TZBWorker::GetDriveProps---------------------------------------------------
1.79 21 May 2005 RP added test for Read-Only volume
1.73 (30 July 2003) RA set VolName to 0 to avoid rubbish in FVolumeName
Check to see if drive in FDrive is a valid drive.
If so, put it's volume label in FVolumeName,
       put it's size in FSizeOfDisk,
       put it's free space in FDiskFree,
       and return true.
was IsDiskPresent
If not valid, return false.
Called by List() and CheckForDisk().
*/
bool __fastcall TZBWorker::GetDriveProps(void)
{
    const int _FILE_READ_ONLY_VOLUME = 0x00080000;
    DWORD SysLen, SysFlags;
    TCHAR DriveLetter, VolName[MAX_PATH];
    VolName[0] = '\0'; // avoid string with rubbish
    if ((DriveLetter = FDrive.UpperCase()[1]) != '\\')
    { // Only for local drives
        if (DriveLetter < 'A' || DriveLetter > 'Z')
            throw EZipBuilder(DS_NotaDrive, FDrive);
        if (!(::GetLogicalDrives() & (1 << (DriveLetter - 'A'))))
            throw EZipBuilder(DS_DriveNoMount, FDrive);
    }
    // Turn off critical errors.
    unsigned int OldMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
    unsigned long DiskSerial;
    if (!::GetVolumeInformation(FDrive.T_STR(), VolName, MAX_PATH, &DiskSerial, &SysLen, &SysFlags, NULL, 0))
    {  // nor for fixed drives
        // W'll get this if there is a disk but it is not or wrong formatted
        // so this disk can only be used when we also want formatting.
        ::SetErrorMode(OldMode);   // v1.52j
        return (GetLastError() == 31 && FAddOptions.Contains(AddDiskSpanErase));
    }
    FVolumeName = VolName;
    FVolumeReadOnly = (SysFlags & _FILE_READ_ONLY_VOLUME);// != 0;
    // Get free disk space and size.
    DiskFreeAndSize(3);
    // Restore old error mode.
    ::SetErrorMode(OldMode);
    // -1 is not very likely to happen since GetVolumeInformation catches errors.
    // But on W95(+OSR1) and a UNC filename w'll get also -1, this would prevent
    // opening the file. !!!Potential error while using spanning with a UNC filename!!!
    return(DriveLetter == '\\' || (DriveLetter != '\\' && FSizeOfDisk != -1));
}

/*? TZBWorker::GetExtrPassword -------------------------------------------------
1.76 25 May 2004 changed
1.76 10 May 2004 change loading strings
  Same as GetAddPassword, but does NOT verify
*/
String __fastcall TZBWorker::GetExtrPassword(TPasswordButton& Response)
{
    String Result;
    if (FUnattended)
        ShowZipMessage(PW_UnatExtPWMiss, "");
    else
        Response = GetPassword(ZipLoadStr(PW_Caption), ZipLoadStr(PW_MessageEnter),
                               DHC_ExtrPwrd, TMsgDlgButtons() << mbOK << mbCancel << mbAll, Result);
    return Result;
}

#ifndef NO_SPAN
/*? TZBWorker::GetLastVolume--------------------------------------------------
1.79 11 Dec 2004 RP added numbering scheme
1.77 21 August 2004 RP improve orphan file tolerance
1.76 9 June 2004 RA fix finding part of non-split file
1.76 25 April 2004 RA do not throw exception for MVolume part with faultly/no EOC
1.75 16 March 2004 RA stop exception if unwanted file does not exist
1.73 16 September 2003 RA use of NameOfPart function
1.73 12 September 2003 RA do not accept negative partnumbers
1.73  (9 July 2003) RA creation of first part name improved
*/
int __fastcall TZBWorker::GetLastVolume(String FileName, ZipEndOfCentral& EOC, bool AllowNotExists)
// Locate last of multi volume or last disk of spanned
// returns
// 0 = OK
// -1 = error
// 1 = user aborted loop = avoid asking for last disk if not needed
{
    String Fname;
    String sName;
    int PartNbr = -1;
    TSearchRec SRec;
    FInFileHandle = -1;
    FDrive = ExtractFileDrive(ExpandFileName(FileName)) + "\\";
    String Ext = UpperCase(ExtractFileExt(FileName));
    String Path = ExtractFilePath(FileName);
    bool FMVolume = false;
    FNumbering = znsNone; // unknow as yet
    try
    {
        FDriveFixed = IsFixedDrive(FDrive);
        if (!GetDriveProps())
            return -1;  // check valid drive
        if (!FileExists(FileName)) // if file exsits we have one volume zip file or spanned on removable disks
        {
            Fname = FileName.SubString(1, FileName.Length() - Ext.Length());// remove extension
            FMVolume = true; // file did not exist maybe it is a multi volume
            if (FDriveFixed) // if file not exists on harddisk then only Multi volume parts are possible
            {  // filename is of type ArchiveXXX.zip
                // MV files are series with consecutive partnbrs in filename, highest number has EOC
                if (Ext == ".ZIP")
                {
                    bool Finding = true;
                    while (Finding)
                    {
                        String s = Fname + IntToStr(1002 + PartNbr).SubString(2, 3) + ".zip";
                        if (!FileExists(s))
                        {
                            PartNbr = -1;
                            break;
                        }
                        ++PartNbr;
                        FInFileName = s;
                        if (OpenEOC(EOC, false))
                        {
                            Finding = false;
                            if (EOC.ThisDiskNo != PartNbr)
                                PartNbr = -1; // not valid last part
                        }
                        CloseFileHandle(FInFileHandle);
                    }
                }
                if (PartNbr == -1)
                {
                    if (AllowNotExists)
                        return 1;
                    throw EZipBuilder(DS_FileOpen); // non found
                }
                FileName = Fname + IntToStr(1001 + PartNbr).SubString(2, 3) + ".zip";
                // check if filename.z01 exists then it is part of MV with compat names and cannot be used
                if (FileExists(ChangeFileExt(FileName, ".z01")))
                    throw EZipBuilder(DS_FileOpen); // cannot be used
            }
            else // if we have an MV archive copied to a removable disk
            {
                // accept any MV filename on disk
                if ((sName = NameOfPart(Fname, false)).IsEmpty())
                    sName = NameOfPart(Fname, true);
                if (sName.IsEmpty())
                {
                    if (AllowNotExists)
                        return 1;
                    throw EZipBuilder(DS_FileOpen);
                }
                FileName = Path + sName;
            }
        }
        // zip file exists or we got an acceptable part in multivolume or spanned archive
        FInFileName = FileName; // use class variable for other functions
        while (!OpenEOC(EOC, false)) // does this part contains the central dir
        {  // it is not the disk with central dir so ask for the last disk
            CloseFileHandle(FInFileHandle);
            FNewDisk = true;       // new last disk
            FDiskNr = -1;          // read operation
            CheckForDisk(false); // does the request for new disk
            if (FDriveFixed)
            {
                if (FMVolume)
                {
                    if (AllowNotExists)
                        return -1;
                    throw EZipBuilder(DS_FileOpen); // it was not a valable part
                }
                AllowNotExists = false; // next error needs to be displayed always
                throw EZipBuilder(DS_NoValidZip); // file with EOC is not on fixed disk
            }
            if (FMVolume)
            { // we have removable disks with multi volume archives get the file name on this disk
                if ((sName = NameOfPart(Fname, FSpanOptions.Contains(spCompatName))).IsEmpty())
                { // no acceptable file on this disk so not a disk of the set
                    ShowZipMessage(DS_FileOpen, "");
                    return -1; //error
                }
                FInFileName = Path + sName;
            }
        }
        if (FMVolume) // got a multi volume part so need more checks
        {
            // is this first file of a spanned
            if (!FIsSpanned && (EOC.ThisDiskNo == 0) && (PartNbr >= 0))
                throw EZipBuilder(DS_FileOpen);
            // part and EOC equal?
            if (FDriveFixed && (EOC.ThisDiskNo != PartNbr))
                throw EZipBuilder(DS_NoValidZip);
        }
    }
    catch (EZipBuilder &ezl)
    {
        if (!AllowNotExists)
            ShowExceptionError(ezl);
        FInFileName = ""; // don't use the file
        CloseFileHandle(FInFileHandle);
        return -1;
    }
    return 0;
}
// TZBWorker::GetLastVolume
#endif

/*? TZBWorker::GetNewDisk------------------------------------------------------
1.77 19 August 2004 RP - improve logic
1.73 (28 July 2003) RP / RA added verbose message
*/
#ifdef NO_SPAN
void __fastcall TZBWorker::GetNewDisk(int /*DiskSeq*/)
{
    throw EZipBuilder(DS_NoDiskSpan);
#else
void __fastcall TZBWorker::GetNewDisk(int DiskSeq)
{
    FileClose(FInFileHandle); // Close the file on the old disk first.
    FDiskNr = DiskSeq;
    while (true)
    {
        do
        {
            FNewDisk = true;
            FInFileHandle = -1; //to prevent second close after user choses cancel
            CheckForDisk(false);
        }
        while (!IsRightDisk());
        if (Verbose)
            Report(zacMessage, 0, ZipFmtLoadStr(TM_GetNewDisk, ARRAYOFCONST((FInFileName))), 0);
        // Open the the input archive on this disk.
        if ((FInFileHandle = FileOpen(FInFileName, fmShareDenyWrite | fmOpenRead)) != -1)
            break; // found
        if (FDriveFixed)
            throw EZipBuilder(DS_NoInFile);
        else
            ShowZipMessage(DS_NoInFile, "");
    }
#endif
}

/*? TZBWorker::GetPassword---------------------------------------------------
1.76 25 May 2004 no external GlobalResult
*/
TPasswordButton __fastcall TZBWorker::GetPassword(const String& DialogCaption,
        const String& MsgTxt,
        int ctx,
        TPasswordButtons pwb,
        String &ResultStr)
{
    TModalResult GModalResult;
    String msg = MsgTxt;
    ResultStr = "";
    GModalResult = ZipMessageDialog(DialogCaption, msg, zmtPassword + (ctx & 0xFFFF), pwb);
    switch (GModalResult)
    {
    case mrOk:
        ResultStr = msg;
        return mbOK;
    case mrCancel:
        return mbCancel;
    case mrAll:
        return mbNoToAll; // CancelAll
    default:
        return mbAbort;
    }
}

/*? TZBWorker::GetPassword----------------------------------------------------
1.76 25 May 2004 no external GlobalResult
*/
TPasswordButton __fastcall TZBWorker::GetPassword(const String& DialogCaption,
        const String& MsgTxt,
        TPasswordButtons pwb,
        String &ResultStr)
{
    return GetPassword(DialogCaption, MsgTxt, DHC_Password, pwb, ResultStr);
}

// TZBWorker::GetZipComment---------------------------------------------------
// 1.73.3.2 11 Oct 2003 RP comment now converted when read
// 1.73 (2 June 2003) RA use ConvertOEM
String __fastcall TZBWorker::GetZipComment(void)
{
    return FZipComment;
}
// TZBWorker::GetZipComment

// TZBWorker::HasExtendedChars ------------------------------------------------
// 1.73.3.2 8 Oct 2003 RA New function to test if file name has extended chars
bool __fastcall TZBWorker::HasExtendedChars(const String& FileName)
{
    int Len = FileName.Length();
    for (int i = 1; i < Len; ++i)
        if (extend[static_cast<unsigned char>(FileName[i])])
            return true;
    return false;
}
// TZBWorker::HasExtendedChars

/*? TZBWorker::CheckDestWritable--------------------------------------------------
1.79  2007 Feb 22 - replaces IsDestWritable
*/
bool __fastcall TZBWorker::CheckDestWritable(const String& fname, bool NoExc)
{
    bool Result;
    int FileHandle = -1;
    // test if destination can be written
    String xname = ExpandUNCFileName(fname);
    FDrive = ExtractFileDrive(ExpandFileName(fname)) + "\\";
    int drt = ::GetDriveType(FDrive.T_STR());
    GetDriveProps();  // check valid drive
    bool XP = IsWinXP();
    SetLastError(0);
    if (XP || (drt != DRIVE_CDROM))
    {
        // look for read-only or locked file
        TSearchRec SRec;
        if (FindFirst(xname, faAnyFile, SRec) == 0)
        {
            Result = (SRec.Attr & faReadOnly) == 0;
            FindClose(SRec);
            if (Result)
            {
                // exists and is not read-only - test locked
                FileHandle = FileOpen(xname, fmOpenWrite);
                Result = FileHandle > -1;
                if (Result)
                    CloseFileHandle(FileHandle);
            }
        }
        else
        {
            // test if we can createfile
            FileHandle = FileCreate(xname);
            if (FileHandle > -1)
            {
                CloseFileHandle(FileHandle);
                Result = true;
                if (!DeleteFile(xname))
                    throw Exception(Format("Delete failed [%d] for: %s",
                                           ARRAYOFCONST((static_cast<int>(GetLastError()), xname))));
            }
        }
        if (!Result && !NoExc)
            throw EZipBuilder(DS_NotChangeable, xname, GetLastError());
    }
    return Result;
}

//---------------------------------------------------------------------------
bool __fastcall TZBWorker::IsFixedDrive(const String& drv)
{
    int drt = ::GetDriveType(drv.T_STR());
    return !(drt == DRIVE_REMOVABLE || drt == DRIVE_CDROM);
}

/*? TZBWorker::IsPathOnly--------------------------------------------------------
1.75.1.1 20 March 2004 RA fix IsPathOnly
1.75 RP new function
*/
bool __fastcall TZBWorker::IsPathOnly(const String& f)
{
    if (!f.IsEmpty())
    {
        char c = f[f.Length()];
        if ((c == '/') || (c == '\\'))
            return true;
    }
    return false;
}

#ifndef NO_SPAN
/*? TZBWorker::IsRightDisk-----------------------------------------------------
1.79 14 July 2005 RA re-use origuinal FInFileName if not correct disk
1.79 12 Dec 2004 RP detect used numberscheme
1.73 (29 June 2003) RP amended
*/
bool __fastcall TZBWorker::IsRightDisk(void)
{
    if ((FNumbering < znsName) && !FDriveFixed &&
            (FVolumeName == "PKBACK# " + IntToStr(1001 + FDiskNr).SubString(2, 3)) &&
            FileExists(FInFileName))
    {
        FNumbering = znsVolume;
        return true;
    }
    String fn = FInFileName;
    if (FNumbering == znsNone) // not known yet
    {
        CreateMVFileName(FInFileName, true, true);  // make compat name
        if (FileExists(FInFileName))
        {
            FNumbering = znsExt;
            return true;
        }
        FInFileName = fn;
        CreateMVFileName(FInFileName, true, false);  // make numbered name
        if (FileExists(FInFileName))
        {
            FNumbering = znsName;
            return true;
        }
        if (FDriveFixed)
            return true;     // allways true - only needed name
        FInFileName = fn;      // restore
        return false;
    }
    // numbering scheme already known
    if (FNumbering == znsVolume)
        return false;
    CreateMVFileName(FInFileName, true, (FNumbering == znsExt));
    // fixed drive always true only needed new filename
    if (!FDriveFixed && !FileExists(FInFileName))
    {
        FInFileName = fn; // restore
        return false;
    }
    return true;
}
#endif

#ifndef NO_SFX
// IsZipSFX---------------------------------------------------------------------
/* Return value:
 0 = The specified file is not a SFX
 1 = It is one
*/
int __fastcall TZBWorker::IsZipSFX(const String& SFXExeName)
{
    // SFX = 1 + 128 + 64
    if ((QueryZip(SFXExeName) & (zqbStartEXE | zqbHasEOC | zqbHasCentral)) == (zqbStartEXE | zqbHasEOC | zqbHasCentral))
        return 1;
    return 0;
}
// IsZipSFX
#endif

/*? TZBWorker::List-----------------------------------------------------------
1.79 7 June 2007 RP added test for Z64 when opening EOC
1.79 4 May 2006 RA Add check on FileNameLn to avoid buffer overflow
1.79 6 Dec 2004 RA allow use of SFX with Detached header
1.79 5 Dec 2004 RP call OnDirUpdate when GetLastVolume returns error
1.76  9 June 2004 add Trace messages
1.76 28 April 2004 test Active
1.75 13 March 2004 give progress
1.75 18 February 2004 RP allow file>2G
1.73.3.2 Oct 11 2003 RP convert saved comment
1.73 (26 July 2003) RA - added test for empty ZipFileName
1.73 (15 July 2003) RP / RA ReadJoin
*/
int __fastcall TZBWorker::List(void) // All work is local - no DLL calls.
{
    struct ZipEndOfCentral  EOC, tmpEOC;
    struct ZipCentralHeader CEH;
    ZipDirEntry* pzd;
    __int64 OffsetDiff = 0;
    char Name[255];
    bool LiE = true;

    // Zero out any previous entries.
    FreeZipDirEntryRecords();
    FRealFileSize = 0;
    FZipSOC = 0;
    FSFXOffset = 0;
    FZipComment = "";
    FIsSpanned = false;
    FDirOnlyCount = 0;
    FErrCode = 0;  // reset previous error code
    char MadeOS = 0;
    char MadeVer = 20;
    FInFileHandle = -1;
    unsigned em;
    if (!Active)
    {
        FDelaying = FDelaying << zdiList;
        return FErrCode;
    }
    try
    {
        em = SetErrorMode(SEM_FAILCRITICALERRORS);
#ifndef NO_SPAN
        FInFileName = QueryZip(FZipFileName) == ZB_EXE_DET_HEAD ? ChangeFileExt(FZipFileName, ".zip")
                      : FZipFileName;
        // Locate last of multi volume or last disk of spanned
        GetLastVolume(FInFileName, EOC, true);
#else
        FInFileName = FZipFileName;
        if (!FZipFileName.IsEmpty() && FileExists(FZipFileName))
            OpenEOC(EOC, true);  // exception if not
        else
            FInFileHandle = -1;
#endif
    }
    catch (EZipBuilder &ezl)
    {
        ShowExceptionError(ezl);
    }
    SetErrorMode(em);
    // let the use know ther's no entries
    if (FInFileHandle == -1)    // was problem
    {
        TNotifyEvent tmpDirUpdate = FEvents.FOnDirUpdate;
        if (tmpDirUpdate)
            tmpDirUpdate(Builder);
        return FErrCode;
    }
    try
    {
        StartWaitCursor();
        try
        {
            FTotalDisks = EOC.ThisDiskNo; // Needed if GetNewDisk() is called.
            // This could also be set to True if it's the first and only disk.
            if (EOC.ThisDiskNo > 0)
                FIsSpanned = true;
            // Do we have to request for a previous disk first?
            if (EOC.ThisDiskNo != EOC.CentralDiskNo)
            {
#ifndef NO_SPAN
                GetNewDisk(EOC.CentralDiskNo);
                FFileSize = FileSeek(FInFileHandle, 0, 2);
                OffsetDiff = EOC.CentralOffset;
#else
                throw EZipBuilder(DS_NoDiskSpan, true);
#endif
            }
            else
                // Due to the fact that v1.3, v1.4 and v1.45 programs do not change the archives
                // EOC and CEH records in case of a SFX conversion (and back) we have to
                // make this extra check.
                OffsetDiff = FFileSize - EOC.CentralSize - sizeof(EOC) - EOC.ZipCommentLen;

            FZipSOC  = static_cast<unsigned long >(OffsetDiff);
            FSFXOffset = FFileSize;
            if (FFileSize == 22)
                FSFXOffset = 0;
            FWrongZipStruct = false;
            if (EOC.CentralOffset != OffsetDiff)
            {
                FWrongZipStruct = true;            // We need this in the ConvertXxx functions.
                ShowZipMessage(LI_WrongZipStruct, "");
            }
            // Now we can go to the start of the Central directory.
            if (FileSeek(FInFileHandle, OffsetDiff, 0) == -1)
                throw EZipBuilder(LI_ReadZipError, true);

            Report(zacXItem, zprLoading, ZipLoadStr(PR_Loading), EOC.TotalEntries);
            // Read every entry: The central header and save the information.
#ifdef _DEBUG
            if (FTrace)
                Report(zacMessage, 0, Format("List - expecting %d files",
                                             ARRAYOFCONST((EOC.TotalEntries))), 0);
#endif
            FZipContents->Capacity = EOC.TotalEntries;
            FDirHashs->AutoSize(EOC.TotalEntries); // resize appropriately
            for (int i = 0; i < EOC.TotalEntries; i++)
            {
                // Read a central header.
                while (FileRead(FInFileHandle, &CEH, sizeof(CEH)) != static_cast<int>(sizeof(CEH)))
                {
                    // It's possible that we have the central header split up.
                    if (FDiskNr >= EOC.ThisDiskNo)
                        throw EZipBuilder(DS_CEHBadRead);
                    // We need the next disk with central header info.
#ifndef NO_SPAN
                    GetNewDisk(FDiskNr + 1);
#else
                    throw EZipBuilder(DS_NoDiskSpan, true);
#endif
                }
                if (CEH.HeaderSig != CentralFileHeaderSig)
                    throw EZipBuilder(DS_CEHWrongSig);
                if (CEH.FileNameLen >= MAX_PATH)
                    throw EZipBuilder(DS_CENameLen);
                // Now the filename.
                ReadJoin(Name, CEH.FileNameLen, DS_CENameLen);
                // Save version info globally for use by codepage translation routine
                FHostVer = CEH.VersionMadeBy0;
                if ((FHostNum = CEH.VersionMadeBy1) > 0) // not msdos
                {
                    MadeOS = FHostNum;
                    MadeVer = FHostVer;
                }
                Name[CEH.FileNameLen] = '\0';
                String Fn = ConvertOEM(String(Name), cpdOEM2ISO);
#ifdef _DEBUG
                if (FTrace)
                    Report(zacMessage, 0, Format("List - [%d] \"%s\"",
                                                 ARRAYOFCONST((i, Fn))), 0);
#endif
                // Create a new ZipDirEntry record
                pzd = new ZipDirEntry; // These will be deleted in: FreeZipDirEntryRecords().
                // Copy the information from the central header.
                memcpy(pzd, &CEH.VersionMadeBy0, LocalDirEntrySize);
                pzd->FileName  = SetSlash(Fn, psdExternal);
                pzd->Encrypted = pzd->Flag & 0x01;
                // Read the extra data if present
                pzd->ExtraData = "";  // empty
                if (CEH.ExtraLen)
                {
                    pzd->ExtraData.SetLength(CEH.ExtraLen);
                    ReadJoin(pzd->ExtraData.c_str(), CEH.ExtraLen , LI_ReadZipError);
                }
                // Read the FileComment if present and save.
                if (CEH.FileComLen)
                {
                   ReadJoin(&Name[0], CEH.FileComLen , DS_CECommentLen);
                   pzd->FileComment = ConvertOEM(String(Name), cpdOEM2ISO);
                }
                if (FUseDirOnlyEntries || !ExtractFileName(pzd->FileName).IsEmpty())
                {
#ifdef _DEBUG
                    if (FTrace)
                        Report(zacMessage, 0, Format("List - adding \"%s\" [%s]",
                                                     ARRAYOFCONST((pzd->FileName, pzd->FileComment))), 0);
#endif
                    // Add it to our contents tabel.
                    FZipContents->Add(pzd);
                    FDirHashs->Add(pzd); // ignore duplicates
                    // Notify user of the next entry in the ZipDir.
                    TNewNameEvent tmpNewName = FEvents.FOnNewName;
                    if (tmpNewName)
                        tmpNewName(Builder, i + 1, pzd);
                }
                else
                {
                    FDirOnlyCount++;
#ifdef _DEBUG
                    if (FTrace)
                        Report(zacMessage, 0, Format("List - dropped dir [%d]",
                                                     ARRAYOFCONST((FDirOnlyCount))), 0);
#endif
                    pzd->ExtraData = "";
                    pzd->FileName = "";
                    pzd->FileComment = "";
                    delete pzd;
                }
                // Calculate the earliest Local Header start.
                if (FSFXOffset > CEH.RelOffLocal)
                    FSFXOffset = CEH.RelOffLocal;
                Report(zacXProgress, zprLoading, ZipLoadStr(PR_Loading), 1);
                if (Cancel)
                    throw EZipAbort();
            }
            // Read the end of central header entry
            while (FileRead(FInFileHandle, &tmpEOC, sizeof(tmpEOC)) != static_cast<int>(sizeof(tmpEOC)))
            {
                // It's possible that we have the central header split up.
                if (FDiskNr >= EOC.ThisDiskNo)
                    throw EZipBuilder(DS_CEHBadRead);
                // We need the next disk with central header info.
#ifndef NO_SPAN
                GetNewDisk(FDiskNr + 1);
#else
                throw EZipBuilder(DS_NoDiskSpan, true);
#endif
            }
            //validate the signature of the central header entry
            if (tmpEOC.HeaderSig != EndCentralDirSig)
                throw EZipBuilder(DS_EOCBadRead);
            FTotalDisks = EOC.ThisDiskNo;   // We need this when we are going to extract.
            LiE = false;
        }
        catch (EZipBuilder &ezl)
        {  // Catch all Zip List specific errors.
            ShowExceptionError(ezl);
        }
        catch (const MEMEXCEPT &me)
        {
            ShowZipMessage(GE_NoMem, "");
        }
        catch (const Exception &E)
        {
            ShowZipMessage(LI_ErrorUnknown, "\n" + E.Message);
        }
        catch (...)
        {
            ShowZipMessage(LI_ErrorUnknown, "");
        }
    }
    __finally
    {
        Report(zacEndOfBatch, zprLoading, "", 0);
        StopWaitCursor();
        CloseFileHandle(FInFileHandle);
        if (LiE)
        {
            FZipFileName = "";
            FSFXOffset = 0;
        }
        else
        {
            FSFXOffset += static_cast<int>(OffsetDiff - EOC.CentralOffset); // Correct the offset for v1.3, 1.4 and 1.45
            FHostNum = MadeOS;  // if any not dos assume comment not oem
            FHostVer = MadeVer;
            FZipComment = ConvertOEM(FEOCComment, cpdOEM2ISO);
        }
        // Let user's program know we just refreshed the zip dir contents.
        TNotifyEvent tmpDirUpdate = FEvents.FOnDirUpdate;
        if (tmpDirUpdate)
            tmpDirUpdate(Builder);
    }
    return FErrCode;
}
// TZBWorker::List

/*? TZMWorker.MakeString
*/
#ifndef NO_SPAN
String __fastcall TZBWorker::MakeString(char* Buffer, int Size)
{
    String Result;
#ifdef UNICODE
    AnsiString tmp(Buffer, Size);
    Result = String(tmp);
#else
    Result.SetLength(Size);
    StrLCopy(Result.T_STR(), Buffer, Size);
#endif
    return Result;
}
#endif


/*? TZBWorker::NameOfPart-----------------------------------------------------
1.77 19 August 2004 RA Allow >99 parts with compatnames
1.73 18 September 2003 RA accept file extension .zip for compat as valid last part
1.73 16 September 2003 RP new function to avoid negative part numbers
*/
#ifndef NO_SPAN
String __fastcall TZBWorker::NameOfPart(const String& fn, bool compat)
{
    TSearchRec SRec;
    String Result;
    String fs = compat ? fn + ".z??*" : fn + "???.zip";
    int n, r = FindFirst(fs , faAnyFile, SRec);
    while (r == 0)
    {
        if (compat)
        {
            fs = UpperCase(ExtractFileExt(SRec.Name).SubString(3, 20));
            n = (fs == "IP") ? 99999 : StrToIntDef(fs, 0);
        }
        else
            n = StrToIntDef(SRec.Name.SubString(SRec.Name.Length() - 6, 3), 0);
        if (n > 0)
        {
            Result = SRec.Name; // possible name
            break;
        }
        r = FindNext(SRec);
    }
    FindClose(SRec);
    return Result;
}
#endif

/*? TZBWorker::OpenEOC--------------------------------------------------------
1.78 18 Mar 2005 RP Fix bug with only data descriptor uncompressed size
1.79 24 January 2005 RP fix Overflow and Range errors
1.77 19 August 2004 RA Allow >99 parts with compatnames
1.76 30 May 2004 Check file <4G
1.75 18 February 2004 allow >2G
1.73.3.2 8 Oct 2003 RA RP allow max 512 whitespace chars at end of file and save comment
changed 1.73 (22 May 2003) RA use ReadJoin
Function to find the EOC record at the end of the archive (on the last disk.)
We can get a return value( true::Found, false::Not Found ) or an exception if not found.
*/
bool __fastcall TZBWorker::OpenEOC(struct ZipEndOfCentral &EOC, bool DoExcept)
{
    I64Rec SizeOfFile;
    unsigned long  Sig;
    int DiskNo;
    bool      First = false;
    AnsiString ZipBuf;

    FZipComment = "";
    FZipEOC     =  0;
    FEOCComment = "";
    // Open the input archive, presumably the last disk.
    if ((FInFileHandle = FileOpen(FInFileName, fmShareDenyWrite | fmOpenRead)) == -1)
    {
        if (DoExcept)
            throw EZipBuilder(DS_NoInFile);
        ShowZipMessage(DS_FileOpen, "");
        return false;
    }
    // First a check for the first disk of a spanned archive,
    // could also be the last so we don't issue a warning yet.
    if (FileRead(FInFileHandle, &Sig, 4) == 4 && Sig == ExtLocalSig &&
            FileRead(FInFileHandle, &Sig, 4) == 4 && Sig == LocalFileHeaderSig)
    {
        First = true;
        FIsSpanned = true;
    }
    // Next we do a check at the end of the file to speed things up if
    // there isn't a Zip archive comment.
    SizeOfFile.I = FileSeek(FInFileHandle, -static_cast<__int64>(sizeof(EOC)), 2);
    if (SizeOfFile.I >= 0)
        SizeOfFile.I += sizeof(ZipEndOfCentral);
    if (SizeOfFile.Rec.Hi)
    {
        CloseFileHandle(FInFileHandle);
        if (Verbose)
            Report(zacMessage, 0, "Opening EOC seek error" +
                   IntToStr(SizeOfFile.I / (1024 * 1024)) + "Mb", 0);
        if (DoExcept)
        {
            if (SizeOfFile.I != -1)
                throw EZipBuilder(LI_FileTooBig);
            throw  EZipBuilder(DS_NoValidZip);
        }
        return false;
    }
    FFileSize = SizeOfFile.Rec.Lo;
    if (FFileSize >= sizeof(ZipEndOfCentral))
    {
        FRealFileSize = FFileSize;  // There could follow a correction on FFileSize.
        if (FileRead(FInFileHandle, &EOC, sizeof(EOC)) == static_cast<int>(sizeof(EOC)) &&
                EOC.HeaderSig == EndCentralDirSig)
        {
            FZipEOC = FFileSize - sizeof(EOC);
            return true;
        }
    }
    // Now we try to find the EOC record within the last 65535 + sizeof( EOC ) bytes
    // of this file because we don't know the Zip archive comment length at this time.
    try
    {
        int Size = min(FFileSize, (65535 + sizeof(EOC)));
        ZipBuf.SetLength(Size);
        if (FileSeek(FInFileHandle, static_cast<__int64>(-Size), 2) == -1)
            throw EZipBuilder(DS_FailedSeek);
        ReadJoin(ZipBuf.c_str(), Size, DS_EOCBadRead);
        for (int i = Size - sizeof(EOC) - 1; i >= 0; --i)
        {
            ZipEndOfCentral* pEOC = reinterpret_cast<ZipEndOfCentral*>(ZipBuf.c_str() + i);
            if (pEOC->HeaderSig == EndCentralDirSig)
            {
                FZipEOC = FFileSize - Size + i;
                memcpy(&EOC, ZipBuf.c_str() + i, sizeof(EOC));
                //If we have ZipComment: Save it,No codepage translation yet, wait for CEH read.
                if (EOC.ZipCommentLen)
                {
                    AnsiString zc = ZipBuf.SubString(i + sizeof(EOC) + 1 , EOC.ZipCommentLen);
                    FEOCComment = String(zc);

                }
                // Check if we really are at the end of the file, if not correct the filesize
                // and give a warning. (It should be an error but we are nice.)
                if (i + sizeof(EOC) + EOC.ZipCommentLen - Size)
                {
          				if(Verbose)
            					Report(zacMessage, 0, ZipLoadStr(LI_GarbageAtEOF), 0);
                    FFileSize += (i + sizeof(EOC) + static_cast<unsigned>(EOC.ZipCommentLen) - Size);
                    // Now we need a check for WinZip Self Extractor which makes SFX files which
                    // allmost always have garbage at the end (Zero filled at 512 byte boundary!)
                    // In this special case 'we' don't give a warning.
//                    bool ShowGarbageMsg = true;
//                    if (FRealFileSize - static_cast<unsigned>(FFileSize) < 512)
//                    {
//                        int j = i + sizeof(EOC) + EOC.ZipCommentLen;
//                        while ((j < Size) && (ZipBuf[j] < '/'))
//                            j++;
//                        if (j == Size + 1)
//                            ShowGarbageMsg = false;
//                    }
//                    if (ShowGarbageMsg)
//                        ShowZipMessage(LI_GarbageAtEOF, "");
                }
                return true;
            }
        }
    }
    catch (...)
    {
        if (DoExcept)
            throw;
    }
    CloseFileHandle(FInFileHandle);    // don't leave it open
    if (DoExcept)
    { // Get the volume number if it's disk from a set.
        DiskNo = 0;
        if (FVolumeName.Pos("PKBACK# ") == 1)
            DiskNo = StrToIntDef(FVolumeName.SubString(9, 3), 0);
        if (DiskNo <= 0)
        {
            String ext = UpperCase(ExtractFileExt(FInFileName));
            DiskNo = 0;
            if (ext.SubString(1, 2) == ".Z")
                DiskNo = StrToIntDef(ext.SubString(2, ext.Length() - 2), 0);
            if (DiskNo <= 0)
                DiskNo = StrToIntDef(FInFileName.SubString(FInFileName.Length() -
                                     ext.Length() - 3 + 1, 3), 0);

        }
        if (!First && (DiskNo > 0))
            throw EZipBuilder(DS_NotLastInSet, DiskNo);
        throw EZipBuilder((First) ? ((DiskNo == 1) ? DS_FirstInSet : DS_FirstFileOnHD) : DS_NoValidZip);
    }
    return false;
}

// TZBWorker::ReadJoin--------------------------------------------------------
// 1.73 (22 May 2003) RP new function to partner WriteJoin
void __fastcall TZBWorker::ReadJoin(void *Buffer, int BufferSize, int DSErrIdent)
{
    if (FileRead(FInFileHandle, Buffer, BufferSize) != BufferSize)
        throw EZipBuilder(DSErrIdent);
}
// TZBWorker::ReadJoin

#ifndef NO_SPAN
/*? TZBWorker::ReadSpan--------------------------------------------------------
1.77 31 August 2004 RA - allow cancel during read
1.77 5 August 2004 RA handle normal/ eXtended progress
1.75 18 February 2004 RP Allow >2G
1.73 (12 July 2003) RA made Report type zacItem for each file
1.73 (30 May 2003) use of ReadJoin + Delimitpath
1.73 (18 May 2003) RA use ZipAbort exception
Function to read a split up Zip source file from multiple disks and write it to one destination file.
Return values:
0   All Ok.
-7   ReadSpan errors. See ZipMsgXX.rc and ZipMsg.h
-8   Memory allocation error.
-9   General unknown ReadSpan error.
*/
int __fastcall TZBWorker::ReadSpan(const String InFileName, String& OutFilePath, bool UseXProgress)
{
    AnsiChar* Buffer = NULL;
    int Result;
    __int64 FullSize;
    bool BatchStarted = false;
    struct  ZipEndOfCentral    EOC;
    struct  ZipLocalHeader     LOH;
    struct  ZipDataDescriptor  DD;
    struct  ZipCentralHeader   CEH;
    TMZipDataList *MDZD = NULL;
    FDrive = DelimitPath(ExtractFileDrive(InFileName), false);
    FDriveFixed = IsFixedDrive(FDrive);
    FDiskNr = -1;
    FNewDisk = false;
    FShowProgress = zspNone;
    FErrCode = 0;
    FInFileName = InFileName;
    FInFileHandle = -1;
    FUnzBusy = true;
    StartWaitCursor();
    try
    {
        Buffer = new AnsiChar[FBufSize];
        // If we don't have a filename we make one first.
        if (!ExtractFileName(OutFilePath).IsEmpty())
        {
            EraseFile(OutFilePath, FHowToDelete);
            OutFilePath = ChangeFileExt(OutFilePath, ".zip");
        }
        else
            if ((OutFilePath = MakeTempFileName("", "")).IsEmpty())
                throw EZipBuilder(DS_NoTempFile);
        // find volume with central dir
        if ((Result = GetLastVolume(FInFileName, EOC, false)) == -1)
        {
            StopWaitCursor();
            FUnzBusy = false;
            delete[] Buffer; // avoid mem leak
            return -9;
        }// error already shown
        if (Result == 1)
            throw EZipBuilder(DS_Canceled, true);
        // Create the output file.
        if ((FOutFileHandle = FileCreate(OutFilePath)) == -1)
            throw EZipBuilder(DS_NoOutFile);
        // Get the date-time stamp and save for later.
        FDateStamp = FileGetDate(FInFileHandle);
        // Now we now the number of zipped entries in the zip archive
        // and the starting disk number of the central directory.
        FTotalDisks = EOC.ThisDiskNo;
        if (EOC.ThisDiskNo != EOC.CentralDiskNo)
            GetNewDisk(EOC.CentralDiskNo); // We have to request a previous disk first.
        // We go to the start of the Central directory.
        if (FileSeek(FInFileHandle, static_cast<__int64>(EOC.CentralOffset), 0) == -1)
            throw EZipBuilder(DS_FailedSeek);

        MDZD = new TMZipDataList(EOC.TotalEntries);
        FullSize = EOC.CentralSize + EOC.ZipCommentLen + sizeof(ZipEndOfCentral);
        // Read for every entry: The central header and save information for later use.
        for (int i = 0; i < EOC.TotalEntries; i++)
        {
            if (FCancel)
                throw EZipAbort();
            // Read a central header.
            while (FileRead(FInFileHandle, &CEH, sizeof(CEH)) != static_cast<int>(sizeof(CEH)))
            {
                // It's possible that we have the central header split up.
                if (FDiskNr >= EOC.ThisDiskNo)
                    throw EZipBuilder(DS_CEHBadRead);
                // We need the next disk with central header info.
                GetNewDisk(FDiskNr + 1);
            }
            if (CEH.HeaderSig != CentralFileHeaderSig)
                throw EZipBuilder(DS_CEHWrongSig);

            // Now the filename.
            if (CEH.FileNameLen >= MAX_PATH)
                throw EZipBuilder(DS_CENameLen);
            ReadJoin(Buffer, CEH.FileNameLen, DS_CENameLen);
            // Save the file name info in the MDZD structure.
            MDZD->Items[i]->SetName(Buffer, CEH.FileNameLen);

            // Save the compressed size, we need this because WinZip sets this
            // sometimes to zero in the local header.
            MDZD->Items[i]->ComprSize = CEH.ComprSize;

            // We need the total number of bytes we are going to read for the progress event.
            FullSize += (sizeof(LOH) + CEH.ComprSize + CEH.FileNameLen + CEH.ExtraLen);
            // plus DataDescriptor if present.
            if (DescUsed(CEH))
                FullSize += sizeof(DD);
            // Seek past the extra field and the file comment.
            if (FileSeek(FInFileHandle, static_cast<__int64>(CEH.ExtraLen + CEH.FileComLen), 1) == -1)
                throw EZipBuilder(DS_FailedSeek);
        }
        // Now we need the first disk and start reading.
        GetNewDisk(0);
        if (UseXProgress)
        {
            Report(zacXItem, zprJoining, ZipLoadStr(PR_Joining), FullSize);
            FShowProgress = zspExtra;
        }
        else
        {
            Report(zacCount, zprCompressed, "", EOC.TotalEntries + 1);
            Report(zacSize, zprCompressed, "", FullSize);
            FShowProgress = zspFull;
            BatchStarted = true;
        }
        // Read extended local Sig. first; is only present if it's a spanned archive.
        unsigned long ExtendedSig;
        if (FileRead(FInFileHandle, &ExtendedSig, 4) != 4 || ExtendedSig != ExtLocalSig)
            throw EZipBuilder(DS_ExtWrongSig);

        // Read for every zipped entry: The local header, variable data, fixed data
        // and if present the Data decriptor area.
        for (int i = 0; i < EOC.TotalEntries; i++)
        {
            if (FCancel)
                throw EZipAbort();
            // First the local header.
            while (FileRead(FInFileHandle, &LOH, sizeof(LOH)) != static_cast<int>(sizeof(LOH)))
            {
                // Check if we are at the end of a input disk not very likely but...
                if (FileSeek(FInFileHandle, 0i64, 1) != FileSeek(FInFileHandle, 0i64, 2))
                    throw EZipBuilder(DS_LOHBadRead);
                // Well, it seems we are at the end, so get a next disk.
                GetNewDisk(FDiskNr + 1);
            }
            if (LOH.HeaderSig != LocalFileHeaderSig)
                throw EZipBuilder(DS_LOHWrongSig);

            // Now the filename, should be on the same disk as the LOH record.
            ReadJoin(Buffer, LOH.FileNameLen , DS_LONameLen);

            // Change some info for later while writing the central dir.
            int k = MDZD->IndexOf(MakeString(Buffer, LOH.FileNameLen));
            MDZD->Items[k]->DiskStart = 0;
            MDZD->Items[k]->RelOffLocal = FileSeek(FOutFileHandle, 0, 1);

            // Give message and progress info on the start of this new file read.
            String MsgStr = ZipLoadStr(GE_CopyFile) +
                            SetSlash(MDZD->Items[k]->FileName, psdExternal);
            Report(zacMessage, 0, MsgStr, 0);
            long BytesToWrite = sizeof(LOH) + LOH.FileNameLen + LOH.ExtraLen + LOH.ComprSize;
            if (DescUsed(LOH))
                BytesToWrite += sizeof(DD);
            if (!UseXProgress)
                Report(zacItem, zprCentral ,
                       SetSlash(MDZD->Items[k]->FileName, psdExternal),
                       BytesToWrite);
            // Write the local header to the destination.
            WriteJoin(&LOH, sizeof(LOH), DS_LOHBadWrite);

            // Write the filename.
            WriteJoin(Buffer, LOH.FileNameLen, DS_LOHBadWrite);

            // And the extra field
            RWJoinData(Buffer, LOH.ExtraLen, DS_LOExtraLen);

            // Read Zipped data, if the size is not known use the size from the central header.
            if (!LOH.ComprSize)
                LOH.ComprSize = MDZD->Items[k]->ComprSize;
            RWJoinData(Buffer, LOH.ComprSize, DS_ZipData);

            // Read DataDescriptor if present.
            if (DescUsed(LOH))
                RWJoinData(reinterpret_cast<char *>(&DD), sizeof(DD), DS_DataDesc);
        } // Now we have written al entries to the (hard)disk.
        // Now write the central directory with changed offsets.
        if (!UseXProgress)
            Report(zacItem, zprCentral, ZipLoadStr(PR_CentrlDir), EOC.TotalEntries);
        for (int i = 0; i < EOC.TotalEntries; i++)
        {
            if (FCancel)
                throw EZipAbort();
            // Read a central header which can be span more than one disk.
            while (FileRead(FInFileHandle, &CEH, sizeof(CEH)) != static_cast<int>(sizeof(CEH)))
            {
                // Check if we are at the end of a input disk.
                if (FileSeek(FInFileHandle, 0i64, 1) != FileSeek(FInFileHandle, 0i64, 2))
                    throw EZipBuilder(DS_CEHBadRead);
                // Well, it seems we are at the end, so get a next disk.
                GetNewDisk(FDiskNr + 1);
            }
            if (CEH.HeaderSig != CentralFileHeaderSig)
                throw EZipBuilder(DS_CEHWrongSig);

            // Now the filename.
            if (CEH.FileNameLen >= MAX_PATH)
                throw EZipBuilder(DS_CENameLen);
            ReadJoin(Buffer, CEH.FileNameLen, DS_CENameLen);
            // Save the first Central directory offset for use in EOC record.
            if (!i)
                EOC.CentralOffset = static_cast<unsigned long>(FileSeek(FOutFileHandle, 0i64, 1));

            // Change the central header info with our saved information.
            int k = MDZD->IndexOf(MakeString(Buffer, CEH.FileNameLen));
            CEH.RelOffLocal = static_cast<unsigned long>(MDZD->Items[k]->RelOffLocal);
            CEH.DiskStart = 0;

            // Write this changed central header to disk
            // and make sure it fit's on one and the same disk.
            WriteJoin(&CEH, sizeof(CEH), DS_CEHBadWrite);

            // Write to destination the central filename and the extra field.
            WriteJoin(Buffer, CEH.FileNameLen, DS_CEHBadWrite);

            // And the extra field
            RWJoinData(Buffer, CEH.ExtraLen, DS_CEExtraLen);

            // And the file comment.
            RWJoinData(Buffer, CEH.FileComLen, DS_CECommentLen);
        }
        // Write the changed EndOfCentral directory record.
        EOC.CentralDiskNo = 0;
        EOC.ThisDiskNo = 0;
        WriteJoin(&EOC, sizeof(EOC), DS_EOCBadWrite);

        // Skip past the original EOC to get to the ZipComment if present.
        if (FileSeek(FInFileHandle, static_cast<__int64>(sizeof(EOC)), 1) == -1)
            throw EZipBuilder(DS_FailedSeek);

        // And finally the archive comment itself.
        RWJoinData(Buffer, EOC.ZipCommentLen, DS_EOArchComLen);
    }
    catch (EZipBuilder &ers)
    { // All ReadSpan specific errors.
        ShowExceptionError(ers);
        Result = -7;
    }
    catch (const MEMEXCEPT &me)
    {  // All memory allocation errors.
        ShowZipMessage(GE_NoMem, "");
        Result = -8;
    }
    catch (const Exception &E)
    {
        ShowZipMessage(DS_ErrorUnknown, "\n" + E.Message);
        Result = -9;
    }
    catch (...)
    {       // The remaining errors, should not occur.
        ShowZipMessage(DS_ErrorUnknown, "");
        Result = -9;
    }
    // Give final progress info at the end.
    if (BatchStarted)
        Report(zacEndOfBatch, 0, "", 0);
    delete[] Buffer;
    if (MDZD)
        delete MDZD;
    CloseFileHandle(FInFileHandle);
    if (FOutFileHandle != -1)
    {
        FileSetDate(FOutFileHandle, FDateStamp);
        CloseFileHandle(FOutFileHandle);
        if (Result)
        {
            DeleteFile(OutFilePath);
            OutFilePath = "";
        }
    }
    FUnzBusy = false;
    StopWaitCursor();
    return Result;
}
#endif

/*? TZBWorker::RecreateMVArchive-----------------------------------------------
1.79 8 Dec 2004 RA New function to recreate a multi volume archive
*/
#ifdef NO_SPAN
void __fastcall TZBWorker::RecreateMVArchive(const String& TmpZipName, bool /*Recreate*/)
{
    ShowZipMessage(DL_NoDelOnSpan, ""); //function not allowed in spanned archive
    return;
#else
void __fastcall TZBWorker::RecreateMVArchive(const String& TmpZipName, bool Recreate)
{
    if (Recreate)
    {
        try
        {
            GetNewDisk(0); // ask to enter the first disk again
        }
        catch (...)
        {
            DeleteFile(TmpZipName); // delete the temp file
            throw;  // throw last exception again
        }
        CloseFileHandle(FInFileHandle); // was opened by getnewdisk
    }

    if (UpperCase(ExtractFileExt(FZipFileName)) == ".EXE")
    {  // now we create the spanned sfx
#ifdef NO_SFX
        ShowZipMessage(DS_NoSFXSpan, "");
        return;
#else
        String outpath = FZipFileName; // for internal sfx outpath should contain filespec
        FZipFileName = TmpZipName; // sfx wants the source in zipfilename
        if (FSFXSlave->ConvertToSpanSFX(outpath))
            FSuccessCnt = 0;
        DeleteFile(TmpZipName); // delete the original archive
        FZipFileName = outpath; // set zipfilename to its original value
#endif
    }
    else
    {
        if (!Recreate)
            FNumbering = znsNone; // use spanoptions
        if (WriteSpan(TmpZipName, FZipFileName, true))
            FSuccessCnt = 0;
        DeleteFile(TmpZipName);
    }
#endif
}

/*? TZBWorker::RejoinMVArchive-------------------------------------------------
1.79 8 Dec 2004 RA New function to rejoin a multi volume archive
returns 0 if OK else FErrCode or ReadSpan errors
*/
#ifdef NO_SPAN
int __fastcall TZBWorker::RejoinMVArchive(String& /*TmpZipName*/, bool /*Rejoin*/)
{
    ShowZipMessage(DL_NoDelOnSpan, ""); //function not allowed in spanned archive
    return FErrCode;
#else
int __fastcall TZBWorker::RejoinMVArchive(String& TmpZipName, bool Rejoin)
{
    TmpZipName = MakeTempFileName("", "");
    if (Verbose)
    {
        TMsgEvent tmpMessage = FEvents.FOnMessage;
        if (tmpMessage)
            tmpMessage(Builder, 0, ZipFmtLoadStr(GE_TempZip, ARRAYOFCONST((TmpZipName))));
    }
    if (Rejoin)
    {
        // rebuild a temp archive
        if (FileExists(FZipFileName) && (QueryZip(FZipFileName) == ZB_EXE_DET_HEAD))
            FInFileName = ChangeFileExt(FZipFileName, ".zip"); // detached sfx
        else
            FInFileName = FZipFileName;
        if (ReadSpan(FInFileName, TmpZipName, true) < 0)
        {
            DoDllUnload();
            return FErrCode;
        }
        FAnswerAll = FAnswerAll << zaaYesOvrwrt;
    }
    return 0;
#endif
}

//------------------------------------------------------------------------------
void   __fastcall TZBWorker::RemoveDriveSpec(String& FullPathName)
{
    if (!ExtractFileDrive(FullPathName).IsEmpty())
    {
        int position = FullPathName.Pos("\\");
        FullPathName = FullPathName.SubString(position + 1, FullPathName.Length() - position);
    }
}

//  return true if contains chars  >#126
bool StrHasExt(const String& astr)
{
  for(int i = 1; i < astr.Length(); ++i)
    if(astr[i] > 126) return true;
  return false;
}
#ifdef UNICODE
bool StrHasExt(const AnsiString& astr)
{
  for(int i = 1; i < astr.Length(); ++i)
    if(astr[i] > 126) return true;
  return false;
}
#endif

// return true if filename is obviously invalid
bool NameIsBad(const String& astr)
{
  if(astr.IsEmpty() || (astr[1] == ' ') || (astr[1] == '\\') || (astr.Length() > MAX_PATH)) return true;
  for(int i = 1; i < astr.Length(); ++i)
      if(CharInSet(astr[i], Schars)) return true;
  return ((AnsiPos("..", astr) > 0) ||
      (AnsiPos("\\ ", astr) > 0) || (AnsiPos(" \\", astr) > 0));
}

/*? TZBWorker::Rename----------------------------------------------------------
1.79.10.07 17 DEC 08 RP control match/replace
1.79.08.02 19 AUG 07 RP don't allow date/time change for encoded files
1.79 3 January 2006 RP handle files > 2G
1.79 12 June 2005 RA changed error handling
1.78 18 Mar 2005 RP Fix bug with only data descriptor uncompressed size
1.79 23 Dec 2004 RA added use test if destination is writable
1.79 20 Dec 2004 RA changed exception handling during opening
1.79 8 Dec 2004 RA Allow Rename in MV archive
1.77 20 August 2004 RA progress adjusted
1.75 18 February 2004 RP allow >2G
1.73.3.2 11 October 2003 RP changed comment variable
1.73.3.2 8 Oct 2003 RA if filename has extended chars change HostVer + extfileattrib
1.73 23 september 2003 RA changing date/time of protected file is not allowed
1.73 7 August 2003 RA init FOutFileHandle
1.73 (16 July 2003) RP use SetSlash + ConvertOEM
1.73 (14 July 2003) RA convertion/re-convertion of filenames with OEM chars
Function to read a Zip archive and change one or more file specifications.
Source and Destination should be of the same type. (path or file)
If NewDateTime is 0 then no change is made in the date/time fields.
Return values:
0            All Ok.
-7           Rename errors. See ZipMsgXX.rc
-8           Memory allocation error.
-9           General unknown Rename error.
-10          Dest should also be a filename.
*/
int __fastcall TZBWorker::Rename(TList &RenameList, int DateTime, TZBRenameOpts How)
{
    ZipEndOfCentral    EOC;
    ZipCentralHeader   CEH;
    ZipLocalHeader     LOH;
    String             HeaderName;
    String             MsgStr;
    String             OutFilePath;
    String TmpZipName;
    AnsiString FNameA;
    Byte* Buffer = NULL;
    __int64 FullSize;
    unsigned TotalBytesWrite;
    int Result = 0;
    ZipRenameRec  *RenRec;
    TMZipDataList* MDZD = NULL;
    FShowProgress = zspNone;
    FInFileName   = FZipFileName;
    FInFileHandle = -1;
    FOutFileHandle = -1;
    FSuccessCnt = FErrCode = 0;
    FZipBusy = true;
    StartWaitCursor();
    try
    {
        // If we only have a source path make sure the destination is also a path.
        for (int i = 0; i < RenameList.Count; i++)
        {
            RenRec = (ZipRenameRec *)RenameList.Items[i];
            RenRec->Source = SetSlash(RenRec->Source, psdExternal);
            RenRec->Dest   = SetSlash(RenRec->Dest, psdExternal);
            RemoveDriveSpec(RenRec->Source);
            RemoveDriveSpec(RenRec->Dest);
            // if dest is empty make same as source (no change)
    				if(RenRec->Dest.IsEmpty())
                   RenRec->Dest = RenRec->Source;
            if (RenRec->Source.POS("*") || RenRec->Source.POS("?") ||
                NameIsBad(RenRec->Dest))
                throw EZipBuilder(AD_InvalidName);
            if (!ExtractFileName(RenRec->Source).Length())
            { // Assume it's a path.
                // Make sure destination is a path also.
                RenRec->Dest   = SetSlash(ExtractFilePath(RenRec->Dest), psdInternal);
                RenRec->Source = SetSlash(RenRec->Source, psdInternal);
            }
            else
                if (!ExtractFileName(RenRec->Dest).Length())
                {
                    StopWaitCursor();
                    FZipBusy = false;
                    return -10;  // Dest should also be a filename.
                }
        }
        CheckDestWritable(FZipFileName, false);
#ifndef NO_SPAN
        if (FileExists(FZipFileName) && QueryZip(FZipFileName) == ZB_EXE_DET_HEAD)
#ifdef NO_SFX
            throw EZipBuilder(SF_NoSFXSupport);
#else
            FInFileName = ChangeFileExt(FZipFileName, ".zip");
#endif
        else
            FInFileName = FZipFileName;
        // Locate last of multi volume or last disk of spanned
        if (GetLastVolume(FInFileName, EOC, false) == 1)
            throw EZipAbort();
#else
        FInFileName = FZipFileName;
        if (!FileExists(FZipFileName))
            throw EZipBuilder(GE_NoZipSpecified);
        OpenEOC(EOC, false);
        if (FIsSpanned)
            throw EZipBuilder(RN_NoRenOnSpan); //function not allowed in spanned archive
#endif
        if (FInFileHandle == -1) // was problem
        {
            StopWaitCursor();
            FZipBusy = false;
            return -7;
        }
        CloseFileHandle(FInFileHandle); // was opened by OpenEoc
        // Get the date-time stamp and save for later
        FDateStamp = FileGetDate(FInFileHandle);
        Buffer = new Byte[FBufSize];
        // Check the input file.
        if ((OutFilePath = MakeTempFileName("", "")).IsEmpty())
            throw EZipBuilder(DS_NoTempFile);
        // Create the output file.
        if ((FOutFileHandle = FileCreate(OutFilePath)) == -1)
            throw EZipBuilder(DS_NoOutFile);
        // Now we now the number of zipped entries in the zip archive
#ifdef NO_SPAN
        FTotalDisks = EOC.ThisDiskNo;
        if (EOC.ThisDiskNo)
            throw EZipBuilder(RN_NoRenOnSpan);
#else
        if (FIsSpanned)
        {
            int RenOutFileHandle = FOutFileHandle; // save handle
            CloseFileHandle(FInFileHandle); // was opened by OpenEOC
            if (RejoinMVArchive(TmpZipName, true))
            {
                CloseFileHandle(RenOutFileHandle);
                DeleteFile(OutFilePath);
                if (FileExists(TmpZipName))
                    DeleteFile(TmpZipName);
                delete [] Buffer;
                StopWaitCursor();
                FZipBusy = false;
                return -7;
            }
            //open the temp file and read the EOC
            FInFileName = TmpZipName; // use the re-joined file as zipfile
            FOutFileHandle = RenOutFileHandle; // restore handle
        }
#endif
        OpenEOC(EOC, true);
        // Go to the start of the input file.
        if (FileSeek(FInFileHandle, 0i64, 0) == -1)
            throw EZipBuilder(DS_FailedSeek);

        // Write the SFX header if present.
        if (FSFXOffset == 4)
            FSFXOffset = 0; // this is not an SFX but the ExtLocSig
        if (CopyBuffer(FInFileHandle, FOutFileHandle, FSFXOffset))
            throw EZipBuilder(RN_ZipSFXData);

        // Go to the start of the Central directory.
        if (FileSeek(FInFileHandle, static_cast<__int64>(EOC.CentralOffset), 0) == -1)
            throw EZipBuilder(DS_FailedSeek);

        MDZD = new TMZipDataList(EOC.TotalEntries);  // create class
        FullSize = EOC.CentralSize + EOC.ZipCommentLen + sizeof(ZipEndOfCentral);
        // Read for every entry: The central header and save information for later use.
        for (int i = 0; i < EOC.TotalEntries; i++)
        {
            // Read a central header.
            ReadJoin(&CEH, sizeof(CEH), DS_CEHBadRead);
            if (CEH.HeaderSig != CentralFileHeaderSig)
                throw EZipBuilder(DS_CEHWrongSig);

            // Now the filename.
            FNameA.SetLength(CEH.FileNameLen);
            ReadJoin(FNameA.c_str(), CEH.FileNameLen, DS_CENameLen);
            String FName = FNameA;
            // Save the file name info in the MDZD structure.
            //convert OEM char set in original file else we don't find the file
            FHostNum = CEH.VersionMadeBy1;
            FHostVer = CEH.VersionMadeBy0;
            FName = ConvertOEM(FName, cpdOEM2ISO);
            MDZD->Items[i]->FileName = FName;
            MDZD->Items[i]->VerMade = static_cast<short>((FHostNum << 8) + FHostVer);
            MDZD->Items[i]->RelOffLocal = CEH.RelOffLocal;
            MDZD->Items[i]->ComprSize  = CEH.ComprSize;   // RP may not be valid in local header
            // if encrypted use CRC32 as flag
            MDZD->Items[i]->CRC32 = CEH.Flag & 1;
            MDZD->Items[i]->DateTime    = DateTime;
            // We need the total number of bytes we are going to read for the progress event.
            FullSize += sizeof(LOH) + CEH.ComprSize + CEH.FileNameLen + CEH.ExtraLen;
            // plus DataDescriptor if present.
            if (DescUsed(CEH))
                FullSize += sizeof(ZipDataDescriptor);
            // Seek past the extra field and the file comment.
            if (FileSeek(FInFileHandle, static_cast<__int64>(CEH.ExtraLen + CEH.FileComLen), 1) == -1)
                throw EZipBuilder(DS_FailedSeek);
        }
        FShowProgress = zspFull; //true;
        Report(zacCount, zprCompressed, "", EOC.TotalEntries);
        Report(zacSize, zprCompressed, "", FullSize);
        // Read for every zipped entry: The local header, variable data, fixed data
        // and if present the Data descriptor area.
        for (int i = 0; i < EOC.TotalEntries; i++)
        {
            // Seek to the first/next entry.
            FileSeek(FInFileHandle, static_cast<__int64>(MDZD->Items[i]->RelOffLocal), 0);

            // First the local header.
            ReadJoin(&LOH, sizeof(LOH), DS_LOHBadRead);
            if (LOH.HeaderSig != LocalFileHeaderSig)
                throw EZipBuilder(DS_LOHWrongSig);

            // Now the filename.
            ReadJoin(Buffer, LOH.FileNameLen, DS_LONameLen);
            // Set message info on the start of this new fileread now we still have the old filename.
            MsgStr = ZipLoadStr(RN_ProcessFile) + MDZD->Items[i]->FileName;
            // Check if the original path and/or filename needs to be changed.
            HeaderName = SetSlash(MDZD->Items[i]->FileName, psdExternal);
            for (int m = 0; m < RenameList.Count; m++)
            {
                RenRec = (ZipRenameRec *)RenameList.Items[m];
                String RenSource = RenRec->Source;
                int k;
                if(How == htrFull)
                  	k = CompareText(RenSource, HeaderName) ? 0 : 1;
                else k = UpperCase(HeaderName).Pos(UpperCase(RenRec->Source));
                if (k)
                {
                    HeaderName.Delete(k, RenSource.Length());
                    HeaderName.Insert(RenRec->Dest, k);
                    LOH.FileNameLen = static_cast<Word>(HeaderName.Length());
                    for (k = 1; k <= HeaderName.Length(); k++)
                        if (HeaderName[k] == '\\')
                            HeaderName[k] = '/';
                    MsgStr += ZipLoadStr(RN_RenameTo) + HeaderName;
          					//**new convert the comment
          					MDZD->Items[i]->NComment = "";
          					if(!RenRec->Comment.IsEmpty())
          					{
            					if((RenRec->Comment.Length() == 1) && (RenRec->Comment[1] == '\0'))
                      	MDZD->Items[i]->NComment = '\0'; // RenRec^.Comment
            					else
              					MDZD->Items[i]->NComment = AnsiString(ConvertOEM(RenRec->Comment, cpdISO2OEM));
          					}
          					bool HasExt = StrHasExt(HeaderName) || StrHasExt(MDZD->Items[i]->NComment);
          						//we replaced the filename/comment look if we need to reconvert it
          					if((MDZD->Items[i]->VerMade & 0xFF) <= 22)
                    {
                      if(HasExt) MDZD->Items[i]->VerMade = 0x0B16; // NTFS 22
                      else MDZD->Items[i]->VerMade = 20;   // DOS 20
                      FHostNum = static_cast<Byte>((MDZD->Items[i]->VerMade & 0xFF00) << 8);
                      FHostVer = static_cast<Byte>(MDZD->Items[i]->VerMade & 0xFF);
                    }
                    else
                    {
                      FHostNum = static_cast<Byte>((MDZD->Items[i]->VerMade & 0xFF00) << 8);
                      FHostVer = static_cast<Byte>(MDZD->Items[i]->VerMade & 0xFF);
                    }
                    HeaderName = ConvertOEM(HeaderName, cpdISO2OEM);
                    MDZD->Items[i]->FileName = HeaderName;

                    // Change Date and Time if needed.
                    try
                    {
                        if (RenRec->DateTime)
                        {
                            // not allowed to change date on protected file
                            if (MDZD->Items[i]->CRC32)
                                throw EZipBuilder(RN_InvalidDateTime);
                            // test if valid date/time will throw error if not
                            FileDateToDateTime(RenRec->DateTime);
                            MDZD->Items[i]->DateTime = RenRec->DateTime;
                        }
                    }
                    catch (...)
                    {
											ShowZipMessage(RN_InvalidDateTime, MDZD->Items[i]->FileName);
                    }
                   if(How != htrDefault) break; // only allow single change
                }
            }
            Report(zacMessage, 0, MsgStr, 0);

            // Change Date and/or Time if needed.
            if (MDZD->Items[i]->DateTime)
            {
                LOH.ModifDate = HIWORD(MDZD->Items[i]->DateTime);
                LOH.ModifTime = LOWORD(MDZD->Items[i]->DateTime);
            }
            // Change info for later while writing the central dir.
            MDZD->Items[i]->RelOffLocal = FileSeek(FOutFileHandle, 0i64, 1);
            // Calculate the bytes we are going to write;
            TotalBytesWrite = sizeof(LOH) + LOH.FileNameLen + LOH.ExtraLen + //LOH.ComprSize;
                              MDZD->Items[i]->ComprSize;
            Report(zacItem, 0, SetSlash(MDZD->Items[i]->FileName, psdExternal), TotalBytesWrite);
            LOH.FileNameLen = static_cast<Word>(MDZD->Items[i]->FileNameA.Length()); // may have changed
            // Write the local header to the destination.
            WriteJoin(&LOH, sizeof(LOH), DS_LOHBadWrite);

            // Write the filename.
            WriteJoin(MDZD->Items[i]->FileNameA.c_str(), LOH.FileNameLen, DS_LOHBadWrite);
            // And the extra field
            if(LOH.ExtraLen && CopyBuffer(FInFileHandle, FOutFileHandle, LOH.ExtraLen))
                throw EZipBuilder(DS_LOExtraLen);

            // Read and write Zipped data
            if (CopyBuffer(FInFileHandle, FOutFileHandle, MDZD->Items[i]->ComprSize))
                throw EZipBuilder(DS_ZipData);

            // Read DataDescriptor if present.
            if (LOH.Flag & 0x0008)
                if (CopyBuffer(FInFileHandle, FOutFileHandle, sizeof(ZipDataDescriptor)))
                    throw EZipBuilder(DS_DataDesc);
        } // Now we have written al entries.

        // Now write the central directory with possibly changed offsets and filename(s).
        FShowProgress = zspNone; //false;
        for (int i = 0; i < EOC.TotalEntries; i++)
        {
            // Read a central header.
            ReadJoin(&CEH, sizeof(CEH), DS_CEHBadRead);
            if (CEH.HeaderSig != CentralFileHeaderSig)
                throw EZipBuilder(DS_CEHWrongSig);

            // Change Date and/or Time if needed.
            if (MDZD->Items[i]->DateTime)
            {
                CEH.ModifDate = HIWORD(MDZD->Items[i]->DateTime);
                CEH.ModifTime = LOWORD(MDZD->Items[i]->DateTime);
            }
            // Now the filename.
            if (CEH.FileNameLen >= MAX_PATH)
                throw EZipBuilder(DS_CENameLen);
            ReadJoin(Buffer, CEH.FileNameLen, DS_CENameLen);

            // Save the first Central directory offset for use in EOC record.
            if (!i)
                EOC.CentralOffset = static_cast<unsigned long>(FileSeek(FOutFileHandle, 0i64, 1));

            // Change the central header info with our saved information.
            CEH.RelOffLocal = static_cast<unsigned long>(MDZD->Items[i]->RelOffLocal);
            Word len = static_cast<Word>(MDZD->Items[i]->FileNameA.Length());
            EOC.CentralSize = EOC.CentralSize - CEH.FileNameLen + len;
            CEH.FileNameLen = len;
            //check if new filename has extended chars if it is we need to set ExtFileAttributes
            // and update version made by
            if (HasExtendedChars(MDZD->Items[i]->FileNameA) && !(MDZD->Items[i]->DiskStart & 0xFF00))
            {
                CEH.VersionMadeBy0 = DEF_HOST_VER & 0xFF;
                CEH.VersionMadeBy1 = (DEF_HOST_VER & 0xFF00) >> 8;
                CEH.ExtFileAtt = (((CEH.ExtFileAtt & FILE_ATTRIBUTE_DIRECTORY ? S_IFDIR + S_IEXEC : S_IFREG) +
                                   S_IREAD +
                                   (CEH.ExtFileAtt & FILE_ATTRIBUTE_READONLY ? 0 : S_IWRITE)) << 16) +
                                 CEH.ExtFileAtt;
            }
            else
            {
                CEH.VersionMadeBy0 = static_cast<unsigned char>(MDZD->Items[i]->DiskStart & 0xFF) ;
                CEH.VersionMadeBy1 = static_cast<unsigned char>((MDZD->Items[i]->DiskStart & 0xFF00) >> 8);
            }
                  //**new set the comment
      			Word OldComLen = CEH.FileComLen;
      			if(!MDZD->Items[i]->NComment.IsEmpty())
      			{
        			if((MDZD->Items[i]->NComment.Length() == 1) && (MDZD->Items[i]->NComment[1] == '\0'))
          			CEH.FileComLen = 0;    // none
        			else
          			CEH.FileComLen = static_cast<Word>(MDZD->Items[i]->NComment.Length() & 0xFF); // limit 255 chars
        			EOC.CentralSize += CEH.FileComLen - OldComLen;
      			}
            // Write this changed central header to disk
            WriteJoin(&CEH, sizeof(CEH), DS_CEHBadWrite);

            // Write to destination the central filename.
            WriteJoin(MDZD->Items[i]->FileNameA.c_str(), CEH.FileNameLen, DS_CEHBadWrite);
            // And the extra field
            if(CEH.ExtraLen && CopyBuffer(FInFileHandle, FOutFileHandle, CEH.ExtraLen))
                throw EZipBuilder(DS_CEExtraLen);

            // And the file comment.
            if(!MDZD->Items[i]->NComment.IsEmpty())
      			{
        			if(OldComLen > 0)
        				{
          			// read and discard the old comment
          				if(OldComLen > BufSize)
                  	Buffer = new Byte[OldComLen];
          				ReadJoin(Buffer, OldComLen, DS_CECommentLen);
                  delete[] Buffer;
        				}
              if(CEH.FileComLen > 0)
          				WriteJoin(MDZD->Items[i]->NComment.c_str(), CEH.FileComLen, DS_CEHBadWrite);
            }
      			else if(OldComLen && CopyBuffer(FInFileHandle, FOutFileHandle, CEH.FileComLen))
                throw EZipBuilder(DS_CECommentLen);
        }
        // Write the changed EndOfCentral directory record.
        EOC.CentralDiskNo = 0;
        EOC.ThisDiskNo = 0;
        WriteJoin(&EOC, sizeof(EOC), DS_EOCBadWrite);

        // Skip past the original EOC to get to the ZipComment if present. v1.52m
        if (FileSeek(FInFileHandle, static_cast<int>(sizeof(EOC)), 1) == -1)
            throw EZipBuilder(DS_FailedSeek);

        // And finally the archive comment
        // ==================Changed By Jim Turner=======
        if (!FEOCComment.IsEmpty())
    		{
     			FNameA = AnsiString(FEOCComment);
    			if(FileWrite(FOutFileHandle, FNameA.c_str(), FEOCComment.Length()) < 0)
      		     throw EZipBuilder(DS_EOArchComLen, true);
    		}
    }
    catch (EZipBuilder &ers)
    { // All Rename specific errors..
        ShowExceptionError(ers);
        Result = -7;
    }
    catch (const MEMEXCEPT &me)
    {  // All memory allocation errors.
        ShowZipMessage(GE_NoMem, "");
        Result = -8;
    }
    catch (const Exception &E)
    {
        ShowZipMessage(DS_ErrorUnknown, "\n" + E.Message);
        Result = -9;
    }
    catch (...)
    {       // The remaining errors, should not occur.
        ShowZipMessage(DS_ErrorUnknown, "");
        Result = -9;
    }
    delete[] Buffer;
    if (MDZD)
        delete MDZD;

    // Give final progress info at the end.
    Report(zacEndOfBatch, 0, "", 0);
    CloseFileHandle(FInFileHandle);
    if (FOutFileHandle != -1)
    {
        FileSetDate(FOutFileHandle, FDateStamp);
        FileClose(FOutFileHandle);
        if (Result)
            DeleteFile(OutFilePath);  // An error somewhere, OutFile is not reliable.
        else
        {
            if (FIsSpanned)
            {
#ifndef NO_SPAN
                try
                {
                    DeleteFile(TmpZipName); // remove the temporary input file
                    FInFileName = ChangeFileExt(FZipFileName, ".zip"); // in case of detached sfx
                    if (FNumbering == znsName)
                        CreateMVFileName(FInFileName, false, false);
                    RecreateMVArchive(OutFilePath, true); // recreate the MV starting from the result
                }
                catch (EZipBuilder &ers)
                { // All Rename specific errors..
                    ShowExceptionError(ers);
                    Result = -7;
                }
                catch (...)
                {       // The remaining errors, should not occur.
                    ShowZipMessage(DS_ErrorUnknown, "");
                    Result = -9;
                }
#endif
            }
            else
            {
                EraseFile(FZipFileName, FHowToDelete);
                RenameFile(OutFilePath, FZipFileName);
                List();
            }
        }
    }
    DeleteFile(TmpZipName); // remove the temporary input file
    FZipBusy = false;
    StopWaitCursor();
    return Result;
}
// TZBWorker::Rename

#ifndef NO_SPAN
/*? TZBWorker::RWJoinData----------------------------------------------------
1.79 3 January 2006 RP copy > 2G
*/
void __fastcall TZBWorker::RWJoinData(char *Buffer, unsigned ReadLen, int DSErrIdent)
{
    int SizeR;
    while (ReadLen > 0)
    {
        int ToRead = FBufSize;
        if (ReadLen < static_cast<unsigned>(FBufSize))
            ToRead = ReadLen;
        if ((SizeR = FileRead(FInFileHandle, Buffer, ToRead)) != ToRead)
        {
            // Check if we are at the end of a input disk.
            if (FileSeek(FInFileHandle, 0, 1) != FileSeek(FInFileHandle, 0, 2))
                throw EZipBuilder(DSErrIdent);
            // It seems we are at the end, so get a next disk.
            GetNewDisk(FDiskNr + 1);
        }
        if (SizeR > 0)
        { // Fix by Scott Schmidt v1.52n
            WriteJoin(Buffer, SizeR, DSErrIdent);
            ReadLen -= static_cast<unsigned>(SizeR);
        }
    }
}
#endif

#ifndef NO_SPAN
/*? TZBWorker.RWSplitData --------------------------------------------------
1.79 3 January 2006 RP copy > 2G
Read data from the input file with a maximum of 8192(BufSize) bytes per read
and write this to the output file.
In case of an error an EZipBuilder Exception is thrown and this will
be caught in WriteSpan.
*/
void __fastcall TZBWorker::RWSplitData(char *Buffer, unsigned ReadLen, int ZSErrVal)
{
    unsigned SizeR;
    while (ReadLen > 0)
    {
        unsigned ToRead = FBufSize;
        if (ReadLen < static_cast<unsigned>(FBufSize))
            ToRead = ReadLen;
        if ((SizeR = FileRead(FInFileHandle, Buffer, ToRead)) != ToRead)
            throw EZipBuilder(ZSErrVal);
        WriteSplit(Buffer, SizeR, 0);
        ReadLen -= SizeR;
    }
}
#endif

/*? TZBWorker::SetActive
1.76
*/
void __fastcall TZBWorker::SetActive(bool Value)
{
    if (Active != Value)
    {
        FActive = Value;
        if (FActive)
        {      // do delayed action
            if (FDelaying.Contains(zdiList))
                List();
            if (FDelaying.Contains(zdiComment))
                SetZipComment(FZipComment);
        }
        FDelaying.Clear();
    }
}

//------------------------------------------------------------------------------
// Add suffixes not contained in the set
void __fastcall TZBWorker::SetExtAddStoreSuffixes(const String Value)
{
    if (!Value.IsEmpty())
    {
        int i = 1;
        String str;
        TCHAR c = ':';
        int Len = Value.Length();
        while (i <= Len)
        {
            if ((c = Value[i]) != '.')
                str += ".";
            while ((c != ':') && (i <= Len))
            {
                c = (Value.IsDelimiter(",;:", i)) ? ':' : Value[i];
                str += c;
                ++i;
            }
        }
        if (c != ':')
            str += ":";
        FAddStoreSuffixes << assEXT;
        FExtAddStoreSuffixes = LowerCase(str);
    }
    else
    {
        FExtAddStoreSuffixes = "";
        FAddStoreSuffixes >> assEXT;
    }
}

/*? TZBWorker::SetCancel-------------------------------------------------------
1.76 10 May 2004 remove AbortDlls
1.76 28 April 2004 add side effect of aborting dll when setting true
1.73 (14 June 2003) RP did not work with multithread dlls
*/
void __fastcall TZBWorker::SetCancel(bool Value)
{
    if (Cancel != Value)
        inherited::SetCancel(Value);
}
// TZBWorker::SetCancel

void __fastcall TZBWorker::SetDirHashs(const TZipDirHashList* Value)
{
  FDirHashs = const_cast<TZipDirHashList*>(Value);
}

/*? TZBulder::SetFileName---------------------------------------------------------------------------
1.76 27 April 2004 List controlled by 'Inactive'
*/
void __fastcall TZBWorker::SetFileName(const String Value)
{
    FZipFileName = Value;
    // Automatically build a new TList of contents in "ZipContents".
    List();
}
//---------------------------------------------------------------------------
void __fastcall TZBWorker::SetPasswordReqCount(unsigned int prc)
{
    if (prc != FPasswordReqCount)
    {
      if(prc >15) prc = 15;
      if(prc < 1) prc = 1;
    	FPasswordReqCount = prc;
    }
}

// SetUnZipSwitches---------------------------------------------------------------
void __fastcall TZBWorker::SetUnZipSwitches(String &NameOfZipFile, pUnZipParms uzParms)
{
    uzParms->fVersion = DELZIPVERSION;
    uzParms->fCaller  = this;    // point to our VCL instance; returned in Report.

    /* Since we don't want the DLLs to generate their own dialog boxes
     for error reporting, we'll set the window handle. Since we
     are doing this, we must also set fQuiet to true.     */
    uzParms->fHandle = FUnattended ? 0 : Handle;    // Used for DLL dialogs - esp: password.
    uzParms->fQuiet  = true;      // We'll report errors upon notification in our Report.
    uzParms->ZCallbackFunc = ZCallback; // Pass addr of function to be called from DLL.

    uzParms->fWantedCodePage = FCodePage;
    uzParms->fTraceEnabled  = FTrace;
    uzParms->fVerboseEnabled = FVerbose;
    if (uzParms->fTraceEnabled && !uzParms->fVerboseEnabled)
        uzParms->fVerboseEnabled = true; // If tracing, we want verbose also.

    uzParms->fComments = false;    // Zipfile comments - not supported.
    uzParms->fConvert = false;    // ASCII/EBCDIC conversion - not supported.
    uzParms->fDirectories = FExtrOptions.Contains(ExtrDirNames);
    uzParms->fOverwrite = FExtrOptions.Contains(ExtrOverWrite);
    uzParms->fFreshen = FExtrOptions.Contains(ExtrFreshen);
    uzParms->fUpdate = FExtrOptions.Contains(ExtrUpdate);
    uzParms->fTest = FExtrOptions.Contains(ExtrTest);
    // Update has precedence over freshen.
    if (uzParms->fFreshen && uzParms->fUpdate)
        uzParms->fFreshen = false;
    uzParms->pZipFN = MakeStrP(NameOfZipFile);

    uzParms->fPwdReqCount = FPasswordReqCount;
    // We have to be carefull doing an unattended Extract when a password is needed
    // for some file in the archive. We set it to an unlikely password, this way
    // encrypted files won't be extracted.
    // From verion 1.60 and up the event OnPasswordError is called in this case.
    uzParms->pZipPassword = MakeStrP(FPassword);
}
// SetUnZipSwitches

// TZBWorker::Setup_extend-------------------------------------------------------
// 1.73.3.2 10 Oct 2003 RA New function to init upper table
void __fastcall TZBWorker::Setup_extend(void)
{
    unsigned char* p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(extend));
    for (int i = 0; i < 256; ++i)
        *p++ = static_cast<unsigned char>(i);
    CharToOemBuffA(extend + 1, const_cast<char*>(extend + 1), 255);
    p = const_cast<unsigned char*>(reinterpret_cast<const unsigned char*>(extend));
    for (int i = 0; i < 256; ++i)
        *p = (*p++ != static_cast<unsigned char>(i));
}
//TZBWorker::Setup_extend

/*? TZBWorker::SetZipComment---------------------------------------------------
1.78 4 Oct 2004 RA add test fileexists for NO_SPAN
1.76 27 April 2004 test Active
1.75 18 February 2004 allow >2G
1.73.3.2 11 Oct 2003 RP allow preset comment
1.73 (10 June 2003) RA bugfix
changed 1.73 (22 May 2003) RA use of ReadJoin
*/
void __fastcall TZBWorker::SetZipComment(const String zComment)
{
    struct ZipEndOfCentral  EOC;
    bool Fatal = false;
    FInFileHandle = -1;
    if (!Active)
    {
        FDelaying = FDelaying  << zdiComment;
        FZipComment = zComment;
        return;
    }
    try
    {
        if (!FZipFileName.Length())
            throw EZipBuilder(GE_NoZipSpecified);
#ifndef NO_SPAN
        // for multi volume find last volume and allow that file does not exists
        GetLastVolume(FZipFileName, EOC, true); // wil read existing comment
#else
        if (FileExists(FZipFileName))
            OpenEOC(EOC, false);
#endif
        FZipComment = zComment;
        if (FInFileHandle != -1)
        {
          CloseFileHandle(FInFileHandle); // must reopen for read/write
          AnsiString CommentBuf;
          if(zComment.Length() > 0) CommentBuf = AnsiString(ConvertOEM(zComment, cpdISO2OEM));
          if(CommentBuf != FEOCComment)
          {
            int len = CommentBuf.Length();
            if ((static_cast<__int64>(FFileSize) + len + sizeof(EOC)) > 0xFFFFFFFE)
                throw EZipBuilder(LI_FileTooBig);
            FInFileHandle = FileOpen(FInFileName, fmShareDenyWrite | fmOpenReadWrite);
            if (FInFileHandle == -1)
                throw EZipBuilder(DS_FileOpen);
            __int64 FileOfs = FZipEOC; // convert to 64 bit
            if (FileSeek(FInFileHandle, FileOfs, 0) == -1)
                throw EZipBuilder(DS_FailedSeek);
            ReadJoin(&EOC, sizeof(EOC), DS_EOCBadRead);
            EOC.ZipCommentLen = static_cast<short>(len);
            FileOfs = -static_cast<__int64>(sizeof(EOC));
            if (FileSeek(FInFileHandle, FileOfs, 1) == -1)
                throw EZipBuilder(DS_FailedSeek);
            Fatal = true;
            if (FileWrite(FInFileHandle, &EOC, sizeof(EOC)) != static_cast<int>(sizeof(EOC)))
                throw EZipBuilder(DS_EOCBadWrite);
            if((len > 0) && (FileWrite(FInFileHandle, FZipComment.c_str(), len) != len))
                throw EZipBuilder(DS_NoWrite);
            Fatal = false;
            // if SetEOF fails we get garbage at the end of the file, not nice but
            // also not important.
            SetEndOfFile((HANDLE)FInFileHandle);
          }
        }
    }
    catch (EZipBuilder &ezl)
    {
        ShowExceptionError(ezl);
        FZipComment = "";
    }
    catch (const MEMEXCEPT &me)
    {
        ShowZipMessage(GE_NoMem, "");
        FZipComment = "";
    }
    CloseFileHandle(FInFileHandle);
    if (Fatal) // Try to read the zipfile, maybe it still works.
        List();
}
// TZBWorker::SetZipComment

/*? SetZipSwitches---------------------------------------------------------------
1.75 13 March 2004 RP - supply app window handle
1.73  1 August 2003 RP - set required dll interface version
*/
void __fastcall TZBWorker::SetZipSwitches(String &NameOfZipFile, PZipParms zpParms)
{
    if (FZipComment.Length())
        zpParms->fArchComment = MakeStrP(FZipComment);
    zpParms->fArchiveFilesOnly = FAddOptions.Contains(AddArchiveOnly);
    zpParms->fResetArchiveBit = FAddOptions.Contains(AddResetArchive);
    // Copy the list of file specs. to exclude to a char pointer array.
    if (FFSpecArgsExcl->Count)
    {
        zpParms->fTotExFileSpecs = FFSpecArgsExcl->Count;
        zpParms->fExFiles = new ExcludedFileSpec[FFSpecArgsExcl->Count];
        for (int i = 0; i < FFSpecArgsExcl->Count; i++)
        {
            memset(zpParms->fExFiles + i, 0, sizeof(ExcludedFileSpec));
            zpParms->fExFiles[i].fFileSpec = MakeStrP(FFSpecArgsExcl->Strings[i]);
        }
    }
    if (FHowToDelete == htdAllowUndo)
        zpParms->fHowToMove = 1;
    // allow OEM2ISo conversion in Delete function
    zpParms->fWantedCodePage = FCodePage;
    if (FTempDir.Length())
    {   // Name of temp directory.
        zpParms->fTempPath = MakeStrP(FTempDir);
    }
    zpParms->fVersion = DELZIPVERSION;
    zpParms->fCaller = this;    // point to our VCL instance; returned in callback.

    /* Since we don't want the DLL's to generate their own dialog boxes
     for error reporting, we'll set the window handle. Since we
     are doing this, we must also set fQuiet to true. */
    zpParms->fHandle = Handle;    // Used for DLL dialogs - esp: password.
    zpParms->fVCLVer = DELZIPVERSION;
    zpParms->fQuiet  = true;       // We'll report errors upon notification in our callback.
    zpParms->ZCallbackFunc = ZCallback; // Pass addr of function to be called from DLL.
    zpParms->fJunkSFX = false;    // if True, Convert input .EXE file to .ZIP.
    String SufStr = AddStoreExtStr(FAddStoreSuffixes);
    if (SufStr.Length())
    {
        SufStr.Delete(SufStr.Length(), 1);
        zpParms->pSuffix = MakeStrP(SufStr);
    }
    // if True, Include volume label from root dir.
    zpParms->fVolume = FAddOptions.Contains(AddVolume);

    zpParms->fExtra  = false;  // if True, Include extended file attributes.
    // if True, Exclude files earlier than specified date.
    zpParms->fUseDate = FAddOptions.Contains(AddFromDate);
    zpParms->fDate = DateTimeToFileDate(FFromDate);
    zpParms->fLevel = FAddCompLevel;  // Compression level (0 - 9, 0=none and 9=best).
    zpParms->fCRLF_LF = false;  // if True, Translate text file CRLF to LF (if dest Unix).
    zpParms->fGrow = !FAddOptions.Contains(AddSafe);
    zpParms->fDeleteEntries = false;  // distinguish between Add and Delete
    zpParms->fTraceEnabled = FTrace;
    zpParms->fVerboseEnabled = FVerbose;
    if (zpParms->fTraceEnabled && !FVerbose)
        zpParms->fVerboseEnabled = true; // If tracing, we want verbose also.

    // Convert all filenames to 8x3 format.
    zpParms->fForce = FAddOptions.Contains(AddForceDOS);
    // Make zipfile's timestamp same as newest file.
    zpParms->fLatestTime = FAddOptions.Contains(AddZipTime);
    // Dangerous, beware!
    zpParms->fMove = FAddOptions.Contains(AddMove);
    zpParms->fFreshen = FAddOptions.Contains(AddFreshen);
    zpParms->fUpdate = FAddOptions.Contains(AddUpdate);
    if (zpParms->fFreshen && zpParms->fUpdate)
        zpParms->fFreshen = false;   // Update has precedence over freshen.
    zpParms->fEncrypt = FAddOptions.Contains(AddEncrypt);
    /* NOTE: If user wants recursion, then he probably also wants
     AddDirNames, but we won't demand it. */
    zpParms->fRecurse = FAddOptions.Contains(AddRecurseDirs);
    // Don't store dirnames by themselves is the default.
    zpParms->fNoDirEntries = !FAddOptions.Contains(AddSeparateDirs);
    zpParms->fJunkDir = !FAddOptions.Contains(AddDirNames);
    zpParms->fSystem  = FAddOptions.Contains(AddHiddenFiles);
    zpParms->pZipFN = MakeStrP(NameOfZipFile);
    if (FPassword.Length())
        zpParms->pZipPassword = MakeStrP(FPassword);
}
// SetZipSwitches

/*? TZBWorker::Starting---------------------------------------------------------
1.77 17 August 2004 RA property BufSize in kBytes
1.76 31 May 2004 New function
*/
void __fastcall TZBWorker::Starting(void)
{
    inherited::Starting();
}

/*? TZBWorker::TZBWorker-----------------------------------------------------
1.79 18 Nov 2004 RA - Use TZipBuilderSFX as internal sfx
1.76 27 April 2004 - clear Active and Delaying
1.75.0.2 3 March 2004 - new event
1.73.3.2 10 Oct 2003 RA added setup_extend
1.73( 30 July 2003) RA TInternalSFX class added
1.73 (24 June 2003) RA set default value TargetHostVer
1.73 (17 May 2003) - RP clear event variables
*/
#ifdef STATIC_LOAD_DELZIP_DLL
__fastcall TZBWorker::TZBWorker(TObject* ABuilder) : TZBStaticBind(ABuilder)
#else
__fastcall TZBWorker::TZBWorker(TObject* ABuilder) : TZBDynamicBind(ABuilder)
#endif
{
    FZipContents   = new TList();
    FFSpecArgs    = new TStringList();
    FFSpecArgsExcl = new TStringList();
  	FDirHashs = new TZipDirHashList();
    FHandle = Application->Handle;
    FDllOperKey = 0;
    fIsDestructing = false;
    FDelaying.Clear();
    FActive = false;
    FZipBusy = false;
    FUnzBusy = false;
    Setup_extend(); // init extend table
    FZipFileName   = "";
    FPassword = "";
    FPasswordReqCount = 1;
    FSuccessCnt    =  0;
    FAddCompLevel = 9;
    FUnattended = false;
    FRealFileSize   =  0;
    FSFXOffset    =  0;
    FZipSOC     =  0;
    FFreeOnDisk1 = 0;
    FFreeOnAllDisks = 0;
    FMaxVolumeSize = 0;
    FMinFreeVolSize = 65536;
    FIsSpanned         = false;
    FZipComment = "";
    FHowToDelete = htdAllowUndo;
    FCodePage = cpAuto;
    FAddStoreSuffixes = DefAddStoreSuffixes;
    FDirOnlyCount   = 0;
    FSpanOptions.Clear();
    FBufSize  = 65536;
#ifndef NO_SPAN
    FConfirmErase = true;
#endif
#ifndef NO_STREAM
    FZipStream    = new TZipStream();
#endif
}

/*? TZipWorker::WriteJoin-------------------------------------------------------
1.79 3 January 2006 RP copy > 2G
1.77 13 August 2004 RP progress
1.73 (16 May 2003) RA changed to use callback
*/
void __fastcall TZBWorker::WriteJoin(const void* Buffer, unsigned BufferSize, int DSErrIdent)
{
    if (static_cast<unsigned>(FileWrite(FOutFileHandle, Buffer, BufferSize)) != BufferSize)
        throw EZipBuilder(DSErrIdent);

    // Give some progress info while writing.
    // While processing the central header we don't want messages.
    if (FShowProgress == zspExtra)
        Report(zacXProgress, 0, "", BufferSize);
    else
        if (FShowProgress == zspFull)
            Report(zacProgress, 0, "", BufferSize);
}

#ifndef NO_SPAN
/*? TZBWorker::WriteSpan-------------------------------------------------------
1.79 3 Nov 2006 when spanning has only one file, filename renamed to name.zip
1.79 3 January 2006 RP copy > 2G
1.79 12 June 2005 RA - added test if destination is writable
1.79 17 Dec 2004 RA - Use FNumbering for last part renaming
1.77 31 August 2004 RA - allow cancel during read
1.77 18 August 2004 RA added All in overwrite dialog
1.77 5 August 2004 RA parameter added to allow different progress types
1.76 1 May 2004 RA init FInFileHAnlde to avoid OS exception in case of error
1.73 20 August 2003 RA bug fix when writing CEH to disk in larger block
1.73 (16 June 2003) bug fix setting of file handles and all errors in try catch
1.73 (30 May 2003) use of ReadJoin + Delimitpath
1.73 (18 May 2003) RA changed to use callback also for message
Function to read a Zip source file and write it back to one or more disks.
Return values:
 0   All Ok.
 -7   WriteSpan errors. See ZipMsgXX.rc and ZipMsg.h
 -8   Memory allocation error.
 -9   General unknown WriteSpan error.
*/
int __fastcall TZBWorker::WriteSpan(const String InFileName, const String OutFileName, bool UseXProgress)
{
    AnsiChar *Buffer = NULL;
    bool BatchStarted = false;
    int Result = 0;
    struct ZipEndOfCentral    EOC;
    struct ZipLocalHeader     LOH;
    struct ZipDataDescriptor  DD;
    struct ZipCentralHeader   CEH;
    TMZipDataList* MDZD = NULL;
    FZipBusy = true;
    FErrCode = 0;
    FFreeOnDisk = 0;
    FNewDisk = true;
    FDiskWritten = 0;
    FInFileHandle = -1;
    FOutFileName = OutFileName;
    FOutFileHandle = -1;
    FShowProgress = zspNone;
    StartWaitCursor();
    try
    {
        if (ExtractFileName(OutFileName).IsEmpty())
            throw EZipBuilder(DS_NoOutFile);
        FDrive = DelimitPath(ExtractFileDrive(OutFileName), false);
        FDriveFixed = IsFixedDrive(FDrive);
        if (FDriveFixed)
            CheckDestWritable(OutFileName, false);
        // now change back to input and do the split
        FInFileName = InFileName;
        FTotalDisks = -1;
        FDiskNr = 0;
        if (!FileExists(InFileName))
            throw EZipBuilder(DS_NoInFile);
        OpenEOC(EOC, true); // fill EOC

        // Get the date-time stamp and save for later.
        FDateStamp = FileGetDate(FInFileHandle);

        // Now we now the number of zipped entries in the zip archive.
        // And we go back to the start the zip archive.
        if (FileSeek(FInFileHandle, 0i64, 0) == -1)
            throw EZipBuilder(DS_FailedSeek);

        // Allocate the read/write buffer.
        Buffer = new AnsiChar[FBufSize];
        MDZD = new TMZipDataList(EOC.TotalEntries);
        // Write extended local Sig. needed for a spanned archive.
        FZipDiskAction = zdaOk; // make sure each split action starts with default action
        WriteSplit(&ExtLocalSig, 4, 0);

        // Read for every zipped entry: The local header, variable data, fixed data
        // and if present the Data decriptor area.
        if (UseXProgress)
        {
            Report(zacXItem, zprSplitting, ZipLoadStr(PR_Splitting), FFileSize);
            FShowProgress = zspExtra;
        }
        else
        {
            Report(zacCount, zprCompressed, "", EOC.TotalEntries);
            Report(zacSize, zprCompressed, "", FFileSize);
            FShowProgress = zspFull;
            BatchStarted = true;
        }
        for (int i = 0; i < EOC.TotalEntries; i++)
        {
            if (FCancel)
                throw EZipAbort();
            // First the local header.
            ReadJoin(&LOH, sizeof(LOH), DS_LOHBadRead);
            if (LOH.HeaderSig != LocalFileHeaderSig)
                throw EZipBuilder(DS_LOHWrongSig);

            // Now the filename.
            ReadJoin(Buffer, LOH.FileNameLen, DS_LONameLen);

            // Save some information for later. ( on the last disk(s) ).
            MDZD->Items[i]->DiskStart = (unsigned short)FDiskNr;
            MDZD->Items[i]->SetName(Buffer, LOH.FileNameLen);
            String Fname = SetSlash(MDZD->Items[i]->FileName, psdExternal);
            // Give message and progress info on the start of this new file read.
            Report(zacMessage, 0, ZipLoadStr(GE_CopyFile) + Fname, 0);
            unsigned TotalBytesWrite = sizeof(LOH) + LOH.FileNameLen + LOH.ExtraLen + LOH.ComprSize;
            if (DescUsed(LOH))
                TotalBytesWrite += sizeof(DD);
            if (!UseXProgress)
                Report(zacItem, zprCompressed, Fname, TotalBytesWrite);
            // Write the local header to the destination.
            WriteSplit(&LOH, sizeof(LOH), sizeof(LOH) + LOH.FileNameLen + LOH.ExtraLen);

            // Save the offset of the LOH on this disk for later.
            MDZD->Items[i]->RelOffLocal = FDiskWritten - sizeof(LOH);

            // Write the filename.
            WriteSplit(Buffer, LOH.FileNameLen, 0);

            // And the extra field
            RWSplitData(Buffer, LOH.ExtraLen, DS_LOExtraLen);

            // Read Zipped data !!!For now assume we know the size!!!
            RWSplitData(Buffer, LOH.ComprSize, DS_ZipData);

            // Read DataDescriptor if present.
            if (DescUsed(LOH))
                RWSplitData((char *)&DD, sizeof(DD), DS_DataDesc);
        } // Ready, we have written al entries to disk.
        Report(zacMessage, 0, ZipLoadStr(GE_CopyFile) + ZipLoadStr(DS_CopyCentral), 0);
        if (!UseXProgress)
            Report(zacItem, 0, ZipLoadStr(DS_CopyCentral),
                   EOC.CentralSize + sizeof(EOC) + EOC.ZipCommentLen);
        // Now write the central directory with changed offsets.
        unsigned StartCentral = FDiskNr;
        unsigned CentralOffset;
        unsigned WritePtr = 0;
        unsigned BlockSize;
        char*  CEHBlock = NULL;
        int CentralEntries = 0;
        int ActualDiskNr = FDiskNr;
        for (int i = 0; i < EOC.TotalEntries; i++)
        {
            if (FCancel)
                throw EZipAbort();
            // Read a central header.
            ReadJoin(&CEH, sizeof(CEH), DS_CEHBadRead);
            if (CEH.HeaderSig != CentralFileHeaderSig)
                throw EZipBuilder(DS_CEHWrongSig);

            // Now the filename.
            if (CEH.FileNameLen >= MAX_PATH)
                throw EZipBuilder(DS_CENameLen);
            ReadJoin(Buffer, CEH.FileNameLen, DS_CENameLen);

            // Change the central directory with information stored previously in MDZD.
            int k = MDZD->IndexOf(Buffer, CEH.FileNameLen);
            CEH.DiskStart = MDZD->Items[k]->DiskStart;
            CEH.RelOffLocal = static_cast<unsigned>(MDZD->Items[k]->RelOffLocal);
            int Blocklen = sizeof(CEH) + CEH.FileNameLen + CEH.ExtraLen + CEH.FileComLen;
            // check if we can write in larger block
            if (i && (FFreeOnDisk > Blocklen)) // new method write in larger block
            {
                if (!CEHBlock)
                {
                    BlockSize = min(FBufSize, static_cast<int>(FFreeOnDisk));
                    CEHBlock = new char[BlockSize];
                }
                if (WritePtr > (BlockSize - Blocklen))
                {
                    WriteSplit(CEHBlock, WritePtr, 0);// not enough room write to disk first
                    WritePtr = 0;
                    delete[] CEHBlock;
                    CEHBlock = NULL;
                    if (FFreeOnDisk > Blocklen)
                    {
                        BlockSize = min(FBufSize, static_cast<int>(FFreeOnDisk)); // adapt to new free on disk if needed
                        CEHBlock = new char[BlockSize];
                    }
                }
                if (CEHBlock)
                {
                    memcpy(&CEHBlock[WritePtr], &CEH, sizeof(CEH));
                    WritePtr += sizeof(CEH);
                    memcpy(&CEHBlock[WritePtr], Buffer, CEH.FileNameLen);
                    WritePtr += CEH.FileNameLen;
                    if (CEH.ExtraLen || CEH.FileComLen)
                    {
                        WriteSplit(CEHBlock, WritePtr, 0);// write to disk first
                        WritePtr = 0;
                        delete[] CEHBlock;
                        CEHBlock = NULL;
                        RWSplitData(Buffer, CEH.ExtraLen, DS_CEExtraLen); // then extra field
                        RWSplitData(Buffer, CEH.FileComLen, DS_CECommentLen); // then the comment
                    }
                }
            }
            if (!CEHBlock) // not enough room use old method or first file
            {
                // Write this changed central header to disk
                // and make sure it fit's on one and the same disk.
                WriteSplit(&CEH, sizeof(CEH), Blocklen);

                // Save the first Central directory offset for use in EOC record.
                if (!i)
                    CentralOffset = FDiskWritten - sizeof(CEH);

                // Write to destination the central filename and the extra field.
                WriteSplit(Buffer, CEH.FileNameLen, 0);

                // And the extra field
                RWSplitData(Buffer, CEH.ExtraLen, DS_CEExtraLen);

                // And the file comment.
                RWSplitData(Buffer, CEH.FileComLen, DS_CECommentLen);

            }
            // check if new part was started
            if (ActualDiskNr != FDiskNr)
            {
                ActualDiskNr = FDiskNr;
                CentralEntries = 0;   // started new part
            }
            ++CentralEntries;
        }
        if (WritePtr)
            WriteSplit(CEHBlock, WritePtr, 0); // write last part
        delete[] CEHBlock;
        // Write the changed EndOfCentral directory record.
        EOC.CentralDiskNo = static_cast<unsigned short>(StartCentral);
        EOC.ThisDiskNo = static_cast<unsigned short>(FDiskNr);
        EOC.CentralOffset = CentralOffset;
        EOC.CentralEntries = static_cast<unsigned short>(CentralEntries);
        WriteSplit(&EOC, sizeof(EOC), sizeof(EOC) + EOC.ZipCommentLen);

        // Skip past the original EOC to get to the ZipComment if present.
        if (FileSeek(FInFileHandle, static_cast<__int64>(sizeof(EOC)), 1) == -1)
            throw EZipBuilder(DS_FailedSeek);

        // And finally the archive comment itself.
        RWSplitData(Buffer, EOC.ZipCommentLen, DS_EOArchComLen);
        FShowProgress = zspNone;
    }
    catch (EZipBuilder &ews)
    { // All WriteSpan specific errors.
        ShowExceptionError(ews);
        Result = -7;
    }
    catch (const MEMEXCEPT &me)
    {  // All memory allocation errors.
        ShowZipMessage(GE_NoMem, "");
        Result = -8;
    }
    catch (const Exception &E)
    {
        ShowZipMessage(DS_ErrorUnknown, "\n" + E.Message);
        Result = -9;
    }
    catch (...)
    {       // The remaining errors, should not occur.
        ShowZipMessage(DS_ErrorUnknown, "");
        Result = -9;
    }
    // Give the last progress info on the end of this file read.
    if (BatchStarted)
        Report(zacEndOfBatch, 0, "", 0);

    delete[] Buffer;
    if (MDZD)
        delete MDZD;
    FileSetDate(FOutFileHandle, FDateStamp);
    CloseFileHandle(FOutFileHandle);
    CloseFileHandle(FInFileHandle);
    if (!Result) // if Result is error then rename is not needed
    {
        int Fix = 0;
        if ((FNumbering == znsExt) || FSpanOptions.Contains(spCompatName))
        {
            if (FDriveFixed)
                Fix = 2;
        }
        else
            if (!FDiskNr && ((FNumbering == znsName) || FSpanOptions.Contains(spNoVolumeName)))
                Fix = 1;
        if (Fix > 0)
        {  // do we need to change the name of the last file written?
            String LastName = FOutFileName;
            CreateMVFileName(LastName, false, Fix == 2);
            FZipDiskStatus.Clear();
            TModalResult Res = IDYES;
            String MsgQ;
            if (FileExists(FOutFileName))
            {
                // get permission to delete
                MsgQ = ZipFmtLoadStr(DS_AskDeleteFile, ARRAYOFCONST((FOutFileName)));
                FZipDiskStatus = FZipDiskStatus << zdsSameFileName;
                if (!FAnswerAll.Contains(zaaYesOvrwrt))
                {
                    TStatusDiskEvent tmpStatusDisk = FEvents.FOnStatusDisk;
                    if (tmpStatusDisk)
                    {
                        FZipDiskAction = zdaOk;
                        tmpStatusDisk(Builder, FDiskNr, FOutFileName, FZipDiskStatus, FZipDiskAction);
                        if (FZipDiskAction == zdaOk)
                            Res = IDYES;
                        else
                            Res = IDNO;
                    }
                    else
                        if (FAnswerAll.Contains(zaaYesOvrwrt))
                            Res = IDYES;
                        else
                            Res = ZipMessageDlg(ZipLoadStr(FM_Confirm), MsgQ,
                                                zmtWarning + DHC_WrtSpnDel, TMsgDlgButtons() << mbYes << mbNo);
                }
                if (!Res)
                    ShowZipMessage(DS_NoMem, "");
                if (Res == IDNO)
                    Report(zacMessage, DS_NoRenamePart,
                           ZipFmtLoadStr(DS_NoRenamePart, ARRAYOFCONST((LastName))), 0);
                if (Res == IDYES)
                    DeleteFile(FOutFileName);         // if it exists delete old one
            }
            if (FileExists(LastName))
                RenameFile(LastName, FOutFileName);
        }
    }
    FTotalDisks = static_cast<unsigned short>(FDiskNr);
    FZipBusy = false;
    StopWaitCursor();
    return Result;
}
#endif

#ifndef NO_SPAN
/*? TZBWorker::WriteSplit------------------------------------------------------
1.79 3 January 2006 RP file > 2G
1.79 12 Dec 2004 RA use FNumbering
1.77 23 Sep 2004 RP - use YesToAll
1.77 18 August 2004 RA added All in overwrite dialog
1.77 16 August 2004 RA allow unattended split on harddisk
1.77 15 August 2004 RP progress
1.73.3.3 15 October 2003 RP Remove duplicated code
1.73.3.2 9 Oct 2003 RA set KeepFreeOnDisk1 and KeepFreeOnAlldisk to sector boundaries
1.73 11 July 2003 RP corrected asking disk status
1.73 (7 July 2003) RA changed OnMessage and OnProgress to Report calls
1.73 (18 May 2003) RP changed to use ZipAbort
This function actually writes the zipped file to the destination while taking care
of disk changes and disk boundary crossings.
In case of an write error or user abort an EZipBuilder Exception is thrown.
*/
void __fastcall TZBWorker::WriteSplit(const void *Buffer, unsigned Len, unsigned MinSize)
{
    int Res;
    unsigned MaxLen;
    const char *Buf = reinterpret_cast<const char *>(Buffer);
    // Used if Buffer doesn't fit on the present disk.
    Report(zacTick, 0, "", 0);
    if (Cancel)
        throw EZipAbort();
    while (true)
    { // Keep writing until error or buffer is empty.
        // Check if we have an output file already opened, if not: create one, do checks gather info.
        if (FOutFileHandle == -1)
        {
            String DiskFile = FOutFileName, MsgQ;
            CheckForDisk(true);
            // If we write on a fixed disk the filename must change.
            // We will get something like: FileNamexxx.zip where xxx is 001,002 etc.
            // if CompatNames are used we get FileName.zxx wher xx is 01, 02 etc..
            if (FNumbering == znsNone)
            {
                if (FSpanOptions.Contains(spNoVolumeName))
                    FNumbering = znsExt;
                else
                    if (FDriveFixed || FSpanOptions.Contains(spNoVolumeName))
                        FNumbering = znsName;
                    else
                        FNumbering = znsVolume;
            }
            if (FNumbering >= znsName)
                CreateMVFileName(DiskFile, false, FNumbering == znsExt);
            if (!FDriveFixed && FSpanOptions.Contains(spWipeFiles))
            {
                if ((FZipDiskAction == zdaErase) || !FEvents.FOnGetNextDisk)
                {
                    // Do we want a format first? To be save we don't do this on a harddisk.
                    FDriveNr = FDrive.UpperCase()[1] - 'A';
                    if (FSpanOptions.Contains(spNoVolumeName))
                        FVolumeName = "ZipSet_" + IntToStr(FDiskNr); //default name
                    else
                        FVolumeName = "PKBACK# " + IntToStr(1001 + FDiskNr).SubString(2, 3);
                    // Ok=6, NoFormat=-3 , Cancel=-2, Error =-1
                    switch (ZipFormat())
                    {   // Start the format and wait until finished...
                    case -1:
                        throw EZipBuilder(DS_Canceled, true);   // Error
                    case -2:
                        throw EZipAbort(); // Canceled
                    default:
                        DiskFreeAndSize(3); // reread disksize values after format
                    }
                }
            }
            // Do we want to overwrite an existing file
            int DiskSeq = FSpanOptions.Contains(spNoVolumeName) ? FDiskNr + 1 :
                          StrToIntDef(FVolumeName.SubString(9, 3), 1);
            FZipDiskStatus.Clear();
            if (FileExists(DiskFile))
            {
                // A more specific check if we have a previous disk from this set. Not too smart but...
                if (FileAge(DiskFile) == FDateStamp && DiskSeq - 1 < FDiskNr && !FDriveFixed) // don't ask for fixed drive
                {
                    MsgQ = ZipFmtLoadStr(DS_AskPrevFile, ARRAYOFCONST((DiskSeq)));
                    FZipDiskStatus << zdsPreviousDisk;
                }
                else
                {
                    MsgQ = ZipFmtLoadStr(DS_AskDeleteFile, ARRAYOFCONST((DiskFile)));
                    FZipDiskStatus << zdsSameFileName;
                }
            }
            else
            {
                if (!FDriveFixed)
                {
                    if (FSizeOfDisk - FFreeOnDisk)
                        FZipDiskStatus << zdsHasFiles;  // But not the same name
                    else
                        FZipDiskStatus << zdsEmpty;
                }
            }
            TStatusDiskEvent tmpStatusDisk = FEvents.FOnStatusDisk;
            if (tmpStatusDisk && !FAnswerAll.Contains(zaaYesOvrwrt))
            {
                FZipDiskAction = zdaOk;     // The default action
                tmpStatusDisk(Builder, DiskSeq, DiskFile, FZipDiskStatus, FZipDiskAction);
                switch (FZipDiskAction)
                {
                case zdaCancel:
                    Res = IDCANCEL;
                    break;
                case zdaReject:
                    Res = IDNO;
                    break;
                case zdaYesToAll:
                    FAnswerAll = FAnswerAll << zaaYesOvrwrt;
                case zdaErase:
                case zdaOk:
                    Res = IDOK;
                    break;
                default :
                    Res = IDOK;
                }
            }
            else
            {
                if ((FZipDiskStatus.Contains(zdsPreviousDisk) || FZipDiskStatus.Contains(zdsSameFileName)) &&
                        !FAnswerAll.Contains(zaaYesOvrwrt) && !Unattended)
                {  // if no OnStatusDisk event
                    Res = ZipMessageDlg(ZipLoadStr(FM_Confirm), MsgQ, zmtWarning + DHC_SpanOvr,
                                        TMsgDlgButtons() << mbYes << mbNo << mbCancel << mbYesToAll);
                    if (Res == mrYesToAll)
                    {
                        FAnswerAll = FAnswerAll << zaaYesOvrwrt;
                        Res = IDOK;
                    }
                }
                else
                    Res = IDOK;
            }
            if (!Res)
                throw EZipBuilder(DS_NoMem);
            if ((Res == IDCANCEL) || ((Res == IDNO) && FDriveFixed))
                throw EZipAbort();
            if (Res == IDNO)
            {  // we will try again...
                FDiskWritten = 0;
                FNewDisk = true;
                continue;
            }
            // Create the output file.
            SetLastError(0);  // reset system errors
            if ((FOutFileHandle = FileCreate(DiskFile)) == -1)
            {//change proposed by Pedro Araujo
                MsgQ = ZipLoadStr(DS_NoOutFile) + " - " + DiskFile + WinErr(GetLastError());
                Res = ZipMessageDlg("", MsgQ, zmtError + DHC_SpanNoOut,
                                    TMsgDlgButtons() << mbRetry << mbCancel);
                if (!Res)
                    throw EZipBuilder(DS_NoMem);
                if (Res != IDRETRY)
                    throw EZipAbort();
                FDiskWritten = 0;
                FNewDisk = true;
                continue;
            }
            // Get the free space on this disk, correct later if neccessary.
            DiskFreeAndSize(1);
            // Set the maximum number of bytes that can be written to this disk(file).
            if (FMaxVolumeSize)
                FFreeOnDisk = min(static_cast<__int64>(FMaxVolumeSize), FFreeOnDisk);
            // Set the maximum number of bytes that can be written to this disk(file).
            // Reserve space on/in all the disk/file.
            if (!FDiskNr && (FFreeOnDisk1 || FFreeOnAllDisks)) // only one calculation if needed
            {  // set these properties on sector boundaries
                DWORD SectorsPCluster, BytesPSector, FreeClusters, TotalClusters;
                if (GetDiskFreeSpace(FDrive.T_STR(), &SectorsPCluster, &BytesPSector, &FreeClusters, &TotalClusters))
                {
                    if (FFreeOnDisk1  % BytesPSector)
                        FFreeOnDisk1 = ((FFreeOnDisk1 / BytesPSector) + 1) * BytesPSector;
                    if (FFreeOnAllDisks  % BytesPSector)
                        FFreeOnAllDisks = ((FFreeOnAllDisks / BytesPSector) + 1) * BytesPSector;
                }
            }
            FFreeOnDisk -= FFreeOnAllDisks;
            // Reserve space on/in the first disk(file).
            if (!FDiskNr)
                FFreeOnDisk -= FFreeOnDisk1;

            // Do we still have enough free space on this disk.
            if (FFreeOnDisk < FMinFreeVolSize)
            { // No, too bad...
                CloseFileHandle(FOutFileHandle);
                DeleteFile(DiskFile);
                TStatusDiskEvent tmpStatusDisk = FEvents.FOnStatusDisk;
                if (tmpStatusDisk)
                {
                    int DiskSeq = FSpanOptions.Contains(spNoVolumeName) ? FDiskNr + 1 :
                                  StrToIntDef(FVolumeName.SubString(9, 3), 1);
                    FZipDiskAction = zdaOk;
                    FZipDiskStatus = TZipDiskStatus() << zdsNotEnoughSpace;
                    tmpStatusDisk(Builder, DiskSeq, DiskFile, FZipDiskStatus, FZipDiskAction);
                    switch (FZipDiskAction)
                    {
                    case zdaCancel:
                        Res = IDCANCEL;
                        break;
                    case zdaOk:
                    case zdaErase:
                    case zdaReject:
                        Res = IDRETRY;
                    }
                }
                else
                {
                    MsgQ = ZipLoadStr(DS_NoDiskSpace);
                    Res = ZipMessageDlg("", MsgQ, zmtError + DHC_SpanSpace,
                                        TMsgDlgButtons() << mbRetry << mbCancel);
                }
                if (!Res)
                    throw EZipBuilder(DS_NoMem);
                if (Res != IDRETRY)
                    throw EZipAbort();
                FDiskWritten = 0;
                FNewDisk = true;
                continue;
            }
            // Set the volume label of this disk if it is not a fixed one.
            if (!FDriveFixed && !FSpanOptions.Contains(spNoVolumeName))
            {
                FVolumeName = "PKBACK# " + IntToStr(1001 + FDiskNr).SubString(2, 3);
                if (!::SetVolumeLabel(FDrive.T_STR(), FVolumeName.T_STR()))
                    throw EZipBuilder(DS_NoVolume);
            }
        }
        // Check if we have at least MinSize available on this disk,
        // headers are not allowed to cross disk boundaries. ( if zero than don't care.)
        if (MinSize && MinSize > FFreeOnDisk)
        {
            FileSetDate(FOutFileHandle, FDateStamp);
            CloseFileHandle(FOutFileHandle);
            FDiskWritten = 0;
            ++FDiskNr;
            FNewDisk = true;
            continue;
        }
        // Don't try to write more bytes than allowed on this disk.
        MaxLen = min(Len, static_cast<unsigned>(FFreeOnDisk));
        Res = FileWrite(FOutFileHandle, Buf, MaxLen);
        // Give some progress info while writing.
        // While processing the central header we don't want messages.
        if (FShowProgress != zspNone)
        {
            if (FShowProgress == zspExtra)
                Report(zacXProgress, 0, "", MaxLen);
            else
                Report(zacProgress, 0, "", MaxLen);
        }
        if (Res == -1)
            throw EZipBuilder(DS_NoWrite); // A write error (disk removed?).
        FDiskWritten += Res;
        FFreeOnDisk  -= MaxLen;
        if (MaxLen == Len)
            break;

        // We still have some data left, we need a new disk.
        FileSetDate(FOutFileHandle, FDateStamp);
        CloseFileHandle(FOutFileHandle);
        FFreeOnDisk = FDiskWritten = 0;
        ++FDiskNr;
        FNewDisk = true;
        Buf += MaxLen;
        Len -= MaxLen;
    }
}
// TZBWorker::WriteSplit
#endif

/*? TZBWorker::ZCallback-------------------------------------------------------
1.76 01 May 2004 RP change return type and value to return flag for exception
1.76 24 April 2004 RP use DllCallback
 1.73 ( 1 June 2003) changed for new callback
 Dennis Passmore (Compuserve: 71640,2464) contributed the idea of passing an
 instance handle to the DLL, and, in turn, getting it back from the callback.
 This lets us reference variables in the TZBWorker class from within the
 callback function.  Way to go Dennis!
 RCV: But while doing it like this you can only access public data and member
 functions and we need access to private data (the properties ErrCode and
 Message are read only).
 This function is now(v1.5) a static member function of TZBWorker. */
int __stdcall TZBWorker::ZCallback(void* Caller, PZCallBackStruct cbr)
{
    TZBWorker *tzb = static_cast<TZBWorker*>(Caller);
    tzb->DllCallBack(cbr);
    if (!tzb->FEventErr.IsEmpty())
        return  Callback_Except_No;  // handled exception
    else
        return tzb->Cancel ? 1 : 0;
}
// TZBWorker::ZCallback

#ifndef NO_SPAN
/*? TZBWorker::ZipFormat--------------------------------------------------------
1.73 (27 June 2003) RP change handling of 'No'
 return values
 -1 Error on last format, Drive may be formatable
 -2 last format cancelled
 -3 drive is not foramtable
*/
int FormatFloppy(HWND WND, String Drive)
{
#ifndef UNICODE
    const unsigned int SHFMT_ID_DEFAULT = 0xFFFF; //defining constants avoids including shlobj.h
    const unsigned int SHFMT_OPT_FULL = 1;

    typedef int __stdcall(*TSHFormatDrive)(HWND WND, unsigned int Drive, unsigned int fmtID,
                                           unsigned int Options);
    TSHFormatDrive SHFormatDrive;
#endif
    int Result = -3;
    char DriveLetter = Drive.UpperCase()[1];
    if (!((Drive.Length() > 1) && (Drive[2] == ':') && CharInSet(Drive[1], Lchars)))
        return Result;
    if (GetDriveType(Drive.T_STR()) != DRIVE_REMOVABLE)
        return Result;
    unsigned int drv = DriveLetter - 'A';
    long OldErrorMode = SetErrorMode(SEM_FAILCRITICALERRORS | SEM_NOGPFAULTERRORBOX);
#ifdef UNICODE
    try
    {
        Result = SHFormatDrive(WND, drv, SHFMT_ID_DEFAULT, SHFMT_OPT_FULL);
    }
    __finally
    {
    SetErrorMode(OldErrorMode);
    }
#else
    if (HINSTANCE hLib = LoadLibrary("Shell32"))
    {
        if ((SHFormatDrive = (TSHFormatDrive)::GetProcAddress(hLib, "SHFormatDrive")) != 0)
        {
            try
            {
                Result = SHFormatDrive(WND, drv, SHFMT_ID_DEFAULT, SHFMT_OPT_FULL);
            }
            __finally
            {
                FreeLibrary(hLib);
            }
            SetErrorMode(OldErrorMode);
        }
    }
#endif
    return Result;
}
//---------------------------------------------------------------------------
int __fastcall TZBWorker::ZipFormat(void)
{
    int Result = -3;
    if (FSpanOptions.Contains(spTryFormat) && !FDriveFixed)
        Result = FormatFloppy(Application->Handle, FDrive);
    if (Result == -3)
    {
        if (FConfirmErase)
        {
            String Msg = ZipFmtLoadStr(FM_Erase, ARRAYOFCONST((FDrive)));
            int Res = ZipMessageDlg(ZipLoadStr(FM_Confirm), Msg,
                                    zmtWarning + DHC_FormErase,
                                    TMsgDlgButtons() << mbYes << mbNo);
            if (Res != IDYES)
                return -3;  // no
        }
        ClearFloppy(FDrive);
        Result = 0;
    }
    String Vol = (FVolumeName.Length() > 11) ? FVolumeName.SubString(1, 11) : FVolumeName;
    if (Result == 0 && !(FSpanOptions.Contains(spNoVolumeName)))
        SetVolumeLabel(FDrive.T_STR(), Vol.T_STR());
    return Result;
}
#endif

/*? TZBWorker::~TZBWorker----------------------------------------------------
1.79 18 Nov 2004 RA - Use TZipBuilderSFX as internal sfx
1.73 (30 July 2003) RA use TInternalSFX
1.73 (16 June 2003) RP set IsDestroying & new dll handling
*/
__fastcall TZBWorker::~TZBWorker(void)
{
    fIsDestructing = true;
    AbortDll();
    FreeZipDirEntryRecords();
    delete FZipContents;
    delete FFSpecArgsExcl;
    delete FFSpecArgs;
    delete FDirHashs;
#ifndef NO_STREAM
    delete FZipStream;
#endif
}























