//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include <stdio.h>
#include <string.h>
#include <tchar.h>
#include "Version.h"
#include "MMMessageBox.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

UnicodeString GetFileVersion()
{
    return getFileVersionFromFixedInfo();
}
//---------------------------------------------------------------------------

UnicodeString GetFileVersionString()
{
    if( isPreReleaseBuild() )
    {
        return getFileVersionFromStringBlock();
    }

    return getFileVersionFromFixedInfo();
}
//---------------------------------------------------------------------------

int GetFileVersionMajor()
{
   DWORD dwWnd;
   AnsiString path = ParamStr(0);
   DWORD dwInfoSize = GetFileVersionInfoSize(path.c_str(), &dwWnd);
   int MajorVerNum = -1;
   if (dwInfoSize != 0)
   {
		char *ptrVerBuf	= new char [dwInfoSize];
		bool Status		= GetFileVersionInfo(path.c_str(), 0, dwInfoSize, ptrVerBuf);
		if(Status)
		{
			PVSFixedFileInfo FI;
			UINT dwVerSize;
			if (VerQueryValue(ptrVerBuf, _T("\\"), (void **) &FI, &dwVerSize))
			{
				MajorVerNum = HIWORD( FI->dwFileVersionMS );
			}
		}
		delete [] ptrVerBuf;
   }
   return MajorVerNum;
}
//---------------------------------------------------------------------------
int GetFileVersionMinor()
{
	DWORD					dwWnd;
	AnsiString path = ParamStr(0);

	DWORD dwInfoSize = GetFileVersionInfoSize(path.c_str(), &dwWnd);

   int MinorVerNum = -1;
   if (dwInfoSize != 0)
   {
		char *ptrVerBuf	= new char [dwInfoSize];
		bool Status		= GetFileVersionInfo(path.c_str(), 0, dwInfoSize, ptrVerBuf);
		if(Status)
		{
			PVSFixedFileInfo FI;
			UINT dwVerSize;
			if (VerQueryValue(ptrVerBuf, _T("\\"), (void **) &FI, &dwVerSize))
			{
				MinorVerNum = LOWORD( FI->dwFileVersionMS );
			}
		}
		delete [] ptrVerBuf;
   }
   return MinorVerNum;
}

//---------------------------------------------------------------------------
bool isPreReleaseBuild()
{
    bool result = false;

    DWORD			dwWnd;
    AnsiString path = ParamStr(0);
    DWORD dwInfoSize = GetFileVersionInfoSize(path.c_str(), &dwWnd);

    if (dwInfoSize != 0)
    {
        char *ptrVerBuf	= new char [dwInfoSize];
        bool Status			= GetFileVersionInfo(path.c_str(), 0, dwInfoSize, ptrVerBuf);
        if(Status)
        {
            PVSFixedFileInfo FI;
            UINT dwVerSize;
            if (VerQueryValue(ptrVerBuf, _T("\\"), (void **) &FI, &dwVerSize))
            {
                DWORD dwFileFlags = FI->dwFileFlags;
                result = dwFileFlags & _PreReleaseBuild;    // if pre-release ( for testing ) build, dwFileFlags contains value 0x02L
            }
            delete [] ptrVerBuf;
        }
    }

    return result;
}

//---------------------------------------------------------------------------
UnicodeString getFileVersionFromStringBlock()
{
   UnicodeString	strVersion = "";
   DWORD			dwWnd;
   AnsiString path = ParamStr(0);
   DWORD dwInfoSize = GetFileVersionInfoSize(path.c_str(), &dwWnd);

	if (dwInfoSize != 0)
	{
		char *ptrVerBuf	= new char [dwInfoSize];
		bool Status			= GetFileVersionInfo(path.c_str(), 0, dwInfoSize, ptrVerBuf);
		if(Status)
		{
			LPVOID pvVersionStr = NULL;
			UINT uiVerStrSize = 0;
			if (VerQueryValue(ptrVerBuf, _T("\\StringFileInfo\\040904b0\\FileVersion"), &pvVersionStr, &uiVerStrSize))
			{
				strVersion = (UnicodeString)((LPCSTR)pvVersionStr);
			}
			delete [] ptrVerBuf;
		}
	}
	return strVersion;
}

//---------------------------------------------------------------------------
UnicodeString getFileVersionFromFixedInfo()
{
   UnicodeString	strVersion = "";
   DWORD			dwWnd;
   AnsiString path = ParamStr(0);
   DWORD dwInfoSize = GetFileVersionInfoSize(path.c_str(), &dwWnd);

	if (dwInfoSize != 0)
	{
		char *ptrVerBuf	= new char [dwInfoSize];
		bool Status			= GetFileVersionInfo(path.c_str(), 0, dwInfoSize, ptrVerBuf);
		if(Status)
		{
			PVSFixedFileInfo FI;
			UINT dwVerSize;
			if (VerQueryValue(ptrVerBuf, _T("\\"), (void **) &FI, &dwVerSize))
			{
				TVarRec v[] = {	HIWORD( FI->dwFileVersionMS ),
										LOWORD( FI->dwFileVersionMS ),
										HIWORD( FI->dwFileVersionLS ),
										LOWORD( FI->dwFileVersionLS ) };
				strVersion = Format( "%d.%d.%d.%d", v, ARRAYSIZE(v) - 1);
			}
			delete [] ptrVerBuf;
		}
	}
	return strVersion;
}

//---------------------------------------------------------------------------
