//---------------------------------------------------------------------------

#ifndef ZipUtilsH
#define ZipUtilsH
#include <vcl.h>
#include "ZipBuilder.h"
//---------------------------------------------------------------------------
enum TPathSlashDirection {psdExternal, psdInternal};

union I64Rec
{
  __int64 I;
  struct
  {
  	unsigned Lo;
    unsigned Hi;
  } Rec;
};



bool FileNameMatch(const AnsiString Pat, const AnsiString Subj); // new 1.73
bool ForceDirectory(const AnsiString Dir);
AnsiString SetSlash(const AnsiString path, bool forward=false);
AnsiString DelimitPath(const AnsiString path, bool sep=true);
bool DirExists( const AnsiString Name );
AnsiString FileVersion(AnsiString fname);
int QueryZip(const AnsiString Name); // new 1.73
AnsiString PathConcat(AnsiString path,AnsiString extra); // new 1.73
bool IsWild(const AnsiString fspec); // new 1.73.4  
int EraseFile(const AnsiString Fname, DeleteOpts How);





#endif

