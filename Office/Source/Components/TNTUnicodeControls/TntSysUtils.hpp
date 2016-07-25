// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntSysUtils.pas' rev: 6.00

#ifndef TntSysUtilsHPP
#define TntSysUtilsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Windows.hpp>	// Pascal unit
#include <SysUtils.hpp>	// Pascal unit
#include <Types.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntsysutils
{
//-- type declarations -------------------------------------------------------
#pragma option push -b-
enum TTntTextLineBreakStyle { tlbsLF, tlbsCRLF, tlbsCR };
#pragma option pop

#pragma pack(push, 4)
struct TSearchRecW
{
	int Time;
	int Size;
	int Attr;
	WideString Name;
	int ExcludeAttr;
	unsigned FindHandle;
	_WIN32_FIND_DATAW FindData;
} ;
#pragma pack(pop)

//-- var, const, procedure ---------------------------------------------------
static const wchar_t CR = wchar_t(0xd);
static const wchar_t LF = wchar_t(0xa);
#define CRLF L"\r\n"
static const wchar_t WideLineSeparator = wchar_t(0x2028);
extern PACKAGE bool Win32PlatformIsUnicode;
extern PACKAGE bool Win32PlatformIsXP;
extern PACKAGE wchar_t * __fastcall StrEndW(wchar_t * Str);
extern PACKAGE unsigned __fastcall StrLenW(wchar_t * Str);
extern PACKAGE wchar_t * __fastcall StrLCopyW(wchar_t * Dest, wchar_t * Source, unsigned MaxLen);
extern PACKAGE wchar_t * __fastcall StrCopyW(wchar_t * Dest, wchar_t * Source);
extern PACKAGE wchar_t * __fastcall StrECopyW(wchar_t * Dest, wchar_t * Source);
extern PACKAGE wchar_t * __fastcall StrPLCopyW(wchar_t * Dest, const AnsiString Source, unsigned MaxLen);
extern PACKAGE wchar_t * __fastcall StrPCopyW(wchar_t * Dest, const AnsiString Source);
extern PACKAGE int __fastcall StrLCompW(wchar_t * Str1, wchar_t * Str2, unsigned MaxLen);
extern PACKAGE int __fastcall StrCompW(wchar_t * Str1, wchar_t * Str2);
extern PACKAGE int __fastcall StrLICompW(wchar_t * Str1, wchar_t * Str2, unsigned MaxLen);
extern PACKAGE int __fastcall StrICompW(wchar_t * Str1, wchar_t * Str2);
extern PACKAGE wchar_t * __fastcall StrLowerW(wchar_t * Str);
extern PACKAGE wchar_t * __fastcall StrUpperW(wchar_t * Str);
extern PACKAGE wchar_t * __fastcall StrPosW(wchar_t * Str, wchar_t * SubStr);
extern PACKAGE wchar_t * __fastcall StrScanW(const wchar_t * Str, wchar_t Chr);
extern PACKAGE wchar_t * __fastcall StrRScanW(const wchar_t * Str, wchar_t Chr);
extern PACKAGE wchar_t * __fastcall StrLCatW(wchar_t * Dest, const wchar_t * Source, unsigned MaxLen);
extern PACKAGE wchar_t * __fastcall StrCatW(wchar_t * Dest, const wchar_t * Source);
extern PACKAGE wchar_t * __fastcall StrMoveW(wchar_t * Dest, const wchar_t * Source, unsigned Count);
extern PACKAGE WideString __fastcall StrPasW(const wchar_t * Str);
extern PACKAGE wchar_t * __fastcall StrAllocW(unsigned Size);
extern PACKAGE unsigned __fastcall StrBufSizeW(const wchar_t * Str);
extern PACKAGE wchar_t * __fastcall StrNewW(const wchar_t * Str);
extern PACKAGE void __fastcall StrDisposeW(wchar_t * Str);
extern PACKAGE WideString __fastcall Tnt_WideUpperCase(const WideString S);
extern PACKAGE WideString __fastcall Tnt_WideLowerCase(const WideString S);
extern PACKAGE wchar_t __fastcall WideLastChar(WideString W);
extern PACKAGE WideString __fastcall WideStringReplace(const WideString S, const WideString OldPattern, const WideString NewPattern, Sysutils::TReplaceFlags Flags, bool WholeWord = false);
extern PACKAGE int __fastcall TntAdjustLineBreaksLength(const WideString S, TTntTextLineBreakStyle Style = (TTntTextLineBreakStyle)(0x1));
extern PACKAGE WideString __fastcall TntAdjustLineBreaks(const WideString S, TTntTextLineBreakStyle Style = (TTntTextLineBreakStyle)(0x1));
extern PACKAGE WideString __fastcall WideQuotedStr(const WideString S, wchar_t Quote = wchar_t(0x22));
extern PACKAGE WideString __fastcall WideExtractQuotedStr(wchar_t * &Src, wchar_t Quote = wchar_t(0x22));
extern PACKAGE WideString __fastcall WideWrapText(const WideString Line, const WideString BreakStr, const Sysutils::TSysCharSet &BreakChars, int MaxCol)/* overload */;
extern PACKAGE WideString __fastcall WideWrapText(const WideString Line, int MaxCol)/* overload */;
extern PACKAGE WideString __fastcall WideIncludeTrailingBackslash(const WideString S);
extern PACKAGE WideString __fastcall WideExcludeTrailingBackslash(const WideString S);
extern PACKAGE bool __fastcall WideIsDelimiter(const WideString Delimiters, const WideString S, int Index);
extern PACKAGE bool __fastcall WideIsPathDelimiter(const WideString S, int Index);
extern PACKAGE int __fastcall WideLastDelimiter(const WideString Delimiters, const WideString S);
extern PACKAGE WideString __fastcall WideChangeFileExt(const WideString FileName, const WideString Extension);
extern PACKAGE WideString __fastcall WideExtractFilePath(const WideString FileName);
extern PACKAGE WideString __fastcall WideExtractFileDir(const WideString FileName);
extern PACKAGE WideString __fastcall WideExtractFileDrive(const WideString FileName);
extern PACKAGE WideString __fastcall WideExtractFileName(const WideString FileName);
extern PACKAGE WideString __fastcall WideExtractFileExt(const WideString FileName);
extern PACKAGE WideString __fastcall WideExtractRelativePath(const WideString BaseName, const WideString DestName);
extern PACKAGE WideString __fastcall WideExpandFileName(const WideString FileName);
extern PACKAGE WideString __fastcall WideExtractShortPathName(const WideString FileName);
extern PACKAGE int __fastcall WideFileCreate(const WideString FileName);
extern PACKAGE int __fastcall WideFileOpen(const WideString FileName, unsigned Mode);
extern PACKAGE bool __fastcall WideDirectoryExists(const WideString Name);
extern PACKAGE bool __fastcall WideFileExists(const WideString Name);
extern PACKAGE unsigned __fastcall WideFileGetAttr(const WideString FileName);
extern PACKAGE bool __fastcall WideFileSetAttr(const WideString FileName, int Attr);
extern PACKAGE bool __fastcall WideForceDirectories(WideString Dir);
extern PACKAGE WideString __fastcall WideFileSearch(const WideString Name, const WideString DirList);
extern PACKAGE bool __fastcall WideRenameFile(const WideString OldName, const WideString NewName);
extern PACKAGE bool __fastcall WideDeleteFile(const WideString FileName);
extern PACKAGE bool __fastcall WideCopyFile(WideString FromFile, WideString ToFile, bool FailIfExists);
extern PACKAGE int __fastcall WideFindFirst(const WideString Path, int Attr, TSearchRecW &F);
extern PACKAGE int __fastcall WideFindNext(TSearchRecW &F);
extern PACKAGE void __fastcall WideFindClose(TSearchRecW &F);
extern PACKAGE bool __fastcall ValidDateTimeStr(WideString Str);
extern PACKAGE bool __fastcall ValidDateStr(WideString Str);
extern PACKAGE bool __fastcall ValidTimeStr(WideString Str);
extern PACKAGE System::TDateTime __fastcall TntStrToDateTime(WideString Str);
extern PACKAGE System::TDateTime __fastcall TntStrToDate(WideString Str);
extern PACKAGE System::TDateTime __fastcall TntStrToTime(WideString Str);
extern PACKAGE WideString __fastcall WideGetLocaleStr(unsigned LocaleID, int LocaleType, WideString Default);
extern PACKAGE unsigned __fastcall WinCheckH(unsigned RetVal);
extern PACKAGE unsigned __fastcall WinCheckFileH(unsigned RetVal);
extern PACKAGE void * __fastcall WinCheckP(void * RetVal);
extern PACKAGE WideString __fastcall WideGetModuleFileName(unsigned Instance);
extern PACKAGE bool __fastcall IsWideCharUpper(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharLower(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharDigit(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharSpace(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharPunct(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharCntrl(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharBlank(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharXDigit(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharAlpha(wchar_t WC);
extern PACKAGE bool __fastcall IsWideCharAlphaNumeric(wchar_t WC);
extern PACKAGE int __fastcall WideTextPos(const WideString SubStr, const WideString S);
extern PACKAGE WideString __fastcall ExtractStringArrayStr(wchar_t * P);
extern PACKAGE WideString __fastcall ExtractStringFromStringArray(wchar_t * &P, wchar_t Separator = wchar_t(0x0));
extern PACKAGE TWideStringDynArray __fastcall ExtractStringsFromStringArray(wchar_t * P, wchar_t Separator = wchar_t(0x0));
extern PACKAGE bool __fastcall IsWideCharMappableToAnsi(const wchar_t WC);
extern PACKAGE bool __fastcall IsWideStringMappableToAnsi(const WideString WS);
extern PACKAGE bool __fastcall IsRTF(const WideString Value);

}	/* namespace Tntsysutils */
using namespace Tntsysutils;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntSysUtils
