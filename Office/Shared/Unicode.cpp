//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "Unicode.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//---------------------------------------------------------------------------
AnsiString WideStringToUTF8(const WideString &inString)
{
/*
	int BufferSize	= WideCharToMultiByte(CP_UTF8 , 0, inString.c_bstr(), inString.Length(), NULL, 0, NULL, NULL);
	char *dest		= new char[BufferSize];

	WideCharToMultiByte(CP_UTF8 , 0, inString.c_bstr(), inString.Length(), dest, BufferSize, NULL, NULL);
	AnsiString RetVal(dest, BufferSize);

	delete [] dest;

	return RetVal;
*/
}
//---------------------------------------------------------------------------
WideString UTF8ToWideString(const AnsiString &inString)
{
/*
	int BufferSize	= MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), NULL, 0);
	wchar_t *dest	= new wchar_t[BufferSize];

	MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), dest, BufferSize);
	WideString RetVal(dest,BufferSize);

	delete [] dest;

	return RetVal;
*/
}
//---------------------------------------------------------------------------
AnsiString WideStringToMultiByte(const WideString &inString, int CodePage)
{
	AnsiString Result;

	int BufferSize	= WideCharToMultiByte(CodePage , 0, inString.c_bstr(), inString.Length(), NULL, 0, NULL, NULL);
	Result.SetLength(BufferSize);
	WideCharToMultiByte(CP_UTF8 , 0, inString.c_bstr(), inString.Length(), Result.c_str(), BufferSize, NULL, NULL);

	return Result;
}
//---------------------------------------------------------------------------
WideString MultiByteToWideString(const AnsiString &inString, int CodePage)
{
	WideString Result;

	int BufferSize	= MultiByteToWideChar(CodePage , 0, inString.c_str(), inString.Length(), NULL, 0);
	Result.SetLength(BufferSize);
	MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), Result.c_bstr(), BufferSize);

	return Result;
/*
	int BufferSize	= MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), NULL, 0);
	wchar_t *dest	= new wchar_t[BufferSize];

	MultiByteToWideChar(CP_UTF8 , 0, inString.c_str(), inString.Length(), dest, BufferSize);
	WideString RetVal(dest,BufferSize);

	delete [] dest;

	return RetVal;
*/
}
//---------------------------------------------------------------------------

