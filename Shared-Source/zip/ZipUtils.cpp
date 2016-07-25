/*
  ZipUtils.cpp - Source file for common utilities for TZipBuilder
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

  last modified 2009-05-18
---------------------------------------------------------------------------*/
#pragma hdrstop

#include "ZipUtils.h"
#include "ZBStructs.h"
#if __BORLANDC__ < 0x0550
#include <shellapi.h>     // needed for BCB4
#endif
#include "ZBCompat.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

/* CloseFileHandle-------------------------------------------------------
1.79 4 May 2005 RA moved form TZipWorker
1.80 28 August 2003 RA new function for closing file handles
*/
void __export CloseFileHandle(int& FileHandle)
{
    if (FileHandle != -1)
    {
        FileClose(FileHandle);
        FileHandle = -1;
    }
}

/* DelimitPath-----------------------------------------------------
 1.73 (26 July 2003) RA changed handling of empty strings
 1.73 (30 May 2003) RP changed signature + allow either slash
 1.73 (22 May 2003) RP new function to add/remove trailing slash
*/
String __export DelimitPath(const String path, bool sep)
{
    String Result = path;
    if (!path.Length())  // if empty or null
    {
        if (sep)
            Result = '\\';
        return Result;
    }
    const TCHAR *e = path.T_STR();
    TCHAR c, b = 0;
    TCHAR used = '\\';
    while ((c = *e++) != 0)
    {
#ifndef UNICODE
        if (LeadBytes.Contains(c))
            ++e;
#endif
        b = c;
        if (b == '/' || b == '\\')
            used = b;
    }
    if ((b == '\\' || b == '/') != sep)
    {
        Result = sep ? path + used : String(path.T_STR(), path.Length() - 1);
    }
    return Result;
}
//? DelimitPath

/*? DirExists-------------------------------------------------------
 1.73 (12 July 2003) RP return true empty string (current directory)
 1.73 (20 May 2003) RA made function a member of ZipBuilder
*/
bool __export DirExists(const String Name)
{
    if (Name.IsEmpty())
        return true;
    DWORD Code = ::GetFileAttributes(Name.T_STR());
    return (Code != 0xFFFFFFFF) & ((FILE_ATTRIBUTE_DIRECTORY & Code) != 0);
}
//? DirExists

/* EraseFile---------------------------------------------------------------------
1.77 moved from ZBuilder
 Delete a file and put it in the recyclebin on demand.
*/
int __export EraseFile(const String Fname, TDeleteOpts How)
{
    SHFILEOPSTRUCT SHF;
    TCHAR* FromStr = NULL;
    int Result  = -1;
    String DelFileName = Fname;
    // If we do not have a full path then FOF_ALLOWUNDO does not work!?
    if (ExtractFilePath(Fname).IsEmpty())
        DelFileName = GetCurrentDir() + '\\' + Fname;
    // We need to be able to 'Delete' without getting an error
    // if the file does not exists as in ReadSpan() can occur
    if (!FileExists(DelFileName))
        return Result;
    try
    {
        int dfnLen = DelFileName.Length() + 2;
        FromStr    = new TCHAR[dfnLen];
        SHF.hwnd   = Application->Handle;
        SHF.wFunc  = FO_DELETE;
        SHF.pFrom  = FromStr;
        SHF.pTo    = NULL;
        SHF.fFlags = FOF_NOCONFIRMATION | FOF_SILENT | FOF_NOERRORUI;
        if (How == htdAllowUndo)
            SHF.fFlags |= FOF_ALLOWUNDO;
        StrPLCopy(FromStr, DelFileName, dfnLen);
        FromStr[dfnLen - 1] = '\0';
        Result = SHFileOperation(&SHF);
    }
    __finally
    {
        delete[] FromStr;
    }
    return Result;
}

/* FileNameMatch----------------------------------------------------
 1.73 (22 May 2003) - RP  filename match similar to dlls
 modified for handling MBCS Strings
*/
/* ===========================================================================
  Adapted by R.Peters from many sources
  Break the pattern and string into name and extension parts and match
  each separately using pat_match().
  *s :: Filename to match against wildcard pattern.
  *p :: Dos wildcard pattern to match.
*/
/*
 * Nonrecursively compare the sh pattern p, with the string s,
 * and return 1 if they match, and 0 if they don't.
  *p :: pattern to match (ex: *.*).
  *s :: String to match it to (ex: FNAME.DAT).
  pe :: past end of pattern
  se :: past end of string
 */
static int pat_match(const TCHAR *p, const TCHAR *s,
                     const TCHAR *pe, const TCHAR *se)
{
    TCHAR c, cs;
    const TCHAR *pa = 0;  // pattern position last '*' - null = none
    const TCHAR *sa;  // string position last '*'
    for (;;)
    { // keep scanning until end or unrecoverable mismatch
        if (s > se)
        { // run out of string - only '*''s allowed in pattern
            while((p <= pe) && (*p == '*'))
                p++;
            return (p >= pe);  // ok if end of pattern
        }
        if(p > se) return false; // run out of pattern
        // more string to process
        if ((c = *p) == '*')
        {  // on mask
            if (++p >= pe)   // last char - rest of string must match
                return 1;
            // note scan point incase must back up
            pa = p;
            sa = s;
        }
        else
        {
            cs = *s;
            // if successful match, inc pointers and keep trying
            if (((cs == c) || (toupper(cs) == toupper(c))
                    || (cs == '\\' && c == '/') || (cs == '/' && c == '\\')
                    || (c == '?' &&  cs))
#ifndef UNICODE
                    && (LeadBytes.Contains(c) ? (*++p == *++s) : true)
#endif
                    )
            {
                ++p;
                ++s;
            }
            else
            {  // no match
                if (!pa)      // any asterisk?
                    return 0;  // no - no match
                p = pa;                   // back up
                s = ++sa;                 //  next char in string
            }
        }
    }
}
bool __export FileNameMatch(const String pat, const String spec)
{
    const TCHAR *pe, *p2e;  // pattern sections e=end
    const TCHAR *se, *s2e;  // string sections 2=start of second part
    TCHAR c;
    if (!pat.Length() || !spec.Length())
        return false;
    const TCHAR *p = pat.T_STR();
    const TCHAR *s = spec.T_STR();
    // find extension (and eos)
    pe = 0;
    for (p2e = p;(c = *p2e) != 0; ++p2e)
    {
        if (c == '.')
            pe = p2e;
        else
            if (c == '\\' || c == '/')
                pe = 0;               // still in path
#ifndef UNICODE
            else
                if (LeadBytes.Contains(c))
                    ++p2e;        // ignore 2nd byte
#endif
    }
    if (!pe)               // no dot in pattern
    {
        for (s2e = s; *s2e; ++s2e) // find eos
            ;
        return pat_match(p, s, p2e, s2e);
    }
    // find extension (and eos)
    se = 0;
    for (s2e = s;(c = *s2e) != 0; ++s2e)
    {
        if (c == '.')
            se = s2e;   // at last '.'
        else
            if (c == '\\' || c == '/')   // still in path
                se = 0;
#ifndef UNICODE
            else
                if (LeadBytes.Contains(c))
                    ++s2e;        // ignore 2nd byte
#endif
    }
    if (!se)   // no sExt
        se = s2e;    // end
    // if pattern ext <> '.*' then exts must match
    if ((pe[1] != '*' || pe[2]) && !pat_match(pe, se, p2e, s2e))
        return false;
    return pat_match(p, s, pe, se);   // match rest
}
// FileNameMatch

/*? FileVersion-----------------------------------------------------
*/
String __export FileVersion(const String fname)
{
    String Result = "?.?.?.?";
    String Buffer;
    DWORD VersionHandle;
    int VersionSize;
    if ((VersionSize = GetFileVersionInfoSize(fname.T_STR(), & VersionHandle)) != 0)
    {
        Buffer.SetLength(VersionSize);
        GetFileVersionInfo(fname.T_STR(), VersionHandle, VersionSize, Buffer.T_STR());
        TCHAR *buff;
        UINT buflen;
        TCHAR txtstr[100];
        TCHAR sbuff[30];
        struct LANGANDCODEPAGE
        {
            WORD wLanguage;
            WORD wCodePage;
        } *lpTranslate;
        UINT vsize;
        VerQueryValue(Buffer.T_STR(), TEXT("\\VarFileInfo\\Translation"), (LPVOID *)&lpTranslate, &vsize);
        wsprintf(sbuff, TEXT("\\StringFileInfo\\%04x%04x\\"), lpTranslate->wLanguage, lpTranslate->wCodePage);
        if (VerQueryValue(Buffer.T_STR(), StrCat(StrCopy(txtstr, sbuff), TEXT("FileVersion")),
                          reinterpret_cast<void **>(&buff), &buflen))
            Result = String(buff);
    }
    return Result;
}
//? FileVersion

/*? ForceDirectory------------------------------------------------
 1.73 (18 June 2003) RA bug fix wrong parameters for Delimithpath
 1.73 (22 May 2003) RA use of DelimithPath
 1.73 (20 May 2003) RA new function
 force the existence of a directory (and its parents)
 return values
 true = dir exits or creation successfull
*/
bool __export ForceDirectory(const String Dir)
{
    if (Dir.IsEmpty())
        return true;
    String sDir = DelimitPath(Dir, false);
    if (DirExists(sDir) || (ExtractFilePath(sDir) == sDir))
        return true; // avoid 'c:\xyz:\' problem.
    if (!ForceDirectory(ExtractFilePath(sDir)))
        return false;
    return CreateDirectory(sDir.T_STR(), NULL);
}
//? ForceDirectory

/* GetVolumeLabel---------------------------------------------------------
1.79 4 May 2004 RP new function
*/
String __export GetVolumeLabel(const String drive)
{
    String Result;
    String drv = UpperCase(ExpandFileName(drive));
    TCHAR DriveLetter;
    if ((DriveLetter = drv[1]) != '\\')  // Only for local drives
    {
        if ((DriveLetter < 'A') || (DriveLetter > 'Z'))
            return Result;
        if (!(::GetLogicalDrives() & (1 << (DriveLetter - 'A'))))
            return Result;
    }
    // Turn off critical errors.
    unsigned int OldMode = ::SetErrorMode(SEM_FAILCRITICALERRORS);
    unsigned NamLen = 255;
    DWORD SysLen = 255;
    TCHAR VolNameAry[256] = {0};
    DWORD SysFlags;
    if (::GetVolumeInformation(drv.T_STR(), VolNameAry, NamLen, NULL, &SysLen, &SysFlags, NULL, 0))
    {  // nor for fixed drives
        // W'll get this if there is a disk but it is not or wrong formatted
        // so this disk can only be used when we also want formatting.
        Result = VolNameAry;
        ::SetErrorMode(OldMode);
    }
    return Result;
}

/* IsSameFile---------------------------------------------------------
1.79 4 May 2004 RP new function
*/
bool __export IsSameFile(const String Fname1, const String Fname2)
{
    if (CompareText(ExpandFileName(Fname1), ExpandFileName(Fname2)) == 0)
        return true;
    bool Result;
    // in windows no alias so names must match
    if (CompareText(ExtractFileName(Fname1), ExtractFileName(Fname2)) == 0)
    {
        TSearchRec sr1, sr2;
        bool ff1 = (::FindFirst(Fname1, faAnyFile, sr1) == 0);
        bool ff2 = (::FindFirst(Fname2, faAnyFile, sr2) == 0);
        if ((ff1 == ff2) && !ff1)
            return false; // neither found assume different
        if (ff1 == ff2) // both exist
            Result = CompareMem(&sr1.FindData, &sr2.FindData, 2 + (4 * 4));
        if (ff1)
            FindClose(sr1);
        if (ff2)
            FindClose(sr2);
    }
    return Result;
}

/*? IsWild
1.73.4
 returns true if filespec contains wildcard(s)
*/
bool __export IsWild(const String fspec)
{
    return fspec.Pos("*") || fspec.Pos("?");
}
//? IsWild

//  Returns a boolean indicating whether or not we're running under XP or later.
bool __export IsWinXP(void)
{
    OSVERSIONINFO osv = {sizeof(OSVERSIONINFO)};
    GetVersionEx(&osv);
    return (osv.dwMajorVersion > 5) ||
           ((osv.dwMajorVersion == 5) && (osv.dwMinorVersion >= 1));
}

/*? PathConcat-------------------------------------------------
 1.73 RP new function
 add extra to path ensuring single backslash
*/
String __export PathConcat(const String path, const String extra)
{
    int PathLen = path.Length();
    String Result = path;
    if (PathLen)
    {
        TCHAR PathLast = path[PathLen];
        if ((PathLast != ':') && extra.Length())
        {
            if ((extra[1] == '\\') == (PathLast == '\\'))
            {
                if (PathLast == '\\')
                    Result  = path.SubString(1, PathLen - 1); // remove trailing
                else
                    Result = path + "\\"; // append trailing
            }
        }
    }
    Result += extra;
    return Result;
}
//? PathConcat

/*? QueryZip-----------------------------------------------------------
1.75.0.5 10 March 2004 RA change filesize to int64
1.75 18 February 2004 allow >2G
1.73 02 July 2003 RP
*/

int __export QueryZip(const String Name)
{
    ZipEndOfCentral EOC;
    ZipCentralHeader CentralHead;
    unsigned Sig;
    bool EOCPossible = false;
    int fileHandle = -1;
    int res = 0;
    int Result = -7;
    int Size;
    __int64 fileSize;
    unsigned char* ZipBuf = NULL;
    bool NotDone = true;
    try
    {
        try
        {
            // Open the input archive, presumably the last disk.
            if ((fileHandle = FileOpen(Name, fmShareDenyWrite | fmOpenRead)) == -1)
                return Result;
            Result = 0; // rest errors normally file too small
            // first we check if the start of the file has an IMAGE_DOS_SIGNATURE
            if (FileRead(fileHandle, &Sig, 4) != 4)
                return Result;
            if (LOWORD(Sig) == IMAGE_DOS_SIGNATURE)
                res = 1;
            else
                if (Sig == LocalFileHeaderSig)
                    res = 2;
                else
                    if (Sig == CentralFileHeaderSig)
                        res = 8; // part of split Central Directory
                    else
                        if (Sig == ExtLocalSig)
                            res = 4;        // first part of span
            // A test for a zip archive without a ZipComment.
            if ((fileSize = FileSeek(fileHandle, -static_cast<__int64>(sizeof(EOC)), 2)) == -1)
                return Result;  // not zip - too small
            // A test for a zip archive without a ZipComment.
            if (FileRead(fileHandle, &EOC, sizeof(EOC)) == static_cast<int>(sizeof(EOC)) &&
                    EOC.HeaderSig == EndCentralDirSig)
            {
                EOCPossible = true;
                res |= 128;              // EOC
                if (!EOC.ZipCommentLen)
                    res |= 16;     // good comment length
                if (EOC.CentralDiskNo == EOC.ThisDiskNo)
                {                           // verify start of central
                    if ((FileSeek(fileHandle, static_cast<__int64>(EOC.CentralOffset), 0) != -1)
                            && (FileRead(fileHandle, &CentralHead, sizeof(CentralHead)) == static_cast<int>(sizeof(CentralHead)))
                            && CentralHead.HeaderSig == CentralFileHeaderSig)
                    {
                        res |= 64;           // has linked Central
                        if ((CentralHead.DiskStart == EOC.ThisDiskNo)
                                && (FileSeek(fileHandle, static_cast<__int64>(CentralHead.RelOffLocal), 0) != -1)
                                && (FileRead(fileHandle, &Sig, sizeof(Sig)) == static_cast<int>(sizeof(Sig)))
                                && Sig == LocalFileHeaderSig)
                            res |= 32;         // linked local
                    }
                    NotDone = false;
                }
                if (NotDone)
                    res &= 0x01F;            // remove rest
            }
            if (NotDone)
            {
                // try to locate EOC
                fileSize += sizeof(EOC);
                Size = 65535 + sizeof(EOC);
                if (Size > fileSize)
                    Size = static_cast<int>(fileSize);
                ZipBuf = new unsigned char[Size + 1];
                __int64 ReadPos = fileSize - Size;
                if ((FileSeek(fileHandle, ReadPos, 0) != -1)
                        && (FileRead(fileHandle, ZipBuf, Size) == Size))
                {
                    // Finally try to find the EOC record within the last 65K...
                    ZipEndOfCentral* pEOC = reinterpret_cast<ZipEndOfCentral*>(ZipBuf + Size - sizeof(EOC));
                    while (reinterpret_cast<unsigned char*>(pEOC) > ZipBuf)    // reverse search
                    {
                        --reinterpret_cast<char*>(pEOC);
                        if (pEOC->HeaderSig == EndCentralDirSig)
                        {                         // possible EOC found
                            res |= 128;          // EOC
                            // check correct length comment
                            unsigned BufPos = static_cast<unsigned>(ReadPos) + (reinterpret_cast<unsigned char*>(pEOC) - ZipBuf);
                            if (static_cast<int>(BufPos + sizeof(EOC)) + pEOC->ZipCommentLen == fileSize)
                                res |= 16; // good comment length
                            if (pEOC->CentralDiskNo == pEOC->ThisDiskNo)
                            {                       // verify start of central
                                if ((FileSeek(fileHandle, static_cast<__int64>(pEOC->CentralOffset), 0) != -1)
                                        && (FileRead(fileHandle, &CentralHead, sizeof(CentralHead)) == static_cast<int>(sizeof(CentralHead)))
                                        && CentralHead.HeaderSig == CentralFileHeaderSig)
                                {
                                    res |= 64;       // has linked Central
                                    if (CentralHead.DiskStart == pEOC->ThisDiskNo
                                            && (FileSeek(fileHandle, static_cast<__int64>(CentralHead.RelOffLocal), 0) != -1)
                                            && (FileRead(fileHandle, &Sig, sizeof(Sig)) == static_cast<int>(sizeof(Sig)))
                                            && Sig == LocalFileHeaderSig)
                                        res |= 32;     // linked local
                                }
                                break;
                            }
                            res &= 0x01F;        // remove rest
                        }
                    }                            // while
                }
                if (EOCPossible && (res & 128) == 0)
                    res |= 128;
            }
        }
        catch (...)
        {
            Result = -9;                     // exception
        }
    }
    __finally
    {
        CloseFileHandle(fileHandle);
        if (!Result)
            Result = res;
        if (ZipBuf)
            delete[] ZipBuf;
    }
    return Result;
}
//? QueryZip

//--------------------------------------------------------
// 1.73 (22 May 2003) RP new MBCS safe function to change path separator
String __export SetSlash(const String path, TPathSlashDirection dir)
{
    TCHAR f, r;
    TCHAR c;
    String Result = path;
    const TCHAR* pth = path.T_STR();
    int len = path.Length();
    if (dir == psdInternal)
    {
        f = '\\';
        r = '/';
    }
    else
    {
        f = '/';
        r = '\\';
    }
    if (len)
    {
        unsigned i = 1;       // first result char
        while ((c = *pth) != 0)
        {
            if (c == f)
                Result[i] = r;    // will change address of Result.data
#ifndef UNICODE
            else
                if (LeadBytes.Contains(c))
                {
                    ++pth;
                    ++i;
                }
#endif
            ++pth;
            ++i;
        }
    }
    return Result;
}
// SetSlash



