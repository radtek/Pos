//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unicode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
UnicodeString WideStringToUTF8(const WideString &inString)
{
	int BufferSize	= WideCharToMultiByte(CP_UTF8 , 0, inString.c_bstr(), inString.Length(), NULL, 0, NULL, NULL);
	char *dest		= new char[BufferSize];

	WideCharToMultiByte(CP_UTF8 , 0, inString.c_bstr(), inString.Length(), dest, BufferSize, NULL, NULL);
	UnicodeString RetVal(dest, BufferSize);

	delete [] dest;

	return RetVal;
}
//---------------------------------------------------------------------------
WideString UTF8ToWideString(const AnsiString &inString)
{
	int BufferSize	= MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), NULL, 0);
	wchar_t *dest	= new wchar_t[BufferSize];

	MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), dest, BufferSize);
	WideString RetVal(dest,BufferSize);

	delete [] dest;

	return RetVal;
}
//---------------------------------------------------------------------------

