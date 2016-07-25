//---------------------------------------------------------------------------

#ifndef ZipLibsH
#define ZipLibsH

#include <vcl\classes.hpp>
#include "ZipBase.h"
/* Dll loader unit for ZipMaster
 by R. Peters
 edited 10 September 2003
         2 October 2003
        18 April 2004
        28 April 2004 - Added procedure Abort;
 */

//---------------------------------------------------------------------------
namespace TZiplibloader
{

typedef const char *dllargs[3];
typedef int __stdcall (*ZDllExecAddress)(void*);
typedef int __stdcall (*ZDllVersionAddress)(void);
typedef int __stdcall (*ZDllPrivFuncAddress)(void);


#define  RDLL_Zip   11605
#define  RDLL_Unz   11606
#define  RDLL_ZVer  11607
#define  RDLL_UVer  11608


class TZipLibLoader
{
private:
	HINSTANCE hndl;
	bool IsZip;
	AnsiString fPath;
  TObject* fOwner;
	int loadLevel;
	int fVer;
  int fPriv;
	AnsiString TmpFileName;
  int __fastcall GetVer(void);
  AnsiString __fastcall GetPath(void);
  int __fastcall GetPriv(void);
	int __fastcall GetLoaded(void);
	AnsiString __fastcall GetLoadedPath(void);
	ZDllExecAddress fExecFunc;
	ZDllVersionAddress fVersFunc;
  ZDllPrivFuncAddress fPrivFunc;
protected:
    void Clear(void);
		int DoExec(void *info, int minver);
		int DoLoad(int level);
		int DoUnload(int level);
    int Expand(AnsiString src,AnsiString dest);
		AnsiString __fastcall GetResDllPath(void);
    int LoadLib(AnsiString FullPath, bool AllowNotExist);
public:
		TZipLibLoader(TObject *owner, bool type);
		~TZipLibLoader(void);
		int LoadDll(int minver, bool level=false);	// true = keep loaded
		void Unload(bool level=false);
    void Abort(void);
		__property String Path = {read=GetPath};
		__property int Version = {read=GetVer};
		__property int Loaded = {read=GetLoaded};
    __property int Build = {read=GetPriv};
};

} /* namespace TZipLibLoader */
#if !defined(NO_IMPLICIT_NAMESPACE_USE)
using namespace TZiplibloader;
#endif
#endif

