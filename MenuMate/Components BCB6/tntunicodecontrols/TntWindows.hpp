// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'TntWindows.pas' rev: 6.00

#ifndef TntWindowsHPP
#define TntWindowsHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <ShlObj.hpp>	// Pascal unit
#include <ShellAPI.hpp>	// Pascal unit
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Tntwindows
{
//-- type declarations -------------------------------------------------------
#pragma pack(push, 4)
struct TSHNameMappingHeaderA
{
	unsigned cNumOfMappings;
	_SHNAMEMAPPINGA *lpNM;
} ;
#pragma pack(pop)

typedef TSHNameMappingHeaderA *PSHNameMappingHeaderA;

#pragma pack(push, 4)
struct TSHNameMappingHeaderW
{
	unsigned cNumOfMappings;
	_SHNAMEMAPPINGW *lpNM;
} ;
#pragma pack(pop)

typedef TSHNameMappingHeaderW *PSHNameMappingHeaderW;

//-- var, const, procedure ---------------------------------------------------
static const int DT_NOFULLWIDTHCHARBREAK = 0x80000;
static const unsigned INVALID_FILE_ATTRIBUTES = 0xffffffff;
static const char VQV_FIXEDFILEINFO = '\x5c';
#define VQV_VARFILEINFO_TRANSLATION "\\VarFileInfo\\Translation"
#define VQV_STRINGFILEINFO "\\StringFileInfo"
#define VER_COMMENTS "Comments"
#define VER_INTERNALNAME "InternalName"
#define VER_PRODUCTNAME "ProductName"
#define VER_COMPANYNAME "CompanyName"
#define VER_LEGALCOPYRIGHT "LegalCopyright"
#define VER_PRODUCTVERSION "ProductVersion"
#define VER_FILEDESCRIPTION "FileDescription"
#define VER_LEGALTRADEMARKS "LegalTrademarks"
#define VER_PRIVATEBUILD "PrivateBuild"
#define VER_FILEVERSION "FileVersion"
#define VER_ORIGINALFILENAME "OriginalFilename"
#define VER_SPECIALBUILD "SpecialBuild"
extern PACKAGE BOOL __fastcall Tnt_SetWindowTextW(HWND hWnd, wchar_t * lpString);
extern PACKAGE BOOL __fastcall Tnt_RemoveDirectoryW(wchar_t * lpPathName);
extern PACKAGE unsigned __fastcall Tnt_GetShortPathNameW(wchar_t * lpszLongPath, wchar_t * lpszShortPath, unsigned cchBuffer);
extern PACKAGE unsigned __fastcall Tnt_GetFullPathNameW(wchar_t * lpFileName, unsigned nBufferLength, wchar_t * lpBuffer, wchar_t * &lpFilePart);
extern PACKAGE unsigned __fastcall Tnt_CreateFileW(wchar_t * lpFileName, unsigned dwDesiredAccess, unsigned dwShareMode, Windows::PSecurityAttributes lpSecurityAttributes, unsigned dwCreationDisposition, unsigned dwFlagsAndAttributes, unsigned hTemplateFile);
extern PACKAGE unsigned __fastcall Tnt_FindFirstFileW(wchar_t * lpFileName, _WIN32_FIND_DATAW &lpFindFileData);
extern PACKAGE BOOL __fastcall Tnt_FindNextFileW(unsigned hFindFile, _WIN32_FIND_DATAW &lpFindFileData);
extern PACKAGE unsigned __fastcall Tnt_GetFileAttributesW(wchar_t * lpFileName);
extern PACKAGE BOOL __fastcall Tnt_SetFileAttributesW(wchar_t * lpFileName, unsigned dwFileAttributes);
extern PACKAGE BOOL __fastcall Tnt_CreateDirectoryW(wchar_t * lpPathName, Windows::PSecurityAttributes lpSecurityAttributes);
extern PACKAGE BOOL __fastcall Tnt_MoveFileW(wchar_t * lpExistingFileName, wchar_t * lpNewFileName);
extern PACKAGE BOOL __fastcall Tnt_CopyFileW(wchar_t * lpExistingFileName, wchar_t * lpNewFileName, BOOL bFailIfExists);
extern PACKAGE BOOL __fastcall Tnt_DeleteFileW(wchar_t * lpFileName);
extern PACKAGE int __fastcall Tnt_DrawTextW(HDC hDC, wchar_t * lpString, int nCount, Types::TRect &lpRect, unsigned uFormat);
extern PACKAGE BOOL __fastcall Tnt_GetDiskFreeSpaceW(wchar_t * lpRootPathName, unsigned &lpSectorsPerCluster, unsigned &lpBytesPerSector, unsigned &lpNumberOfFreeClusters, unsigned &lpTotalNumberOfClusters);
extern PACKAGE unsigned __fastcall Tnt_GetModuleFileNameW(unsigned hModule, wchar_t * lpFilename, unsigned nSize);
extern PACKAGE unsigned __fastcall Tnt_GetTempPathW(unsigned nBufferLength, wchar_t * lpBuffer);
extern PACKAGE unsigned __fastcall Tnt_GetWindowsDirectoryW(wchar_t * lpBuffer, unsigned uSize);
extern PACKAGE unsigned __fastcall Tnt_GetSystemDirectoryW(wchar_t * lpBuffer, unsigned uSize);
extern PACKAGE BOOL __fastcall Tnt_GetComputerNameW(wchar_t * lpBuffer, unsigned &nSize);
extern PACKAGE BOOL __fastcall Tnt_GetUserNameW(wchar_t * lpBuffer, unsigned &nSize);
extern PACKAGE unsigned __fastcall Tnt_ShellExecuteW(HWND hWnd, wchar_t * Operation, wchar_t * FileName, wchar_t * Parameters, wchar_t * Directory, int ShowCmd);
extern PACKAGE unsigned __fastcall Tnt_LoadLibraryW(wchar_t * lpLibFileName);
extern PACKAGE unsigned __fastcall Tnt_LoadLibraryExW(wchar_t * lpLibFileName, unsigned hFile, unsigned dwFlags);
extern PACKAGE BOOL __fastcall Tnt_CreateProcessW(wchar_t * lpApplicationName, wchar_t * lpCommandLine, Windows::PSecurityAttributes lpProcessAttributes, Windows::PSecurityAttributes lpThreadAttributes, BOOL bInheritHandles, unsigned dwCreationFlags, void * lpEnvironment, wchar_t * lpCurrentDirectory, const _STARTUPINFOA &lpStartupInfo, _PROCESS_INFORMATION &lpProcessInformation);
extern PACKAGE int __fastcall Tnt_GetCurrencyFormatW(unsigned Locale, unsigned dwFlags, wchar_t * lpValue, Windows::PCurrencyFmtW lpFormat, wchar_t * lpCurrencyStr, int cchCurrency);
extern PACKAGE int __fastcall Tnt_CompareStringW(unsigned Locale, unsigned dwCmpFlags, wchar_t * lpString1, int cchCount1, wchar_t * lpString2, int cchCount2);
extern PACKAGE wchar_t * __fastcall Tnt_CharUpperW(wchar_t * lpsz);
extern PACKAGE unsigned __fastcall Tnt_CharUpperBuffW(wchar_t * lpsz, unsigned cchLength);
extern PACKAGE wchar_t * __fastcall Tnt_CharLowerW(wchar_t * lpsz);
extern PACKAGE unsigned __fastcall Tnt_CharLowerBuffW(wchar_t * lpsz, unsigned cchLength);
extern PACKAGE BOOL __fastcall Tnt_GetStringTypeExW(unsigned Locale, unsigned dwInfoType, wchar_t * lpSrcStr, int cchSrc, void *lpCharType);
extern PACKAGE int __fastcall Tnt_LoadStringW(unsigned hInstance, unsigned uID, wchar_t * lpBuffer, int nBufferMax);
extern PACKAGE BOOL __fastcall Tnt_InsertMenuItemW(HMENU hMenu, unsigned uItem, BOOL fByPosition, const tagMENUITEMINFOW &lpmii);
extern PACKAGE HICON __fastcall Tnt_ExtractAssociatedIconW(unsigned hInst, wchar_t * lpIconPath, Word &lpiIcon);
extern PACKAGE unsigned __fastcall Tnt_GetFileVersionInfoSizeW(wchar_t * lptstrFilename, unsigned &lpdwHandle);
extern PACKAGE BOOL __fastcall Tnt_GetFileVersionInfoW(wchar_t * lptstrFilename, unsigned dwHandle, unsigned dwLen, void * lpData);
extern PACKAGE BOOL __fastcall Tnt_VerQueryValueW(void * pBlock, wchar_t * lpSubBlock, void * &lplpBuffer, unsigned &puLen);
extern PACKAGE int __fastcall Tnt_SHFileOperationW(_SHFILEOPSTRUCTW &lpFileOp);
extern PACKAGE void __fastcall Tnt_SHFreeNameMappings(unsigned hNameMappings);
extern PACKAGE Shlobj::PItemIDList __fastcall Tnt_SHBrowseForFolderW(_browseinfoW &lpbi);
extern PACKAGE BOOL __fastcall Tnt_SHGetPathFromIDListW(Shlobj::PItemIDList pidl, wchar_t * pszPath);
extern PACKAGE unsigned __fastcall Tnt_SHGetFileInfoW(wchar_t * pszPath, unsigned dwFileAttributes, _SHFILEINFOW &psfi, unsigned cbFileInfo, unsigned uFlags);
extern PACKAGE bool __fastcall Tnt_Is_IntResource(wchar_t * ResStr);
extern PACKAGE Word __fastcall LANGIDFROMLCID(unsigned lcid);
extern PACKAGE Word __fastcall MAKELANGID(Word usPrimaryLanguage, Word usSubLanguage);
extern PACKAGE unsigned __fastcall MAKELCID(Word wLanguageID, Word wSortID = (Word)(0x0));
extern PACKAGE Word __fastcall PRIMARYLANGID(Word lgid);
extern PACKAGE Word __fastcall SORTIDFROMLCID(unsigned lcid);
extern PACKAGE Word __fastcall SUBLANGID(Word lgid);

}	/* namespace Tntwindows */
using namespace Tntwindows;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// TntWindows
