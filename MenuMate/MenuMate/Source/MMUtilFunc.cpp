//---------------------------------------------------------------------------


#pragma hdrstop

#include "MMUtilFunc.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

AnsiString UnicodeToUTF8AnsiString(UnicodeString in)
{
	UTF8String utf8str = UTF8Encode(in);
    AnsiString out = AnsiString(utf8str.c_str(),utf8str.Length());
	return out;
}
