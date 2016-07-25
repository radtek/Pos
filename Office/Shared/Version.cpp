//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Version.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
TFileVersion::TFileVersion() : Major(0), Minor(0), Release(0), Build(0)
{
	GetFileVersion(ParamStr(0));
}
//---------------------------------------------------------------------------
TFileVersion::TFileVersion(int major, int minor, int release, int build) :
		Major(major), Minor(minor), Release(release), Build(build)
{
}
//---------------------------------------------------------------------------
TFileVersion::TFileVersion(AnsiString FileName) : Major(0), Minor(0), Release(0), Build(0)
{
	GetFileVersion(FileName);
}
//---------------------------------------------------------------------------
void TFileVersion::GetFileVersion(AnsiString FileName)
{
	DWORD dwWnd;
	DWORD dwInfoSize = GetFileVersionInfoSize(FileName.c_str(), &dwWnd);

	if (dwInfoSize == 0) return;

	AnsiString VerBuf;
	VerBuf.SetLength(dwInfoSize + 1);

	if (!GetFileVersionInfo(FileName.c_str(), 0, dwInfoSize, VerBuf.c_str())) return;

	PVSFixedFileInfo	FI;
	UINT					dwVerSize;
	
	if (VerQueryValue(VerBuf.c_str(), "\\", (void **) &FI, &dwVerSize))
	{
		Major		= HIWORD(FI->dwFileVersionMS);
		Minor		= LOWORD(FI->dwFileVersionMS);
		Release	= HIWORD(FI->dwFileVersionLS);
		Build		= LOWORD(FI->dwFileVersionLS);
	}
}
//---------------------------------------------------------------------------
AnsiString TFileVersion::Version()
{
	TVarRec v[] = { Major, Minor, Release, Build }; 
	return Format( "%d.%d.%d.%d", v, ARRAYSIZE(v) - 1);
}
//---------------------------------------------------------------------------

