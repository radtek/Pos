//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unicode.h"

//---------------------------------------------------------------------------
AnsiString WideStringToUTF8(WideString inString)
{
	int BufferSize   = WideCharToMultiByte(CP_UTF8 , 0, inString.c_bstr(), inString.Length(), NULL, 0, NULL, NULL);
	char *dest    = new char[BufferSize];
	WideCharToMultiByte(CP_UTF8 , 0, inString.c_bstr(), inString.Length(), dest, BufferSize, NULL, NULL);
	AnsiString RetVal(dest,BufferSize);
	delete [] dest;
	return RetVal;
}

WideString UTF8ToWideString(AnsiString inString)
{
	int BufferSize   = MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), NULL, 0);
	wchar_t *dest    = new wchar_t[BufferSize];
	MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), dest, BufferSize);
	WideString RetVal(dest,BufferSize);
	delete [] dest;
	return RetVal;
}


#pragma package(smart_init)
