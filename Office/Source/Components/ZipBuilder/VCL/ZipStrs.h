//---------------------------------------------------------------------------

#ifndef ZipBldStrH
#define ZipBldStrH
#include <system.hpp>
#include "ZipMsg.h"
#include "ZipBuildDefs.h" 
//---------------------------------------------------------------------------

typedef void __fastcall (__closure *TZipStrEvent)(int Ident, AnsiString& DefStr);

struct TZipResRec
{
    Word i;
    const char* s;
};

// helper functions
const char* Find(int id);
AnsiString FindOld(int id);

// Global functions to load resource string - new 1.76
TZipStrEvent SetZipStr(TZipStrEvent handler, bool set); // bool added cannot pass -1 has __closure
AnsiString ZipLoadStr(int ident);
AnsiString ZipLoadStr(int ident, const AnsiString DefStr);
AnsiString ZipFmtLoadStr(int Ident, const System::TVarRec* Args, const int Args_Size);
AnsiString ZipLookup(int ident);

#endif

